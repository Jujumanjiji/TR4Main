#include "framework.h"
#include "lara.h"
#include "3dsystem/3d_gen.h"
#include "lara_utils.h"
#include "camera.h"
#include "sound.h"
#include "utils/utils.h"

void lara_as_tread(ITEM_INFO* item, COLL_INFO* coll)
{
    if (item->hit_points <= 0)
    {
        item->state_next = STATE_LARA_WATER_DEATH;
        return;
    }

    if (CHK_ANY(TrInput, IN_ROLL))
    {
        SetAnimationForItem(item, ANIMATION_LARA_UNDERWATER_ROLL_BEGIN, STATE_LARA_UNDERWATER_TURNAROUND, STATE_LARA_UNDERWATER_TURNAROUND);
        return;
    }

    if (CHK_ANY(TrInput, IN_LOOK))
        LookUpDown();

    SwimTurn(item);

    if (CHK_ANY(TrInput, IN_JUMP))
        item->state_next = STATE_LARA_UNDERWATER_FORWARD;

    LARA_CLAMPN(item->fallspeed, WATER_FRICTION, 0);

    if (lara.gun_status == LHS_HANDBUSY)
        lara.gun_status = LHS_ARMLESS;
}