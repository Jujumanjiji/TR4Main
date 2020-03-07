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
void injector::inject_box()
{
    /*
    this->inject(0x0043FB30, InitialiseCreature);
    this->inject(0x0043FB70, CreatureActive);
    this->inject(0x0043FBE0, CreatureAIInfo);
    this->inject(0x0043FF70, UpdateLOT);
    this->inject(0x0043FFF0, SearchLOT);
    this->inject(0x004401F0, TargetBox);
    this->inject(0x00440290, EscapeBox);
    this->inject(0x00440340, ValidBox);
    this->inject(0x004403E0, GetCreatureMood);
    this->inject(0x00440620, CreatureMood);
    this->inject(0x00440940, StalkBox);
    //this->inject(0x00440A40, CalculateTarget);   // the most difficult
    this->inject(0x00440E90, CreatureCreature);
    this->inject(0x00440FD0, BadFloor);
    this->inject(0x00441080, CreatureDie);
    //this->inject(0x00441230, CreatureAnimation); // the most difficult
    this->inject(0x00441C60, CreatureTurn);
    this->inject(0x00441EE0, CreatureTilt);
    this->inject(0x00441F20, CreatureJoint);
    this->inject(0x00441F80, CreatureFloat);
    this->inject(0x00442080, CreatureUnderwater);
    this->inject(0x00442110, CreatureEffect);
    this->inject(0x00442170, CreatureEffectAlternate);
    this->inject(0x004421D0, CreatureVault);
    this->inject(0x00442380, CreatureKill);
    this->inject(0x00442550, AlertAllGuards);
    this->inject(0x004425D0, AlertNearbyGuards);
    this->inject(0x00442670, AIGuard);
    this->inject(0x00442720, GetAITarget);
    this->inject(0x00442A60, FindAITargetObject);
    this->inject(0x00442C40, SameZone);*/
}
#endif