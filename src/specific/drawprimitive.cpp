#include "framework.h"
#include "drawprimitive.h"
#include "3dsystem/3d_gen.h"
#include "3dsystem/3d_gen_a.h"
#include "game/control.h"
#include "specific/specific.h"
#include "utils/utils.h"

// original function (now using SetGunFlash_Left/Right() in utils !)
void SetGunFlash(int weapon_type)
{
    int pos[3];
    pos[0] = 0; // X
    pos[1] = 0; // Y
    pos[2] = 0; // Z

    switch (weapon_type)
    {
        case LG_PISTOLS: // PISTOLS
            pos[1] = 190;
            pos[2] = 40;
            break;
        case LG_REVOLVER: // REVOLVER
            pos[1] = 215;
            pos[2] = 65;
            break;
        case LG_UZIS: // UZI
            pos[1] = 190;
            pos[2] = 50;
            break;
        case LG_SHOTGUN: // SHOTGUN
        case LG_GRENADEGUN: 
        case LG_CROSSBOW:
        case LG_FLARE: 
            return;
    }

    SetupGunFlash(pos);
}

void SetupGunFlash(int* pos)
{
    GUNFLASH_STRUCT *gunflash = gunflashes;
    int i = 0;

    while (gunflash->on)
    {
        ++gunflash;
        if (++i >= 4)
            return;
    }

    phd_TranslateRel(*pos, pos[1], pos[2]);
    phd_RotX(-16380);
    phd_SwapGunMatrix(gunflash->matrix, phd_mxptr, false);
    gunflash->on = 1;
}

void DrawGunFlashes(void)
{
    GUNFLASH_STRUCT* gun;

    for (int i = 0; i < MAX_GUNFLASH; i++)
    {
        gun = &gunflashes[i];

        if (gun->on)
        {
            phd_top = 0;
            phd_left = 0;
            phd_right = phd_winwidth;
            phd_bottom = phd_winheight;
            
            phd_PushMatrix();

            phd_SwapGunMatrix(gun->matrix, phd_mxptr, true);
            phd_RotZ(2 * GetRandomDraw());
            GunFlashColor = 0xFF2F2F00;
            phd_PutPolygons(meshes[objects[GUN_FLASH].mesh_index], NO_CLIP);
            gun->on = false;

            phd_PopMatrix();
        }
    }
}

void DrawGunShells(void)
{
    GUNSHELL_STRUCT* shell;

    phd_left = 0;
    phd_right = phd_winwidth;
    phd_top = 0;
    phd_bottom = phd_winheight;

    for (int i = 0; i < MAX_GUNSHELLS; i++)
    {
        shell = &gunshells[i];

        if (shell->counter)
        {
            phd_PushMatrix();
            phd_TranslateAbs(shell->pos.x, shell->pos.y, shell->pos.z);
            phd_RotYXZ(shell->pos.y_rot, shell->pos.x_rot, shell->pos.z_rot);
            phd_PutPolygons(meshes[objects[shell->object_number].mesh_index], NO_CLIP);
            phd_PopMatrix();
        }
    }
}

void DrawFires(void)
{
    FIRE_LIST* fire;
    ROOM_INFO* r;
    BYTE size;

    for (int i = 0; i < MAX_FIRE_LIST; i++)
    {
        fire = &firelists[i];

        if (fire->on)
        {
            size = -fire->size;
            size &= 0x80;

            fire_bounds[1] = size + 256;
            fire_bounds[3] = size + 256;
            fire_bounds[5] = size + 256;
            fire_bounds[0] = -fire_bounds[1];
            fire_bounds[4] = -fire_bounds[1];
            fire_bounds[2] = -2 * (size * 3 + 256);

            r = &rooms[fire->room_number];
            phd_left = r->left;
            phd_right = r->right;
            phd_top = r->top;
            phd_bottom = r->bottom;

            phd_PushMatrix();
            phd_TranslateAbs(fire->x, fire->y, fire->z);
            
            if (S_GetObjectBounds(fire_bounds))
            {
                if (fire->on)
                    DrawFire(fire->size, NO_ROOM);
                else
                    DrawFire(fire->size, fire->room_number);
            }

            phd_PopMatrix();
        }
    }

    phd_top = 0;
    phd_left = 0;
    phd_right = phd_winwidth;
    phd_bottom = phd_winheight;
}

void injector::inject_drawprimitive()
{
    //this->inject(0x0047F620, DrawHorizon);
    //this->inject(0x00488950, DrawHorizonLayer);
    //this->inject(0x00489480, CalcHorizonAndLayer);
    //this->inject(0x0041D140, DrawLara);
    //this->inject(0x00455800, DrawLara_Mirror);
    //this->inject(0x00439B80, SetGunFlash);
    //this->inject(0x00439AE0, SetupGunFlash);
    //this->inject(0x00439C00, DrawGunFlashes);
    //this->inject(0x00481DF0, InitialiseFogBulbs);
    //this->inject(0x004819B0, CreateFXBulbs);
    //this->inject(0x004790F0, InitDynamicLighting);
    //this->inject(0x00464030, DrawTrainEffect);
    //this->inject(0x004395B0, DrawGunShells);
    //this->inject(0x00459410, DrawRope);
    //this->inject(0x0043A1B0, DrawSparks);
    //this->inject(0x0048A840, DrawWeather);
    //this->inject(0x00438560, DrawFires);
    //this->inject(0x00487260, DrawSmoke);
    //this->inject(0x00486430, DrawSplashes);
    //this->inject(0x004860D0, DrawBubbles);
    //this->inject(0x00489B90, DrawDebris);
    //this->inject(0x0048A0C0, DrawBlood);
    //this->inject(0x0048A430, DrawDrips);
    //this->inject(0x0048CAA0, DrawShockwaves);
    //this->inject(0x040E6300, DrawLightning);
    //this->inject(0x00489950, DrawLaserSightSprite);
}