#pragma once

/*
extern void InitialiseCreature(short itemNumber);
extern BOOL CreatureActive(short itemNumber);
extern int UpdateLOT(LOT_INFO* LOT, int expansion);
extern int SearchLOT(LOT_INFO* LOT, int expansion);
extern void CreatureAIInfo(ITEM_INFO* item, AI_INFO* info);
extern void TargetBox(LOT_INFO* LOT, short box_number);
extern BOOL EscapeBox(ITEM_INFO *item, ITEM_INFO *enemy, short box_number);
extern BOOL ValidBox(ITEM_INFO* item, short zone_number, short box_number);
extern void GetCreatureMood(ITEM_INFO* item, AI_INFO* info, int violent);
extern void CreatureMood(ITEM_INFO* item, AI_INFO* info, int violent);
extern BOOL StalkBox(ITEM_INFO* item, ITEM_INFO* enemy, short box_number);
extern TARGET_TYPE CalculateTarget(PHD_VECTOR* target, ITEM_INFO* item, LOT_INFO* LOT);
extern int CreatureCreature(short itemNumber);
extern BOOL BadFloor(int x, int y, int z, int box_height, int next_height, short roomNumber, LOT_INFO* LOT);
extern void CreatureDie(short itemNumber, BOOL explode);
extern int CreatureAnimation(short itemNumber, short angle, short tilt);
extern short CreatureTurn(ITEM_INFO* item, short maximum_turn);
extern void CreatureTilt(ITEM_INFO* item, short angle);
extern void CreatureJoint(ITEM_INFO* item, short joint, short required);
extern void CreatureFloat(short itemNumber);
extern void CreatureUnderwater(ITEM_INFO* item, int depth);
extern short CreatureEffect(ITEM_INFO* item, BITE_INFO* bite, lpEffect effect);
extern short CreatureEffectAlternate(ITEM_INFO* item, BITE_INFO* bite, short speed, short y_rot, lpEffect effect);
extern int CreatureVault(short itemNumber, short angle, int vault, int shift);
extern void CreatureKill(ITEM_INFO* item, int kill_anim, int kill_state, int kill_lara_anim);
extern void AlertAllGuards(short itemNumber);
extern void AlertNearbyGuards(ITEM_INFO* item);
extern short AIGuard(CREATURE_INFO* creature);
extern void GetAITarget(CREATURE_INFO* creature);
extern void FindAITargetObject(CREATURE_INFO* creature, short objectNumber);
extern BOOL SameZone(CREATURE_INFO *creature, ITEM_INFO *target_item);
*/

#ifdef DLL_INJECT
#define InitialiseCreature ((void(__cdecl*)(short itemNumber)) 0x0043FB30)
#define CreatureActive ((BOOL(__cdecl*)(short itemNumber)) 0x0043FB70)
#define CreatureAIInfo ((void(__cdecl*)(ITEM_INFO* item, AI_INFO* info)) 0x0043FBE0)
#define UpdateLOT ((int(__cdecl*)(LOT_INFO* LOT, int expansion)) 0x0043FF70)
#define SearchLOT ((int(__cdecl*)(LOT_INFO* LOT, int expansion)) 0x0043FFF0)
#define TargetBox ((void(__cdecl*)(LOT_INFO* LOT, short box_number)) 0x004401F0)
#define EscapeBox ((BOOL(__cdecl*)(ITEM_INFO* item, ITEM_INFO* enemy, short box_number)) 0x00440290)
#define ValidBox ((BOOL(__cdecl*)(ITEM_INFO* item, short zone_number, short box_number)) 0x00440340)
#define GetCreatureMood ((void(__cdecl*)(ITEM_INFO* item, AI_INFO* info, int violent)) 0x004403E0)
#define CreatureMood ((void(__cdecl*)(ITEM_INFO* item, AI_INFO* info, int violent)) 0x00440620)
#define StalkBox ((BOOL(__cdecl*)(ITEM_INFO* item, ITEM_INFO* enemy, short box_number)) 0x00440940)
#define CalculateTarget ((TARGET_TYPE(__cdecl*)(PHD_VECTOR* pos, ITEM_INFO* item, LOT_INFO* LOT)) 0x00440A40)
#define CreatureCreature ((int(__cdecl*)(short itemNumber)) 0x00440E90)
#define BadFloor ((BOOL(__cdecl*)(int x, int y, int z, int box_height, int next_height, short roomNumber, LOT_INFO* LOT)) 0x00440FD0)
#define CreatureDie ((void(__cdecl*)(short itemNumber, BOOL explode)) 0x00441080)
#define CreatureAnimation ((int(__cdecl*)(short itemNumber, short angle, short tilt)) 0x00441230)
#define CreatureTurn ((short(__cdecl*)(ITEM_INFO* item, short maximumTurn)) 0x00441C60)
#define CreatureTilt ((void(__cdecl*)(ITEM_INFO* item, short angle)) 0x00441EE0)
#define CreatureJoint ((void(__cdecl*)(ITEM_INFO* item, short joint, short required)) 0x00441F20)
#define CreatureFloat ((void(__cdecl*)(short itemNumber)) 0x00441F80)
#define CreatureUnderwater ((void(__cdecl*)(ITEM_INFO* item, int depth)) 0x00442080)
#define CreatureEffect ((short(__cdecl*)(ITEM_INFO* item, BITE_INFO* bite, short(*generate)(int x, int y, int z, short speed, short y_rot, short roomNumber))) 0x00442110)
#define CreatureEffect2 ((short(__cdecl*)(ITEM_INFO* item, BITE_INFO* bite, short speed, short yrot, short(*generate)(int x, int y, int z, short speed, short y_rot, short roomNumber))) 0x00442170)
#define CreatureVault ((int(__cdecl*)(short itemNumber, short angle, int vault, int shift)) 0x004421D0)
#define CreatureKill ((void(__cdecl*)(ITEM_INFO* item, short kill_anim, short kill_state, short kill_lara_state)) 0x00442380)
#define AlertAllGuards ((void(__cdecl*)(short itemNumber)) 0x00442550)
#define AlertNearbyGuards ((void(__cdecl*)(ITEM_INFO* item)) 0x004425D0)
#define AIGuard ((int(__cdecl*)(CREATURE_INFO* creature)) 0x00442670)
#define GetAITarget ((void(__cdecl*)(CREATURE_INFO* creature)) 0x00442720)
#define FindAITargetObject ((void(__cdecl*)(CREATURE_INFO* creature, short objectNumber)) 0x00442A60)
#define SameZone ((BOOL(__cdecl*)(CREATURE_INFO* creature, ITEM_INFO* target_item)) 0x00442C40)
#define ExplodingDeath ((void(__cdecl*)(short itemNumber, int mesh_bits, short flags)) 0x0043A690) // in missile.cpp
#endif