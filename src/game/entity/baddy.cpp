#include "framework.h"
#include "../oldobjects.h"
#include "3dsystem/3d_gen.h"
#include "3dsystem/3d_gen_a.h"
#include "game/box.h"
#include "game/draw.h"
#include "game/control.h"
#include "game/effect2.h"
#include "game/items.h"
#include "game/people.h"
#include "game/sphere.h"
#include "specific/output.h"
#include "specific/specific.h"
#include "utils/utils.h"

enum BADDY1_ANIMATION
{
    ABAD1_RUN,
    ABAD1_RUN_START,
    ABAD1_RUN_END,
    ABAD1_EMPTY3,
    ABAD1_EMPTY4,
    ABAD1_EMPTY5,
    ABAD1_EMPTY6,
    ABAD1_EMPTY7,
    ABAD1_EMPTY8,
    ABAD1_MONKEY_START,
    ABAD1_MONKEY_IDLE,
    ABAD1_MONKEY_FORWARD,
    ABAD1_MONKEY_IDLE_TO_FORWARD,
    ABAD1_MONKEY_STOP_LEFT,
    ABAD1_MONKEY_STOP_RIGHT,
    ABAD1_MONKEY_FALL_LAND,
    ABAD1_MONKEY_PUSHOFF,
    ABAD1_EMPTY9,
    ABAD1_IDLE,
    ABAD1_EMPTY10,
    ABAD1_DRAWGUN,
    ABAD1_UNDRAWGUN,
    ABAD1_DRAWSWORD,
    ABAD1_UNDRAWSWORD,
    ABAD1_IDLE_TO_ROLLLEFT,
    ABAD1_ROLL_LEFT_START,
    ABAD1_ROLL_LEFT_MIDDLE,
    ABAD1_ROLL_LEFT_END,
    ABAD1_ROLL_LEFT_TO_CROUCH,
    ABAD1_CROUCH_IDLE,
    ABAD1_CROUCH_TO_IDLE,
    ABAD1_IDLE_TO_WALK,
    ABAD1_WALK,
    ABAD1_WALK_TO_RUN,
    ABAD1_IDLE_TO_AIM,
    ABAD1_AIM,
    ABAD1_FIRE,
    ABAD1_FIRE_TO_IDLE,
    ABAD1_ATK_FRONT,
    ABAD1_CROUCH_PICKUP,
    ABAD1_IDLE_TO_CROUCH,
    ABAD1_HIT_RIGHT,
    ABAD1_HIT_RIGHT_TO_LEFT,
    ABAD1_HIT_RIGHT_TO_IDLE,
    ABAD1_HIT_LEFT,
    ABAD1_IDLE_DEATH,
    ABAD1_WALK_SWORD_HIT_RIGHT,
    ABAD1_IDLE_TO_JUMP_RIGHT,
    ABAD1_JUMP_RIGHT_START,
    ABAD1_JUMP_RIGHT_MIDDLE,
    ABAD1_JUMP_RIGHT_END,
    ABAD1_RUN_TO_WALK,
    ABAD1_EMPTY11,
    ABAD1_EMPTY12,
    ABAD1_WALK_STOP_RIGHT,
    ABAD1_IDLE_TO_JUMP_FORWARD,
    ABAD1_JUMP_FORWARD_1BLOCK,
    ABAD1_JUMP_FORWARD_FALL,
    ABAD1_JUMP_FORWARD_LAND,
    ABAD1_MONKEY_TO_FREEFALL,
    ABAD1_FREEFALL,
    ABAD1_FREEFALL_DEATH,
    ABAD1_CLIMB4,
    ABAD1_CLIMB3,
    ABAD1_CLIMB2,
    ABAD1_JUMPOFF4,
    ABAD1_JUMPOFF3,
    ABAD1_JUMP_FORWARD_2CLICK,
    ABAD1_BLIND,
    ABAD1_BLIND_TO_IDLE,
    ABAD1_DEAD,
};

enum BADDY_STATE
{
    SBAD1_IDLE,
    SBAD1_WALK,
    SBAD1_RUN,
    SBAD1_NOSTATE3,
    SBAD1_NOSTATE4,
    SBAD1_NOSTATE5,
    SBAD1_NOSTATE6,
    SBAD1_NOSTATE7,
    SBAD1_NOSTATE8,
    SBAD1_NOSTATE9,
    SBAD1_DRAWGUN,
    SBAD1_UNDRAWGUN,
    SBAD1_DRAWSWORD,
    SBAD1_UNDRAWSWORD,
    SBAD1_FIRE,
    SBAD1_SWORDHITFRONT,
    SBAD1_SWORDHITRIGHT,
    SBAD1_SWORDHITLEFT,
    SBAD1_MONKEYGRAB,
    SBAD1_MONKEYIDLE,
    SBAD1_MONKEYFORWARD,
    SBAD1_MONKEYPUSHOFF,
    SBAD1_MONKEYFALLLAND,
    SBAD1_ROLLLEFT,
    SBAD1_JUMPRIGHT,
    SBAD1_IDLETOCROUCH,
    SBAD1_CROUCH,
    SBAD1_CROUCHPICKUP,
    SBAD1_CROUCHTOIDLE,
    SBAD1_WALKSWORDHITRIGHT,
    SBAD1_NOSTATE30,
    SBAD1_AIM,
    SBAD1_DEATH,
    SBAD1_JUMPFORWARD_1BLOCK,
    SBAD1_JUMPFORWARD_FALL,
    SBAD1_MONKEY_TOFREEFALL,
    SBAD1_FREEFALL,
    SBAD1_FREEFALL_DEATH,
    SBAD1_JUMPFORWARD_2BLOCK,
    SBAD1_CLIMB4,
    SBAD1_CLIMB3,
    SBAD1_CLIMB2,
    SBAD1_JUMPOFF4,
    SBAD1_JUMPOFF3,
    SBAD1_BLIND,
};

enum BADDY1_VALUE
{
    VBAD1_CLIMB2 = 2,
    VBAD1_CLIMB3 = 3,
    VBAD1_CLIMB4 = 4,
    VBAD1_FALL3 = -3,
    VBAD1_FALL4 = -4,
    VBAD1_SHIFT = 260,
    VBAD1_TOUCHBITS = 0x1C000,
    VBAD1_DAMAGE = 120,
    VBAD1_GUN_DAMAGE = 15,
    VBAD1_PICKUP_RANGE = SQUARE(STEP_L * 3),
    VBAD1_ANGLE_WALK = ANGLE(7),
    VBAD1_ANGLE_RUN = ANGLE(11),
    VBAD1_RUN_RANGE = SQUARE(WALL_L),
    VBAD1_WALK_RANGE = SQUARE(WALL_L * 2 / 3),
    VBAD1_ATTACK_RANGE = SQUARE(STEP_L * 2),
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

// TODO: need to update DrawAnimatingItem() or Create DrawBaddy()
// or using item->item_flags[] or directly use mesh_bits[] and meshes to switch it ?
enum BADDY1_WEAPON
{
    WBAD1_EMPTY = 0,
    WBAD1_UZI = 1,
    WBAD1_SWORD = 2,
};

enum BADDY1_MESH
{
    BAD_MESH_BUTT,           // push
    BAD_MESH_LLEG1,          // push
    BAD_MESH_LLEG2,
    BAD_MESH_LFOOT,
    BAD_MESH_RLEG1,          // pop/push
    BAD_MESH_RLEG2,
    BAD_MESH_RFOOT,
    BAD_MESH_HOLSTERS_SWORD, // pop/push
    BAD_MESH_TORSO,          // pop
    BAD_MESH_LARM1,          // push
    BAD_MESH_LARM2,
    BAD_MESH_LHAND,
    BAD_MESH_RARM1,          // push/pop
    BAD_MESH_RARM2,
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

#define OCB_TRIGGER(i) (i * OCB_TRIGGER_BADDY)

static BITE_INFO baddy_gun = { 0, 185, 55, 11 };
static BITE_INFO baddy_sword = { 0, 0, 0, 15 };

void DrawBaddy1(ITEM_INFO* item)
{
    OBJECT_INFO* obj, *obj_ms3;
    OBJ_ROTATION* rot;
    int* bone;
    int clip, frac, rate;
    short* frame[2];
    short* rotation1, *rotation2;
    short** mesh;

    frac = GetFrames(item, frame, &rate);
    obj = &objects[BADDY_1];
    if (obj->shadow_size)
        S_PrintShadow(obj->shadow_size, frame[0], item);
    obj_ms3 = &objects[MESHSWAP3];
    rot = (OBJ_ROTATION*)item->data;
    if (rot == NULL)
    {
        rot = (OBJ_ROTATION*)malloc(sizeof(OBJ_ROTATION));
        rot->head_x = 0;
        rot->head_y = 0;
        rot->torso_x = 0;
        rot->torso_y = 0;
    }

    phd_PushMatrix(); // world
    phd_TranslateAbs(item->pos.x, item->pos.y, item->pos.z);
    phd_RotYXZ(item->pos.y_rot, item->pos.x_rot, item->pos.z_rot);

    clip = S_GetObjectBounds(frame[0]);
    CalculateObjectLighting(item, frame[0]);
    bone = &bones[obj->bone_index];
    phd_PushMatrix(); // list

    rotation1 = (short*)(frame[0] + 9);
    rotation2 = (short*)(frame[1] + 9);

    // BONE 0 (ROOT)
    if (frac)
        InitInterpolate(frac, rate);
    phd_SwapTranslateRel(frac, frame[0], frame[1]);
    phd_SwapGarYXZsuperpack(frac, &rotation1, &rotation2);
    mesh = &meshes[MESH(obj, BAD_MESH_BUTT)];
    phd_SwapPutPolygons(frac, clip, mesh);

    // LEFT LEG
    phd_SwapPushMatrix(frac);
    phd_SwapTranslateRel(frac, BONE(bone, 0));
    phd_SwapGarYXZsuperpack(frac, &rotation1, &rotation2);
    mesh = &meshes[MESH(obj, BAD_MESH_LLEG1)];
    phd_SwapPutPolygons(frac, clip, mesh);

    phd_SwapTranslateRel(frac, BONE(bone, 1));
    phd_SwapGarYXZsuperpack(frac, &rotation1, &rotation2);
    mesh = &meshes[MESH(obj, BAD_MESH_LLEG2)];
    phd_SwapPutPolygons(frac, clip, mesh);

    phd_SwapTranslateRel(frac, BONE(bone, 2));
    phd_SwapGarYXZsuperpack(frac, &rotation1, &rotation2);
    mesh = &meshes[MESH(obj, BAD_MESH_LFOOT)];
    phd_SwapPutPolygons(frac, clip, mesh);
    phd_SwapPopMatrix(frac); // TO HIPS

    // RIGHT LEG
    phd_SwapPushMatrix(frac);
    phd_SwapTranslateRel(frac, BONE(bone, 3));
    phd_SwapGarYXZsuperpack(frac, &rotation1, &rotation2);
    if (item->reserved_4 == WBAD1_UZI)
        mesh = &meshes[MESH(obj_ms3, BAD_MESH_RLEG1)];
    else
        mesh = &meshes[MESH(obj, BAD_MESH_RLEG1)];
    phd_SwapPutPolygons(frac, clip, mesh);

    phd_SwapTranslateRel(frac, BONE(bone, 4));
    phd_SwapGarYXZsuperpack(frac, &rotation1, &rotation2);
    mesh = &meshes[MESH(obj, BAD_MESH_RLEG2)];
    phd_SwapPutPolygons(frac, clip, mesh);

    phd_SwapTranslateRel(frac, BONE(bone, 5));
    phd_SwapGarYXZsuperpack(frac, &rotation1, &rotation2);
    mesh = &meshes[MESH(obj, BAD_MESH_RFOOT)];
    phd_SwapPutPolygons(frac, clip, mesh);
    phd_SwapPopMatrix(frac); // TO HIPS

    // SWORD HOLSTERS
    phd_SwapPushMatrix(frac);
    phd_SwapTranslateRel(frac, BONE(bone, 6));
    phd_SwapGarYXZsuperpack(frac, &rotation1, &rotation2);
    if (item->reserved_4 == WBAD1_SWORD)
        mesh = &meshes[MESH(obj_ms3, BAD_MESH_HOLSTERS_SWORD)]; // if sword is drawed then empty holsters.
    else
        mesh = &meshes[MESH(obj, BAD_MESH_HOLSTERS_SWORD)];
    phd_SwapPutPolygons(frac, clip, mesh);
    phd_SwapPopMatrix(frac); // TO HIPS

    // TORSO
    phd_SwapTranslateRel(frac, BONE(bone, 7));
    phd_SwapRotY(frac, rot->torso_y);
    phd_SwapRotX(frac, rot->torso_x);
    phd_SwapGarYXZsuperpack(frac, &rotation1, &rotation2);
    mesh = &meshes[MESH(obj, BAD_MESH_TORSO)];
    phd_SwapPutPolygons(frac, clip, mesh);

    // LEFT ARM
    phd_SwapPushMatrix(frac); // START
    phd_SwapTranslateRel(frac, BONE(bone, 8));
    phd_SwapGarYXZsuperpack(frac, &rotation1, &rotation2);
    mesh = &meshes[MESH(obj, BAD_MESH_LARM1)];
    phd_SwapPutPolygons(frac, clip, mesh);

    phd_SwapTranslateRel(frac, BONE(bone, 9));
    phd_SwapGarYXZsuperpack(frac, &rotation1, &rotation2);
    mesh = &meshes[MESH(obj, BAD_MESH_LARM2)];
    phd_SwapPutPolygons(frac, clip, mesh);

    phd_SwapTranslateRel(frac, BONE(bone, 10));
    phd_SwapGarYXZsuperpack(frac, &rotation1, &rotation2);
    if (item->reserved_4 == WBAD1_UZI)
        mesh = &meshes[MESH(obj, BAD_MESH_LHAND)];
    else
        mesh = &meshes[MESH(obj_ms3, BAD_MESH_LHAND)];
    phd_SwapPutPolygons(frac, clip, mesh);

    if (item->fired_weapon)
    {
        phd_SwapPushMatrix(frac);
        phd_SwapTranslateRel(frac, BONE(baddy_gun.x, baddy_gun.y, baddy_gun.z));
        phd_SwapRotX(frac, -ANGLE(90));
        phd_SwapRotZ(frac, (GetRandomDraw() << 14) + (GetRandomDraw() >> 2) - 4096);
        phd_SwapPutPolygons(frac, clip, &meshes[objects[GUN_FLASH].mesh_index]);
        phd_SwapPopMatrix(frac);
        --item->fired_weapon;
    }
    phd_SwapPopMatrix(frac); // TO TORSO

    // RIGHT ARM
    phd_SwapPushMatrix(frac);
    phd_SwapTranslateRel(frac, BONE(bone, 11));
    phd_SwapGarYXZsuperpack(frac, &rotation1, &rotation2);
    mesh = &meshes[MESH(obj, BAD_MESH_RARM1)];
    phd_SwapPutPolygons(frac, clip, mesh);

    phd_SwapTranslateRel(frac, BONE(bone, 12));
    phd_SwapGarYXZsuperpack(frac, &rotation1, &rotation2);
    mesh = &meshes[MESH(obj, BAD_MESH_RARM2)];
    phd_SwapPutPolygons(frac, clip, mesh);

    phd_SwapTranslateRel(frac, BONE(bone, 13));
    phd_SwapGarYXZsuperpack(frac, &rotation1, &rotation2);
    if (item->reserved_4 == WBAD1_SWORD)
        mesh = &meshes[MESH(obj, BAD_MESH_RHAND)];
    else
        mesh = &meshes[MESH(obj_ms3, BAD_MESH_RHAND)];
    phd_SwapPutPolygons(frac, clip, mesh);

    // RIGHT ARM ADDON
    phd_SwapPushMatrix(frac);
    phd_SwapTranslateRel(frac, BONE(bone, 14));
    phd_SwapGarYXZsuperpack(frac, &rotation1, &rotation2);
    if (item->reserved_4 == WBAD1_SWORD)
        mesh = &meshes[MESH(obj, BAD_MESH_RSWORD_1)];
    else
        mesh = &meshes[MESH(obj_ms3, BAD_MESH_RSWORD_1)];
    phd_SwapPutPolygons(frac, clip, mesh);

    phd_SwapTranslateRel(frac, BONE(bone, 15));
    phd_SwapGarYXZsuperpack(frac, &rotation1, &rotation2);
    if (item->reserved_4 == WBAD1_SWORD)
        mesh = &meshes[MESH(obj, BAD_MESH_RSWORD_2)];
    else
        mesh = &meshes[MESH(obj_ms3, BAD_MESH_RSWORD_2)];
    phd_SwapPutPolygons(frac, clip, mesh);
    phd_SwapPopMatrix(frac);

    phd_SwapPushMatrix(frac);
    phd_SwapTranslateRel(frac, BONE(bone, 16));
    phd_SwapGarYXZsuperpack(frac, &rotation1, &rotation2);
    if (item->reserved_4 == WBAD1_SWORD)
        mesh = &meshes[MESH(obj, BAD_MESH_RSWORD_3)];
    else
        mesh = &meshes[MESH(obj_ms3, BAD_MESH_RSWORD_3)];
    phd_SwapPutPolygons(frac, clip, mesh);

    phd_SwapTranslateRel(frac, BONE(bone, 17));
    phd_SwapGarYXZsuperpack(frac, &rotation1, &rotation2);
    if (item->reserved_4 == WBAD1_SWORD)
        mesh = &meshes[MESH(obj, BAD_MESH_RSWORD_4)];
    else
        mesh = &meshes[MESH(obj_ms3, BAD_MESH_RSWORD_4)];
    phd_SwapPutPolygons(frac, clip, mesh);
    phd_SwapPopMatrix(frac);

    phd_SwapPushMatrix(frac);
    phd_SwapTranslateRel(frac, BONE(bone, 18));
    phd_SwapGarYXZsuperpack(frac, &rotation1, &rotation2);
    if (item->reserved_4 == WBAD1_SWORD)
        mesh = &meshes[MESH(obj, BAD_MESH_RSWORD_5)];
    else
        mesh = &meshes[MESH(obj_ms3, BAD_MESH_RSWORD_5)];
    phd_SwapPutPolygons(frac, clip, mesh);

    phd_SwapTranslateRel(frac, BONE(bone, 19));
    phd_SwapGarYXZsuperpack(frac, &rotation1, &rotation2);
    if (item->reserved_4 == WBAD1_SWORD)
        mesh = &meshes[MESH(obj, BAD_MESH_RSWORD_6)];
    else
        mesh = &meshes[MESH(obj_ms3, BAD_MESH_RSWORD_6)];
    phd_SwapPutPolygons(frac, clip, mesh);
    phd_SwapPopMatrix(frac); // TO TORSO

    phd_SwapTranslateRel(frac, BONE(bone, 20));
    phd_SwapGarYXZsuperpack(frac, &rotation1, &rotation2);
    if (item->reserved_4 == WBAD1_SWORD)
        mesh = &meshes[MESH(obj, BAD_MESH_RSWORD_7)];
    else
        mesh = &meshes[MESH(obj_ms3, BAD_MESH_RSWORD_7)];
    phd_SwapPutPolygons(frac, clip, mesh);

    phd_SwapTranslateRel(frac, BONE(bone, 21));
    phd_SwapGarYXZsuperpack(frac, &rotation1, &rotation2);
    if (item->reserved_4 == WBAD1_SWORD)
        mesh = &meshes[MESH(obj, BAD_MESH_RSWORD_8)];
    else
        mesh = &meshes[MESH(obj_ms3, BAD_MESH_RSWORD_8)];
    phd_SwapPutPolygons(frac, clip, mesh);
    phd_SwapPopMatrix(frac); // TO TORSO

    // HEAD
    phd_SwapTranslateRel(frac, BONE(bone, 22));
    phd_SwapRotY(frac, rot->head_y);
    phd_SwapRotX(frac, rot->head_x);
    phd_SwapGarYXZsuperpack(frac, &rotation1, &rotation2);
    mesh = &meshes[MESH(obj, BAD_MESH_HEAD)];
    phd_SwapPutPolygons(frac, clip, mesh);

    // HEAD ADDON
    phd_SwapTranslateRel(frac, BONE(bone, 23));
    phd_SwapGarYXZsuperpack(frac, &rotation1, &rotation2);
    mesh = &meshes[MESH(obj, BAD_MESH_HEAD2)];
    phd_SwapPutPolygons(frac, clip, mesh);

    phd_SwapTranslateRel(frac, BONE(bone, 24));
    phd_SwapGarYXZsuperpack(frac, &rotation1, &rotation2);
    mesh = &meshes[MESH(obj, BAD_MESH_HEAD3)];
    phd_SwapPutPolygons(frac, clip, mesh);

    phd_SwapTranslateRel(frac, BONE(bone, 25));
    phd_SwapGarYXZsuperpack(frac, &rotation1, &rotation2);
    mesh = &meshes[MESH(obj, BAD_MESH_HEAD4)];
    phd_SwapPutPolygons(frac, clip, mesh);

    phd_PopMatrix(); // !list
    phd_PopMatrix(); // !world
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

void InitialiseBaddy1(short itemNumber)
{
    ITEM_INFO* item;
    OBJECT_INFO* obj;
    short ocb;

    item = &items[itemNumber];
    item->reserved_2 = MAXSHORT;
    item->reserved_3 = 24;        // default ammo
    item->reserved_4 = WBAD1_UZI; // default weapon

    obj = &objects[item->object_number];
    ocb = OCB_MODE(item->ocb_bits);

    if (ocb == OCB_IDLE)
    {
        item->current_anim = obj->anim_index + ABAD1_IDLE;
        item->current_frame = anims[item->current_anim].frame_base;
        item->state_current = SBAD1_IDLE;
        item->state_next = SBAD1_IDLE;
        return;
    }

    switch (ocb)
    {
        case OCB_IDLE:

            return;

        case OCB_JUMPRIGHT:

            return;

        case OCB_LEFTROLL:

            return;

        case OCB_CROUCH:

            return;

        case OCB_CLIMB4: // work fine :D
            item->current_anim = obj->anim_index + ABAD1_CLIMB4;
            item->current_frame = anims[item->current_anim].frame_base;
            item->state_next = SBAD1_CLIMB4;
            item->state_current = SBAD1_CLIMB4;
            item->pos.x += SINN(item->pos.y_rot) << WALL_SHIFT >> W2V_SHIFT;
            item->pos.z += COSN(item->pos.y_rot) << WALL_SHIFT >> W2V_SHIFT;
            return;
           
    }

    if (ocb > OCB_SLIDE)
    {

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
    AI_INFO info, lara_info;
    int height, ceiling;
    short roomNumber;
    short at;
    short tilt, angle;
    bool roll, jump;

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
            case SBAD1_MONKEYGRAB:
            case SBAD1_MONKEYFORWARD:
            case SBAD1_MONKEYIDLE:
                item->current_anim = obj->anim_index + ABAD1_MONKEY_TO_FREEFALL;
                item->current_frame = anims[item->current_anim].frame_base;
                item->state_current = SBAD1_MONKEY_TOFREEFALL;
                item->speed = 0;
                break;
            case SBAD1_DEATH:
                item->gravity_status = TRUE;
                bad->LOT.is_jumping = TRUE;
                if (item->pos.y >= item->floor)
                {
                    item->pos.y = item->floor;
                    item->gravity_status = FALSE;
                    item->fallspeed = 0;
                }
                break;
            case SBAD1_MONKEY_TOFREEFALL:
                item->state_next = SBAD1_FREEFALL;
                item->gravity_status = FALSE;
                break;
            case SBAD1_FREEFALL:
                item->gravity_status = TRUE;
                if (item->pos.y >= height)
                {
                    item->pos.y = height;
                    item->gravity_status = FALSE;
                    item->fallspeed = 0;
                    item->state_next = SBAD1_FREEFALL_DEATH;
                }
                break;
            case SBAD1_FREEFALL_DEATH:
                item->pos.y = height;
                break;
            default:
                bad->LOT.is_jumping = TRUE; // permit the falling in pitfall instead of bugged move
                item->current_anim = obj->anim_index + ABAD1_IDLE_DEATH;
                item->current_frame = anims[item->current_anim].frame_base;
                item->state_current = SBAD1_DEATH;
                
                BaddySpawner(item);
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
            if (found.target != lara_item)
            {
                pos.x = found.target->pos.x - item->pos.x;
                pos.y = found.target->pos.y - item->pos.y;
                pos.z = found.target->pos.z - item->pos.z;
            }
            else
            {
                pos.x = lara_item->pos.x - item->pos.x;
                pos.y = lara_item->pos.y - item->pos.y;
                pos.z = lara_item->pos.z - item->pos.z;
            }
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

        if (item->hit_status || (lara_info.distance < 0x100000 || (TargetVisible(item, &lara_info) && abs(lara_item->pos.y - item->pos.y) < WALL_L)))
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
            case SBAD1_IDLE:
                bad->LOT.is_jumping = FALSE;
                bad->LOT.is_monkeying = FALSE;
                bad->maximum_turn = 0;
                bad->flags = 0;

                if (info.ahead && CHK_NOP(item->ai_bits, GUARD))
                {
                    rot.torso_y = info.angle;
                    rot.torso_x = info.x_angle;
                }

                // first: check if uzi ammo is not empty and it's not the uzi
                // then: switch to uzi
                if (item->reserved_4 == WBAD1_SWORD && item->reserved_3 >= 1)
                {
                    item->state_next = SBAD1_UNDRAWSWORD;
                }
                // second: check if the item is weapon empty and this entity have ammo
                // then: draw the uzi
                else if (item->reserved_4 == WBAD1_EMPTY && item->reserved_3 >= 1)
                {
                    item->state_next = SBAD1_DRAWGUN;
                }
                // third: check if the item weapon is uzi and this entity not have ammo
                // then: undraw gun to draw sword
                else if (item->reserved_4 == WBAD1_UZI && item->reserved_3 <= 0)
                {
                    item->state_next = SBAD1_UNDRAWGUN;
                }
                // fourth: check if the item weapon is empty and this entity not have ammo
                // then: draw sword
                else if (item->reserved_4 == WBAD1_EMPTY && item->reserved_3 <= 0)
                {
                    item->state_next = SBAD1_DRAWSWORD;
                }
                else if (CHK_ANY(item->ai_bits, GUARD))
                {
                    rot.head_y = AIGuard(bad);
                    item->state_next = SBAD1_IDLE;
                }
                else if (Targetable(item, &info) && item->reserved_3 >= 1)
                {
                    if (item->reserved_4 == WBAD1_UZI)
                        item->state_next = SBAD1_AIM;
                    else
                        item->state_next = SBAD1_DRAWGUN;
                }
                else
                {
                    if (CHK_ANY(item->ai_bits, MODIFY))
                    {
                        item->state_next = SBAD1_IDLE;
                        if (item->floor > item->pos.y + (STEP_L * 3))
                            item->ai_bits &= ~(MODIFY);
                    }
                    else if (bad->monkey_ahead)
                    {
                        // the baddy need to have no weapon to handle the monkey animation
                        if (item->reserved_4 == WBAD1_SWORD || item->reserved_4 == WBAD1_UZI)
                        {
                            if (item->reserved_4 == WBAD1_SWORD)
                                item->state_next = SBAD1_UNDRAWSWORD;
                            else if (item->reserved_4 == WBAD1_UZI)
                                item->state_next = SBAD1_UNDRAWGUN;
                            item->reserved_4 = WBAD1_EMPTY;
                        }
                        // when the baddy is hand free
                        else
                        {
                            roomNumber = item->room_number;
                            floor = GetFloor(item->pos.x, item->pos.y, item->pos.z, &roomNumber);
                            height = GetHeight(floor, item->pos.x, item->pos.y, item->pos.z);
                            ceiling = GetCeiling(floor, item->pos.x, item->pos.y, item->pos.z);
                            if (ceiling == height - 1536)
                                item->state_next = SBAD1_MONKEYGRAB;
                            else
                                item->state_next = SBAD1_WALK;
                        }
                    }
                    else if (pjump->can_jump_1click || pjump->can_jump_2click)
                    {
                        bad->maximum_turn = 0;
                        item->current_anim = obj->anim_index + ABAD1_IDLE_TO_JUMP_FORWARD;
                        item->current_frame = anims[item->current_anim].frame_base;
                        item->state_current = SBAD1_JUMPFORWARD_1BLOCK;
                        if (!pjump->can_jump_2click)
                            item->state_next = SBAD1_JUMPFORWARD_1BLOCK;
                        else
                            item->state_next = SBAD1_JUMPFORWARD_2BLOCK;
                        bad->LOT.is_jumping = TRUE;
                    }
                    // check if enemy is smallmedikit or uzi ammo
                    // if true and the distance if correct go pickup animation
                    else if (bad->enemy &&
                            (bad->enemy->object_number == SMALLMEDI_ITEM || bad->enemy->object_number == UZI_AMMO_ITEM) && info.distance < VBAD1_PICKUP_RANGE)
                    {
                        AlignItemToTarget(item, bad->enemy);
                        item->state_next = SBAD1_IDLETOCROUCH;
                        item->state_required = SBAD1_CROUCHPICKUP;
                    }
                    else
                    {
                        if (roll)
                        {
                            bad->maximum_turn = 0;
                            item->state_next = SBAD1_ROLLLEFT;
                        }
                        else if (jump)
                        {
                            bad->maximum_turn = 0;
                            item->state_next = SBAD1_JUMPRIGHT;
                        }
                        else if (!bad->enemy || bad->enemy <= 0 || info.distance >= VBAD1_WALK_RANGE)
                        {
                            item->state_next = SBAD1_WALK;
                        }
                        else if (info.distance >= VBAD1_ATTACK_RANGE && item->reserved_4 == WBAD1_SWORD)
                        {
                            item->state_next = SBAD1_SWORDHITFRONT;
                        }
                        else if ((GetRandomControl() & 1) && item->reserved_4 == WBAD1_SWORD)
                        {
                            item->state_next = SBAD1_SWORDHITLEFT;
                        }
                        else if (item->reserved_4 == WBAD1_SWORD)
                        {
                            item->state_next = SBAD1_SWORDHITRIGHT;
                        }
                    }
                }
                break;
            case SBAD1_WALK:
                bad->LOT.is_jumping = FALSE;
                bad->LOT.is_monkeying = FALSE;
                bad->maximum_turn = VBAD1_ANGLE_WALK;
                bad->flags = 0;

                if (lara_info.ahead)
                    rot.head_y = lara_info.angle;
                else if (info.ahead)
                    rot.head_y = info.angle;

                if (item->reserved_4 == WBAD1_SWORD && item->reserved_3 >= 1)
                {
                    item->state_next = SBAD1_IDLE;
                    item->state_required = SBAD1_UNDRAWSWORD;
                }
                else if (item->reserved_4 == WBAD1_EMPTY && item->reserved_3 >= 1)
                {
                    item->state_next = SBAD1_IDLE;
                    item->state_required = SBAD1_DRAWGUN;
                }
                else if (item->reserved_4 == WBAD1_UZI && item->reserved_3 <= 0)
                {
                    item->state_next = SBAD1_IDLE;
                    item->state_required = SBAD1_UNDRAWGUN;
                }
                else if (item->reserved_4 == WBAD1_EMPTY && item->reserved_3 <= 0)
                {
                    item->state_next = SBAD1_IDLE;
                    item->state_required = SBAD1_DRAWSWORD;
                }
                // uzi have ammo ? then if lara can be targeted go to aim state
                else if (Targetable(item, &info) && item->reserved_3 > 0)
                {
                    item->state_next = SBAD1_IDLE;
                }
                else if(pjump->can_jump_1click || pjump->can_jump_2click)
                {
                    bad->maximum_turn = 0;
                    item->state_next = SBAD1_IDLE;
                }
                else
                {
                    if (bad->monkey_ahead || bad->reached_goal)
                    {
                        item->state_next = SBAD1_IDLE;
                    }
                    else if (info.ahead && info.distance < VBAD1_ATTACK_RANGE)
                    {
                        item->state_next = SBAD1_IDLE;
                    }
                    else if (info.bite && info.distance < VBAD1_WALK_RANGE)
                    {
                        item->state_next = SBAD1_IDLE;
                    }
                    else if (info.distance < VBAD1_RUN_RANGE && item->reserved_4 == WBAD1_SWORD)
                    {
                        item->state_next = SBAD1_WALKSWORDHITRIGHT;
                    }
                    else
                    {
                        if (roll || jump)
                            item->state_next = SBAD1_IDLE;
                        else if (bad->mood == ATTACK_MOOD && !bad->jump_ahead && info.distance > VBAD1_RUN_RANGE)
                            item->state_next = SBAD1_RUN;
                    }
                }
                break;
            case SBAD1_RUN:
                bad->maximum_turn = VBAD1_ANGLE_RUN;
                tilt = (angle / 2);

                if (Targetable(item, &info) && item->reserved_3 >= 1)
                    item->state_next = SBAD1_IDLE;
                else if (CHK_ANY(item->ai_bits, GUARD))
                    item->state_next = SBAD1_IDLE;
                else if (pjump->can_jump_1click || pjump->can_jump_2click)
                    item->state_next = SBAD1_IDLE;
                else if (bad->monkey_ahead || bad->jump_ahead)
                    item->state_next = SBAD1_IDLE;
                else if (info.distance < 0x5C0A4) // attack ?
                    item->state_next = SBAD1_IDLE;
                else if (info.distance < VBAD1_RUN_RANGE)
                    item->state_next = SBAD1_WALK;
                break;

            /// ATTACK
            case SBAD1_AIM:
                bad->maximum_turn = 0;

                if (info.ahead)
                {
                    rot.torso_y = info.angle;
                    rot.torso_x = info.x_angle;
                }

                // this baddy have ammo ?
                if (Targetable(item, &info) && item->reserved_3 >= 1)
                    item->state_next = SBAD1_FIRE;
                else
                    item->state_next = SBAD1_IDLE;
                break;
            case SBAD1_FIRE:
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
                    if (!ShotLara(item, &info, &baddy_gun, rot.torso_y, VBAD1_GUN_DAMAGE))
                        item->state_next = SBAD1_IDLE;
                }
                break;

            case SBAD1_SWORDHITFRONT:
            case SBAD1_SWORDHITLEFT:
            case SBAD1_WALKSWORDHITRIGHT:
                bad->maximum_turn = 0;
                if (info.ahead)
                {
                    rot.torso_y = info.angle;
                    rot.torso_x = info.x_angle;
                }

                if (!bad->flags && (item->touch_bits & VBAD1_TOUCHBITS))
                {
                    CreatureEffectAlternate(item, &baddy_sword, 10, item->pos.y_rot, DoBloodSplat);
                    lara_item->hit_points -= VBAD1_DAMAGE;
                    lara_item->hit_status = TRUE;
                    bad->flags = 1;
                }

                if (item->current_frame == anims[item->current_anim].frame_end - 1)
                    bad->flags = 0;
                break;

            case SBAD1_SWORDHITRIGHT:
                if (info.distance < VBAD1_ATTACK_RANGE)
                    item->state_next = SBAD1_SWORDHITLEFT;

                bad->maximum_turn = 0;
                if (info.ahead)
                {
                    rot.torso_y = info.angle;
                    rot.torso_x = info.x_angle;
                }

                if (!bad->flags && (item->touch_bits & VBAD1_TOUCHBITS))
                {
                    CreatureEffectAlternate(item, &baddy_sword, 10, item->pos.y_rot, DoBloodSplat);
                    lara_item->hit_points -= VBAD1_DAMAGE;
                    lara_item->hit_status = TRUE;
                    bad->flags = 1;
                }

                if (item->current_frame == anims[item->current_anim].frame_end - 1)
                    bad->flags = 0;
                break;

            /// MONKEY
            case SBAD1_MONKEYIDLE:

                break;
            case SBAD1_MONKEYFORWARD:

                break;
            case SBAD1_MONKEYPUSHOFF:

                break;
            
            /// JUMP/ROLL
            case SBAD1_ROLLLEFT:
            case SBAD1_JUMPRIGHT:
                bad->alerted = FALSE;
                bad->maximum_turn = 0;
                item->status = FITEM_ACTIVE;
                break;
            case SBAD1_JUMPFORWARD_1BLOCK:
            case SBAD1_JUMPFORWARD_2BLOCK:
                if (item->reserved_1 < 0 && item->current_anim != (obj->anim_index + ABAD1_IDLE_TO_JUMP_FORWARD))
                    item->reserved_1 += 2; // ??
                break;

            /// CROUCH
            case SBAD1_CROUCH:
                if (item->reserved_1)
                {
                    if (info.distance < 0x718E4)
                    {
                        item->state_next = SBAD1_CROUCHTOIDLE;
                        bad->enemy = nullptr;
                    }
                }
                else if (bad->enemy &&
                        (bad->enemy->object_number == SMALLMEDI_ITEM || bad->enemy->object_number == UZI_AMMO_ITEM) &&
                        info.distance < VBAD1_PICKUP_RANGE)
                {
                    item->state_next = SBAD1_CROUCHPICKUP;
                }
                else if (bad->alerted)
                {
                    item->state_next = SBAD1_CROUCHTOIDLE;
                }
                break;
            case SBAD1_CROUCHPICKUP:
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
            case SBAD1_BLIND:
                if (!EntityBlindTimer) // no random anymore, it's not like this in reality :D
                    item->state_next = SBAD1_IDLE;
                break;

            /// DRAW/UNDRAW
            case SBAD1_DRAWGUN:
                if (frame == 21)
                    item->reserved_4 = WBAD1_UZI;
                break;
            case SBAD1_UNDRAWGUN:
                if (frame == 21)
                    item->reserved_4 = WBAD1_EMPTY; // to not switch the render directly
                break;

            case SBAD1_DRAWSWORD:
                if (frame == 12)
                    item->reserved_4 = WBAD1_SWORD;
                break;
            case SBAD1_UNDRAWSWORD:
                if (frame == 22)
                    item->reserved_4 = WBAD1_EMPTY; // to not switch the render directly
                break;

            /// CLIMB/JUMPOFF (stop rotation)
            case SBAD1_CLIMB2:
            case SBAD1_CLIMB3:
            case SBAD1_CLIMB4:
            case SBAD1_JUMPOFF3:
            case SBAD1_JUMPOFF4:
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

    if (EntityBlindTimer > 16)
    {
        bad->maximum_turn = 0;
        item->current_anim = obj->anim_index + ABAD1_BLIND;
        item->current_frame = anims[item->current_anim].frame_base;
        item->state_current = SBAD1_BLIND;
        item->state_next = SBAD1_BLIND;
    }
    // can climb ?
    else if (!(item->state_current >= SBAD1_DEATH && item->state_current <= SBAD1_JUMPOFF3))
    {
        switch (CreatureVault(itemNumber, angle, 2, VBAD1_SHIFT))
        {
            case VBAD1_FALL3:
                item->current_anim = obj->anim_index + ABAD1_JUMPOFF3;
                item->current_frame = anims[item->current_anim].frame_base;
                item->state_current = SBAD1_JUMPOFF3;
                item->state_next = SBAD1_JUMPOFF3;
                break;
            case VBAD1_FALL4:
                item->current_anim = obj->anim_index + ABAD1_JUMPOFF4;
                item->current_frame = anims[item->current_anim].frame_base;
                item->state_current = SBAD1_JUMPOFF4;
                item->state_next = SBAD1_JUMPOFF4;
                break;
            case VBAD1_CLIMB2:
                item->current_anim = obj->anim_index + ABAD1_CLIMB2;
                item->current_frame = anims[item->current_anim].frame_base;
                item->state_current = SBAD1_CLIMB2;
                item->state_next = SBAD1_CLIMB2;
                break;
            case VBAD1_CLIMB3:
                item->current_anim = obj->anim_index + ABAD1_CLIMB3;
                item->current_frame = anims[item->current_anim].frame_base;
                item->state_current = SBAD1_CLIMB3;
                item->state_next = SBAD1_CLIMB3;
                break;
            case VBAD1_CLIMB4:
                item->current_anim = obj->anim_index + ABAD1_CLIMB4;
                item->current_frame = anims[item->current_anim].frame_base;
                item->state_current = SBAD1_CLIMB4;
                item->state_next = SBAD1_CLIMB4;
                break;
        }
    }
    // animate entity (if not climbing !)
    else
    {
        CreatureAnimation(itemNumber, angle, tilt);
    }
}