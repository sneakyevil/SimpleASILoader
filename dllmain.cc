//====================================================================
// Defines

#define PROJECT_NAME        "Simple ASI Loader"


//====================================================================
// Includes (Important)

#include "includes/crt.hh"

//====================================================================
// Core

#include "core/functions.hh"
#include "core/loader.hh"

//====================================================================
// Export Manager

#include "includes/exportmanager.hh"

//====================================================================
// Exports

// List
#if defined(EXPORTS_CRYPTBASE) || defined(EXPORTS_ALL)
    #include "exports/cryptbase.hh"
#endif
#if defined(EXPORTS_DINPUT8) || defined(EXPORTS_ALL)
    #include "exports/dinput8.hh"
#endif
#if defined(EXPORTS_VERSION) || defined(EXPORTS_ALL)
    #include "exports/version.hh"
#endif
#if defined(EXPORTS_VORBISFILE) || defined(EXPORTS_ALL)
    #include "exports/vorbisfile.hh"
#endif

//====================================================================
// Entrypoint

int __stdcall DllMain(HMODULE p_Module, DWORD p_Reason, void* p_Reserved)
{
    if (p_Reason == DLL_PROCESS_ATTACH)
    {
        //MessageBoxA(0, "Loaded!", "Simple ASI Loader", MB_OK);

        DisableThreadLibraryCalls(p_Module);
        CRT::Initialize();

        if (!ExportMgr::InitializeExportNodes(p_Module)) {
            return 0;
        }

        core::StartLoader();
    }

    return 1;
}