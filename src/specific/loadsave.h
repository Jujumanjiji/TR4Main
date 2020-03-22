#pragma once

//extern int InitSaveGame(BYTE level_id, int type); // like gfCurrentLevel variable type !
//extern int LoadSaveThread(int key_pressed, int skipMonoScreen);
//extern void sgSaveGame(void);
//extern void sgRestoreGame(void);
//extern int LoadGameInfo(void);
//extern int SaveGameInfo(void);
extern void WriteSG(char* pointer, int size);
extern void ReadSG(char* pointer, int size);

#ifdef DLL_INJECT
#define savegame_current_level VAR_U_(0x007F7850, char)
#define savegame_found ARRAY_(0x007F781E, BYTE, [10])
#define SGcount VAR_I_(0x004BF6D8, int, 0)
#define SGpoint VAR_I_(0x004BF6D4, char*, nullptr)
#define MGSaveGamePtr VAR_U_(0x007F7889, char*)

#define InitSaveGame ((int(__cdecl*)(BYTE level_id, int type)) 0x0045A370)
#define FoundLoadSaveCount ((void(__cdecl*)(void)) 0x0047A6F0)
#define LoadSaveThread ((int(__cdecl*)(int key_pressed, int skipMonoScreen)) 0x0047CD20)
#define do_loadsave_mode ((int(__cdecl*)(int key_pressed)) 0x0047A880) // return the current selected slots
#define sgSaveGame ((void(__cdecl*)(void)) 0x0045A2E0)
#define sgRestoreGame ((void(__cdecl*)(void)) 0x0045B040)
#define LoadGameInfo ((int(__cdecl*)(void)) 0x0043B710)
#define SaveGameInfo ((int(__cdecl*)(void)) 0x0043B730)
#define SaveLevelData ((int(__cdecl*)(void)) 0x0045A4B0)
#define RestoreLevelData ((void(__cdecl*)(BOOL fullsave)) 0x0045B230)
#define RestoreLaraData ((void(__cdecl*)(BOOL ignoreItemNumber)) 0x0045B080)
#define WriteSaveGame ((void(__cdecl*)(int savegame_id)) 0x00477430)
#define ReadSaveGame ((void(__cdecl*)(int savegame_id)) 0x00477600)
///#define WriteSG ((void(__cdecl*)(char* pointer, int size)) 0x0045B000)
///#define ReadSG ((void(__cdecl*)(char* pointer, int size)) 0x0045BD80)
#endif