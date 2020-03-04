#include "framework.h"
#include "../oldobjects.h"
#include "3dsystem/3d_gen.h"
#include "game/box.h"
#include "game/control.h"
#include "game/draw.h"
#include "game/items.h"
#include "game/effect2.h"
#include "game/people.h"
#include "utils/utils.h"
#include "utils/utils_render.h"

enum BADDY_ANIMATION
{
    ABAD_RUN,
    ABAD_RUN_START,
    ABAD_RUN_END,
    ABAD_EMPTY3,
    ABAD_EMPTY4,
    ABAD_EMPTY5,
    ABAD_EMPTY6,
    ABAD_EMPTY7,
    ABAD_EMPTY8,
    ABAD_MONKEY_START,
    ABAD_MONKEY_IDLE,
    ABAD_MONKEY_FORWARD,
    ABAD_MONKEY_IDLE_TO_FORWARD,
    ABAD_MONKEY_STOP_LEFT,
    ABAD_MONKEY_STOP_RIGHT,
    ABAD_MONKEY_FALL_LAND,
    ABAD_MONKEY_PUSHOFF,
    ABAD_EMPTY9,
    ABAD_IDLE,
    ABAD_EMPTY10,
    ABAD_DRAWGUN,
    ABAD_UNDRAWGUN,
    ABAD_DRAWSWORD,
    ABAD_UNDRAWSWORD,
    ABAD_IDLE_TO_ROLLLEFT,
    ABAD_ROLL_LEFT_START,
    ABAD_ROLL_LEFT_MIDDLE,
    ABAD_ROLL_LEFT_END,
    ABAD_ROLL_LEFT_TO_CROUCH,
    ABAD_CROUCH_IDLE,
    ABAD_CROUCH_TO_IDLE,
    ABAD_IDLE_TO_WALK,
    ABAD_WALK,
    ABAD_WALK_TO_RUN,
    ABAD_IDLE_TO_AIM,
    ABAD_AIM,
    ABAD_FIRE,
    ABAD_FIRE_TO_IDLE,
    ABAD_ATK_FRONT,
    ABAD_CROUCH_PICKUP,
    ABAD_IDLE_TO_CROUCH,
    ABAD_HIT_RIGHT,
    ABAD_HIT_RIGHT_TO_LEFT,
    ABAD_HIT_RIGHT_TO_IDLE,
    ABAD_HIT_LEFT,
    ABAD_IDLE_DEATH,
    ABAD_WALK_SWORD_HIT_RIGHT,
    ABAD_IDLE_TO_JUMP_RIGHT,
    ABAD_JUMP_RIGHT_START,
    ABAD_JUMP_RIGHT_MIDDLE,
    ABAD_JUMP_RIGHT_END,
    ABAD_RUN_TO_WALK,
    ABAD_EMPTY11,
    ABAD_EMPTY12,
    ABAD_WALK_STOP_RIGHT,
    ABAD_IDLE_TO_JUMP_FORWARD,
    ABAD_JUMP_FORWARD_1BLOCK,
    ABAD_JUMP_FORWARD_FALL,
    ABAD_JUMP_FORWARD_LAND,
    ABAD_MONKEY_TO_FREEFALL,
    ABAD_FREEFALL,
    ABAD_FREEFALL_DEATH,
    ABAD_CLIMB4,
    ABAD_CLIMB3,
    ABAD_CLIMB2,
    ABAD_JUMPOFF4,
    ABAD_JUMPOFF3,
    ABAD_JUMP_FORWARD_2CLICK,
    ABAD_BLIND,
    ABAD_BLIND_TO_IDLE,
    ABAD_DEAD,
};

enum BADDY_STATE
{
    SBAD_IDLE,
    SBAD_WALK,
    SBAD_RUN,
    SBAD_NOSTATE3,
    SBAD_NOSTATE4,
    SBAD_NOSTATE5,
    SBAD_NOSTATE6,
    SBAD_NOSTATE7,
    SBAD_NOSTATE8,
    SBAD_NOSTATE9,
    SBAD_DRAWGUN,
    SBAD_UNDRAWGUN,
    SBAD_DRAWSWORD,
    SBAD_UNDRAWSWORD,
    SBAD_FIRE,
    SBAD_SWORDHITFRONT,
    SBAD_SWORDHITRIGHT,
    SBAD_SWORDHITLEFT,
    SBAD_MONKEYGRAB,
    SBAD_MONKEYIDLE,
    SBAD_MONKEYFORWARD,
    SBAD_MONKEYPUSHOFF,
    SBAD_MONKEYFALLLAND,
    SBAD_ROLLLEFT,
    SBAD_JUMPRIGHT,
    SBAD_IDLETOCROUCH,
    SBAD_CROUCH,
    SBAD_CROUCHPICKUP,
    SBAD_CROUCHTOIDLE,
    SBAD_WALKSWORDHITRIGHT,
    SBAD_NOSTATE30,
    SBAD_AIM,
    SBAD_DEATH,
    SBAD_JUMPFORWARD_1BLOCK,
    SBAD_JUMPFORWARD_FALL,
    SBAD_MONKEY_TOFREEFALL,
    SBAD_FREEFALL,
    SBAD_FREEFALL_DEATH,
    SBAD_JUMPFORWARD_2BLOCK,
    SBAD_CLIMB4,
    SBAD_CLIMB3,
    SBAD_CLIMB2,
    SBAD_JUMPOFF4,
    SBAD_JUMPOFF3,
    SBAD_BLIND,
};

enum BADDY_VALUE
{
    VBAD_CLIMB2 = 2,
    VBAD_CLIMB3 = 3,
    VBAD_CLIMB4 = 4,
    VBAD_FALL3 = -3,
    VBAD_FALL4 = -4,
    VBAD_SHIFT = 260,
    VBAD_TOUCHBITS = 0x1C000,
    VBAD_DAMAGE = 120,
    VBAD_GUN_DAMAGE = 15,
    VBAD_DETECT_RANGE = SQUARE(WALL_L),
    VBAD_ANGLE_WALK = ANGLE(7),
    VBAD_ANGLE_RUN = ANGLE(11),
    VBAD_RUN_RANGE = SQUARE(WALL_L),
    VBAD_WALK_RANGE = SQUARE(WALL_L * 2 / 3),
    VBAD_ATTACK_RANGE = SQUARE(STEP_L * 2),
};

enum BADDY1_OCB
{
    OCB_IDLE = 0x0,
    OCB_JUMPRIGHT = 0x1,
    OCB_LEFTROLL = 0x2,
    OCB_CROUCH = 0x3,
    OCB_CLIMB4 = 0x4,
    OCB_SLIDE = 0x64, // 100 to 104
    OCB_TRIGGER_BADDY = 0x3E8, // use OCB_TRIGGER()
};

enum BADDY_WEAPON
{
    WBAD1_EMPTY,
    WBAD1_UZI,
    WBAD1_SWORD
};

enum BADDY_MESH
{
    BAD_MESH_BUTT,           // push
    BAD_MESH_LLEG,          // push
    BAD_MESH_LKNEE,
    BAD_MESH_LFOOT,
    BAD_MESH_RLEG,          // pop/push
    BAD_MESH_RKNEE,
    BAD_MESH_RFOOT,
    BAD_MESH_HOLSTERS_SWORD, // pop/push
    BAD_MESH_TORSO,          // pop
    BAD_MESH_LSHOULDER,          // push
    BAD_MESH_LELBOW,
    BAD_MESH_LHAND,
    BAD_MESH_RSHOULDER,          // push/pop
    BAD_MESH_RELBOW,
    BAD_MESH_RHAND,
    BAD_MESH_RSWORD_1,       // push
    BAD_MESH_RSWORD_2,
    BAD_MESH_RSWORD_3,       // push/pop
    BAD_MESH_RSWORD_4,
    BAD_MESH_RSWORD_5,       // push/pop
    BAD_MESH_RSWORD_6,
    BAD_MESH_RSWORD_7,       // pop
    BAD_MESH_RSWORD_8,
    BAD_MESH_HEAD,           // pop
    BAD_MESH_HEAD2,
    BAD_MESH_HEAD3,
    BAD_MESH_HEAD4,
};

static BITE_INFO baddy_gun = { 0, 185, 55, 11 };
static BITE_INFO baddy_sword = { 0, 0, 0, 15 };

short** DrawUziHolsters(ITEM_INFO* item, OBJECT_INFO* obj, OBJECT_INFO* obj_sec, int meshID)
{
    if (item->reserved_4 == WBAD1_UZI)
        return &meshes[MESH(obj_sec, meshID)]; // if sword is drawed then empty holsters.
    else
        return &meshes[MESH(obj, meshID)];
}

short** DrawSwordHolsters(ITEM_INFO* item, OBJECT_INFO* obj, OBJECT_INFO* obj_sec, int meshID)
{
    if (item->reserved_4 == WBAD1_SWORD)
        return &meshes[MESH(obj_sec, meshID)]; // if sword is drawed then empty holsters.
    else
        return &meshes[MESH(obj, meshID)];
}

short** DrawUzi(ITEM_INFO* item, OBJECT_INFO* obj, OBJECT_INFO* obj_sec, int meshID)
{
    if (item->reserved_4 == WBAD1_UZI)
        return &meshes[MESH(obj, meshID)];
    else
        return &meshes[MESH(obj_sec, meshID)];
}

short** DrawSword(ITEM_INFO* item, OBJECT_INFO* obj, OBJECT_INFO* obj_sec, int meshID)
{
    if (item->reserved_4 == WBAD1_SWORD)
        return &meshes[MESH(obj, meshID)];
    else
        return &meshes[MESH(obj_sec, meshID)];
}

void DrawBaddy1(ITEM_INFO* item)
{
    baddy1shape.assign_item(item);
    baddy1shape.assign_object(BADDY_1);
    baddy1shape.assign_object_second(MESHSWAP3);
    baddy1shape.assign_bone();
    baddy1shape.create_interpolate();
    baddy1shape.create_shadow();
    baddy1shape.create_rotation();
    baddy1shape.set_world();
    baddy1shape.set_clipping();
    baddy1shape.set_light();
    baddy1shape.set_list();
    baddy1shape.assign_rotation();
    baddy1shape.add_mesh(RENDER(BAD_MESH_BUTT, false, false));
    baddy1shape.add_mesh(RENDER(0, BAD_MESH_LLEG, true, false));
    baddy1shape.add_mesh(RENDER(1, BAD_MESH_LKNEE, false, false));
    baddy1shape.add_mesh(RENDER(2, BAD_MESH_LFOOT, false, true));
    baddy1shape.add_mesh(RENDER(3, BAD_MESH_RLEG, DrawUziHolsters, true, false));
    baddy1shape.add_mesh(RENDER(4, BAD_MESH_RKNEE, false, false));
    baddy1shape.add_mesh(RENDER(5, BAD_MESH_RFOOT, false, true));
    baddy1shape.add_mesh(RENDER(6, BAD_MESH_HOLSTERS_SWORD, DrawSwordHolsters, true, true));
    baddy1shape.add_mesh(RENDER(7, BAD_MESH_TORSO, false, true, false, false));
    baddy1shape.add_mesh(RENDER(8, BAD_MESH_LSHOULDER, true, false));
    baddy1shape.add_mesh(RENDER(9, BAD_MESH_LELBOW, false, false));
    baddy1shape.add_mesh(RENDER(10, BAD_MESH_LHAND, DrawUzi, false, false));
    baddy1shape.add_flash(&baddy_gun, true);
    baddy1shape.add_mesh(RENDER(11, BAD_MESH_RSHOULDER, true, false));
    baddy1shape.add_mesh(RENDER(12, BAD_MESH_RELBOW, false, false));
    baddy1shape.add_mesh(RENDER(13, BAD_MESH_RHAND, DrawSword, false, false));
    baddy1shape.add_mesh(RENDER(14, BAD_MESH_RSWORD_1, DrawSword, true, false));
    baddy1shape.add_mesh(RENDER(15, BAD_MESH_RSWORD_2, DrawSword, false, true));
    baddy1shape.add_mesh(RENDER(16, BAD_MESH_RSWORD_3, DrawSword, true, false));
    baddy1shape.add_mesh(RENDER(17, BAD_MESH_RSWORD_4, DrawSword, false, true));
    baddy1shape.add_mesh(RENDER(18, BAD_MESH_RSWORD_5, DrawSword, true, false));
    baddy1shape.add_mesh(RENDER(19, BAD_MESH_RSWORD_6, DrawSword, false, true));
    baddy1shape.add_mesh(RENDER(20, BAD_MESH_RSWORD_7, DrawSword, false, false));
    baddy1shape.add_mesh(RENDER(21, BAD_MESH_RSWORD_8, DrawSword, false, true));
    baddy1shape.add_mesh(RENDER(22, BAD_MESH_HEAD, false, true, false, false, false));
    baddy1shape.add_mesh(RENDER(23, BAD_MESH_HEAD2, false, false));
    baddy1shape.add_mesh(RENDER(24, BAD_MESH_HEAD3, false, false));
    baddy1shape.add_mesh(RENDER(25, BAD_MESH_HEAD4, false, false));
    baddy1shape.end_list();
    baddy1shape.end_world();
}

// true if state_current is monkeying..
static bool IsBaddyMonkey(ITEM_INFO* item)
{
    return (item->state_current == SBAD_MONKEYGRAB
    ||      item->state_current == SBAD_MONKEYIDLE
    ||      item->state_current == SBAD_MONKEYFORWARD
    ||      item->state_current == SBAD_MONKEYPUSHOFF
    ||      item->state_current == SBAD_MONKEYFALLLAND);
}

static bool GetBaddyFrameToShot(ITEM_INFO* item)
{
    short   frame = GetCurrentFrame(item);
    return (frame == 1
    ||      frame == 3
    ||      frame == 5
    ||      frame == 7
    ||      frame == 9
    ||      frame == 11
    );
}

void InitialiseBaddy(short itemNumber)
{
    ITEM_INFO* item;
    OBJECT_INFO* obj;

    item = &items[itemNumber];
    item->reserved_2 = MAXSHORT;
    item->reserved_3 = 24;        // default ammo
    item->reserved_4 = WBAD1_UZI; // default weapon

    obj = &objects[item->object_number];

    if (item->ocb_bits == OCB_IDLE)
    {
        item->current_anim = obj->anim_index + ABAD_IDLE;
        item->current_frame = anims[item->current_anim].frame_base;
        item->state_current = SBAD_IDLE;
        item->state_next = SBAD_IDLE;
        return;
    }

    if (item->ocb_bits & OCB_JUMPRIGHT)
    {
        item->current_anim = obj->anim_index + ABAD_IDLE_TO_JUMP_RIGHT;
        item->current_frame = anims[item->current_anim].frame_base;
        item->state_current = SBAD_JUMPRIGHT;
        item->state_next = SBAD_JUMPRIGHT;
        return;
    }
    
    if (item->ocb_bits & OCB_LEFTROLL)
    {
        item->current_anim = obj->anim_index + ABAD_IDLE_TO_ROLLLEFT;
        item->current_frame = anims[item->current_anim].frame_base;
        item->state_current = SBAD_ROLLLEFT;
        item->state_next = SBAD_ROLLLEFT;
        return;
    }

    if (item->ocb_bits & OCB_CROUCH)
    {
        item->current_anim = obj->anim_index + ABAD_CROUCH_IDLE;
        item->current_frame = anims[item->current_anim].frame_base;
        item->state_current = SBAD_CROUCH;
        item->state_next = SBAD_CROUCH;
        return;
    }

    if (item->ocb_bits & OCB_CLIMB4)
    {
        item->current_anim = obj->anim_index + ABAD_CLIMB4;
        item->current_frame = anims[item->current_anim].frame_base;
        item->state_next = SBAD_CLIMB4;
        item->state_current = SBAD_CLIMB4;
        item->pos.x += SINN(item->pos.y_rot) << WALL_SHIFT >> W2V_SHIFT;
        item->pos.z += COSN(item->pos.y_rot) << WALL_SHIFT >> W2V_SHIFT;
        return;
    }

    if (item->ocb_bits > OCB_SLIDE)
    {
        item->current_anim = obj->anim_index + ABAD_CROUCH_IDLE;
        item->current_frame = anims[item->current_anim].frame_base;
        item->state_next = SBAD_CROUCH;
        item->state_current = SBAD_CROUCH;
        item->pos.x += SINN(item->pos.y_rot) << WALL_SHIFT >> W2V_SHIFT;
        item->pos.z += COSN(item->pos.y_rot) << WALL_SHIFT >> W2V_SHIFT;
    }
}

// baddy is equiped with uzi by default and have 24 ammo !
void Baddy1Control(short itemNumber)
{
    if (!CreatureActive(itemNumber))
        return;

    FLOOR_INFO* floor;
    ITEM_INFO* item;
    CREATURE_INFO* bad;
    OBJECT_INFO* obj;
    OBJECT_FOUND found;
    PHD_VECTOR pos;
    OBJ_ROTATION rot;
    ENTITY_JUMP* pjump;
    BOUNDS* bounds;
    AI_INFO info, lara_info;
    int height, ceiling;
    short roomNumber;
    short at;
    short tilt, angle;
    bool roll, jump;
    short ls;

    item = &items[itemNumber];
    if (item->data == NULL)
        return;
    obj = &objects[item->object_number];
    bad = (CREATURE_INFO*)item->data;
    tilt = 0;
    angle = 0;
    rot.head_x = 0;
    rot.head_y = 0;
    rot.torso_x = 0;
    rot.torso_y = 0;

    pjump = &CheckMegaJumpPossibility(item, bad);
    if (item->hit_points <= 0)
    {
        rot.head_x = 0;
        rot.head_y = 0;
        rot.torso_x = 0;
        rot.torso_y = 0;
        bad->LOT.is_jumping = FALSE;

        roomNumber = item->room_number;
        floor = GetFloor(item->pos.x, item->pos.y, item->pos.z, &roomNumber);
        height = GetHeight(floor, item->pos.x, item->pos.y, item->pos.z);
        item->floor = height;

        switch (item->state_current)
        {
            case SBAD_MONKEYGRAB:
            case SBAD_MONKEYFORWARD:
            case SBAD_MONKEYIDLE:
                item->current_anim = obj->anim_index + ABAD_MONKEY_TO_FREEFALL;
                item->current_frame = anims[item->current_anim].frame_base;
                item->state_current = SBAD_MONKEY_TOFREEFALL;
                item->speed = 0;
                break;
            case SBAD_DEATH:
                item->gravity_status = TRUE;
                bad->LOT.is_jumping = TRUE;
                if (item->pos.y >= item->floor)
                {
                    item->pos.y = item->floor;
                    item->gravity_status = FALSE;
                    item->fallspeed = 0;
                }
                break;
            case SBAD_MONKEY_TOFREEFALL:
                item->state_next = SBAD_FREEFALL;
                item->gravity_status = FALSE;
                break;
            case SBAD_FREEFALL:
                item->gravity_status = TRUE;
                if (item->pos.y >= height)
                {
                    item->pos.y = height;
                    item->gravity_status = FALSE;
                    item->fallspeed = 0;
                    item->state_next = SBAD_FREEFALL_DEATH;
                }
                break;
            case SBAD_FREEFALL_DEATH:
                item->pos.y = height;
                break;
            default:
                bad->LOT.is_jumping = TRUE; // permit the falling in pitfall instead of bugged move
                item->current_anim = obj->anim_index + ABAD_IDLE_DEATH;
                item->current_frame = anims[item->current_anim].frame_base;
                item->state_current = SBAD_DEATH;
                
                Spawner(item);
                break;
        }
    }
    else
    {
        found = FoundItem(item, bad, SMALLMEDI_ITEM, UZI_AMMO_ITEM);

        if (item->ai_bits)
            GetAITarget(bad);
        else if (!found.target)
            bad->enemy = lara_item;
        else
            bad->enemy = found.target;

        CreatureAIInfo(item, &info); // define the default target to lara.

        if (bad->enemy == lara_item)
        {
            lara_info.angle = info.angle;
            lara_info.ahead = info.ahead;
            lara_info.distance = info.distance;
        }
        else
        {
            pos.x = found.target->pos.x - item->pos.x;
            pos.y = found.target->pos.y - item->pos.y;
            pos.z = found.target->pos.z - item->pos.z;
            at = phd_atan(pos.z, pos.x) - item->pos.y_rot;
            lara_info.angle = at;
            lara_info.ahead = (at > -ANGLE(90) && at < ANGLE(90));
            lara_info.distance = SQUARE(pos.x) + SQUARE(pos.z);
        }

        GetCreatureMood(item, &info, VIOLENT);

        if (lara.skidoo != NO_ITEM) // info.bite will not be ok !!
            bad->mood = ESCAPE_MOOD;

        CreatureMood(item, &info, VIOLENT);
        angle = CreatureTurn(item, bad->maximum_turn);

        if (item->hit_status || (lara_info.distance < VBAD_DETECT_RANGE || (TargetVisible(item, &lara_info) && abs(lara_item->pos.y - item->pos.y) < WALL_L)))
            bad->alerted = TRUE;

        // check for jump and roll
        if (item != lara.target || lara_info.distance <= 942 || lara_info.angle <= -10240 || lara_info.angle >= 10240)
        {
            jump = false;
            roll = false;
        }
        else
        {
            jump = CheckJumpPossibility(item);
            roll = CheckRollPossibility(item);
        }

        short frame = GetCurrentFrame(item); // get the current frame from the current anim.
        switch (item->state_current)
        {
            /// BASIC
            case SBAD_IDLE:
                bad->LOT.is_jumping = FALSE;
                bad->LOT.is_monkeying = FALSE;
                bad->maximum_turn = 0;
                bad->flags = 0;

                if (info.ahead && CHK_NOP(item->ai_bits, GUARD))
                {
                    rot.torso_y = info.angle;
                    rot.torso_x = info.x_angle;
                }

                if (item->state_required)
                {
                    item->state_next = item->state_required;
                    break;
                }

                // monkey state need to be unarmed !
                // dont switch to another weapon
                if (!bad->monkey_ahead || !IsBaddyMonkey(item))
                {
                    switch (item->reserved_4)
                    {
                        case WBAD1_EMPTY:
                            if (item->reserved_3 >= 1)
                                item->state_next = SBAD_DRAWGUN;
                            else
                                item->state_next = SBAD_DRAWSWORD;
                            break;

                        case WBAD1_SWORD:
                            if (item->reserved_3 >= 1)
                                item->state_next = SBAD_UNDRAWSWORD;
                            break;

                        case WBAD1_UZI:
                            if (item->reserved_3 <= 0)
                                item->state_next = SBAD_UNDRAWGUN;
                            break;
                    }
                }
                
                if (CHK_ANY(item->ai_bits, GUARD))
                {
                    rot.head_y = AIGuard(bad);
                    item->state_next = SBAD_IDLE;
                }
                else if (Targetable(item, &info) && item->reserved_3 >= 1)
                {
                    if (item->reserved_4 == WBAD1_UZI)
                        item->state_next = SBAD_AIM;
                    else
                        item->state_next = SBAD_DRAWGUN;
                }
                else
                {
                    if (CHK_ANY(item->ai_bits, MODIFY))
                    {
                        item->state_next = SBAD_IDLE;
                        if (item->floor > item->pos.y + (STEP_L * 3))
                            item->ai_bits &= ~(MODIFY);
                    }
                    else if (bad->monkey_ahead)
                    {
                        // the baddy need to have no weapon to handle the monkey animation
                        if (item->reserved_4 == WBAD1_SWORD || item->reserved_4 == WBAD1_UZI)
                        {
                            if (item->reserved_4 == WBAD1_SWORD)
                                item->state_next = SBAD_UNDRAWSWORD;
                            else if (item->reserved_4 == WBAD1_UZI)
                                item->state_next = SBAD_UNDRAWGUN;
                            item->reserved_4 = WBAD1_EMPTY;
                        }

                        // when the baddy is hand free
                        roomNumber = item->room_number;
                        floor = GetFloor(item->pos.x, item->pos.y, item->pos.z, &roomNumber);
                        height = GetHeight(floor, item->pos.x, item->pos.y, item->pos.z);
                        ceiling = GetCeiling(floor, item->pos.x, item->pos.y, item->pos.z);
                        bounds = GetBoundsAccurate(item);

                        // TODO: finish monkey
                    }
                    else if (pjump->can_jump_1click || pjump->can_jump_2click)
                    {
                        bad->maximum_turn = 0;
                        item->current_anim = obj->anim_index + ABAD_IDLE_TO_JUMP_FORWARD;
                        item->current_frame = anims[item->current_anim].frame_base;
                        item->state_current = SBAD_JUMPFORWARD_1BLOCK;
                        if (!pjump->can_jump_2click)
                            item->state_next = SBAD_JUMPFORWARD_1BLOCK;
                        else
                            item->state_next = SBAD_JUMPFORWARD_2BLOCK;
                        bad->LOT.is_jumping = TRUE;
                    }
                    // check if enemy is smallmedikit or uzi ammo
                    // if true and the distance if correct go pickup animation
                    else if (bad->enemy &&
                            (bad->enemy->object_number == SMALLMEDI_ITEM || bad->enemy->object_number == UZI_AMMO_ITEM) && info.distance < VBAD_DETECT_RANGE)
                    {
                        AlignItemToTarget(item, bad->enemy);
                        item->state_next = SBAD_IDLETOCROUCH;
                        item->state_required = SBAD_CROUCHPICKUP;
                    }
                    else
                    {
                        if (roll)
                        {
                            bad->maximum_turn = 0;
                            item->state_next = SBAD_ROLLLEFT;
                        }
                        else if (jump)
                        {
                            bad->maximum_turn = 0;
                            item->state_next = SBAD_JUMPRIGHT;
                        }
                        else if (!bad->enemy || bad->enemy <= 0 || info.distance >= VBAD_WALK_RANGE)
                        {
                            item->state_next = SBAD_WALK;
                        }
                        else if (info.distance >= VBAD_ATTACK_RANGE && item->reserved_4 == WBAD1_SWORD)
                        {
                            item->state_next = SBAD_SWORDHITFRONT;
                        }
                        else if ((GetRandomControl() & 1) && item->reserved_4 == WBAD1_SWORD)
                        {
                            item->state_next = SBAD_SWORDHITLEFT;
                        }
                        else if (item->reserved_4 == WBAD1_SWORD)
                        {
                            item->state_next = SBAD_SWORDHITRIGHT;
                        }
                    }
                }
                break;
            case SBAD_WALK:
                bad->LOT.is_jumping = FALSE;
                bad->LOT.is_monkeying = FALSE;
                bad->maximum_turn = VBAD_ANGLE_WALK;
                bad->flags = 0;

                if (lara_info.ahead)
                    rot.head_y = lara_info.angle;
                else if (info.ahead)
                    rot.head_y = info.angle;

                // monkey state need to be unarmed !
                // dont switch to another weapon
                if (!bad->monkey_ahead || !IsBaddyMonkey(item))
                {
                    switch (item->reserved_4)
                    {
                        case WBAD1_EMPTY:
                            if (item->reserved_3 >= 1)
                            {
                                item->state_required = SBAD_DRAWGUN;
                                item->state_next = SBAD_IDLE;
                            }
                            else
                            {
                                item->state_required = SBAD_DRAWSWORD;
                                item->state_next = SBAD_IDLE;
                            }
                            break;

                        case WBAD1_SWORD:
                            if (item->reserved_3 >= 1)
                            {
                                item->state_required = SBAD_UNDRAWSWORD;
                                item->state_next = SBAD_IDLE;
                            }
                            break;

                        case WBAD1_UZI:
                            if (item->reserved_3 <= 0)
                            {
                                item->state_required = SBAD_UNDRAWGUN;
                                item->state_next = SBAD_IDLE;
                            }
                            break;
                    }
                }

                // uzi have ammo ? then if lara can be targeted go to aim state
                if (Targetable(item, &info) && item->reserved_3 > 0)
                {
                    item->state_next = SBAD_IDLE;
                }
                else if(pjump->can_jump_1click || pjump->can_jump_2click)
                {
                    bad->maximum_turn = 0;
                    item->state_next = SBAD_IDLE;
                }
                else
                {
                    if (bad->monkey_ahead || bad->reached_goal)
                    {
                        item->state_next = SBAD_IDLE;
                    }
                    else if (info.ahead && info.distance < VBAD_ATTACK_RANGE)
                    {
                        item->state_next = SBAD_IDLE;
                    }
                    else if (info.bite && info.distance < VBAD_WALK_RANGE)
                    {
                        item->state_next = SBAD_IDLE;
                    }
                    else if (info.distance < VBAD_RUN_RANGE && item->reserved_4 == WBAD1_SWORD)
                    {
                        item->state_next = SBAD_WALKSWORDHITRIGHT;
                    }
                    else
                    {
                        if (roll || jump)
                            item->state_next = SBAD_IDLE;
                        else if (bad->mood == ATTACK_MOOD && !bad->jump_ahead && info.distance > VBAD_RUN_RANGE)
                            item->state_next = SBAD_RUN;
                    }
                }
                break;
            case SBAD_RUN:
                bad->maximum_turn = VBAD_ANGLE_RUN;
                tilt = (angle / 2);

                if (Targetable(item, &info) && item->reserved_3 >= 1)
                    item->state_next = SBAD_IDLE;
                else if (CHK_ANY(item->ai_bits, GUARD))
                    item->state_next = SBAD_IDLE;
                else if (pjump->can_jump_1click || pjump->can_jump_2click)
                    item->state_next = SBAD_IDLE;
                else if (bad->monkey_ahead || bad->jump_ahead)
                    item->state_next = SBAD_IDLE;
                else if (info.distance < 0x5C0A4) // attack ?
                    item->state_next = SBAD_IDLE;
                else if (info.distance < VBAD_RUN_RANGE)
                    item->state_next = SBAD_WALK;
                break;

            /// ATTACK
            case SBAD_AIM:
                bad->maximum_turn = 0;

                if (info.ahead)
                {
                    rot.torso_y = info.angle;
                    rot.torso_x = info.x_angle;
                }

                // this baddy have ammo ?
                if (Targetable(item, &info) && item->reserved_3 >= 1)
                    item->state_next = SBAD_FIRE;
                else
                    item->state_next = SBAD_IDLE;
                break;
            case SBAD_FIRE:
                bad->maximum_turn = 0;

                if (info.ahead)
                {
                    rot.torso_y = info.angle;
                    rot.torso_x = info.x_angle;
                }

                // 1 - 3 - 5 - 7 - 9 - 11
                if (GetBaddyFrameToShot(item))
                {
                    item->fired_weapon = 1;
                    if (CHK_NOP(item->ai_bits, MODIFY))
                        item->reserved_3--; // decrease ammo.
                    if (!ShotLara(item, &info, &baddy_gun, rot.torso_y, VBAD_GUN_DAMAGE))
                        item->state_next = SBAD_IDLE;
                }
                break;

            case SBAD_SWORDHITFRONT:
            case SBAD_SWORDHITLEFT:
            case SBAD_WALKSWORDHITRIGHT:
                bad->maximum_turn = 0;
                if (info.ahead)
                {
                    rot.torso_y = info.angle;
                    rot.torso_x = info.x_angle;
                }

                if (!bad->flags && (item->touch_bits & VBAD_TOUCHBITS))
                {
                    CreatureEffectAlternate(item, &baddy_sword, 10, item->pos.y_rot, DoBloodSplat);
                    lara_item->hit_points -= VBAD_DAMAGE;
                    lara_item->hit_status = TRUE;
                    bad->flags = 1;
                }

                if (item->current_frame == anims[item->current_anim].frame_end - 1)
                    bad->flags = 0;
                break;

            case SBAD_SWORDHITRIGHT:
                if (info.distance < VBAD_ATTACK_RANGE)
                    item->state_next = SBAD_SWORDHITLEFT;

                bad->maximum_turn = 0;
                if (info.ahead)
                {
                    rot.torso_y = info.angle;
                    rot.torso_x = info.x_angle;
                }

                if (!bad->flags && (item->touch_bits & VBAD_TOUCHBITS))
                {
                    CreatureEffectAlternate(item, &baddy_sword, 10, item->pos.y_rot, DoBloodSplat);
                    lara_item->hit_points -= VBAD_DAMAGE;
                    lara_item->hit_status = TRUE;
                    bad->flags = 1;
                }

                if (item->current_frame == anims[item->current_anim].frame_end - 1)
                    bad->flags = 0;
                break;

            /// MONKEY
            case SBAD_MONKEYIDLE:
                rot.torso_y = 0;
                rot.torso_x = 0;
                rot.head_y = 0;
                rot.head_x = 0;
                bad->LOT.is_monkeying = TRUE;
                bad->LOT.is_jumping = TRUE;
                bad->maximum_turn = 0;
                bad->flags = 0;
                
                ls = lara_item->state_current;
                if (lara_info.ahead && lara_info.distance < VBAD_WALK_RANGE && ((ls > 74 && ls < 80) || ls == 82 || ls == 83))
                {
                    item->state_next = SBAD_MONKEYPUSHOFF;
                }
                else if (item->box_number != bad->LOT.target_box)
                {
                    item->state_next = SBAD_MONKEYFORWARD;
                }
                else
                {
                    item->state_next = SBAD_MONKEYFALLLAND;
                    bad->LOT.is_jumping = FALSE;
                    bad->LOT.is_monkeying = FALSE;
                }

                break;
            case SBAD_MONKEYFORWARD:
                rot.torso_y = 0;
                rot.torso_x = 0;
                rot.head_y = 0;
                rot.head_x = 0;
                bad->maximum_turn = VBAD_ANGLE_WALK;
                bad->flags = 0;

                roomNumber = item->room_number;
                floor = GetFloor(item->pos.x, item->pos.y, item->pos.z, &roomNumber);
                height = GetHeight(floor, item->pos.x, item->pos.y, item->pos.z);
                ceiling = GetCeiling(floor, item->pos.x, item->pos.y, item->pos.z);
                ls = lara_item->state_current;

                if (item->box_number == bad->LOT.target_box || !bad->monkey_ahead)
                {
                    if (ceiling <= height - 1536)
                        item->state_next = SBAD_MONKEYIDLE;
                }

                if (lara_info.ahead && lara_info.distance < VBAD_WALK_RANGE)
                {
                    if ((ls > 74 && ls < 80) || ls == 82 || ls == 83)
                        item->state_next = SBAD_MONKEYIDLE;
                }
                break;
            case SBAD_MONKEYPUSHOFF:
                rot.torso_y = 0;
                rot.torso_x = 0;
                rot.head_y = 0;
                rot.head_x = 0;
                bad->LOT.is_monkeying = TRUE;
                bad->LOT.is_jumping = TRUE;
                bad->maximum_turn = VBAD_ANGLE_WALK;
                bad->flags = 0;

                if (!bad->flags && item->touch_bits)
                {
                    lara_item->state_current = 28;
                    lara_item->state_next = 28;
                    lara_item->current_anim = 28;
                    lara_item->current_frame = anims[lara_item->current_anim].frame_base;
                    lara_item->gravity_status = TRUE;
                    lara_item->speed = 2;
                    lara_item->fallspeed = 1;
                    lara_item->pos.y += 192;
                    lara.gun_status = 0;
                    bad->flags = 1;
                }
                break;
            
            /// JUMP/ROLL
            case SBAD_ROLLLEFT:
            case SBAD_JUMPRIGHT:
                bad->alerted = FALSE;
                bad->maximum_turn = 0;
                item->status = FITEM_ACTIVE;
                break;
            case SBAD_JUMPFORWARD_1BLOCK:
            case SBAD_JUMPFORWARD_2BLOCK:
                if (item->reserved_1 < 0 && item->current_anim != (obj->anim_index + ABAD_IDLE_TO_JUMP_FORWARD))
                    item->reserved_1 += 2; // ??
                break;

            /// CROUCH
            case SBAD_CROUCH:
                if (item->reserved_1)
                {
                    if (info.distance < 0x718E4)
                    {
                        item->state_next = SBAD_CROUCHTOIDLE;
                        bad->enemy = nullptr;
                    }
                }
                else if (bad->enemy &&
                        (bad->enemy->object_number == SMALLMEDI_ITEM || bad->enemy->object_number == UZI_AMMO_ITEM) &&
                        info.distance < VBAD_DETECT_RANGE)
                {
                    item->state_next = SBAD_CROUCHPICKUP;
                }
                else if (bad->alerted)
                {
                    item->state_next = SBAD_CROUCHTOIDLE;
                }
                break;
            case SBAD_CROUCHPICKUP:
                if (bad->enemy && frame == 9)
                {
                    if (bad->enemy->object_number != SMALLMEDI_ITEM && bad->enemy->object_number != UZI_AMMO_ITEM)
                        break;

                    if (bad->enemy->room_number == NO_ROOM || bad->enemy->status == FITEM_INVISIBLE || CHK_ANY(bad->enemy->flags, IFLAG_KILLED_ITEM))
                    {
                        bad->enemy = nullptr;
                        break;
                    }

                    if (bad->enemy->object_number == SMALLMEDI_ITEM)
                    {
                        item->hit_points += objects[item->object_number].hit_points >> 1;
                    }
                    else if (bad->enemy->object_number == UZI_AMMO_ITEM)
                    {
                        item->reserved_3 += 24;
                    }

                    KillItem(found.item_number);
                    bad->enemy = nullptr;
                }
                break;

            /// BLIND
            case SBAD_BLIND:
                if (!lara.blind_timer) // no random anymore, it's not like this in reality :D
                    item->state_next = SBAD_IDLE;
                break;

            /// DRAW/UNDRAW
            case SBAD_DRAWGUN:
                if (frame == 21)
                    item->reserved_4 = WBAD1_UZI;
                break;
            case SBAD_UNDRAWGUN:
                if (frame == 21)
                    item->reserved_4 = WBAD1_EMPTY; // to not switch the render directly
                break;

            case SBAD_DRAWSWORD:
                if (frame == 12)
                    item->reserved_4 = WBAD1_SWORD;
                break;
            case SBAD_UNDRAWSWORD:
                if (frame == 22)
                    item->reserved_4 = WBAD1_EMPTY; // to not switch the render directly
                break;

            /// CLIMB/JUMPOFF (stop rotation)
            case SBAD_CLIMB2:
            case SBAD_CLIMB3:
            case SBAD_CLIMB4:
            case SBAD_JUMPOFF3:
            case SBAD_JUMPOFF4:
                bad->maximum_turn = 0;
                break;
        }
    }

    DrawFlashWithSmoke(item, &baddy_gun);
    CreatureTilt(item, tilt);
    CreatureJoint(item, 0, rot.torso_y);
    CreatureJoint(item, 1, rot.torso_x);
    CreatureJoint(item, 2, rot.head_y);
    CreatureJoint(item, 3, rot.head_x);

    // for any train level.
    if (CHK_ANY(gf_level_flags, SLEV_TRAIN) && item->pos.y > -STEP_L)
    {

    }

    if (lara.blind_timer > 16)
    {
        bad->maximum_turn = 0;
        item->current_anim = obj->anim_index + ABAD_BLIND;
        item->current_frame = anims[item->current_anim].frame_base;
        item->state_current = SBAD_BLIND;
        item->state_next = SBAD_BLIND;
    }
    // can climb ?
    else if (!(item->state_current >= SBAD_DEATH && item->state_current <= SBAD_JUMPOFF3) && !(item->state_current >= SBAD_MONKEYGRAB && item->state_current <= SBAD_MONKEYFALLLAND))
    {
        switch (CreatureVault(itemNumber, angle, 2, VBAD_SHIFT))
        {
            case VBAD_FALL3:
                item->current_anim = obj->anim_index + ABAD_JUMPOFF3;
                item->current_frame = anims[item->current_anim].frame_base;
                item->state_current = SBAD_JUMPOFF3;
                item->state_next = SBAD_JUMPOFF3;
                break;
            case VBAD_FALL4:
                item->current_anim = obj->anim_index + ABAD_JUMPOFF4;
                item->current_frame = anims[item->current_anim].frame_base;
                item->state_current = SBAD_JUMPOFF4;
                item->state_next = SBAD_JUMPOFF4;
                break;
            case VBAD_CLIMB2:
                item->current_anim = obj->anim_index + ABAD_CLIMB2;
                item->current_frame = anims[item->current_anim].frame_base;
                item->state_current = SBAD_CLIMB2;
                item->state_next = SBAD_CLIMB2;
                break;
            case VBAD_CLIMB3:
                item->current_anim = obj->anim_index + ABAD_CLIMB3;
                item->current_frame = anims[item->current_anim].frame_base;
                item->state_current = SBAD_CLIMB3;
                item->state_next = SBAD_CLIMB3;
                break;
            case VBAD_CLIMB4:
                item->current_anim = obj->anim_index + ABAD_CLIMB4;
                item->current_frame = anims[item->current_anim].frame_base;
                item->state_current = SBAD_CLIMB4;
                item->state_next = SBAD_CLIMB4;
                break;
        }
    }
    // animate entity (if not climbing !)
    else
    {
        CreatureAnimation(itemNumber, angle, tilt);
    }
}