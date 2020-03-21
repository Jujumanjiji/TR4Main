#include "framework.h"
#include "utils.h"

BOOL __declspec(dllexport) CALLBACK DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
        case DLL_PROCESS_ATTACH:
#ifdef DLL_INJECT
            inject->inject_3dsystem();
            inject->inject_game();
            inject->inject_specific();
#endif
            break;
        case DLL_PROCESS_DETACH:
            break;
    }
    return TRUE;
}