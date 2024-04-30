#pragma once
#include <psapi.h>
#include <strsafe.h>

struct ExportNode_t* g_ExportNode = nullptr;

struct ExportNode_t
{
    void** m_pAddress;
    const char* m_szModuleName;
    const char* m_szImportName;
    ExportNode_t* m_pNext;

    ExportNode_t(void** p_Address, const char* p_ModuleName, const char* p_ImportName) : m_pAddress(p_Address), m_szModuleName(p_ModuleName), m_szImportName(p_ImportName)
    {
        m_pNext = g_ExportNode;
        g_ExportNode = this;
    }
};

//============================================================================================
// Preprocessors

#ifdef _WIN64
    #define ADD_EXPORT_LINK(name) __pragma(comment(linker, "-export:"#name"="#name"_"))
#else
    #define ADD_EXPORT_LINK(name) __pragma(comment(linker, "-export:"#name"=_"#name"_"))
#endif

#define ADD_EXPORT(moduleName, name) \
    ADD_EXPORT_LINK(name) \
    void* g_##name; \
    ExportNode_t g_##name##_Node = { &g_##name, moduleName, #name }; \
    __attribute__((naked)) extern "C" void name##_() \
    { \
        asm volatile("jmp *%0;" : : "r"(g_##name)); \
    }

//============================================================================================

namespace ExportMgr
{
    HMODULE GetOriginalModule(const char* p_szModuleName)
    {
        char szModulePath[MAX_PATH] = { '\0' };

        // Try load original from current folder
        {
            StringCchCatA(szModulePath, sizeof(szModulePath), p_szModuleName);
            StringCchCatA(szModulePath, sizeof(szModulePath), "_original");

            HMODULE hModule = LoadLibraryA(szModulePath);
            if (hModule) {
                return hModule;
            }
        }

        // Try load original from system folder
        {
            GetSystemDirectoryA(szModulePath, sizeof(szModulePath));
            StringCchCatA(szModulePath, sizeof(szModulePath), "\\");
            StringCchCatA(szModulePath, sizeof(szModulePath), p_szModuleName);

            HMODULE hModule = LoadLibraryA(szModulePath);
            if (hModule) {
                return hModule;
            }
        }

        return nullptr;
    }

    bool InitializeExportNodes(HMODULE p_hCurrentModule)
    {
    #ifdef EXPORTS_ALL
        char szModuleName[MAX_PATH];
        int szModuleNameLength = static_cast<int>(K32GetModuleBaseNameA(GetCurrentProcess(), p_hCurrentModule, szModuleName, sizeof(szModuleName)));
    #endif

        while (g_ExportNode)
        {
            const char* szExportModuleName = g_ExportNode->m_szModuleName;

        #ifdef EXPORTS_ALL
            int szExportModuleNameLength;
            StringLengthWorkerA(szExportModuleName, SIZE_MAX, reinterpret_cast<size_t*>(&szExportModuleNameLength));

            // Only initialize exports based on current module name
            if (CompareStringA(LOCALE_SYSTEM_DEFAULT, NORM_IGNORECASE, szModuleName, min(szModuleNameLength, szExportModuleNameLength), szExportModuleName, szExportModuleNameLength) == CSTR_EQUAL)
        #endif
            {
                HMODULE hModule = GetOriginalModule(szExportModuleName);
                if (hModule) {
                    *g_ExportNode->m_pAddress = GetProcAddress(hModule, g_ExportNode->m_szImportName);
                }
                else
                {
                    auto pStr = Core::GetFormattedMessage("Failed to load original module: %1", g_ExportNode->m_szModuleName);
                    MessageBoxA(0, pStr, PROJECT_NAME, MB_OK | MB_ICONERROR);
                    LocalFree(pStr);
                    return false;
                }
            }

            g_ExportNode = g_ExportNode->m_pNext;
        }

        return true;
    }
}