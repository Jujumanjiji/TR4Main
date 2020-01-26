#pragma once

extern void phd_SwapMatrix(int* dest, int* src);
extern void phd_SwapGunMatrix(PHD_MATRIX& dest, int* src, bool swap);
extern void SpawnPickup(ITEM_INFO* item);

extern void SetGunFlash_Left(int weapon_type);
extern void SetGunFlash_Right(int weapon_type);

extern short init_health(short objNumber);