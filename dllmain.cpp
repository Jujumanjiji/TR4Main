#include "framework.h"
#include "utils.h"
#include "gameflow_utils.h"

DLL_EXPORT BOOL CALLBACK DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
        case DLL_PROCESS_ATTACH:
#ifdef DLL_INJECT
            g_inject.inject_3dsystem();
            g_inject.inject_game();
            g_inject.inject_specific();
#endif
            break;
        case DLL_PROCESS_DETACH:
            
            break;
    }
    return TRUE;
}