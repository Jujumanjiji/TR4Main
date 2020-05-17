#include "framework.h"
#include "oldobjects.h"
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
    short unknown5;
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

#define MIN_MOMENTUM_TURN 273
#define MAX_MOMENTUM_TURN ANGLE(45)
#define MOTORBIKE_SLIP 100
#define MOTORBIKE_SLIP_SIDE 50

static MOTORBIKE_INFO* GetMotorbikeInfo(ITEM_INFO* item)
{
    return (MOTORBIKE_INFO*)item->data;
}

void InitialiseMotorBike(short item_number) // (F) (D)
{
    ITEM_INFO* item;
    MOTORBIKE_INFO* motorbike;

    item = &items[item_number];
    motorbike = (MOTORBIKE_INFO*)game_malloc(sizeof(MOTORBIKE_INFO));
    item->data = (void*)motorbike;
    motorbike->velocity = 0;
    motorbike->bikeTurn = 0;
    motorbike->pitch = 0;
    motorbike->momentumAngle = item->pos.y_rot;
    motorbike->wallShiftRotation = 0;
    motorbike->extraRotation = 0;
    motorbike->flags = NULL;
    motorbike->unknown5 = 0;
    motorbike->wheelLeft = 0; // left wheel
    motorbike->wheelRight = 0; // two wheel in the principal body
    item->mesh_bits = 0x3F7;
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
    rnd = (2 * motorbike->unknown5) - (GetRandomControl() & 0xF);
    if (rnd <= 0)
        SpotLightEnabled = FALSE;
    else
        CreateSpotLight(&start, &target, item->pos.y_rot, rnd);
}

BOOL GetOnMotorBike(short item_number) // (F) (D)
{
    ITEM_INFO* item;
    FLOOR_INFO* floor;
    int dx, dz, distance, height;
    unsigned short tempangle;
    short angle;
    short room_number;

    item = &items[item_number];
    if (item->flags & IFLAG_ONESHOT || lara.gun_status == LHS_HANDBUSY || lara_item->gravity_status)
        return FALSE;

    if ((abs(item->pos.y - lara_item->pos.y) >= 256 || !(TrInput & IN_ACTION)) && GLOBAL_invitemchosen != PUZZLE_ITEM1)
        return FALSE;

    dx = lara_item->pos.x - item->pos.x;
    dz = lara_item->pos.z - item->pos.z;
    distance = SQUARE(dx) + SQUARE(dz);
    if (distance > 170000)
        return FALSE;

    room_number = item->room_number;
    floor = GetFloor(item->pos.x, item->pos.y, item->pos.z, &room_number);
    height = GetHeight(floor, item->pos.x, item->pos.y, item->pos.z);
    if (height < -32000)
        return FALSE;

    angle = phd_atan(item->pos.z - lara_item->pos.z, item->pos.x - lara_item->pos.x) - item->pos.y_rot;
    tempangle = angle - item->pos.y_rot;
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

void MotorBikeCollision(short item_number, ITEM_INFO* laraitem, COLL_INFO* coll) // (F) (D)
{
    ITEM_INFO* item;
    MOTORBIKE_INFO* motorbike;

    if (laraitem->hit_points >= 0 && lara.skidoo == NO_ITEM)
    {
        item = &items[item_number];
        motorbike = GetMotorbikeInfo(item);
        
        // update motorbike light
        if (motorbike->bikeTurn)
        {
            motorbike->bikeTurn -= (motorbike->bikeTurn >> 3) - 1;
            DrawMotorbikeLight(item);
        }

        if (GetOnMotorBike(item_number))
        {
            lara.skidoo = item_number;

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

            short angle = phd_atan(item->pos.z - laraitem->pos.z, item->pos.x - laraitem->pos.x) - item->pos.y_rot;
            if (angle <= -8190 || angle >= 24570)
            {
                if (GLOBAL_invitemchosen == PUZZLE_ITEM1)
                {
                    laraitem->anim_number = objects[VEHICLE_EXTRA].anim_index + BIKE_UNLOCK_ANIM;
                    GLOBAL_invitemchosen = -1;
                }
                else
                {
                    laraitem->anim_number = objects[VEHICLE_EXTRA].anim_index + BIKE_ENTER_ANIM;
                }
                laraitem->state_next = BIKE_ENTER_ANIM;
                laraitem->state_current = BIKE_ENTER_ANIM;
            }
            laraitem->frame_number = anims[laraitem->anim_number].frame_base;

            item->hit_points = 1;
            laraitem->pos.x = item->pos.x;
            laraitem->pos.y = item->pos.y;
            laraitem->pos.z = item->pos.z;
            laraitem->pos.y_rot = item->pos.y_rot;
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
            ObjectCollision(item_number, laraitem, coll);
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
    sptr->def = (unsigned char)objects[DEFAULT_SPRITES].mesh_index;
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

    if (lara_item->state_current != BIKE_ENTER && lara_item->state_current != BIKE_EXIT)
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
            TriggerMotorbikeExhaustSmoke(pos.x, pos.y, pos.z, item->pos.y_rot - ANGLE(180), 64 - speed, TRUE);
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

        TriggerMotorbikeExhaustSmoke(pos.x, pos.y, pos.z, item->pos.y_rot - ANGLE(180), speed, FALSE);
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
    if (rooms[item->room_number].flags & 1)
    {
        TriggerUnderwaterExplosion(item, TRUE);
    }
    else
    {
        TriggerExplosionSparks(item->pos.x, item->pos.y, item->pos.z, 3, -2, 0, item->room_number);
        for (int i = 0; i < 2; i++)
            TriggerExplosionSparks(item->pos.x, item->pos.y, item->pos.z, 3, -1, 0, item->room_number);
    }

    ExplodingDeath(lara.skidoo, -2, 256);
    ExplodingDeath(lara.item_number, -2, 258); // enable blood
    lara_item->hit_points = 0;
    KillItem(lara.skidoo);
    item->status = FITEM_INVISIBLE;
    SoundEffect(SFX_EXPLOSION1, nullptr, NULL);
    SoundEffect(SFX_EXPLOSION2, nullptr, NULL);
    lara.skidoo = NO_ITEM;
}

int MotorBikeCheckGetOff(void)
{
    ITEM_INFO* item;

    item = &items[lara.skidoo];
    if (lara_item->state_current == BIKE_EXIT && lara_item->frame_number == anims[lara_item->anim_number].frame_end)
    {
        lara_item->pos.y_rot -= 0x4000;
        lara_item->anim_number = ANIMATION_LARA_STAY_SOLID;
        lara_item->frame_number = anims[lara_item->anim_number].frame_base;
        lara_item->state_next = STATE_LARA_IDLE;
        lara_item->state_current = STATE_LARA_IDLE;
        lara_item->pos.x -= 2 * SIN(item->pos.y_rot) >> W2V_SHIFT;
        lara_item->pos.z -= 2 * COS(item->pos.y_rot) >> W2V_SHIFT;
        lara_item->pos.x_rot = 0;
        lara_item->pos.z_rot = 0;
        lara.skidoo = NO_ITEM;
        lara.gun_status = LHS_ARMLESS;
        lara.dash_timer = 120;
        return TRUE;
    }

    if (lara_item->frame_number != anims[lara_item->anim_number].frame_end)
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
    short room_number;

    pos->y = item->pos.y + ((dx * SIN(item->pos.z_rot) >> W2V_SHIFT) - (dz * SIN(item->pos.x_rot)) >> W2V_SHIFT);
    c = COS(item->pos.y_rot);
    s = SIN(item->pos.y_rot);
    pos->z = item->pos.z + ((dz * c - dx * s) >> W2V_SHIFT);
    pos->x = item->pos.x + ((dx * c + dz * s) >> W2V_SHIFT);

    room_number = item->room_number;
    floor = GetFloor(pos->x, pos->y, pos->z, &room_number);
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

    if (height > *y)
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
                return flags + fallspeed;
            else
                fallspeed += GRAVITY;
        }
    }
    else
    {
        if (flags)
        {
            return fallspeed;
        }
        else
        {
            // On ground
            kick = (height - *y);

            if (kick < 80)
                kick = 80;

            fallspeed += ((kick - fallspeed) >> 4);

            if (*y > height)
                *y = height;
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
    int momentum, height, collide, shift;
    short rot, room_number, speed, newspeed;
    short rotadd, rotadd2;

    NoGetOff = FALSE;
    motorbike = GetMotorbikeInfo(item);

    hfl_old = TestMotorbikeHeight(item, 500, -350, &fl_old);
    hmf_old = TestMotorbikeHeight(item, 500, 128, &mtf_old);
    hbl_old = TestMotorbikeHeight(item, -500, -350, &bl_old);
    hbr_old = TestMotorbikeHeight(item, -500, 128, &br_old);
    hmtb_old = TestMotorbikeHeight(item, -500, 0, &mtb_old);

    oldpos.x = item->pos.x;
    oldpos.y = item->pos.y;
    oldpos.z = item->pos.z;

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

    if (item->pos.y <= (item->floor - 8))
    {
        if (motorbike->bikeTurn < -91)
            motorbike->bikeTurn += 91;
        else if(motorbike->bikeTurn > 91)
            motorbike->bikeTurn -= 91;
        else
            motorbike->bikeTurn = 0;

        item->pos.y_rot += LOWORD(motorbike->bikeTurn) + motorbike->extraRotation;
        motorbike->momentumAngle += (item->pos.y_rot - motorbike->momentumAngle) >> 5;
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

        item->pos.y_rot += LOWORD(motorbike->bikeTurn) + motorbike->extraRotation;
        rot = item->pos.y_rot - motorbike->momentumAngle;
        momentum = 728 - (2 * motorbike->velocity >> 10);

        if (!(TrInput & IN_ACTION) && motorbike->velocity > 0)
            momentum += (momentum >> 1);
        
        if (rot >= -273)
        {
            if (rot > 273)
            {
                if (rot <= 8190)
                {
                    motorbike->momentumAngle += momentum;
                }
                else
                {
                    motorbike->momentumAngle -= 8190;
                }
            }
        }
        else if (rot < -8190)
        {
            motorbike->momentumAngle += 8190;
        }
        else
        {
            motorbike->momentumAngle -= momentum;
        }
    }

    room_number = item->room_number;
    floor = GetFloor(item->pos.x, item->pos.y, item->pos.z, &room_number);
    height = GetHeight(floor, item->pos.x, item->pos.y, item->pos.z);
    if (item->pos.y < height)
        speed = item->speed;
    else
        speed = item->speed * COS(item->pos.x_rot) >> W2V_SHIFT;

    item->pos.z += speed * COS(motorbike->momentumAngle) >> W2V_SHIFT;
    item->pos.x += speed * SIN(motorbike->momentumAngle) >> W2V_SHIFT;

    if (item->pos.y >= height)
    {
        short anglex = 100 * SIN(item->pos.x_rot) >> W2V_SHIFT;
        if (abs(anglex) > 16)
        {
            anglex = 100 * SIN(item->pos.x_rot) >> W2V_SHIFT;
            if (abs(anglex) > 24)
                NoGetOff = TRUE;
            anglex *= 16;
            motorbike->velocity -= anglex;
        }

        //short anglez = 100 * SIN(item->pos.z_rot) >> W2V_SHIFT;
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

    moved.x = item->pos.x;
    moved.z = item->pos.z;

    if (!(item->flags & IFLAG_ONESHOT))
    {
        MotorBikeBaddieCollision(item);
        MotorBikeStaticCollision(item->pos.x, item->pos.y, item->pos.z, item->room_number, (WALL_L / 2));
    }
    
    rotadd2 = 0;
    rotadd = 0;

    int hfl = TestMotorbikeHeight(item, 500, -350, &fl);
    if (hfl < fl_old.y - STEP_L)
    {
        rotadd = abs(4 * DoShift(item, &fl, &fl_old));
    }

    int hbl = TestMotorbikeHeight(item, -500, -350, &bl);
    if (hbl < bl_old.y - STEP_L)
    {
        if (rotadd)
        {
            shift = 4 * DoShift(item, &bl, &bl_old);
            rotadd += abs(shift);
        }
        else
        {
            shift = 4 * DoShift(item, &bl, &bl_old);
            rotadd = (shift >> 31) - (shift >> 31) ^ shift;
        }
    }

    int hmtf = TestMotorbikeHeight(item, 500, 128, &mtf);
    if (hmtf < mtf_old.y - STEP_L)
    {
        shift = 4 * DoShift(item, &mtf, &mtf_old);
        rotadd2 = (shift >> 31) - (shift >> 31) ^ shift;
    }

    int hmtb = TestMotorbikeHeight(item, -500, 0, &mtb);
    if (hmtb < mtb_old.y - STEP_L)
        DoShift(item, &mtb, &mtb_old);

    int hbr = TestMotorbikeHeight(item, -500, 128, &br);
    if (hbr < br_old.y - STEP_L)
    {
        if (rotadd2)
        {
            shift = 4 * DoShift(item, &br, &br_old);
            rotadd2 -= abs(shift);
        }
        else
        {
            shift = 4 * DoShift(item, &br, &br_old);
            rotadd2 = (shift ^ shift) - shift;
        }
    }

    if (rotadd)
        rotadd2 = rotadd;

    room_number = item->room_number;
    floor = GetFloor(item->pos.x, item->pos.y, item->pos.z, &room_number);
    height = GetHeight(floor, item->pos.x, item->pos.y, item->pos.z);
    if (height < (item->pos.y - STEP_L))
        DoShift(item, (PHD_VECTOR*)&item->pos, &oldpos);

    if (!motorbike->velocity)
        rotadd2 = 0;

    motorbike->wallShiftRotation = (motorbike->wallShiftRotation + rotadd2) >> 1;
    if (abs(motorbike->wallShiftRotation) < 2)
        motorbike->wallShiftRotation = 0;
    
    if (abs(motorbike->wallShiftRotation - motorbike->extraRotation) >= 4)
        motorbike->extraRotation += ((motorbike->wallShiftRotation - motorbike->extraRotation) >> 2);
    else
        motorbike->extraRotation = motorbike->wallShiftRotation;

    collide = GetCollisionAnim(item, &moved);
    if (collide)
    {
        newspeed = ((item->pos.x - oldpos.x) * SIN(motorbike->momentumAngle)
                 +  (item->pos.z - oldpos.z) * COS(motorbike->momentumAngle)) >> 6;
        if (&items[lara.skidoo] == item && motorbike->velocity >= 0x8000 && newspeed < (motorbike->velocity - 10))
        {
            lara_item->hit_points -= (motorbike->velocity - newspeed) >> 7;
            lara_item->hit_status = TRUE;
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
    short room_number, angle;

    item = &items[lara.skidoo];
    angle = item->pos.y_rot + 0x4000;
    x = item->pos.x + ((500 * SIN(angle)) >> W2V_SHIFT);
    y = item->pos.y;
    z = item->pos.z + ((500 * COS(angle)) >> W2V_SHIFT);

    room_number = item->room_number;
    floor = GetFloor(x, y, z, &room_number);
    height = GetHeight(floor, x, y, z);
    if (height_type == BIG_SLOPE || height_type == DIAGONAL || height == -NO_HEIGHT)
        return FALSE;

    if (abs(height - item->pos.y) > STEP_L)
        return FALSE;
    ceiling = GetCeiling(floor, x, y, z);
    if ((ceiling - item->pos.y) > -LARA_HITE)
        return FALSE;
    if ((height - ceiling) < LARA_HITE)
        return FALSE;

    return TRUE;
}

void AnimateMotorbike(ITEM_INFO* item, int collide, BOOL dead)
{
    MOTORBIKE_INFO* motorbike;
    motorbike = GetMotorbikeInfo(item);

    if (item->pos.y == item->floor 
    ||  lara_item->state_current == BIKE_FALLING
    ||  lara_item->state_current == BIKE_LANDING
    ||  lara_item->state_current == BIKE_EMPTY6
    ||  dead)
    {
        if (!collide
        ||  lara_item->state_current == BIKE_HITBACK
        ||  lara_item->state_current == BIKE_HITFRONT
        ||  lara_item->state_current == BIKE_HITLEFT
        ||  lara_item->state_current == BIKE_EMPTY6
        ||  motorbike->velocity <= 10922
        ||  dead)
        {
            switch (lara_item->state_current)
            {
            case BIKE_IDLE:
                if (dead)
                {
                    lara_item->state_next = BIKE_DEATH;
                }
                else if (CHK_NOP(TrInput, IN_JUMP) && CHK_NOP(TrInput, IN_RIGHT) || motorbike->velocity != 0 || NoGetOff)
                {
                    if (TrInput & IN_ACTION && !(TrInput & IN_JUMP))
                        lara_item->state_next = BIKE_MOVING_FRONT;
                    if (TrInput & IN_BACK)
                        lara_item->state_next = BIKE_MOVING_BACK;
                }
                else if (CHK_EXI(TrInput, IN_JUMP) && CHK_EXI(TrInput, IN_RIGHT) && MotorbikeCanGetOff())
                {
                    lara_item->state_next = BIKE_EXIT;
                }
                break;
            case BIKE_MOVING_FRONT:
                if (dead)
                {
                    if (motorbike->velocity <= 0x4000)
                        lara_item->state_next = BIKE_DEATH;
                    else
                        lara_item->state_next = BIKE_EMPTY5;
                }
                else if (motorbike->velocity & -256 || TrInput & (IN_ACTION | IN_JUMP))
                {
                    if (TrInput & IN_LEFT)
                    {
                        lara_item->state_next = BIKE_MOVING_LEFT;
                    }
                    else if (TrInput & IN_RIGHT)
                    {
                        lara_item->state_next = BIKE_MOVING_RIGHT;
                    }
                    else if (TrInput & IN_JUMP)
                    {
                        if (motorbike->velocity <= 0x5554)
                            lara_item->state_next = BIKE_EMPTY3;
                        else
                            lara_item->state_next = BIKE_STOP;
                    }
                    else if (TrInput & IN_BACK && motorbike->velocity <= 0x800)
                    {
                        lara_item->state_next = BIKE_MOVING_BACK;
                    }
                }
                else
                {
                    lara_item->state_next = BIKE_IDLE;
                }
                break;
            case BIKE_MOVING_LEFT:
                if (motorbike->velocity & -256)
                {
                    if (TrInput & IN_RIGHT || !(TrInput & IN_LEFT))
                        lara_item->state_next = BIKE_MOVING_FRONT;
                }
                else
                {
                    lara_item->state_next = BIKE_IDLE;
                }
                break;
            case BIKE_MOVING_BACK:
                if (!(TrInput & IN_BACK))
                    lara_item->state_next = BIKE_IDLE;
                else
                    lara_item->state_next = BIKE_MOVING_BACK_LOOP;
                break;
            case BIKE_MOVING_RIGHT:
                if (motorbike->velocity & -256)
                {
                    if (TrInput & IN_LEFT || !(TrInput & IN_RIGHT))
                        lara_item->state_next = BIKE_MOVING_FRONT;
                }
                else
                {
                    lara_item->state_next = BIKE_IDLE;
                }
                break;
            case BIKE_EMPTY3:
            case BIKE_STOP:
            case BIKE_ACCELERATE:
                if (motorbike->velocity & -256)
                {
                    if (TrInput & IN_LEFT)
                        lara_item->state_next = BIKE_MOVING_LEFT;
                    else if (TrInput & IN_RIGHT)
                        lara_item->state_next = BIKE_MOVING_RIGHT;
                }
                else
                {
                    lara_item->state_next = BIKE_IDLE;
                }
                break;
            case BIKE_FALLING:
                if (item->pos.y == item->floor)
                {
                    lara_item->state_next = BIKE_LANDING;

                    int fallspeed_damage = item->fallspeed - 140;
                    if (fallspeed_damage > 0)
                    {
                        if (fallspeed_damage <= 100)
                            lara_item->hit_points -= (-LARA_HITPOINTS * fallspeed_damage * fallspeed_damage) / 10000;
                        else
                            lara_item->hit_points = 0;
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
                    lara_item->state_next = BIKE_MOVING_FRONT;
                break;
            }
        }
        else
        {
            switch (collide)
            {
            case 13:
                lara_item->anim_number = objects[VEHICLE_EXTRA].anim_index + 12;
                lara_item->state_current = BIKE_HITBACK;
                lara_item->state_next = BIKE_HITBACK;
                lara_item->frame_number = anims[lara_item->anim_number].frame_base;
                break;
            case 14:
                lara_item->anim_number = objects[VEHICLE_EXTRA].anim_index + 11;
                lara_item->state_current = BIKE_HITFRONT;
                lara_item->state_next = BIKE_HITFRONT;
                lara_item->frame_number = anims[lara_item->anim_number].frame_base;
                break;
            case 11:
                lara_item->anim_number = objects[VEHICLE_EXTRA].anim_index + 14;
                lara_item->state_current = BIKE_HITRIGHT;
                lara_item->state_next = BIKE_HITRIGHT;
                lara_item->frame_number = anims[lara_item->anim_number].frame_base;
                break;
            default:
            case 12:
                lara_item->anim_number = objects[VEHICLE_EXTRA].anim_index + 13;
                lara_item->state_current = BIKE_HITLEFT;
                lara_item->state_next = BIKE_HITLEFT;
                lara_item->frame_number = anims[lara_item->anim_number].frame_base;
                break;
            }
        }
    }
    else
    {
        if (motorbike->velocity >= 0)
            lara_item->anim_number = objects[VEHICLE_EXTRA].anim_index + 22;
        else
            lara_item->anim_number = objects[VEHICLE_EXTRA].anim_index + 6;
        lara_item->frame_number = anims[lara_item->anim_number].frame_base;
        lara_item->state_current = BIKE_FALLING;
        lara_item->state_next = BIKE_FALLING;
    }

    if (rooms[item->room_number].flags & 1)
    {
        lara_item->state_next = BIKE_EMPTY6;
        lara_item->hit_points = 0;
        MotorBikeExplode(item);
    }
}

int MotorbikeUserControl(ITEM_INFO* item, int height, int* pitch)
{
    MOTORBIKE_INFO* motorbike;
    PHD_VECTOR pos;
    int drive = 0;
    int rotation_speed, vel, newvel;

    motorbike = GetMotorbikeInfo(item);
    if (motorbike->unknown5 < 127)
    {
        motorbike->unknown5 += (GetRandomControl() & 7) + 3;
        if (motorbike->unknown5 > 127)
            motorbike->unknown5 = 127;
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

    if (item->pos.y >= (height - STEP_L))
    {
        if (!motorbike->velocity && CHK_EXI(TrInput, IN_LOOK))
            LookUpDown(); // not used since it ignore the vehicle (lara.skidoo == NO_ITEM) !!

        if (motorbike->velocity > 0)
        {

        }
        else if (motorbike->velocity < 0)
        {

        }

        if (CHK_EXI(TrInput, IN_JUMP))
        {

        }
        else if (CHK_EXI(TrInput, IN_ACTION))
        {

        }
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
    laraitem->state_current = BIKE_IDLE;
    laraitem->state_next = BIKE_IDLE;
    laraitem->anim_number = objects[VEHICLE_EXTRA].anim_index + BIKE_IDLE_ANIM;
    laraitem->frame_number = anims[laraitem->anim_number].frame_base;
    laraitem->gravity_status = FALSE;
    item->anim_number = laraitem->anim_number + (objects[MOTORBIKE].anim_index - objects[VEHICLE_EXTRA].anim_index);
    item->frame_number = laraitem->frame_number + (anims[MOTORBIKE].frame_base - anims[VEHICLE_EXTRA].frame_base);
    item->hit_points = 1;
    item->flags = short(IFLAG_KILLED_ITEM); // hmm... maybe wrong name (it can be IFLAG_CODEBITS) ?
    motorbike->revs = 0;
}

int MotorBikeControl(void)
{
    ITEM_INFO* item;
    MOTORBIKE_INFO* motorbike;
    FLOOR_INFO* floor;
    PHD_VECTOR pos, fl, fr, fm;
    int drive, collide, pitch{0}, dead, height, ceiling;
    short room_number;

    item = &items[lara.skidoo];
    motorbike = GetMotorbikeInfo(item);
    drive = -1;
    collide = MotorBikeDynamics(item);

    pos.x = item->pos.x;
    pos.y = item->pos.y;
    pos.z = item->pos.z;

    room_number = item->room_number;

    floor = GetFloor(item->pos.x, item->pos.y, item->pos.z, &room_number);
    height = GetHeight(floor, item->pos.x, item->pos.y, item->pos.z);
    ceiling = GetCeiling(floor, item->pos.x, item->pos.y, item->pos.z);

    int hfl = TestMotorbikeHeight(item, 500, -350, &fl);
    int hfr = TestMotorbikeHeight(item, 500, 128, &fr);
    int hfm = TestMotorbikeHeight(item, -500, 0, &fm);

    TestTriggers(trigger_index, FALSE, FALSE);
    TestTriggers(trigger_index, TRUE, FALSE);

    if (lara_item->hit_points <= 0)
    {
        TrInput &= ~(IN_LEFT | IN_RIGHT | IN_BACK | IN_FORWARD);
        dead = 1;
    }
    else
    {
        dead = 0;
    }

    if (motorbike->flags)
    {
        collide = 0;
    }
    else
    {
        switch (lara_item->state_current)
        {
        case BIKE_ENTER:
        case BIKE_EXIT:
            drive = -1;
            collide = 0;
            break;

        default:
            DrawMotorbikeLight(item);
            drive = MotorbikeUserControl(item, height, &pitch);
            break;
        }
    }

    if (motorbike->velocity || motorbike->revs)
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
    short rotation = motorbike->velocity >> 2;
    motorbike->wheelLeft = rotation;
    motorbike->wheelRight = rotation;
    int newy = item->pos.y;
    item->fallspeed = DoMotorBikeDynamics(height, item->fallspeed, &item->pos.y, 0);

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
            xrot = phd_atan(500, hfm - item->pos.y);
            zrot = phd_atan(350, r2 - fl.y);
        }
    }
    else if (r1 >= (hfl + hfr) >> 1)
    {
        xrot = phd_atan(500, item->pos.y - r1);
        zrot = phd_atan(350, r2 - fl.y);
    }
    else
    {
        xrot = phd_atan(125, newy - item->pos.y);
        zrot = phd_atan(350, r2 - fl.y);
    }
    
    item->pos.x_rot += ((xrot - item->pos.x_rot) >> 2);
    item->pos.z_rot += ((zrot - item->pos.z_rot) >> 2);

    if (motorbike->flags >= 0)
    {
        if (room_number != item->room_number)
        {
            ItemNewRoom(lara.skidoo, room_number);
            ItemNewRoom(lara.item_number, room_number);
        }
        lara_item->pos.x = item->pos.x;
        lara_item->pos.y = item->pos.y;
        lara_item->pos.z = item->pos.z;
        lara_item->pos.y_rot = item->pos.y_rot;
        lara_item->pos.x_rot = item->pos.x_rot;
        lara_item->pos.z_rot = item->pos.z_rot;
        AnimateMotorbike(item, collide, dead);
        AnimateItem(lara_item);
        item->anim_number = lara_item->anim_number + (objects[MOTORBIKE].anim_index - objects[VEHICLE_EXTRA].anim_index);
        item->frame_number = lara_item->frame_number + (anims[item->anim_number].frame_base - anims[lara_item->anim_number].frame_base);
        camera.target_elevation = -5460;

        if (motorbike->flags & 0x40)
        {
            if (item->pos.y == item->floor)
            {
                ExplodingDeath(lara.item_number, -1, 256);
                lara_item->hit_points = 0;
                lara_item->flags = IFLAG_ONESHOT;
                MotorBikeExplode(item);
                return 0;
            }
        }
    }

    if (lara_item->state_current == BIKE_ENTER || lara_item->state_current == BIKE_EXIT)
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
