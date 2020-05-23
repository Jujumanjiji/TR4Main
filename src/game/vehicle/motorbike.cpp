#include "framework.h"
#include "motorbike.h"
#include "init.h"
#include "lara.h"
#include "control.h"
#include "collide.h"
#include "3d_gen.h"
#include "inventory_2d.h"
#include "sphere.h"
#include "effect2.h"
#include "box.h"
#include "items.h"
#include "sound.h"
#include "camera.h"
#include "drawprimitive.h"

#define MotorBikeItem VAR_U_(0x007F504C, ITEM_INFO*)

#pragma pack(push, 1)
struct MOTORBIKE_INFO
{
    int wheelRight;  // (two wheel: front and back)
    int wheelLeft;   // (one wheel: left)
    int velocity;
    int revs;
    int engineRevs;
    short momentumAngle;
    short extraRotation;
    short wallShiftRotation;
    int bikeTurn;
    int pitch;
    short flags;
    short lightPower;
};
#pragma pack(pop)

enum MOTORBIKE_STATE
{
    BIKE_EMPTY,
    BIKE_MOVING_FRONT,
    BIKE_MOVING_LEFT,
    BIKE_MOVING_BACK,
    BIKE_MOVING_BACK_LOOP,
    BIKE_EMPTY3,
    BIKE_STOP,
    BIKE_DEATH,
    BIKE_FALLING,
    BIKE_ENTER, // include unlocking state
    BIKE_EXIT,
    BIKE_HITFRONT,
    BIKE_HITBACK,
    BIKE_HITRIGHT,
    BIKE_HITLEFT,
    BIKE_IDLE,
    BIKE_LOADING_BOOST, // not used !
    BIKE_LANDING,
    BIKE_ACCELERATE,
    BIKE_EMPTY5,
    BIKE_EMPTY6,
    BIKE_NOTUSED,
    BIKE_MOVING_RIGHT
};


#define BIKE_IDLE_ANIM 18
#define BIKE_ENTER_ANIM 9
#define BIKE_UNLOCK_ANIM 28

#define BIKE_DEATH_FLAG 0x80

#define BIKE_FRONT 500
#define BIKE_SIDE 350
#define BIKE_RADIUS 500

#define MIN_MOMENTUM_TURN ANGLE(4)
#define MAX_MOMENTUM_TURN (ANGLE(1)+(ANGLE(1)/2))
#define MOTORBIKE_MAX_MOM_TURN ANGLE(150)
#define MOTORBIKE_DEFAULT_HTURN 273
#define MOTORBIKE_ACCEL_1 0x4000
#define MOTORBIKE_ACCEL_2 0x7000
#define MOTORBIKE_ACCEL_MAX 0xC000
#define MOTORBIKE_SLIP 100
#define MOTORBIKE_SLIP_SIDE 50
#define MOTORBIKE_HTURN (ANGLE(1) / 2)
#define MOTORBIKE_MAX_HTURN ANGLE(5)
#define MOTORBIKE_FRICTION 0x180

static MOTORBIKE_INFO* GetMotorbikeInfo(ITEM_INFO* item)
{
    return (MOTORBIKE_INFO*)item->data;
}

void InitialiseMotorBike(short itemNumber) // (F) (D)
{
    ITEM_INFO* item;
    MOTORBIKE_INFO* motorbike;

    item = &Items[itemNumber];
    motorbike = (MOTORBIKE_INFO*)game_malloc(sizeof(MOTORBIKE_INFO));
    item->data = (void*)motorbike;
    motorbike->velocity = 0;
    motorbike->bikeTurn = 0;
    motorbike->pitch = 0;
    motorbike->momentumAngle = item->pos.yRot;
    motorbike->wallShiftRotation = 0;
    motorbike->extraRotation = 0;
    motorbike->flags = NULL;
    motorbike->lightPower = 0;
    motorbike->wheelLeft = 0; // left wheel
    motorbike->wheelRight = 0; // two wheel in the principal body
    item->meshBits = 0x3F7;
    MotorBikeItem = item;
}

void DrawMotorbikeLight(ITEM_INFO* item) // (F) (D)
{
    MOTORBIKE_INFO* motorbike;
    PHD_VECTOR start, target;
    int rnd;

    motorbike = GetMotorbikeInfo(item);
    start.x = 0;
    start.y = -470;
    start.z = 1836;
    GetJointAbsPosition(item, &start, 0);
    target.x = 0;
    target.y = -470;
    target.z = 20780;
    GetJointAbsPosition(item, &target, 0);
    rnd = (2 * motorbike->lightPower) - (GetRandomControl() & 0xF);
    if (rnd <= 0)
        SpotLightEnabled = FALSE;
    else
        CreateSpotLight(&start, &target, item->pos.yRot, rnd);
}

BOOL GetOnMotorBike(short itemNumber) // (F) (D)
{
    ITEM_INFO* item;
    FLOOR_INFO* floor;
    int dx, dz, distance, height;
    unsigned short tempangle;
    short angle;
    short roomNumber;

    item = &Items[itemNumber];
    if (item->flags & IFLAG_ONESHOT || lara.gun_status == LHS_HANDBUSY || LaraItem->gravityStatus)
        return FALSE;

    if ((abs(item->pos.yPos - LaraItem->pos.yPos) >= 256 || !(TrInput & IN_ACTION)) && GLOBAL_invitemchosen != PUZZLE_ITEM1)
        return FALSE;

    dx = LaraItem->pos.xPos - item->pos.xPos;
    dz = LaraItem->pos.zPos - item->pos.zPos;
    distance = SQUARE(dx) + SQUARE(dz);
    if (distance > 170000)
        return FALSE;

    roomNumber = item->roomNumber;
    floor = GetFloor(item->pos.xPos, item->pos.yPos, item->pos.zPos, &roomNumber);
    height = GetHeight(floor, item->pos.xPos, item->pos.yPos, item->pos.zPos);
    if (height < -32000)
        return FALSE;

    angle = phd_atan(item->pos.zPos - LaraItem->pos.zPos, item->pos.xPos - LaraItem->pos.xPos) - item->pos.yRot;
    tempangle = angle - item->pos.yRot;
    if (angle > -ANGLE(45) && angle < ANGLE(135))
    {
        // left
        if (tempangle > -ANGLE(45) && angle < ANGLE(135))
            return FALSE;
    }
    else
    {
        // right
        if (tempangle > ANGLE(225) && tempangle < ANGLE(315))
            return FALSE;
    }
    return TRUE;
}

void MotorBikeCollision(short itemNumber, ITEM_INFO* laraitem, COLL_INFO* coll) // (F) (D)
{
    ITEM_INFO* item;
    MOTORBIKE_INFO* motorbike;

    if (laraitem->hitPoints >= 0 && lara.skidoo == NO_ITEM)
    {
        item = &Items[itemNumber];
        motorbike = GetMotorbikeInfo(item);
        
        // update motorbike light
        if (motorbike->bikeTurn)
        {
            motorbike->bikeTurn -= (motorbike->bikeTurn >> 3) - 1;
            DrawMotorbikeLight(item);
        }

        if (GetOnMotorBike(itemNumber))
        {
            lara.skidoo = itemNumber;

            if (lara.gun_type == LG_FLARE)
            {
                CreateFlare(FLARE_ITEM, FALSE);
                undraw_flare_meshes();
                lara.flare_control_left = FALSE;
                lara.gun_type = LG_UNARMED;
                lara.gun_request_type = LG_UNARMED;
                lara.flare_age = 0;
            }

            lara.gun_status = LHS_HANDBUSY;

            short angle = phd_atan(item->pos.zPos - laraitem->pos.zPos, item->pos.xPos - laraitem->pos.xPos) - item->pos.yRot;
            if (angle <= -8190 || angle >= 24570)
            {
                if (GLOBAL_invitemchosen == PUZZLE_ITEM1)
                {
                    laraitem->animNumber = Objects[VEHICLE_EXTRA].animIndex + BIKE_UNLOCK_ANIM;
                    GLOBAL_invitemchosen = -1;
                }
                else
                {
                    laraitem->animNumber = Objects[VEHICLE_EXTRA].animIndex + BIKE_ENTER_ANIM;
                }
                laraitem->goalAnimState = BIKE_ENTER_ANIM;
                laraitem->currentAnimState = BIKE_ENTER_ANIM;
            }
            laraitem->frameNumber = Anims[laraitem->animNumber].frameBase;

            item->hitPoints = 1;
            laraitem->pos.xPos = item->pos.xPos;
            laraitem->pos.yPos = item->pos.yPos;
            laraitem->pos.zPos = item->pos.zPos;
            laraitem->pos.yRot = item->pos.yRot;
            lara.head_y_rot = 0;
            lara.head_x_rot = 0;
            lara.torso_y_rot = 0;
            lara.torso_x_rot = 0;
            lara.hit_direction = -1;
            AnimateItem(laraitem);
            motorbike->revs = 0;
            item->collidable = TRUE;
        }
        else
        {
            ObjectCollision(itemNumber, laraitem, coll);
        }
    }
}

void TriggerMotorbikeExhaustSmoke(int x, int y, int z, short angle, short speed, BOOL moving) // (F) (D)
{
    SPARKS* sptr;
    int rnd = 0;
    BYTE trans, size;

    sptr = &sparks[GetFreeSparks()];
    sptr->dR = 96;
    sptr->dG = 96;
    sptr->on = 1;
    sptr->sR = 0;
    sptr->sG = 0;
    sptr->sB = 0;
    sptr->dB = 128;
    if (moving)
    {
        trans = (speed << 7) >> 5;
        sptr->dR = trans;
        sptr->dG = trans;
        sptr->dB = trans;
    }
    sptr->colfadespeed = 4;
    sptr->fadetoblack = 4;
    rnd = (GetRandomControl() & 3) - (speed >> 12) + 20;
    if (rnd < 9)
    {
        sptr->life = 9;
        sptr->sLife = 9;
    }
    else
    {
        sptr->life = rnd;
        sptr->sLife = rnd;
    }

    sptr->transtype = TS_COLADD;
    sptr->x = x + (GetRandomControl() & 0xF) - 8;
    sptr->y = y + (GetRandomControl() & 0xF) - 8;
    sptr->z = z + (GetRandomControl() & 0xF) - 8;
    sptr->xvel = (speed * SIN(angle)) >> 16;
    sptr->yvel = (GetRandomControl() & 7) - 8;
    sptr->zvel = (speed * COS(angle)) >> 16;
    sptr->friction = 4;

    if (GetRandomControl() & 1)
    {
        sptr->flags = SPF_EXPDEF | SPF_ROTATE | SPF_DEF | SPF_SCALE;
        sptr->rotang = (GetRandomControl() & 0xFFF);
        if (GetRandomControl() & 1)
            sptr->rotadd = (GetRandomControl() & 7) - 24;
        else
            sptr->rotadd = (GetRandomControl() & 7) + 24;
    }
    else
    {
        sptr->flags = SPF_EXPDEF | SPF_DEF | SPF_SCALE;
    }

    sptr->scalar = 1;
    sptr->def = (unsigned char)Objects[DEFAULT_SPRITES].meshIndex;
    sptr->gravity = (GetRandomControl() & 3) - 4;
    sptr->maxyvel = (GetRandomControl() & 7) - 8;
    size = (GetRandomControl() & 7) + (speed >> 7) + 32;
    sptr->dSize = size;
    sptr->sSize = size >> 1;
    sptr->size = size >> 1;
}

void DrawMotorBikeSmoke(ITEM_INFO* item) // (F) (D)
{
    if (lara.skidoo == NO_ITEM)
        return;

    if (LaraItem->currentAnimState != BIKE_ENTER && LaraItem->currentAnimState != BIKE_EXIT)
    {
        PHD_VECTOR pos;
        int speed = 0;

        pos.x = 56;
        pos.y = -144;
        pos.z = -500;
        GetJointAbsPosition(item, &pos, 0);

        speed = item->speed;
        if (speed > 32 && speed < 64)
        {
            TriggerMotorbikeExhaustSmoke(pos.x, pos.y, pos.z, item->pos.yRot - ANGLE(180), 64 - speed, TRUE);
            return;
        }

        if (ExhaustStart >= 16)
        {
            if (GetRandomControl() & 3)
                speed = 0;
            else
                speed = ((GetRandomControl() & 0xF) + (GetRandomControl() & 0x10)) << 6;
        }
        else
        {
            speed = ((GetRandomControl() & 0xF) + (GetRandomControl() & 0x10) + 2 * ExhaustStart) << 6;
        }

        TriggerMotorbikeExhaustSmoke(pos.x, pos.y, pos.z, item->pos.yRot - ANGLE(180), speed, FALSE);
    }
}

void DrawMotorBikeEffect(ITEM_INFO* item) // (F) (D)
{
    MOTORBIKE_INFO* motorbike = GetMotorbikeInfo(item);
    if (lara.skidoo != NO_ITEM)
        DrawMotorBikeSpeedoMeter(phd_winwidth - 64, phd_winheight - 16, motorbike->velocity, ANGLE(180), ANGLE(270), 32); // angle are 2D angle...
    DrawMotorBikeSmoke(item);
}

// now lara explode with the vehicle !
void MotorBikeExplode(ITEM_INFO* item) // (F) (D)
{
    if (Rooms[item->roomNumber].flags & 1)
    {
        TriggerUnderwaterExplosion(item, TRUE);
    }
    else
    {
        TriggerExplosionSparks(item->pos.xPos, item->pos.yPos, item->pos.zPos, 3, -2, 0, item->roomNumber);
        for (int i = 0; i < 2; i++)
            TriggerExplosionSparks(item->pos.xPos, item->pos.yPos, item->pos.zPos, 3, -1, 0, item->roomNumber);
    }

    ExplodingDeath(lara.skidoo, -2, 256);
    ExplodingDeath(lara.itemNumber, -2, 258); // enable blood
    LaraItem->hitPoints = 0;
    KillItem(lara.skidoo);
    item->status = FITEM_INVISIBLE;
    SoundEffect(SFX_EXPLOSION1, nullptr, NULL);
    SoundEffect(SFX_EXPLOSION2, nullptr, NULL);
    lara.skidoo = NO_ITEM;
}

int MotorBikeCheckGetOff(void)
{
    ITEM_INFO* item;

    item = &Items[lara.skidoo];
    if (LaraItem->currentAnimState == BIKE_EXIT && LaraItem->frameNumber == Anims[LaraItem->animNumber].frameEnd)
    {
        LaraItem->pos.yRot -= 0x4000;
        LaraItem->animNumber = ANIMATION_LARA_STAY_SOLID;
        LaraItem->frameNumber = Anims[LaraItem->animNumber].frameBase;
        LaraItem->goalAnimState = STATE_LARA_IDLE;
        LaraItem->currentAnimState = STATE_LARA_IDLE;
        LaraItem->pos.xPos -= 2 * SIN(item->pos.yRot) >> W2V_SHIFT;
        LaraItem->pos.zPos -= 2 * COS(item->pos.yRot) >> W2V_SHIFT;
        LaraItem->pos.xRot = 0;
        LaraItem->pos.zRot = 0;
        lara.skidoo = NO_ITEM;
        lara.gun_status = LHS_ARMLESS;
        lara.dash_timer = 120;
        return TRUE;
    }

    if (LaraItem->frameNumber != Anims[LaraItem->animNumber].frameEnd)
        return TRUE;

    // exit when falling
    // if (lara_item->state_current == BIKE_EMPTY6) {

    // }
    // else if (lara_item->state_current == BIKE_EMPTY5) {
    // lara death when falling too much
    // }

    return FALSE;
}

int TestMotorbikeHeight(ITEM_INFO* item, int dz, int dx, PHD_VECTOR* pos) // (F) (D)
{
    FLOOR_INFO* floor;
    int c, s, ceiling, height;
    short roomNumber;

    pos->y = item->pos.yPos - ((dx * SIN(item->pos.zRot) >> W2V_SHIFT) + (dz * SIN(item->pos.xRot)) >> W2V_SHIFT);
    c = COS(item->pos.yRot);
    s = SIN(item->pos.yRot);
    pos->z = item->pos.zPos + ((dz * c - dx * s) >> W2V_SHIFT);
    pos->x = item->pos.xPos + ((dz * s + dx * c) >> W2V_SHIFT);

    roomNumber = item->roomNumber;
    floor = GetFloor(pos->x, pos->y, pos->z, &roomNumber);
    ceiling = GetCeiling(floor, pos->x, pos->y, pos->z);

    if (pos->y < ceiling || ceiling == -NO_HEIGHT)
        return -NO_HEIGHT;
    height = GetHeight(floor, pos->x, pos->y, pos->z);
    if (pos->y > height)
        pos->y = height;
    return height;
}

int DoMotorBikeDynamics(int height, int fallspeed, int* y, int flags) // (F) (D)
{
    int kick;

    if (height <= *y)
    {
        if (flags)
        {
            return fallspeed;
        }
        else
        {
            // On ground
            kick = (height - *y);

            if (kick < -80)
                kick = -80;

            fallspeed += ((kick - fallspeed) >> 4);

            if (*y > height)
                *y = height;
        }
    }
    else
    {
        // In air
        *y += fallspeed;
        if (*y > height - 32)
        {
            *y = height;
            fallspeed = 0;
        }
        else
        {
            if (flags)
                fallspeed += flags;
            else
                fallspeed += GRAVITY;
        }
    }

    return fallspeed;
}

int MotorBikeDynamics(ITEM_INFO* item)
{
    MOTORBIKE_INFO* motorbike;
    PHD_VECTOR bl_old, mtb_old, br_old, mtf_old, fl_old;
    PHD_VECTOR fl, bl, mtf, mtb, br;
    PHD_VECTOR oldpos, moved;
    FLOOR_INFO* floor;
    int hmf_old, hbl_old, hbr_old, hmtb_old, hfl_old;
    int height, collide, speed, newspeed;
    short momentum = 0, rot, roomNumber;

    NoGetOff = FALSE;
    motorbike = GetMotorbikeInfo(item);

    hfl_old = TestMotorbikeHeight(item, 500, -350, &fl_old);
    hmf_old = TestMotorbikeHeight(item, 500, 128, &mtf_old);
    hbl_old = TestMotorbikeHeight(item, -500, -350, &bl_old);
    hbr_old = TestMotorbikeHeight(item, -500, 128, &br_old);
    hmtb_old = TestMotorbikeHeight(item, -500, 0, &mtb_old);

    oldpos.x = item->pos.xPos;
    oldpos.y = item->pos.yPos;
    oldpos.z = item->pos.zPos;

    if (bl_old.y > hbl_old)
        bl_old.y = hbl_old;
    if (br_old.y > hbr_old)
        br_old.y = hbr_old;
    if (fl_old.y > hfl_old)
        fl_old.y = hfl_old;
    if (mtf_old.y > hmf_old)
        mtf_old.y = hmf_old;
    if (mtb_old.y > hmtb_old)
        mtb_old.y = hmtb_old;

    if (item->pos.yPos <= (item->floor - 8))
    {
        if (motorbike->bikeTurn < -91)
            motorbike->bikeTurn += 91;
        else if(motorbike->bikeTurn > 91)
            motorbike->bikeTurn -= 91;
        else
            motorbike->bikeTurn = 0;

        item->pos.yRot += motorbike->bikeTurn + motorbike->extraRotation;
        motorbike->momentumAngle += (item->pos.yRot - motorbike->momentumAngle) >> 5;
    }
    else
    {
        if (motorbike->bikeTurn >= -182)
        {
            if (motorbike->bikeTurn <= 182)
                motorbike->bikeTurn = 0;
            else
                motorbike->bikeTurn -= 182;
        }
        else
        {
            motorbike->bikeTurn += 182;
        }

        item->pos.yRot += motorbike->bikeTurn + motorbike->extraRotation;
        rot = item->pos.yRot - motorbike->momentumAngle;
        momentum = MIN_MOMENTUM_TURN - (2 * motorbike->velocity >> WALL_SHIFT);

        if (!(TrInput & IN_ACTION) && motorbike->velocity > 0)
            momentum += (momentum >> 1);
        
        if (rot < -MAX_MOMENTUM_TURN)
        {
            if (rot < -MOTORBIKE_MAX_MOM_TURN)
            {
                rot = -MOTORBIKE_MAX_MOM_TURN;
                motorbike->momentumAngle = item->pos.yRot - rot;
            }
            else
            {
                motorbike->momentumAngle -= momentum;
            }
        }
        else if (rot > MAX_MOMENTUM_TURN)
        {
            if (rot > MOTORBIKE_MAX_MOM_TURN)
            {
                rot = MOTORBIKE_MAX_MOM_TURN;
                motorbike->momentumAngle = item->pos.yRot - rot;
            }
            else
            {
                motorbike->momentumAngle += momentum;
            }
        }
        else
        {
            motorbike->momentumAngle = item->pos.yRot;
        }
    }

    roomNumber = item->roomNumber;
    floor = GetFloor(item->pos.xPos, item->pos.yPos, item->pos.zPos, &roomNumber);
    height = GetHeight(floor, item->pos.xPos, item->pos.yPos, item->pos.zPos);
    if (item->pos.yPos >= height)
        speed = (item->speed * COS(item->pos.xRot)) >> W2V_SHIFT;
    else
        speed = item->speed;

    item->pos.zPos += (speed * COS(motorbike->momentumAngle)) >> W2V_SHIFT;
    item->pos.xPos += (speed * SIN(motorbike->momentumAngle)) >> W2V_SHIFT;

    if (item->pos.yPos >= height)
    {
        short anglex = 100 * SIN(item->pos.xRot) >> W2V_SHIFT;
        if (ABS(anglex) > 16)
        {
            short anglex2 = 100 * SIN(item->pos.xRot) >> W2V_SHIFT;
            if (anglex < 0)
                anglex2 = -anglex;
            if (anglex2 > 24)
                NoGetOff = TRUE;
            anglex *= 16;
            motorbike->velocity -= anglex;
        }

        short anglez = 100 * SIN(item->pos.zRot) >> W2V_SHIFT;
        if (ABS(anglez) > 32)
        {
            short ang, angabs;
            NoGetOff = TRUE;
            if (anglez >= 0)
                ang = item->pos.yRot + 0x4000;
            else
                ang = item->pos.yRot - 0x4000;
            angabs = abs(anglez) - 24;
            item->pos.xPos += angabs * SIN(ang) >> W2V_SHIFT;
            item->pos.zPos += angabs * COS(ang) >> W2V_SHIFT;
        }
    }

    if (motorbike->velocity <= 0x8000 || motorbike->flags & 1) // boost
    {
        if (motorbike->velocity <= 0xC000)
        {
            if (motorbike->velocity < -0x3000)
                motorbike->velocity = -0x3000;
        }
        else
        {
            motorbike->velocity = 0xC000;
        }
    }
    else
    {
        motorbike->velocity -= 1088;
    }

    moved.x = item->pos.xPos;
    moved.z = item->pos.zPos;

    if (!(item->flags & IFLAG_ONESHOT))
    {
        MotorBikeBaddieCollision(item);
        MotorBikeStaticCollision(item->pos.xPos, item->pos.yPos, item->pos.zPos, item->roomNumber, (WALL_L / 2));
    }
    
    int rot1 = 0;
    int rot2 = 0;

    int hfl = TestMotorbikeHeight(item, 500, -350, &fl);
    if (hfl < fl_old.y - STEP_L)
    {
        rot1 = abs(4 * DoShift(item, &fl, &fl_old));
    }

    int hbl = TestMotorbikeHeight(item, -500, -350, &bl);
    if (hbl < bl_old.y - STEP_L)
    {
        if (rot1)
            rot1 += abs(4 * DoShift(item, &bl, &bl_old));
        else
            rot1 -= abs(4 * DoShift(item, &bl, &bl_old));
    }

    int hmtf = TestMotorbikeHeight(item, 500, 128, &mtf);
    if (hmtf < mtf_old.y - STEP_L)
        rot2 -= abs(4 * DoShift(item, &bl, &bl_old));

    int hmtb = TestMotorbikeHeight(item, -500, 0, &mtb);
    if (hmtb < mtb_old.y - STEP_L)
        DoShift(item, &mtb, &mtb_old);

    int hbr = TestMotorbikeHeight(item, -500, 128, &br);
    if (hbr < br_old.y - STEP_L)
    {
        if (rot2)
            rot2 -= abs(4 * DoShift(item, &bl, &bl_old));
        else
            rot2 += abs(4 * DoShift(item, &bl, &bl_old));
    }

    if (rot1)
        rot2 = rot1;

    roomNumber = item->roomNumber;
    floor = GetFloor(item->pos.xPos, item->pos.yPos, item->pos.zPos, &roomNumber);
    height = GetHeight(floor, item->pos.xPos, item->pos.yPos, item->pos.zPos);
    if (height < (item->pos.yPos - STEP_L))
        DoShift(item, (PHD_VECTOR*)&item->pos, &oldpos);
    
    if (!motorbike->velocity)
        rot2 = 0;

    motorbike->wallShiftRotation = (motorbike->wallShiftRotation + rot2) >> 1;
    if (abs(motorbike->wallShiftRotation) < 2)
        motorbike->wallShiftRotation = 0;
    
    if (abs(motorbike->wallShiftRotation - motorbike->extraRotation) >= 4)
        motorbike->extraRotation += ((motorbike->wallShiftRotation - motorbike->extraRotation) >> 2);
    else
        motorbike->extraRotation  = motorbike->wallShiftRotation;

    collide = GetCollisionAnim(item, &moved);
    if (collide)
    {
        newspeed = ((item->pos.zPos - oldpos.z) * COS(motorbike->momentumAngle) +  (item->pos.xPos - oldpos.x) * SIN(motorbike->momentumAngle)) >> 6;
        if (&Items[lara.skidoo] == item && motorbike->velocity >= 0x8000 && newspeed < (motorbike->velocity - 10))
        {
            LaraItem->hitPoints -= (motorbike->velocity - newspeed) >> 7;
            LaraItem->hitStatus = TRUE;
        }

        if (motorbike->velocity > 0 && newspeed < motorbike->velocity)
            motorbike->velocity = (newspeed < 0) ? 0 : newspeed;
        else if (motorbike->velocity < 0 && newspeed > motorbike->velocity)
            motorbike->velocity = (newspeed > 0) ? 0 : newspeed;

        if (motorbike->velocity < -0x3000)
            motorbike->velocity = -0x3000;
    }

    return collide;
}

BOOL MotorbikeCanGetOff(void)
{
    ITEM_INFO* item;
    FLOOR_INFO* floor;
    int x, y, z;
    int height, ceiling;
    short roomNumber, angle;

    item = &Items[lara.skidoo];
    angle = item->pos.yRot + 0x4000;
    x = item->pos.xPos + ((500 * SIN(angle)) >> W2V_SHIFT);
    y = item->pos.yPos;
    z = item->pos.zPos + ((500 * COS(angle)) >> W2V_SHIFT);

    roomNumber = item->roomNumber;
    floor = GetFloor(x, y, z, &roomNumber);
    height = GetHeight(floor, x, y, z);
    if (height_type == BIG_SLOPE || height_type == DIAGONAL || height == -NO_HEIGHT)
        return FALSE;

    if (abs(height - item->pos.yPos) > STEP_L)
        return FALSE;
    ceiling = GetCeiling(floor, x, y, z);
    if ((ceiling - item->pos.yPos) > -LARA_HITE)
        return FALSE;
    if ((height - ceiling) < LARA_HITE)
        return FALSE;

    return TRUE;
}

void AnimateMotorbike(ITEM_INFO* item, int collide, BOOL dead)
{
    MOTORBIKE_INFO* motorbike;
    motorbike = GetMotorbikeInfo(item);

    if (item->pos.yPos == item->floor 
    ||  LaraItem->currentAnimState == BIKE_FALLING
    ||  LaraItem->currentAnimState == BIKE_LANDING
    ||  LaraItem->currentAnimState == BIKE_EMPTY6
    ||  dead)
    {
        if (!collide
        ||  LaraItem->currentAnimState == BIKE_HITBACK
        ||  LaraItem->currentAnimState == BIKE_HITFRONT
        ||  LaraItem->currentAnimState == BIKE_HITLEFT
        ||  LaraItem->currentAnimState == BIKE_EMPTY6
        ||  motorbike->velocity <= 10922
        ||  dead)
        {
            switch (LaraItem->currentAnimState)
            {
            case BIKE_IDLE:
                if (dead)
                {
                    LaraItem->goalAnimState = BIKE_DEATH;
                }
                else if (CHK_NOP(TrInput, IN_JUMP) && CHK_NOP(TrInput, IN_RIGHT) || motorbike->velocity != 0 || NoGetOff)
                {
                    if (TrInput & IN_ACTION && !(TrInput & IN_JUMP))
                        LaraItem->goalAnimState = BIKE_MOVING_FRONT;
                    if (TrInput & IN_BACK)
                        LaraItem->goalAnimState = BIKE_MOVING_BACK;
                }
                else if (CHK_EXI(TrInput, IN_JUMP) && CHK_EXI(TrInput, IN_RIGHT) && MotorbikeCanGetOff())
                {
                    LaraItem->goalAnimState = BIKE_EXIT;
                }
                break;
            case BIKE_MOVING_FRONT:
                if (dead)
                {
                    if (motorbike->velocity <= 0x4000)
                        LaraItem->goalAnimState = BIKE_DEATH;
                    else
                        LaraItem->goalAnimState = BIKE_EMPTY5;
                }
                else if (motorbike->velocity & -256 || TrInput & (IN_ACTION | IN_JUMP))
                {
                    if (TrInput & IN_LEFT)
                    {
                        LaraItem->goalAnimState = BIKE_MOVING_LEFT;
                    }
                    else if (TrInput & IN_RIGHT)
                    {
                        LaraItem->goalAnimState = BIKE_MOVING_RIGHT;
                    }
                    else if (TrInput & IN_JUMP)
                    {
                        if (motorbike->velocity <= 0x5554)
                            LaraItem->goalAnimState = BIKE_EMPTY3;
                        else
                            LaraItem->goalAnimState = BIKE_STOP;
                    }
                    else if (TrInput & IN_BACK && motorbike->velocity <= 0x800)
                    {
                        LaraItem->goalAnimState = BIKE_MOVING_BACK;
                    }
                }
                else
                {
                    LaraItem->goalAnimState = BIKE_IDLE;
                }
                break;
            case BIKE_MOVING_LEFT:
                if (motorbike->velocity & -256)
                {
                    if (TrInput & IN_RIGHT || !(TrInput & IN_LEFT))
                        LaraItem->goalAnimState = BIKE_MOVING_FRONT;
                }
                else
                {
                    LaraItem->goalAnimState = BIKE_IDLE;
                }
                break;
            case BIKE_MOVING_BACK:
                if (!(TrInput & IN_BACK))
                    LaraItem->goalAnimState = BIKE_IDLE;
                else
                    LaraItem->goalAnimState = BIKE_MOVING_BACK_LOOP;
                break;
            case BIKE_MOVING_RIGHT:
                if (motorbike->velocity & -256)
                {
                    if (TrInput & IN_LEFT || !(TrInput & IN_RIGHT))
                        LaraItem->goalAnimState = BIKE_MOVING_FRONT;
                }
                else
                {
                    LaraItem->goalAnimState = BIKE_IDLE;
                }
                break;
            case BIKE_EMPTY3:
            case BIKE_STOP:
            case BIKE_ACCELERATE:
                if (motorbike->velocity & -256)
                {
                    if (TrInput & IN_LEFT)
                        LaraItem->goalAnimState = BIKE_MOVING_LEFT;
                    else if (TrInput & IN_RIGHT)
                        LaraItem->goalAnimState = BIKE_MOVING_RIGHT;
                }
                else
                {
                    LaraItem->goalAnimState = BIKE_IDLE;
                }
                break;
            case BIKE_FALLING:
                if (item->pos.yPos == item->floor)
                {
                    LaraItem->goalAnimState = BIKE_LANDING;

                    int fallspeed_damage = item->fallspeed - 140;
                    if (fallspeed_damage > 0)
                    {
                        if (fallspeed_damage <= 100)
                            LaraItem->hitPoints -= (-LARA_HITPOINTS * fallspeed_damage * fallspeed_damage) / 10000;
                        else
                            LaraItem->hitPoints = 0;
                    }
                }
                else if (item->fallspeed > 220)
                {
                    motorbike->flags |= 0x40;
                }
                break;
            case BIKE_HITFRONT:
            case BIKE_HITBACK:
            case BIKE_HITRIGHT:
            case BIKE_HITLEFT:
                if (TrInput & (IN_ACTION|IN_JUMP))
                    LaraItem->goalAnimState = BIKE_MOVING_FRONT;
                break;
            }
        }
        else
        {
            switch (collide)
            {
            case 13:
                LaraItem->animNumber = Objects[VEHICLE_EXTRA].animIndex + 12;
                LaraItem->currentAnimState = BIKE_HITBACK;
                LaraItem->goalAnimState = BIKE_HITBACK;
                LaraItem->frameNumber = Anims[LaraItem->animNumber].frameBase;
                break;
            case 14:
                LaraItem->animNumber = Objects[VEHICLE_EXTRA].animIndex + 11;
                LaraItem->currentAnimState = BIKE_HITFRONT;
                LaraItem->goalAnimState = BIKE_HITFRONT;
                LaraItem->frameNumber = Anims[LaraItem->animNumber].frameBase;
                break;
            case 11:
                LaraItem->animNumber = Objects[VEHICLE_EXTRA].animIndex + 14;
                LaraItem->currentAnimState = BIKE_HITRIGHT;
                LaraItem->goalAnimState = BIKE_HITRIGHT;
                LaraItem->frameNumber = Anims[LaraItem->animNumber].frameBase;
                break;
            default:
            case 12:
                LaraItem->animNumber = Objects[VEHICLE_EXTRA].animIndex + 13;
                LaraItem->currentAnimState = BIKE_HITLEFT;
                LaraItem->goalAnimState = BIKE_HITLEFT;
                LaraItem->frameNumber = Anims[LaraItem->animNumber].frameBase;
                break;
            }
        }
    }
    else
    {
        if (motorbike->velocity >= 0)
            LaraItem->animNumber = Objects[VEHICLE_EXTRA].animIndex + 22;
        else
            LaraItem->animNumber = Objects[VEHICLE_EXTRA].animIndex + 6;
        LaraItem->frameNumber = Anims[LaraItem->animNumber].frameBase;
        LaraItem->currentAnimState = BIKE_FALLING;
        LaraItem->goalAnimState = BIKE_FALLING;
    }

    if (Rooms[item->roomNumber].flags & 1)
    {
        LaraItem->goalAnimState = BIKE_EMPTY6;
        LaraItem->hitPoints = 0;
        MotorBikeExplode(item);
    }
}

int MotorbikeUserControl(ITEM_INFO* item, int height, int* pitch)
{
    MOTORBIKE_INFO* motorbike;
    PHD_VECTOR pos;
    int drive = 0;
    int vel, newvel;

    motorbike = GetMotorbikeInfo(item);
    if (motorbike->lightPower < 127)
    {
        motorbike->lightPower += (GetRandomControl() & 7) + 3;
        if (motorbike->lightPower > 127)
            motorbike->lightPower = 127;
    }

    if (motorbike->revs > 0x10)
    {
        motorbike->velocity += motorbike->revs >> 4;
        motorbike->revs -= motorbike->revs >> 3;
    }
    else
    {
        motorbike->revs = 0;
    }

    if ((TrInput & IN_SPRINT) && (TrInput & IN_ACTION) && lara.dash_timer)
    {
        motorbike->flags |= 0x1;
        lara.dash_timer -= 2;
        if (lara.dash_timer > 0x8000)
        {
            motorbike->flags &= ~0x1;
            lara.dash_timer = 0;
        }
    }
    else
    {
        motorbike->flags &= ~0x1;
    }

    if (item->pos.yPos >= (height - STEP_L))
    {
        if (!motorbike->velocity && CHK_EXI(TrInput, IN_LOOK))
            LookUpDown(); // not used since it ignore the vehicle (lara.skidoo == NO_ITEM) !!

        if (motorbike->velocity > 0)
        {
            if (CHK_EXI(TrInput, IN_LEFT))
            {
                motorbike->bikeTurn -= motorbike->velocity <= MOTORBIKE_ACCEL_1 ? ONE_DEGREE - (MOTORBIKE_HTURN * motorbike->velocity >> W2V_SHIFT) : MOTORBIKE_DEFAULT_HTURN;
                if (motorbike->bikeTurn < -MOTORBIKE_MAX_HTURN)
                    motorbike->bikeTurn = -MOTORBIKE_MAX_HTURN;
            }
            else if (CHK_EXI(TrInput, IN_RIGHT))
            {
                motorbike->bikeTurn += motorbike->velocity <= MOTORBIKE_ACCEL_1 ? ONE_DEGREE + (MOTORBIKE_HTURN * motorbike->velocity >> W2V_SHIFT) : MOTORBIKE_DEFAULT_HTURN;
                if (motorbike->bikeTurn > MOTORBIKE_MAX_HTURN)
                    motorbike->bikeTurn = MOTORBIKE_MAX_HTURN;
            }
        }
        else if (motorbike->velocity < 0)
        {
            if (CHK_EXI(TrInput, IN_LEFT))
            {
                motorbike->bikeTurn += MOTORBIKE_HTURN;
                if (motorbike->bikeTurn > MOTORBIKE_MAX_HTURN)
                    motorbike->bikeTurn = MOTORBIKE_MAX_HTURN;
            }
            else if (CHK_EXI(TrInput, IN_RIGHT))
            {
                motorbike->bikeTurn -= MOTORBIKE_HTURN;
                if (motorbike->bikeTurn < -MOTORBIKE_MAX_HTURN)
                    motorbike->bikeTurn = -MOTORBIKE_MAX_HTURN;
            }
        }

        if (CHK_EXI(TrInput, IN_JUMP))
        {
            pos.x = 0;
            pos.y = -144;
            pos.z = -1024;
            GetJointAbsPosition(item, &pos, NULL);
            TriggerDynamic(pos.x, pos.y, pos.z, 10, 0x40, 0, 0);
            item->meshBits = 0x5F7;
        }
        else
        {
            item->meshBits = 0x3F7;
        }

        if (CHK_EXI(TrInput, IN_JUMP))
        {
            if (motorbike->velocity < 0)
            {
                motorbike->velocity += 768;
                if (motorbike->velocity > 0)
                    motorbike->velocity = 0;
            }
            else
            {
                motorbike->velocity -= 768;
                if (motorbike->velocity < 0)
                    motorbike->velocity = 0;
            }
        }
        else if (CHK_EXI(TrInput, IN_ACTION))
        {
            if (motorbike->velocity < MOTORBIKE_ACCEL_MAX)
            {
                if (motorbike->velocity < MOTORBIKE_ACCEL_1)
                    motorbike->velocity += 8 + ((MOTORBIKE_ACCEL_1 + 0x800 - motorbike->velocity) >> 3);
                else if (motorbike->velocity < MOTORBIKE_ACCEL_2)
                    motorbike->velocity += 4 + ((MOTORBIKE_ACCEL_2 + 0x800 - motorbike->velocity) >> 4);
                else if (motorbike->velocity < MOTORBIKE_ACCEL_MAX)
                    motorbike->velocity += 2 + ((MOTORBIKE_ACCEL_MAX       - motorbike->velocity) >> 4);

                if (motorbike->flags & 1)
                    motorbike->velocity += 256;
            }
            else
            {
                motorbike->velocity = MOTORBIKE_ACCEL_MAX;
            }

            // apply friction according to turn
            motorbike->velocity -= abs(item->pos.yRot - motorbike->momentumAngle) >> 6;
        }
        else if (motorbike->velocity > MOTORBIKE_FRICTION)
        {
            motorbike->velocity -= MOTORBIKE_FRICTION;
        }
        else if (motorbike->velocity < MOTORBIKE_FRICTION)
        {
            motorbike->velocity += MOTORBIKE_FRICTION;
        }
        else
        {
            motorbike->velocity = 0;
        }

        if (LaraItem->currentAnimState == BIKE_MOVING_BACK)
        {
            short framenow = LaraItem->frameNumber;
            short framebase = Anims[LaraItem->animNumber].frameBase;

            if (framenow >= framebase + 24 && framenow <= framebase + 29)
            {
                if (motorbike->velocity > -0x3000)
                    motorbike->velocity -= 0x600;
            }
        }

        item->speed = motorbike->velocity >> 8;

        if (motorbike->engineRevs > MOTORBIKE_ACCEL_MAX)
            motorbike->engineRevs = (GetRandomControl() & 0x1FF) + 0xBF00;
        int newpitch = motorbike->velocity;
        if (motorbike->velocity < 0)
            newpitch >>= 1;
        motorbike->engineRevs += ((abs(newpitch) - 0x2000 - motorbike->engineRevs) >> 3);
        *pitch = motorbike->engineRevs;

    }
    else
    {
        if (motorbike->engineRevs < 0xFFFF)
            motorbike->engineRevs += (motorbike->engineRevs - 0xFFFF) >> 3;
        *pitch = motorbike->engineRevs;
    }

    return drive;
}

void SetLaraOnMotorBike(ITEM_INFO* item, ITEM_INFO* laraitem)
{
    MOTORBIKE_INFO* motorbike;
    motorbike = GetMotorbikeInfo(item);

    lara.gun_status = LHS_HANDBUSY;
    lara.hit_direction = -1;
    laraitem->currentAnimState = BIKE_IDLE;
    laraitem->goalAnimState = BIKE_IDLE;
    laraitem->animNumber = Objects[VEHICLE_EXTRA].animIndex + BIKE_IDLE_ANIM;
    laraitem->frameNumber = Anims[laraitem->animNumber].frameBase;
    laraitem->gravityStatus = FALSE;
    item->animNumber = laraitem->animNumber + (Objects[MOTORBIKE].animIndex - Objects[VEHICLE_EXTRA].animIndex);
    item->frameNumber = laraitem->frameNumber + (Anims[MOTORBIKE].frameBase - Anims[VEHICLE_EXTRA].frameBase);
    item->hitPoints = 1;
    item->flags = short(IFLAG_KILLED_ITEM); // hmm... maybe wrong name (it can be IFLAG_CODEBITS) ?
    motorbike->revs = 0;
}

int MotorBikeControl(void)
{
    ITEM_INFO* item;
    MOTORBIKE_INFO* motorbike;
    FLOOR_INFO* floor;
    PHD_VECTOR oldpos, fl, fr, fm;
    int drive, collide, pitch = 0, dead, height = 0, ceiling;
    short roomNumber;

    item = &Items[lara.skidoo];
    motorbike = GetMotorbikeInfo(item);
    collide = MotorBikeDynamics(item);
    drive = -1;

    oldpos.x = item->pos.xPos;
    oldpos.y = item->pos.yPos;
    oldpos.z = item->pos.zPos;

    int hfl = TestMotorbikeHeight(item, 500, -350, &fl);
    int hfr = TestMotorbikeHeight(item, 500, 128, &fr);
    int hfm = TestMotorbikeHeight(item, -500, 0, &fm);

    roomNumber = item->roomNumber;
    floor = GetFloor(item->pos.xPos, item->pos.yPos, item->pos.zPos, &roomNumber);
    height = GetHeight(floor, item->pos.xPos, item->pos.yPos, item->pos.zPos);

    TestTriggers(TriggerIndex, FALSE, FALSE);
    TestTriggers(TriggerIndex, TRUE, FALSE);

    if (LaraItem->hitPoints <= 0)
    {
        TrInput &= ~(IN_LEFT | IN_RIGHT | IN_BACK | IN_FORWARD);
        dead = TRUE;
    }
    else
    {
        dead = FALSE;
    }

    if (motorbike->flags)
    {
        collide = 0;
    }
    else
    {
        DrawMotorbikeLight(item);
        if (LaraItem->currentAnimState < BIKE_ENTER || LaraItem->currentAnimState > BIKE_EXIT)
        {
            drive = MotorbikeUserControl(item, height, &pitch);
        }
        else
        {
            drive = -1;
            collide = 0;
        }
    }

    if (motorbike->velocity > 0 || motorbike->revs)
    {
        motorbike->pitch = pitch;

        if (motorbike->pitch < -0x8000)
            motorbike->pitch = -0x8000;
        else if (motorbike->pitch > 0xA000)
            motorbike->pitch = 0xA000;

        SoundEffect(SFX_BIKE_MOVING, &item->pos, (motorbike->pitch << 8) + 0x1000004);
    }
    else
    {
        if (drive != -1)
        {
            SoundEffect(SFX_BIKE_IDLE, &item->pos, 0);
            SoundEffect(SFX_BIKE_MOVING, &item->pos, (motorbike->pitch << 8) + 0x1000004);
        }
        motorbike->pitch = 0;
    }

    item->floor = height;
    int rotation = motorbike->velocity >> 2;
    motorbike->wheelLeft = rotation;
    motorbike->wheelRight = rotation;
    int newy = item->pos.yPos;
    item->fallspeed = DoMotorBikeDynamics(height, item->fallspeed, &item->pos.yPos, 0);

    short xrot = 0, zrot = 0;
    int r1, r2;
    r1 = (fr.y + fl.y) >> 1;
    r2 = (fr.y + fl.y) >> 1;

    if (fm.y >= hfm)
    {
        if (r1 >= (hfl + hfr) >> 1)
        {
            xrot = phd_atan(1000, hfm - r1);
            zrot = phd_atan(350, r2 - fl.y);
        }
        else
        {
            xrot = phd_atan(500, hfm - item->pos.yPos);
            zrot = phd_atan(350, r2 - fl.y);
        }
    }
    else if (r1 >= (hfl + hfr) >> 1)
    {
        xrot = phd_atan(500, item->pos.yPos - r1);
        zrot = phd_atan(350, r2 - fl.y);
    }
    else
    {
        xrot = phd_atan(125, newy - item->pos.yPos);
        zrot = phd_atan(350, r2 - fl.y);
    }
    
    item->pos.xRot += ((xrot - item->pos.xRot) >> 2);
    item->pos.zRot += ((zrot - item->pos.zRot) >> 2);

    if (motorbike->flags >= 0)
    {
        if (roomNumber != item->roomNumber)
        {
            ItemNewRoom(lara.skidoo, roomNumber);
            ItemNewRoom(lara.itemNumber, roomNumber);
        }
        LaraItem->pos.xPos = item->pos.xPos;
        LaraItem->pos.yPos = item->pos.yPos;
        LaraItem->pos.zPos = item->pos.zPos;
        LaraItem->pos.yRot = item->pos.yRot;
        LaraItem->pos.xRot = item->pos.xRot;
        LaraItem->pos.zRot = item->pos.zRot;
        AnimateMotorbike(item, collide, dead);
        AnimateItem(LaraItem);
        item->animNumber = LaraItem->animNumber + (Objects[MOTORBIKE].animIndex - Objects[VEHICLE_EXTRA].animIndex);
        item->frameNumber = LaraItem->frameNumber + (Anims[item->animNumber].frameBase - Anims[LaraItem->animNumber].frameBase);
        camera.target_elevation = -5460;

        if (motorbike->flags & 0x40)
        {
            if (item->pos.yPos == item->floor)
            {
                ExplodingDeath(lara.itemNumber, -1, 256);
                LaraItem->hitPoints = 0;
                LaraItem->flags = IFLAG_ONESHOT;
                MotorBikeExplode(item);
                return 0;
            }
        }
    }

    if (LaraItem->currentAnimState == BIKE_ENTER || LaraItem->currentAnimState == BIKE_EXIT)
    {
        ExhaustStart = 0;
        return MotorBikeCheckGetOff();
    }

    return MotorBikeCheckGetOff();
}

#ifdef DLL_INJECT
void injector::f_game::inject_motorbike()
{
    inject(0x00464610, InitialiseMotorBike);
    inject(0x00464680, MotorBikeCollision);
    inject(0x00464850, DrawMotorbikeLight);
    inject(0x004648F0, GetOnMotorBike);
    inject(0x00464A30, DrawMotorBikeSmoke);
    inject(0x00464C20, DrawMotorBikeEffect);
    inject(0x00464C70, MotorBikeControl);
    inject(0x00465220, TriggerMotorbikeExhaustSmoke);
    inject(0x004653D0, MotorBikeExplode);
    inject(0x004654A0, MotorBikeCheckGetOff);
    inject(0x00465660, TestMotorbikeHeight);
    inject(0x00465770, DoMotorBikeDynamics);
    inject(0x004657E0, MotorBikeDynamics);
    //MotorBikeStaticCollision
    //MotorBikeBaddieCollision
    inject(0x004664B0, AnimateMotorbike);
    inject(0x004668E0, MotorbikeCanGetOff);
    inject(0x00466AB0, MotorbikeUserControl);
    inject(0x00466E90, SetLaraOnMotorBike);
}
#endif
