#pragma once

extern bool isFolderExists(LPCSTR folder_name);
extern bool isFileExists(LPCSTR file_name);
extern void createFolders(LPCSTR folder_name);
extern short GetCurrentFrame(ITEM_INFO* item);
extern void phd_SwapMatrix(int* dest, int* src);
extern void phd_SwapGunMatrix(PHD_MATRIX& dest, int* src, bool swap);
extern void SpawnPickup(ITEM_INFO* item);
extern void SetGunFlash_Left(int weapon_type);
extern void SetGunFlash_Right(int weapon_type);
extern ITEM_INFO* FoundTarget(short itemNumber, ITEM_INFO* src, CREATURE_INFO* creature, short objectToTarget);
extern short initHealth(short objNumber);
extern void DrawFlashWithSmoke(ITEM_INFO* item, BITE_INFO* bite);
extern ENTITY_JUMP CheckJumpPossibility(ITEM_INFO* item, CREATURE_INFO* creature);

/// new:
extern void phd_SwapPushMatrix(int frac);
extern void phd_SwapPopMatrix(int frac);
extern void phd_SwapTranslateRel(int frac, int bone1, int bone2, int bone3, short* frame1, short* frame2, bool start);
extern void phd_SwapGarYXZsuperpack(int frac, short** rotation1, short** rotation2);
extern void phd_SwapRotY(int frac, short y);
extern void phd_SwapRotX(int frac, short x);
extern void phd_SwapRotZ(int frac, short z);
extern void phd_SwapRotYXZ(int frac, short y, short x, short z);
extern void phd_SwapPutPolygons(int frac, int clip, short** mesh);