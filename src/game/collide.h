#pragma once

extern void ShiftItem(ITEM_INFO* item, COLL_INFO* coll);
extern void UpdateLaraRoom(ITEM_INFO* item, int height);
extern BOOL TestLaraPosition(BOUNDARY* bounds, ITEM_INFO* item, ITEM_INFO* laraitem);
extern void AlignLaraPosition(PHD_VECTOR* pos, ITEM_INFO* item, ITEM_INFO* laraitem);
extern BOOL MoveLaraPosition(PHD_VECTOR* pos, ITEM_INFO* item, ITEM_INFO* laraitem);

#ifdef DLL_INJECT
#define GetCollisionInfo ((void(__cdecl*)(COLL_INFO* coll, int x, int y, int z, short roomNumber, int height)) 0x00446170)
#define GetCollidedObjects ((BOOL(__cdecl*)(ITEM_INFO *CollidingItem, int radius, int flag1, ITEM_INFO **collidedItems, MESH_INFO **collidedMeshes, int flag2)) 0x00448DA0)
#define CreatureCollision COLLISION(0x00447470)
#define ObjectCollision COLLISION(0x004475F0)
#define MovBlockCollision COLLISION(0x00448840)
#define CogCollision COLLISION(0x00448CC0)
#define LaraBaddieCollision ((void(__cdecl*)(ITEM_INFO* item, COLL_INFO* coll)) 0x004471B0)
#define Move3DPosTo3DPos ((BOOL(__cdecl*)(PHD_3DPOS* src, PHD_3DPOS* dest, int velocity, short angleAdder)) 0x00448140)
#endif