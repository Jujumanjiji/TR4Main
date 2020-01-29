#pragma once

extern bool isFolderExists(LPCSTR folder_name);
extern bool isFileExists(LPCSTR file_name);
extern void createFolders(LPCSTR folder_name);
extern void phd_SwapMatrix(int* dest, int* src);
extern void phd_SwapGunMatrix(PHD_MATRIX& dest, int* src, bool swap);
extern void SpawnPickup(ITEM_INFO* item);
extern void SetGunFlash_Left(int weapon_type);
extern void SetGunFlash_Right(int weapon_type);
extern short initHealth(short objNumber);
