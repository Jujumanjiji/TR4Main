#include "framework.h"
#include "lara.h"

void InitialiseLaraLoad(short itemNumber)
{
    lara.item_number = itemNumber;
    LaraItem = &Items[itemNumber];
}

void InitialiseLaraAnims(ITEM_INFO* item)
{
    if (RWATER(item->roomNumber))
    {
        lara.water_status = LWS_UNDERWATER;
        item->goalAnimState = STATE_LARA_UNDERWATER_STOP;
        item->currentAnimState = STATE_LARA_UNDERWATER_STOP;
        item->animNumber = ANIMATION_LARA_UNDERWATER_IDLE;
        item->frameNumber = Anims[item->animNumber].frameBase;
        item->fallspeed = 0;
    }
    else
    {
        lara.water_status = LWS_ABOVEWATER;
        item->goalAnimState = STATE_LARA_IDLE;
        item->currentAnimState = STATE_LARA_IDLE;
        item->animNumber = ANIMATION_LARA_STAY_SOLID;
        item->frameNumber = Anims[item->animNumber].frameBase;
    }
}

void LaraInitialiseMeshes(void)
{
    ResetLaraMeshSkin();

    // NOTE: normally you have the weapon if the gun_type is set to these weapon...
    switch (lara.gun_type)
    {
    case LG_SHOTGUN:
        if (CHK_EXI(lara.shotgun_type_carried, CR_PRESENT))
            lara.back_gun = SHOTGUN_ANIM;
        break;
    case LG_GRENADEGUN:
        if (CHK_EXI(lara.grenadegun_type_carried, CR_PRESENT))
            lara.back_gun = GRENADE_GUN_ANIM;
        break;
    case LG_CROSSBOW:
        if (CHK_EXI(lara.crossbow_type_carried, CR_PRESENT))
            lara.back_gun = CROSSBOW_ANIM;
        break;
    }

    lara.gun_status = LHS_ARMLESS;
    lara.l_arm.frame_number = 0;
    lara.r_arm.frame_number = 0;
    lara.target = NULL;
    lara.r_arm.lock = FALSE;
    lara.l_arm.lock = FALSE;
}

#ifdef DLL_INJECT
void injector::f_game::inject_laramisc()
{
    inject(0x00430EB0, InitialiseLaraLoad);
    inject(0x00430EE0, InitialiseLaraAnims);
    inject(0x00430140, LaraInitialiseMeshes);
}
#endif