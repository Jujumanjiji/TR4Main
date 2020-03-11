#include "framework.h"
#include "lara.h"
#include "3d_gen.h"
#include "control.h"
#include "effect2.h"
#include "items.h"
#include "sphere.h"
#include "sound.h"
#include "utils.h"

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

            if (ammo != INFINITE_AMMO)
                weapon_ammo.decrease(LG_CROSSBOW, 1);

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


}


#ifdef DLL_INJECT
void injector::inject_larafire()
{
    this->inject(ADDRESS_STRUCT(0x0042EA70, weapon_object));
    this->inject(ADDRESS_STRUCT(0x0042EAC0, weapon_meshes));
    this->inject(ADDRESS_STRUCT(0x00429260, fire_shotgun));
    this->inject(ADDRESS_STRUCT(0x00429480, fire_grenade));
    this->inject(ADDRESS_STRUCT(0x0042A270, fire_crossbow));
    this->inject(ADDRESS_STRUCT(0x0042DDF0, InitialiseNewWeapons));
    this->inject(ADDRESS_STRUCT(0x0042F480, GetCurrentAmmo));
    this->inject(ADDRESS_STRUCT(0x0042DF30, LaraTargetInfo));
    this->inject(ADDRESS_STRUCT(0x0042E0D0, LaraGetNewTarget));
}
#endif