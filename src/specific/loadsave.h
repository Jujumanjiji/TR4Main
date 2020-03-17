#pragma once

#ifdef DLL_INJECT
#define LoadGame ((int(__cdecl*)(void)) 0x0043B710)
#define SaveGame ((int(__cdecl*)(void)) 0x0043B730)
#endif