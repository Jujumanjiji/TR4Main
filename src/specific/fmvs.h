#pragma once

#ifdef DLL_INJECT
#define S_PlayFMV ((int(__cdecl*)(int fmvID)) 0x0049C3E0)
#endif
