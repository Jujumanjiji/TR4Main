#pragma once

#define GetLaraCollisionInfo ((void(__cdecl*)(ITEM_INFO *item, COLL_INFO *coll)) 0x00422350)
#define LaraFloorFront ((short(__cdecl*)(ITEM_INFO *item, short ang, int dist)) 0x00421620)
#define LaraCeilingFront ((short(__cdecl*)(ITEM_INFO *item, short ang, int dist, int h)) 0x00421620)
#define UseSpecialItem ((BOOL(__cdecl*)(ITEM_INFO* item)) 0x00424E90)
#define TestLaraSlide ((BOOL(__cdecl*)(ITEM_INFO* item, COLL_INFO* coll)) 0x00420EA0)
#define TestLaraVault ((BOOL(__cdecl*)(ITEM_INFO* item, COLL_INFO* coll)) 0x00422480)
#define SwimTurn ((void(__cdecl*)(ITEM_INFO* item)) 0x00432690)
#define MonkeySwingFall ((void(__cdecl*)(ITEM_INFO* item)) 0x00422E50)
#define FallFromRope ((void(__cdecl*)(ITEM_INFO* item)) 0x004241D0)
#define SetCornerAnim ((void(__cdecl*)(ITEM_INFO* item, COLL_INFO* coll, short angle, short flip)) 0x00426C50)
#define AnimateLara ((void(__cdecl*)(ITEM_INFO* item)) 0x00430B60)