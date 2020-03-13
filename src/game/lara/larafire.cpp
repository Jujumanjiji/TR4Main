#include "framework.h"
#include "lara.h"
#include "3d_gen.h"
#include "debris.h"
#include "draw.h"
#include "control.h"
#include "effect2.h"
#include "items.h"
#include "sphere.h"
#include "sound.h"
#include "switchs.h"
#include "utils.h"

#define NEAR_ANGLE ANGLE(15)

// work nice :D
void TriggerShotgunSparks(int x, int y, int z, int xv, int yv, int zv)
{
    SPARKS *sptr = &sparks[GetFreeSparks()];
    sptr->on = TRUE;
    sptr->sR = 255;
    sptr->sG = 255;
    sptr->sB = 0;
    sptr->dR = 255;
    sptr->dG = (GetRandomControl() & 127) + 64;
    sptr->dB = 0;
    sptr->colfadespeed = 3;
    sptr->fadetoblack = 5;
    sptr->sLife = sptr->life = 10;
    sptr->transtype = TS_COLADD;
    sptr->dynamic = -1;
    sptr->x = x + (GetRandomControl() & 7) - 3;
    sptr->y = y + (GetRandomControl() & 7) - 3;
    sptr->z = z + (GetRandomControl() & 7) - 3;
    sptr->xvel = xv + ((GetRandomControl() & 511) - 256);
    sptr->yvel = yv + ((GetRandomControl() & 511) - 256);
    sptr->zvel = zv + ((GetRandomControl() & 511) - 256);
    sptr->friction = 0;
    sptr->flags = SPF_SCALE;
    sptr->scalar = 2;
    sptr->size = sptr->sSize = (GetRandomControl() & 3) + 4;
    sptr->dSize = 1;// 20 is more good !
    sptr->gravity = sptr->maxyvel = 0;
}

int weapon_object(int weapon_type)
{
    switch (weapon_type)
    {
        case LG_REVOLVER:
            return REVOLVER_ANIM;
        case LG_UZIS:
            return UZI_ANIM;
        case LG_SHOTGUN:
            return SHOTGUN_ANIM;
        case LG_GRENADEGUN:
            return GRENADE_GUN_ANIM;
        case LG_CROSSBOW:
            return CROSSBOW_ANIM;
        case LG_PISTOLS:
        default:
            return PISTOLS_ANIM;
    }
}

int weapon_meshes(int weapon_type)
{
    switch (weapon_type)
    {
        case LG_REVOLVER:
            return CHK_ANY(lara.revolver_type_carried, CR_LASERSIGHT) ? LARA_REVOLVER_LASER : REVOLVER_ANIM;
        case LG_UZIS:
            return UZI_ANIM;
        case LG_SHOTGUN:
            return SHOTGUN_ANIM;
        case LG_GRENADEGUN:
            return GRENADE_GUN_ANIM;
        case LG_CROSSBOW:
            return CHK_ANY(lara.crossbow_type_carried, CR_LASERSIGHT) ? LARA_CROSSBOW_LASER : CROSSBOW_ANIM;
        case LG_PISTOLS:
        default:
            return PISTOLS_ANIM;
    }
}

void fire_shotgun(void)
{
    PHD_VECTOR pos;
    int random;
    int x, y, z, r;
    short dangles[2];
    short angles[2];
    bool fired = false;

    dangles[0] = lara.l_arm.y_rot + lara_item->pos.y_rot;
    dangles[1] = lara.l_arm.x_rot;
    if (!lara.l_arm.lock)
    {
        dangles[0] += lara.torso_y_rot;
        dangles[1] += lara.torso_x_rot;
    }

    random = CHK_ANY(lara.shotgun_type_carried, CR_AMMO1) ? SHOTGUN_RANDOM_AMMO1 : SHOTGUN_RANDOM_AMMO2;
    for (int i = 0; i < SHOTGUN_BULLET_MAX; i++)
    {
        r = (int)((GetRandomControl() - 0x4000) * random / 0x10000);
        angles[0] = dangles[0] + r;
        r = (int)((GetRandomControl() - 0x4000) * random / 0x10000);
        angles[1] = dangles[1] + r;
        if (FireWeapon(LG_SHOTGUN, lara.target, lara_item, angles))
            fired = true;
    }

    if (fired)
    {
        pos.x = SHOTGUN_GUNPOS_X;
        pos.y = SHOTGUN_GUNPOS_Y; // min
        pos.z = SHOTGUN_GUNPOS_Z;
        GetLaraHandAbsPosition(&pos, GUN_HAND_RIGHT);
        x = pos.x;
        y = pos.y;
        z = pos.z;
        pos.x = SHOTGUN_GUNPOS_X;
        pos.y = SHOTGUN_GUNPOS_Y + SHOTGUN_GUNPOS_SMOKEDIST_MAX; // max
        pos.z = SHOTGUN_GUNPOS_Z;
        GetLaraHandAbsPosition(&pos, GUN_HAND_RIGHT);

        SmokeCountL = 32;
        SmokeWeapon = LG_SHOTGUN;

        if (lara_item->mesh_bits)
        {
            for (int i = 0; i < SHOTGUN_SMOKE_MAX; i++)
                TriggerGunSmoke(x, y, z, pos.x - x, pos.y - y, pos.z - z, 1, SmokeWeapon, SmokeCountL);
            // TODO: add customize to that !
            //for (int j = 0; j < SHOTGUN_SPARKS_MAX; j++)
            //    TriggerShotgunSparks(x, y, z, (pos.x - x) << 1, (pos.y - y) << 1, (pos.z - z) << 1);
        }

        lara.r_arm.flash_gun = weapons[LG_SHOTGUN].flash_time;
        SoundEffect(SFX_EXPLOSION1, &lara_item->pos, PITCH_SHIFT | 0x1400000);
        SoundEffect(weapons[LG_SHOTGUN].sample_id, &lara_item->pos, 0);
        ++savegame_ammoused;
    }
}

void fire_grenade(void)
{
    short ammo = weapon_ammo.get(LG_GRENADEGUN);
    if (ammo)
    {
        ITEM_INFO* item;
        FLOOR_INFO* floor;
        PHD_VECTOR pos;
        int x, y, z;
        int height;

        short item_number = CreateItem();
        lara.has_fired = TRUE;
        if (item_number != NO_ITEM)
        {
            item = &items[item_number];
            item->shade = -15856;
            item->object_number = GRENADE;
            item->room_number = lara_item->room_number;

            pos.x = 0;
            pos.y = 276;
            pos.z = 80;
            GetLaraHandAbsPosition(&pos, GUN_HAND_RIGHT);
            x = pos.x;
            y = pos.y;
            z = pos.z;
            floor = GetFloor(pos.x, pos.y, pos.z, &item->room_number);
            height = GetHeight(floor, pos.x, pos.y, pos.z);
            if (height < pos.y)
            {
                item->pos.x = lara_item->pos.x;
                item->pos.y = pos.y;
                item->pos.z = lara_item->pos.z;
                item->room_number = lara_item->room_number;
            }
            else
            {
                item->pos.x = pos.x;
                item->pos.y = pos.y;
                item->pos.z = pos.z;
            }

            pos.x = 0;
            pos.y = 1204;
            pos.z = 80;
            GetLaraHandAbsPosition(&pos, GUN_HAND_RIGHT);

            SmokeCountL = 32;
            SmokeWeapon = LG_GRENADEGUN;

            if (lara_item->mesh_bits)
            {
                for (int i = 0; i < GRENADEGUN_SMOKE_MAX; i++)
                    TriggerGunSmoke(x, y, z, pos.x - x, pos.y - y, pos.z - z, 1, SmokeWeapon, SmokeCountL);
            }

            InitialiseItem(item_number);
            item->pos.x_rot = lara_item->pos.x_rot + lara.l_arm.x_rot;
            item->pos.y_rot = lara_item->pos.y_rot + lara.l_arm.y_rot;
            item->pos.z_rot = 0;

            if (!lara.l_arm.lock)
            {
                item->pos.x_rot += lara.torso_x_rot;
                item->pos.y_rot += lara.torso_y_rot;
            }

            item->speed          = 128;
            item->fallspeed      = -item->speed * SIN(item->pos.x_rot) >> W2V_SHIFT;
            item->state_current  = item->pos.x_rot;    // x rotation
            item->state_next     = item->pos.y_rot;    // y rotation
            item->state_required = FALSE;              // rolling on floor ?
            item->hit_points     = GRENADE_TIMER;
            AddActiveItem(item_number);

            if (ammo != INFINITE_AMMO)
                weapon_ammo.decrease(LG_GRENADEGUN, 1);

            if (CHK_ANY(lara.grenadegun_type_carried, CR_AMMO1))
                item->reserved_1 = 1; // normal
            else if (CHK_ANY(lara.grenadegun_type_carried, CR_AMMO2))
                item->reserved_1 = 2; // super
            else if (CHK_ANY(lara.grenadegun_type_carried, CR_AMMO3))
                item->reserved_1 = 3; // flash
            ++savegame_ammoused;
        }
    }
}

void fire_crossbow(PHD_3DPOS* haveDefinedPos)
{
    short ammo = weapon_ammo.get(LG_GRENADEGUN);
    if (ammo)
    {
        ITEM_INFO* item;
        FLOOR_INFO* floor;
        PHD_VECTOR pos;
        int x, y, z;
        int height;

        short item_number = CreateItem();
        lara.has_fired = TRUE;
        if (item_number != NO_ITEM)
        {
            item = &items[item_number];
            item->object_number = CROSSBOW_BOLT;
            item->shade = -15856;

            if (haveDefinedPos)
            {
                item->pos.x = haveDefinedPos->x;
                item->pos.y = haveDefinedPos->y;
                item->pos.z = haveDefinedPos->z;
                item->room_number = lara_item->room_number;
                InitialiseItem(item_number);
                item->pos.x_rot = haveDefinedPos->x_rot;
                item->pos.y_rot = haveDefinedPos->y_rot;
                item->pos.z_rot = haveDefinedPos->z_rot;
            }
            else
            {
                pos.x = 0;
                pos.y = 228;
                pos.z = 32;
                GetLaraHandAbsPosition(&pos, GUN_HAND_RIGHT);
                item->room_number = lara_item->room_number;
                floor = GetFloor(pos.x, pos.y, pos.z, &item->room_number);
                height = GetHeight(floor, pos.x, pos.y, pos.z);

                if (height < pos.y)
                {
                    item->pos.x = lara_item->pos.x;
                    item->pos.y = pos.y;
                    item->pos.z = lara_item->pos.z;
                    item->room_number = lara_item->room_number;
                }
                else
                {
                    item->pos.x = pos.x;
                    item->pos.y = pos.y;
                    item->pos.z = pos.z;
                }

                InitialiseItem(item_number);
                item->pos.x_rot = lara_item->pos.x_rot + lara.l_arm.x_rot;
                item->pos.y_rot = lara_item->pos.y_rot + lara.l_arm.y_rot;
                item->pos.z_rot = 0;

                if (!lara.l_arm.lock)
                {
                    item->pos.x_rot += lara.torso_x_rot;
                    item->pos.y_rot += lara.torso_y_rot;
                }
            }

            item->speed = 512;
            AddActiveItem(item_number);

            //if (ammo != INFINITE_AMMO)
            //    weapon_ammo.decrease(LG_CROSSBOW, 1);

            if (CHK_ANY(lara.crossbow_type_carried, CR_AMMO1))
                item->reserved_1 = 1; // normal
            else if (CHK_ANY(lara.crossbow_type_carried, CR_AMMO2))
                item->reserved_1 = 2; // poison
            else if (CHK_ANY(lara.crossbow_type_carried, CR_AMMO3))
                item->reserved_1 = 3; // explosive
            SoundEffect(SFX_LARA_CROSSBOW, &lara_item->pos, 0);
            ++savegame_ammoused;
        }
    }
}

void FireCrossbowBoltFromLaserSight(GAME_VECTOR* dest, GAME_VECTOR* src)
{
    PHD_3DPOS pos;
    short angles[2];
    phd_GetVectorAngles(src->x - dest->x, src->y - dest->y, src->z - dest->z, angles);
    pos.x = dest->x;
    pos.y = dest->y;
    pos.z = dest->z;
    pos.x_rot = angles[1];
    pos.y_rot = angles[0];
    pos.z_rot = 0;
    fire_crossbow(&pos);
}

void InitialiseNewWeapons(void)
{
    lara.target = nullptr;

    lara.l_arm.frame_curr = 0;
    lara.l_arm.x_rot = 0;
    lara.l_arm.y_rot = 0;
    lara.l_arm.z_rot = 0;
    lara.l_arm.lock = FALSE;
    lara.l_arm.flash_gun = 0;

    lara.r_arm.frame_curr = 0;
    lara.r_arm.x_rot = 0;
    lara.r_arm.y_rot = 0;
    lara.r_arm.z_rot = 0;
    lara.r_arm.lock = FALSE;
    lara.r_arm.flash_gun = 0;

    switch (lara.gun_type)
    {
        case LG_PISTOLS:
        case LG_UZIS:
            lara.r_arm.frame_base = objects[PISTOLS_ANIM].frame_base;
            lara.l_arm.frame_base = objects[PISTOLS_ANIM].frame_base;
            if (lara.gun_status != LHS_ARMLESS)
                draw_pistol_meshes(lara.gun_type);
            break;
        case LG_REVOLVER:
        case LG_SHOTGUN:
        case LG_GRENADEGUN:
            lara.l_arm.frame_base = lara.r_arm.frame_base = objects[weapon_object(lara.gun_type)].frame_base;
            if (lara.gun_status != LHS_ARMLESS)
                draw_shotgun_meshes(lara.gun_type);
            break;
        case LG_FLARE:
            lara.l_arm.frame_base = lara.r_arm.frame_base = objects[FLARE_ANIM].frame_base;
            if (lara.gun_status != LHS_ARMLESS)
                draw_flare_meshes();
            break;
        default:
            lara.l_arm.frame_base = lara.r_arm.frame_base = anims[lara_item->current_anim].frame_ptr;
            break;
    }
}

// dont used anymore...
// use weapon_ammo variable in utils.h
short* GetCurrentAmmo(int weapon_type)
{
    switch (weapon_type)
    {
        case LG_REVOLVER:
            return &lara.revolver_ammo_count;

        case LG_UZIS:
            return &lara.uzi_ammo_count;

        case LG_SHOTGUN:
            if (CHK_ANY(lara.shotgun_type_carried, CR_AMMO1))
                return &lara.shotgun_ammo1_count;
            else
                return &lara.shotgun_ammo2_count;
            break;
        case LG_GRENADEGUN:
            if (CHK_ANY(lara.grenadegun_type_carried, CR_AMMO1))
                return &lara.grenade_ammo1_count;
            else if (CHK_ANY(lara.grenadegun_type_carried, CR_AMMO2))
                return &lara.grenade_ammo2_count;
            else
                return &lara.grenade_ammo3_count;
            break;

        case LG_CROSSBOW:
            if (CHK_ANY(lara.crossbow_type_carried, CR_AMMO1))
                return &lara.crossbow_ammo1_count;
            else if (CHK_ANY(lara.crossbow_type_carried, CR_AMMO2))
                return &lara.crossbow_ammo2_count;
            else
                return &lara.crossbow_ammo3_count;
            break;

        case LG_PISTOLS:
        default:
            return &lara.pistol_ammo_count;
    }
}

void LaraTargetInfo(WEAPON_INFO* winfo)
{
    ITEM_INFO *item;
    GAME_VECTOR src;
    GAME_VECTOR target;
    short angles[2];

    item = lara.target;
    if (!item)
    {
        lara.r_arm.lock = FALSE;
        lara.l_arm.lock = FALSE;
        lara.target_angles[1] = 0;
        lara.target_angles[0] = 0;
        return;
    }

    src.x = 0;
    src.y = 0;
    src.z = 0;
    GetLaraHandAbsPosition((PHD_VECTOR*)&src, GUN_HAND_RIGHT);
    src.room_number = lara_item->room_number;

    find_target_point(item, &target);
    phd_GetVectorAngles(target.x - src.x, target.y - src.y, target.z - src.z, angles);
    angles[0] -= lara_item->pos.y_rot;
    angles[1] -= lara_item->pos.x_rot;

    if (LOS(&src, &target))
    {
        if (angles[0] >= winfo->lock_angle[0]
        &&  angles[0] <= winfo->lock_angle[1]
        &&  angles[1] >= winfo->lock_angle[2]
        &&  angles[1] <= winfo->lock_angle[3])
        {
            lara.r_arm.lock = TRUE;
            lara.l_arm.lock = TRUE;
        }
        else
        {
            if (lara.l_arm.lock)
            {
                if (angles[0] < winfo->left_angle[0]
                ||  angles[0] > winfo->left_angle[1]
                ||  angles[1] < winfo->left_angle[2]
                ||  angles[1] > winfo->left_angle[3])
                    lara.l_arm.lock = FALSE;
            }

            if (lara.r_arm.lock)
            {
                if (angles[0] < winfo->right_angle[0]
                ||  angles[0] > winfo->right_angle[1]
                ||  angles[1] < winfo->right_angle[2]
                ||  angles[1] > winfo->right_angle[3])
                    lara.r_arm.lock = FALSE;
            }
        }
    }
    else
    {
        lara.r_arm.lock = FALSE;
        lara.l_arm.lock = FALSE;
    }

    lara.target_angles[0] = angles[0];
    lara.target_angles[1] = angles[1];
}

void LaraGetNewTarget(WEAPON_INFO* winfo)
{
    if (BinocularRange)
    {
        lara.target = nullptr;
        return;
    }

    ITEM_INFO* item, *bestitem;
    CREATURE_INFO* creature;
    GAME_VECTOR src, target;
    int x, y, z;
    int dist, maxdist, maxdist2, bestdist;
    short angles[2];
    short bestyrot, yrot;

    src.x = lara_item->pos.x;
    src.y = lara_item->pos.y - 650;
    src.z = lara_item->pos.z;
    src.room_number = lara_item->room_number;
    item = nullptr;
    bestitem = nullptr;
    bestyrot = MAXSHORT;
    bestdist = MAXINT;
    maxdist = winfo->target_dist;
    maxdist2 = SQUARE(maxdist);

    creature = baddie_slots;
    for (int slots = 0; slots < NUM_SLOTS; slots++, creature++)
    {
        if (creature->item_number == NO_ITEM || creature->item_number == lara.item_number)
            continue;

        item = &items[creature->item_number];
        if (item->hit_points <= 0)
            continue;
        if (!objects[item->object_number].intelligent)
            continue;
        
        x = item->pos.x - src.x;
        y = item->pos.y - src.y;
        z = item->pos.z - src.z;

        if (abs(x) > maxdist || abs(y) > maxdist || abs(z) > maxdist)
            continue;

        dist = SQUARE(x) + SQUARE(y) + SQUARE(z);
        if (dist < maxdist2)
        {
            find_target_point(item, &target);

            if (LOS(&src, &target))
            {
                phd_GetVectorAngles(target.x - src.x, target.y - src.y, target.z - src.z, angles);
                angles[0] -= lara_item->pos.y_rot + lara.torso_y_rot;
                angles[1] -= lara_item->pos.x_rot = lara.torso_x_rot;

                if (angles[0] >= winfo->lock_angle[0]
                &&  angles[0] <= winfo->lock_angle[1]
                &&  angles[1] >= winfo->lock_angle[2]
                &&  angles[1] <= winfo->lock_angle[3])
                {
                    yrot = abs(angles[0]);
                    if (yrot < (bestyrot + NEAR_ANGLE) && dist < bestdist)
                    {
                        bestdist = dist;
                        bestyrot = yrot;
                        bestitem = item;
                    }
                }
            }
        }
    }

    lara.target = bestitem;
    LaraTargetInfo(winfo);
}

void AimWeapon(WEAPON_INFO* winfo, LARA_ARM* arm)
{
    short curr, speed = winfo->aim_speed;
    short destx, desty;
    
    if (arm->lock)
    {
        desty = lara.target_angles[0];
        destx = lara.target_angles[1];
    }
    else
    {
        desty = 0;
        destx = 0;
    }

    // move y
    curr = arm->y_rot;
    if (curr >= (desty - speed) && curr <= (desty + speed))
        curr = desty;
    else if (curr < desty)
        curr += speed;
    else
        curr -= speed;
    arm->y_rot = curr;

    // move x
    curr = arm->x_rot;
    if (curr >= (destx - speed) && curr <= (destx + speed))
        curr = destx;
    else if (curr < destx)
        curr += speed;
    else
        curr -= speed;
    arm->x_rot = curr;
    arm->z_rot = 0;
}

void find_target_point(ITEM_INFO* item, GAME_VECTOR* target)
{
    short* bounds;
    int x, y, z, c, s;

    bounds = GetBestFrame(item);
    x = (int)((bounds[0] + bounds[1]) / 2);
    y = (int)(bounds[2] + (bounds[3] - bounds[2]) / 3);
    z = (int)((bounds[4] + bounds[5]) / 2);
    s = SIN(item->pos.y_rot);
    c = COS(item->pos.y_rot);

    target->x = item->pos.x + ((c * x + s * z) >> W2V_SHIFT);
    target->y = item->pos.y + y;
    target->z = item->pos.z + ((c * z - s * x) >> W2V_SHIFT);
    target->room_number = item->room_number;
}

//  1 = BADDIE HIT
// -1 = BADDIE MISSED
//  0 = OUT OF AMMO
int FireWeapon(int weapon_type, ITEM_INFO* target, ITEM_INFO* src, short angles[2])
{
    PHD_3DPOS viewpos;
    GAME_VECTOR vsrc, vdest;
    WEAPON_INFO* winfo;
    SPHERE* sptr, slist[33];
    FLOOR_INFO* floor;
    int r, i, nums, bestdist, best;
    int x, z;
    short room_number, angle;
    short ammo;
    
    winfo = &weapons[weapon_type];
    ammo = weapon_ammo.get(weapon_type);
    if (ammo)
    {
        if (ammo != INFINITE_AMMO) // fix negative ammo !
            weapon_ammo.decrease(weapon_type, 1);

        viewpos.x = src->pos.x;
        viewpos.y = src->pos.y - winfo->gun_height;
        viewpos.z = src->pos.z;
        r = (int)((GetRandomControl() - 0x4000) * winfo->shot_accuracy) / 0x10000;
        viewpos.x_rot = angles[1] + (short)r;
        r = (int)((GetRandomControl() - 0x4000) * winfo->shot_accuracy) / 0x10000;
        viewpos.y_rot = angles[0] + (short)r;
        viewpos.z_rot = 0;

        phd_GenerateW2V(&viewpos);
        nums = GetSpheres(target, slist, 0);
        sptr = &slist[0];
        best = -1;
        bestdist = MAXINT;
        
        for (i = 0; i < nums; i++, sptr++)
        {
            r = sptr->r;

            if (abs(sptr->x) < r && abs(sptr->y) < r && sptr->z > r && (SQUARE(sptr->x) + SQUARE(sptr->y) <= SQUARE(r)))
            {
                if ((sptr->z - r) < bestdist)
                {
                    bestdist = sptr->z - r;
                    best = i;
                }
            }
        }

        room_number = src->room_number;
        GetFloor(viewpos.x, viewpos.y, viewpos.z, &room_number);

        lara.has_fired = TRUE;
        vdest.x = viewpos.x;
        vdest.y = viewpos.y;
        vdest.z = viewpos.z;
        vdest.room_number = room_number;

        if (best < 0)
        {
            vsrc.x = vdest.x + (SECTOR(20) * phd_mxptr[M20] >> W2V_SHIFT);
            vsrc.y = vdest.y + (SECTOR(20) * phd_mxptr[M21] >> W2V_SHIFT);
            vsrc.z = vdest.z + (SECTOR(20) * phd_mxptr[M22] >> W2V_SHIFT);
            GetTargetOnLOS(&vdest, &vsrc, FALSE, TRUE);
            return -1;
        }
        else
        {
            savegame_ammohit++;
            vsrc.x = vdest.x + (bestdist * phd_mxptr[M20] >> W2V_SHIFT);
            vsrc.y = vdest.y + (bestdist * phd_mxptr[M21] >> W2V_SHIFT);
            vsrc.z = vdest.z + (bestdist * phd_mxptr[M22] >> W2V_SHIFT);
            if (!GetTargetOnLOS(&vdest, &vsrc, FALSE, TRUE))
                HitTarget(target, &vsrc, winfo->damage, 0);
            return 1;
        }
    }
    else
    {
        return 0; // no more ammo !
    }
}

int GetTargetOnLOS(GAME_VECTOR* dest, GAME_VECTOR* src, BOOL drawtarget, BOOL firing)
{
    ITEM_INFO* item;
    OBJECT_INFO* obj;
    ROOM_INFO* r;
    MESH_INFO* mesh;
    FLOOR_INFO* floor;
    PHD_VECTOR vec;
    int los, closest;
    short room_number;
    short static_obj;

    los = LOS(dest, src);
    GetFloor(src->x, src->y, src->z, &src->room_number);
    if (firing && LaserSight && lara.gun_type == LG_REVOLVER)
        SoundEffect(SFX_DESERT_EAGLE_FIRE, &lara_item->pos, 0);

    closest = ObjectOnLOS2(dest, src, &vec, &mesh);
    if (closest == 999)
    {
        if (lara.gun_type == LG_CROSSBOW)
        {
            if (firing && LaserSight)
                FireCrossbowBoltFromLaserSight(dest, src);
        }
        else
        {
            src->x -= (src->x - dest->x) >> 5;
            src->y -= (src->y - dest->y) >> 5;
            src->z -= (src->z - dest->z) >> 5;
            if (firing && !los)
                TriggerRicochetSpark(src, lara_item->pos.y_rot, 8, 0);
        }
    }
    else
    {
        src->x = vec.x - ((vec.x - dest->x) >> 5);
        src->y = vec.y - ((vec.y - dest->y) >> 5);
        src->z = vec.z - ((vec.z - dest->z) >> 5);

        if (firing)
        {
            item = &items[closest];
            obj = &objects[item->object_number];

            if (lara.gun_type == LG_CROSSBOW)
            {
                if (LaserSight)
                    FireCrossbowBoltFromLaserSight(dest, src);
            }
            else if (closest < 0)
            {
                static_obj = mesh->static_number;
                if (static_obj >= SHATTER0 && static_obj <= SHATTER7)
                {
                    ShatterObject(nullptr, mesh, 128, src->room_number, FALSE);
                    short mesh_count = SmashedMeshCount++;
                    SmashedMeshRoom[mesh_count] = src->room_number;
                    SmashedMesh[mesh_count] = src;
                    mesh->ocb = MESH_INVISIBLE;
                    SoundEffect(SFX_HIT_ROCK, (PHD_3DPOS*)src, 0);
                }
                TriggerRicochetSpark(src, lara_item->pos.y_rot, 3, 0);
                TriggerRicochetSpark(src, lara_item->pos.y_rot, 3, 0);
            }
            else if (item->object_number != SWITCH_TYPE7 && item->object_number != SWITCH_TYPE8)
            {
                if (ShatterItem.bit & obj->explodable_meshbits && LaserSight)
                {
                    ShatterObject(&ShatterItem, nullptr, 128, src->room_number, FALSE);
                    item->mesh_bits &= ~ShatterItem.bit;
                    TriggerRicochetSpark(src, lara_item->pos.y_rot, 3, 0);
                }
                else if (drawtarget && lara.gun_type == LG_REVOLVER)
                {
                    if (obj->intelligent)
                        HitTarget(item, src, weapons[LG_REVOLVER].damage, 0);
                }
                
                switch (obj->hit_effect)
                {
                case HIT_BLOOD:
                    DoBloodSplat(src->x, src->y, src->z, (GetRandomControl() & 3) + 3, item->pos.y_rot, item->room_number);
                    break;
                case HIT_SMOKE:
                    TriggerRicochetSpark(src, lara_item->pos.y_rot, 3, -5); // no fragment, just smoke !
                    break;
                case HIT_FRAGMENT:
                    TriggerRicochetSpark(src, lara_item->pos.y_rot, 3, 0);
                    break;
                }

                item->hit_status = TRUE;
                if (!obj->undead)
                    item->hit_points -= weapons[lara.gun_type].damage;
            }
            else if (ShatterItem.bit == 1 << (obj->nmeshes - 1) && CHK_NOP(item->flags, IFLAG_SWITCH_ONESHOT))
            {
                if (item->object_number == SWITCH_TYPE7)
                    ExplodeItemNode(item, obj->nmeshes - 1, 0, 64);

                room_number = item->room_number;
                if (CHK_EXI(item->flags, IFLAG_CODEBITS) && CHK_EQA(item->flags, IFLAG_CODEBITS))
                {
                    floor = GetFloor(item->pos.x, item->pos.y - STEP_L, item->pos.z, &room_number);
                    GetHeight(floor, item->pos.x, item->pos.y - STEP_L, item->pos.z);
                    TestTriggers(trigger_index, TRUE, CHK_EXI(item->flags, IFLAG_CODEBITS));
                }
                else
                {
                    short item_number;
                    short switch_count = GetSwitchTrigger(item, &item_number, 1);
                    
                    for (int i = 0; i < switch_count; i++)
                    {
                        AddActiveItem(item_number);
                        items[item_number].flags |= IFLAG_CODEBITS | IFLAG_SWITCH_ONESHOT;
                        items[item_number].status = FITEM_ACTIVE;
                    }
                }
            }
            else
            {
                TriggerRicochetSpark(src, lara_item->pos.y_rot, 3, 0);
            }
        }
    }

    if (drawtarget && !los)
    {
        LaserSightActive = TRUE;
        LaserSightPos.z = src->x;
        LaserSightPos.x = src->y;
        LaserSightPos.y = src->z;
        TriggerDynamic(src->x, src->y, src->z, 16, 32, 0, 0);
    }
    return TRUE;
}

#ifdef DLL_INJECT
void injector::inject_larafire()
{
    this->inject(ADDRESS_STRUCT(0x0042EA70, weapon_object));
    this->inject(ADDRESS_STRUCT(0x0042EAC0, weapon_meshes));
    this->inject(ADDRESS_STRUCT(0x00429260, fire_shotgun));
    this->inject(ADDRESS_STRUCT(0x00429480, fire_grenade));
    this->inject(ADDRESS_STRUCT(0x0042A270, fire_crossbow));
    this->inject(ADDRESS_STRUCT(0x0044D820, FireCrossbowBoltFromLaserSight));
    this->inject(ADDRESS_STRUCT(0x0042DDF0, InitialiseNewWeapons));
    this->inject(ADDRESS_STRUCT(0x0042F480, GetCurrentAmmo));
    this->inject(ADDRESS_STRUCT(0x0042DF30, LaraTargetInfo));
    this->inject(ADDRESS_STRUCT(0x0042E0D0, LaraGetNewTarget));
    this->inject(ADDRESS_STRUCT(0x0042E560, AimWeapon));
    this->inject(ADDRESS_STRUCT(0x0042E4A0, find_target_point));
    this->inject(ADDRESS_STRUCT(0x0042E630, FireWeapon));
    this->inject(ADDRESS_STRUCT(0x0044D890, GetTargetOnLOS));
}
#endif