#include "framework.h"

#define DLL_EXPORT __declspec(dllexport)
BOOL DLL_EXPORT CALLBACK DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
        case DLL_PROCESS_ATTACH:
            inject->inject_3dsystem();
            inject->inject_game();
            inject->inject_specific();
            break;
        case DLL_PROCESS_DETACH:
            
            break;
    }
    return TRUE;
}

