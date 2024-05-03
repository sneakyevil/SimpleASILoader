//====================================================================
// Defines

#define PROJECT_NAME        "Simple ASI Loader"


//====================================================================
// Includes (Important)

#include "Includes/CRT.hh"

//====================================================================
// Core

#include "Core/Functions.hh"
#include "Core/Loader.hh"

//====================================================================
// Export Manager

#include "Includes/ExportManager.hh"

//====================================================================
// Exports

// List
#if defined(EXPORTS_DINPUT8) || defined(EXPORTS_ALL)
    #include "Exports/DInput8.hh"
#endif
#if defined(EXPORTS_VERSION) || defined(EXPORTS_ALL)
    #include "Exports/Version.hh"
#endif
#if defined(EXPORTS_VORBISFILE) || defined(EXPORTS_ALL)
    #include "Exports/VorbisFile.hh"
#endif

//====================================================================
// Entrypoint

int __stdcall DllMain(HMODULE p_Module, DWORD p_Reason, void* p_Reserved)
{
    if (p_Reason == DLL_PROCESS_ATTACH)
    {
        DisableThreadLibraryCalls(p_Module);
        CRT::Initialize();

        if (!ExportMgr::InitializeExportNodes(p_Module)) {
            return 0;
        }

        Core::StartLoader();
    }

    return 1;
}