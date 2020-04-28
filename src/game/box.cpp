#include "framework.h"
#include "box.h"
#include "3d_gen.h"
#include "control.h"
#include "draw.h"
#include "items.h"
#include "lot.h"
#include "sphere.h"
#include "utils.h"

#define ATTACK_RANGE SQUARE(WALL_L * 3)
#define BORED_DIST (WALL_L * 6)
#define STALK_DIST (WALL_L * 3)
#define ESCAPE_DIST (WALL_L * 5)
#define ESCAPE_CHANCE 0x800
#define RECOVER_CHANCE 0x100
#define MAX_X_ROT ANGLE(20)
#define GUNFIRE_ALERT_RADIUS 8000

#ifdef DLL_INJECT
void injector::f_game::inject_box()
{
    /*
    inject(0x0043FB30, InitialiseCreature);
    inject(0x0043FB70, CreatureActive);
    inject(0x0043FBE0, CreatureAIInfo);
    inject(0x0043FF70, UpdateLOT);
    inject(0x0043FFF0, SearchLOT);
    inject(0x004401F0, TargetBox);
    inject(0x00440290, EscapeBox);
    inject(0x00440340, ValidBox);
    inject(0x004403E0, GetCreatureMood);
    inject(0x00440620, CreatureMood);
    inject(0x00440940, StalkBox);
    //inject(0x00440A40, CalculateTarget);   // the most difficult
    inject(0x00440E90, CreatureCreature);
    inject(0x00440FD0, BadFloor);
    inject(0x00441080, CreatureDie);
    //inject(0x00441230, CreatureAnimation); // the most difficult
    inject(0x00441C60, CreatureTurn);
    inject(0x00441EE0, CreatureTilt);
    inject(0x00441F20, CreatureJoint);
    inject(0x00441F80, CreatureFloat);
    inject(0x00442080, CreatureUnderwater);
    inject(0x00442110, CreatureEffect);
    inject(0x00442170, CreatureEffectAlternate);
    inject(0x004421D0, CreatureVault);
    inject(0x00442380, CreatureKill);
    inject(0x00442550, AlertAllGuards);
    inject(0x004425D0, AlertNearbyGuards);
    inject(0x00442670, AIGuard);
    inject(0x00442720, GetAITarget);
    inject(0x00442A60, FindAITargetObject);
    inject(0x00442C40, SameZone);
    */
}

#endif
