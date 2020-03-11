#pragma once

extern LPCSTR lpWeaponName[12];

class WEAPON_AMMO
{
public:
    short get(int weapon_type);
    void increase(int weapon_type, int value);
    void decrease(int weapon_type, int value);
};
extern WEAPON_AMMO weapon_ammo;

extern void dummy_func(ITEM_INFO* item, COLL_INFO* coll); // for lara control & collision routines !
extern bool isFolderExists(LPCSTR folderName);
extern bool isFileExists(LPCSTR fileName);
extern void createFolders(LPCSTR folderName);
extern void S_Unimplemented(LPCSTR LT_Function_Flags);
extern void S_LogValue(LPCSTR content, ...);
extern void S_Log(LPCSTR content, ...);
extern void S_Log(LPCSTR content, bool isEntered, ...);
extern void S_Log(LPCSTR funcName, LPCSTR content, bool isEntered, ...);
extern LPCSTR WriteWorldItemAngle(ITEM_INFO* item);
extern void SetAnimationForItemAS(ITEM_INFO* item, int animation, int state);
extern void SetAnimationForItemASN(ITEM_INFO* item, int animation, int state, bool needStateNext);
extern void SetAnimationForItemASF(ITEM_INFO* item, int animation, int state, int frameNow);
extern void SetAnimationForItemASS(ITEM_INFO* item, int animation, int state_current, int state_next);
extern void SetAnimationForItemASSF(ITEM_INFO* item, int animation, int state_current, int state_next, int frameNow);
extern short GetCurrentFrame(ITEM_INFO* item);
extern void TriggerDynamicSwap(int x, int y, int z, int intensity, BYTE red, BYTE green, BYTE blue);
extern void phd_SwapMatrix(int* dest, int* src);
extern void phd_SwapGunMatrix(PHD_MATRIX& dest, int* src, bool swap);
extern void SpawnPickup(ITEM_INFO* item);
extern void SetGunFlash_Left(int weapon_type);
extern void SetGunFlash_Right(int weapon_type);
extern PHD_VECTOR GetGunFlashPosition(int weapon_type, bool right);
extern void set_gun_smoke_left(int weapon_type);
extern void set_gun_smoke_right(int weapon_type);
extern ITEM_INFO* FoundTarget(short itemNumber, ITEM_INFO* src, CREATURE_INFO* creature, short objectToTarget);
extern short initHealth(short objNumber);
extern void DrawFlashWithSmoke(ITEM_INFO* item, BITE_INFO* bite);
extern ENTITY_JUMP CheckMegaJumpPossibility(ITEM_INFO* item, CREATURE_INFO* creature);
extern bool CheckRollPossibility(ITEM_INFO* item);
extern bool CheckJumpPossibility(ITEM_INFO* item);
extern OBJECT_FOUND FoundItem(ITEM_INFO* src, CREATURE_INFO* creature, short slotID, short slotID2);
extern OBJECT_FOUND FoundEntityWithOCB(ITEM_INFO* item, short slotID, short ocb);
extern bool AlignItemToTarget(ITEM_INFO* src, ITEM_INFO* target);
extern void ActivateEntity(short itemNumber);
extern bool FoundEntityAndActivate(ITEM_INFO* item, short slotid, short ocb);
extern void Spawner(ITEM_INFO* item);
extern int CalculateLaraDistance(ITEM_INFO* item);
extern int CalculateItemDistanceToTarget(ITEM_INFO* src, ITEM_INFO* target);
extern short* classic_meshes(short objNumber, short meshID);
extern void TestTriggersCollision(ITEM_INFO* item, COLL_INFO* coll);
extern short GetCatchAngle(ITEM_INFO* item, short angleToCheck); // for lara grapping angle.
extern void LaraSlideAngle(ITEM_INFO* item, COLL_INFO* coll, short adif, short angle);

/// new:
extern void phd_SwapPushMatrix(int frac);
extern void phd_SwapPopMatrix(int frac);
extern void phd_SwapTranslateRel(int frac, short* frame1, short* frame2);
extern void phd_SwapTranslateRel(int frac, BONE bone);
extern void phd_SwapGarYXZsuperpack(int frac, short** rotation1, short** rotation2);
extern void phd_SwapRotY(int frac, short y);
extern void phd_SwapRotX(int frac, short x);
extern void phd_SwapRotZ(int frac, short z);
extern void phd_SwapRotYXZ(int frac, short y, short x, short z);
extern void phd_SwapPutPolygons(int frac, int clip, short** mesh);