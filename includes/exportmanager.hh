#pragma once
#include <psapi.h>
#include <strsafe.h>

struct qExportNode* gExportNode = nullptr;

struct qExportNode
{
    void** mAddress;
    const char* mModuleName;
    const char* mImportName;
    qExportNode* mNext;

    qExportNode(void** address, const char* mod_name, const char* import_name) : mAddress(address), mModuleName(mod_name), mImportName(import_name)
    {
        mNext = gExportNode;
        gExportNode = this;
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
    qExportNode g_##name##_Node = { &g_##name, moduleName, #name }; \
    __attribute__((naked)) extern "C" void name##_() \
    { \
        asm volatile("jmp *%0;" : : "r"(g_##name)); \
    }

//============================================================================================

namespace ExportMgr
{
    HMODULE GetOriginalModule(const char* p_szModuleName)
    {
        char path[MAX_PATH] = { '\0' };

        // Try load original from current folder
        {
            StringCchCatA(path, sizeof(path), p_szModuleName);
            StringCchCatA(path, sizeof(path), "_original");

            if (auto mod = LoadLibraryA(path)) {
                return mod;
            }
        }

        // Try load original from system folder
        {
            GetSystemDirectoryA(path, sizeof(path));
            StringCchCatA(path, sizeof(path), "\\");
            StringCchCatA(path, sizeof(path), p_szModuleName);

            if (auto mod = LoadLibraryA(path)) {
                return mod;
            }
        }

        return nullptr;
    }

    bool InitializeExportNodes(HMODULE p_hCurrentModule)
    {
    #ifdef EXPORTS_ALL
        char mod_name[MAX_PATH];
        int mod_name_len = static_cast<int>(K32GetModuleBaseNameA(GetCurrentProcess(), p_hCurrentModule, mod_name, sizeof(mod_name)));
    #endif

        for (auto* node = gExportNode; node; node = node->mNext)
        {
        #ifdef EXPORTS_ALL
            size_t name_len;
            StringLengthWorkerA(node->mModuleName, SIZE_MAX, &name_len);

            // Only initialize exports based on current module name
            if (CompareStringA(LOCALE_SYSTEM_DEFAULT, NORM_IGNORECASE, mod_name, min(mod_name_len, static_cast<int>(name_len)), node->mModuleName, static_cast<int>(name_len)) == CSTR_EQUAL)
        #endif
            {
                if (auto mod = GetOriginalModule(node->mModuleName)) {
                    *node->mAddress = GetProcAddress(mod, node->mImportName);
                }
                else
                {
                    auto* str = core::GetFormattedMessage("Failed to load original module: %1", node->mModuleName);
                    MessageBoxA(0, str, PROJECT_NAME, MB_OK | MB_ICONERROR);
                    LocalFree(str);
                    return false;
                }
            }
        }

        return true;
    }
}