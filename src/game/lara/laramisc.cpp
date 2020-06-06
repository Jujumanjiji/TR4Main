#include "framework.h"
#include "lara.h"

void InitialiseLaraLoad(short itemNumber)
{
    Lara.itemNumber = itemNumber;
    LaraItem = &Items[itemNumber];
}

void InitialiseLaraAnims(ITEM_INFO* item)
{
    if (RWATER(item->roomNumber))
    {
        Lara.waterStatus = LWS_UNDERWATER;
        item->goalAnimState = STATE_LARA_UNDERWATER_STOP;
        item->currentAnimState = STATE_LARA_UNDERWATER_STOP;
        item->animNumber = ANIMATION_LARA_UNDERWATER_IDLE;
        item->frameNumber = Anims[item->animNumber].frameBase;
        item->fallspeed = 0;
    }
    else
    {
        Lara.waterStatus = LWS_ABOVEWATER;
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
    switch (Lara.gunType)
    {
    case LG_SHOTGUN:
        if (CHK_EXI(Lara.shotgun_type_carried, CR_PRESENT))
            Lara.backGun = SHOTGUN_ANIM;
        break;
    case LG_GRENADEGUN:
        if (CHK_EXI(Lara.grenadegun_type_carried, CR_PRESENT))
            Lara.backGun = GRENADE_GUN_ANIM;
        break;
    case LG_CROSSBOW:
        if (CHK_EXI(Lara.crossbow_type_carried, CR_PRESENT))
            Lara.backGun = CROSSBOW_ANIM;
        break;
    }

    Lara.gunStatus = LHS_ARMLESS;
    Lara.leftArm.frameNumber = 0;
    Lara.rightArm.frameNumber = 0;
    Lara.target = NULL;
    Lara.rightArm.lock = FALSE;
    Lara.leftArm.lock = FALSE;
}

#ifdef DLL_INJECT
void injector::f_game::inject_laramisc()
{
    inject(0x00430EB0, InitialiseLaraLoad);
    inject(0x00430EE0, InitialiseLaraAnims);
    inject(0x00430140, LaraInitialiseMeshes);
}
#endif