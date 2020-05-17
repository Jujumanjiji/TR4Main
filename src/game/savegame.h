#pragma once

#define MAX_SAVEGAME 14

#ifdef DLL_INJECT
#define savegame_count          ARRAY_(0x00536F74, SAVEGAME_COUNT, [MAX_SAVEGAME])
#define num_savecount           VAR_U_(0x005355B4, int)
#define num_save                VAR_U_(0x00536F20, int)

#define InitSaveGame ((int(__cdecl*)(BYTE level_id, int type)) 0x0045A370)
#define FoundSaveGameInfo ((void(__cdecl*)(void)) 0x0047A6F0)
#define LoadSaveThread ((int(__cdecl*)(int key_pressed, int skipMonoScreen)) 0x0047CD20)
#define do_loadsave_mode ((int(__cdecl*)(int key_pressed)) 0x0047A880) // return the current selected slots
#define sgSaveGame ((void(__cdecl*)(void)) 0x0045A2E0)
#define sgRestoreGame ((void(__cdecl*)(void)) 0x0045B040)
#define LoadGameInfo ((int(__cdecl*)(void)) 0x0043B710)
#define SaveGameInfo ((int(__cdecl*)(void)) 0x0043B730)
#define SaveLevelData ((int(__cdecl*)(void)) 0x0045A4B0)
#define SaveGame ((void(__cdecl*)(void)) 0x0045BDC0)
#define LoadGame ((void(__cdecl*)(void)) 0x0045BDF0)
#define RestoreLevelData ((void(__cdecl*)(BOOL fullsave)) 0x0045B230)
#define RestoreLaraData ((void(__cdecl*)(BOOL ignoreItemNumber)) 0x0045B080)
#define WriteSaveGame ((void(__cdecl*)(int savegame_id)) 0x00477430)
#define ReadSaveGame ((void(__cdecl*)(int savegame_id)) 0x00477600)
#define WriteSG ((void(__cdecl*)(char* pointer, int size)) 0x0045B000)
#define ReadSG ((void(__cdecl*)(char* pointer, int size)) 0x0045BD80)
#define CheckLaraData ((BOOL(__cdecl*)(void* data)) 0x0045A0E0)
#endif