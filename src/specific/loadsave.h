#pragma once

extern int LoadGameInfo(void);
extern int SaveGameInfo(void);

#ifdef DLL_INJECT
#define LoadSaveThread ((int(__cdecl*)(int key_pressed, int draw_monoscreen)) 0x0047CD20)
///#define LoadGame ((int(__cdecl*)(void)) 0x0043B710)
///#define SaveGame ((int(__cdecl*)(void)) 0x0043B730)
#endif