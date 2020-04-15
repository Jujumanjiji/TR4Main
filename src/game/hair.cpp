#include "framework.h"
#include "hair.h"
#include "3d_gen.h"
#include "3d_gen_a.h"
#include "draw.h"
#include "control.h"
#include "gameflow.h"

//BOOL first_hair[HAIR_TYPE];
//HAIR_STRUCT hairs[HAIR_TYPE][HAIR_SEGMENTS + 1];

enum SPHERE_TYPE
{
    S_HIPS,
    S_TORSO,
    S_UARM_R,
    S_UARM_L,
    S_HEAD,
    S_MAX
};

void InitialiseHair(void)
{
    int i, j;
    HAIR_STRUCT* hptr;
    BONE_STRUCT* bone;

    bone = (BONE_STRUCT*)&bones[objects[HAIR].bone_index + 4];
    for (j = 0; j < HAIR_COUNT; j++)
    {
        hptr = &hairs[j][0];
        hptr->pos.y_rot = 0;
        hptr->pos.x_rot = -ANGLE(90);
        first_hair[j] = 1;

        for (i = 0; i < (HAIR_SEGMENTS + 1); i++)
        {
            hptr->pos.x = bone->x;
            hptr->pos.y = bone->y;
            hptr->pos.z = bone->z;

            hptr->pos.x_rot = -ANGLE(90);
            hptr->pos.y_rot = 0;
            hptr->pos.z_rot = 0;

            hptr->vel.x = 0;
            hptr->vel.y = 0;
            hptr->vel.z = 0;

            hptr++;
            bone++;
        }

        bone = (BONE_STRUCT*)&bones[objects[HAIR].bone_index];
    }
}

static void phd_GetMatrixPosition(ITEM_INFO* item)
{
    phd_mxptr->m03 = item->pos.x << W2V_SHIFT;
    phd_mxptr->m13 = item->pos.y << W2V_SHIFT;
    phd_mxptr->m23 = item->pos.z << W2V_SHIFT;
}

static void phd_SetMatrixPosition(ITEM_INFO* item)
{
    phd_mxptr->m03 = item->pos.x >> W2V_SHIFT;
    phd_mxptr->m13 = item->pos.y >> W2V_SHIFT;
    phd_mxptr->m23 = item->pos.z >> W2V_SHIFT;
}

static void SetSphere(SPHERE& sphere, short* objptr, bool isArm)
{
    sphere.x = phd_mxptr->m03 >> W2V_SHIFT;
    sphere.y = phd_mxptr->m13 >> W2V_SHIFT;
    sphere.z = phd_mxptr->m23 >> W2V_SHIFT;
    if (isArm)
        sphere.r = (int)objptr[3] * 3 / 2;
    else
        sphere.r = (int)objptr[3];
}

void HairControl(BOOL inWater, BOOL isYoung, short* frame)
{
    HAIR_STRUCT* hair;
    FLOOR_INFO* floor;
    SPHERE sphere[S_MAX];
    PHD_VECTOR pos;
    int* bone, *bone_young;
    int distance, range;
    int x, y, z;
    int height;
    int wh;
    short* pprot;
    short* objptr;
    short room_number;

    if (!frame)
    {
        if (lara.hit_direction >= 0)
        {
            short spaz;
            switch (lara.hit_direction)
            {
                case NORTH:
                    spaz = lara.is_ducked ? ANIMATION_LARA_CROUCH_SMASH_BACKWARD : ANIMATION_LARA_AH_FORWARD;
                    break;
                case SOUTH:
                    spaz = lara.is_ducked ? ANIMATION_LARA_CROUCH_SMASH_FORWARD : ANIMATION_LARA_AH_BACKWARD;
                    break;
                case EAST:
                    spaz = lara.is_ducked ? ANIMATION_LARA_CROUCH_SMASH_RIGHT : ANIMATION_LARA_AH_LEFT;
                    break;
                case WEST:
                    spaz = lara.is_ducked ? ANIMATION_LARA_CROUCH_SMASH_LEFT : ANIMATION_LARA_AH_RIGHT;
                    break;
            }
            frame = &anims[spaz].frame_ptr[lara.hit_frame * (anims[spaz].interpolation >> 8)];
        }
        else
        {
            frame = GetBestFrame(lara_item);
        }
    }

    phd_PushUnitMatrix();
    phd_GetMatrixPosition(lara_item);
    phd_RotYXZ(lara_item->pos.y_rot, lara_item->pos.x_rot, lara_item->pos.z_rot);

    // Switch to sint32 pointer for packed_rotation values
    pprot = frame + 9;
    bone = &bones[objects[LARA].bone_index];

    // Pass through minimum parts of hierarchy in order to find hair location
    phd_TranslateRel((int)frame[6], (int)frame[7], (int)frame[8]);
    gar_RotYXZsuperpack(&pprot, 0);

    // HIPS SPHERE
    phd_PushMatrix();
    objptr = lara.mesh.hips;
    phd_TranslateRel(objptr[0], objptr[1], objptr[2]);
    sphere[S_HIPS].x = phd_mxptr->m03 >> W2V_SHIFT;
    sphere[S_HIPS].y = phd_mxptr->m13 >> W2V_SHIFT;
    sphere[S_HIPS].z = phd_mxptr->m23 >> W2V_SHIFT;
    sphere[S_HIPS].r = (int)objptr[3];
    phd_PopMatrix();

    // TORSO SPHERE
    phd_TranslateRel(bone[25], bone[26], bone[27]);
    gar_RotYXZsuperpack(&pprot, 6);
    phd_RotYXZ(lara.torso_y_rot, lara.torso_x_rot, lara.torso_z_rot);

    phd_PushMatrix();
    objptr = lara.mesh.torso;
    phd_TranslateRel(objptr[0], objptr[1], objptr[2]);
    sphere[S_TORSO].x = phd_mxptr->m03 >> W2V_SHIFT;
    sphere[S_TORSO].y = phd_mxptr->m13 >> W2V_SHIFT;
    sphere[S_TORSO].z = phd_mxptr->m23 >> W2V_SHIFT;
    sphere[S_TORSO].r = (int)objptr[3];
    if (CHK_EXI(gfLevelFlags, SLEV_YOUNG_LARA))
        sphere[1].r -= (sphere[1].r >> 2) + (sphere[1].r >> 3);
    phd_PopMatrix();

    // RIGHT ARM SPHERE
    phd_PushMatrix();
    phd_TranslateRel(bone[29], bone[30], bone[31]);
    gar_RotYXZsuperpack(&pprot, 2);
    objptr = lara.mesh.uarm_r;
    phd_TranslateRel(objptr[0], objptr[1], objptr[2]);
    sphere[S_UARM_R].x = phd_mxptr->m03 >> W2V_SHIFT;
    sphere[S_UARM_R].y = phd_mxptr->m13 >> W2V_SHIFT;
    sphere[S_UARM_R].z = phd_mxptr->m23 >> W2V_SHIFT;
    sphere[S_UARM_R].r = (int)objptr[3] * 3 / 2;
    phd_PopMatrix();

    // LEFT ARM SPHERE
    phd_PushMatrix();
    phd_TranslateRel(bone[41], bone[42], bone[43]);
    gar_RotYXZsuperpack(&pprot, 2);
    objptr = lara.mesh.uarm_l;
    phd_TranslateRel(objptr[0], objptr[1], objptr[2]);
    sphere[S_UARM_L].x = phd_mxptr->m03 >> W2V_SHIFT;
    sphere[S_UARM_L].y = phd_mxptr->m13 >> W2V_SHIFT;
    sphere[S_UARM_L].z = phd_mxptr->m23 >> W2V_SHIFT;
    sphere[S_UARM_L].r = (int)objptr[3] * 3 / 2;
    phd_PopMatrix();

    // HEAD SPHERE
    phd_TranslateRel(bone[53], bone[54], bone[55]);
    gar_RotYXZsuperpack(&pprot, 2);
    phd_RotYXZ(lara.head_y_rot, lara.head_x_rot, lara.head_z_rot);
    phd_PushMatrix();
    objptr = lara.mesh.head;
    phd_TranslateRel(objptr[0], objptr[1], objptr[2]);
    sphere[S_HEAD].x = phd_mxptr->m03 >> W2V_SHIFT;
    sphere[S_HEAD].y = phd_mxptr->m13 >> W2V_SHIFT;
    sphere[S_HEAD].z = phd_mxptr->m23 >> W2V_SHIFT;
    sphere[S_HEAD].r = (int)objptr[3];
    phd_PopMatrix();

    pos.x = phd_mxptr->m03 >> W2V_SHIFT;
    pos.y = phd_mxptr->m13 >> W2V_SHIFT;
    pos.z = phd_mxptr->m23 >> W2V_SHIFT;

    if (CHK_EXI(gfLevelFlags, SLEV_YOUNG_LARA)) // torso
    {
        sphere[S_TORSO].x = (sphere[S_TORSO].x + pos.x) >> 1;
        sphere[S_TORSO].y = (sphere[S_TORSO].y + pos.y) >> 1;
        sphere[S_TORSO].z = (sphere[S_TORSO].z + pos.z) >> 1;
    }

    if (isYoung)
        phd_TranslateRel(44, -48, -50);
    else if (CHK_EXI(gfLevelFlags, SLEV_YOUNG_LARA))
        phd_TranslateRel(-52, -48, -50);
    else
        phd_TranslateRel(-4, -4, -48);

    pos.x = phd_mxptr->m03 >> W2V_SHIFT;
    pos.y = phd_mxptr->m13 >> W2V_SHIFT;
    pos.z = phd_mxptr->m23 >> W2V_SHIFT;
    phd_PopMatrix();

    hair = hairs[isYoung];
    bone = &bones[objects[HAIR].bone_index];
    if (first_hair[isYoung])
    {
        first_hair[isYoung] = FALSE;
        hair[0].pos.x = pos.x;
        hair[0].pos.y = pos.y;
        hair[0].pos.z = pos.z;

        for (int i = 0; i < HAIR_SEGMENTS; i++)
        {
            phd_PushUnitMatrix();
            phd_mxptr->m03 = hair[i].pos.x << W2V_SHIFT;
            phd_mxptr->m13 = hair[i].pos.y << W2V_SHIFT;
            phd_mxptr->m23 = hair[i].pos.z << W2V_SHIFT;
            phd_RotYXZ(hair->pos.y_rot, hair->pos.x_rot, hair->pos.z_rot);
            phd_TranslateRel(bone[1], bone[2], bone[3]);
            bone += 4;
            hair[i + 1].pos.x = phd_mxptr->m03 >> W2V_SHIFT;
            hair[i + 1].pos.y = phd_mxptr->m13 >> W2V_SHIFT;
            hair[i + 1].pos.z = phd_mxptr->m23 >> W2V_SHIFT;
            phd_PopMatrix();
        }

        wind = 0;
        wind_x = 0;
        wind_z = 0;
        wind_angle = 0x800;
        dwind_angle = 0x800;
    }
    else
    {
        hair[0].pos.x = pos.x;
        hair[0].pos.y = pos.y;
        hair[0].pos.z = pos.z;
        
        x = lara_item->pos.x + (frame[0] + frame[1]) / 2;
        y = lara_item->pos.y + (frame[2] + frame[3]) / 2;
        z = lara_item->pos.z + (frame[4] + frame[5]) / 2;
        room_number = lara_item->room_number;

        if (inWater)
            wh = -NO_HEIGHT;
        else
            wh = GetWaterHeight(x, y, z, room_number);

        {
            int rdm;
            int diff;
            rdm = (GetRandomControl() & 7);
            wind += rdm - 3;
            if (wind <= -2)
                wind++;
            else if (wind >= 9)
                wind--;

            dwind_angle += ((GetRandomControl() & 63) - 32) << 1;
            dwind_angle &= 8190;

            if (dwind_angle < 1024)
                dwind_angle += (1024 - dwind_angle) << 1;
            else if (dwind_angle > 3072)
                dwind_angle -= (dwind_angle - 3072) << 1;

            diff = dwind_angle - wind_angle;
            wind_angle += diff >> 3;
            wind_angle &= 8190;
        }

        wind_x = 0/*(SINN(wind_angle) * wind) >> 12*/;
        wind_z = 0/*(COSN(wind_angle) * wind) >> 12*/;

        for (int i = 1; i < (HAIR_SEGMENTS + 1); i++, bone += 4)
        {
            // Store current position in vel[0] (which isn't used otherwise)
            hair[0].vel.x = hair[i].pos.x;
            hair[0].vel.y = hair[i].pos.y;
            hair[0].vel.z = hair[i].pos.z;

            if (inWater)
            {
                height = NO_HEIGHT;
            }
            else
            {
                floor = GetFloor(hair[i].pos.x, hair[i].pos.y, hair[i].pos.z, &room_number);
                height = GetHeight(floor, hair[i].pos.x, hair[i].pos.y, hair[i].pos.z);
            }

            // Add velocity
            hair[i].pos.x += hair[i].vel.x * 3 / 4;
            hair[i].pos.y += hair[i].vel.y * 3 / 4;
            hair[i].pos.z += hair[i].vel.z * 3 / 4;

            if (lara.water_status == LWS_ABOVEWATER && CHK_EXI(rooms[room_number].flags, ROOM_OUTSIDE))
            {
                hair[i].pos.x += wind_x;
                hair[i].pos.z += wind_z;
            }

            switch (lara.water_status)
            {
                case LWS_ABOVEWATER:
                    // Gravity on next hair segment point
                    hair[i].pos.y += 10;

                    if (wh != -NO_HEIGHT && hair[i].pos.y > wh)
                    {
                        hair[i].pos.y = wh;
                    }
                    else
                    {
                        hair[i].pos.x = hair[0].vel.x;
                        hair[i].pos.z = hair[0].vel.z;
                    }
                    break;
                case LWS_UNDERWATER:
                case LWS_SURFACE:
                case LWS_WADE:
                    // Don't allow hair above water surface or below floor
                    if (hair[i].pos.y < wh)
                        hair[i].pos.y = wh;
                    else if (hair[i].pos.y > height)
                        hair[i].pos.y = wh;
                    break;
            }

            // Do collision with lara spheres to see if this point is inside
            // if so, move out
            for (int j = 0; j < S_MAX; j++)
            {
                x = hair[i].pos.x - sphere[j].x;
                y = hair[i].pos.y - sphere[j].y;
                z = hair[i].pos.z - sphere[j].z;
                distance = SQUARE(x) + SQUARE(y) + SQUARE(z);
                range = SQUARE(sphere[j].r);

                if (distance < range)
                {
                    distance = phd_sqrt(distance);
                    if (distance == 0) // fix divised by zero error
                        distance = 1;

                    hair[i].pos.x = sphere[j].x + (x * sphere[j].r) / distance;
                    hair[i].pos.y = sphere[j].y + (y * sphere[j].r) / distance;
                    hair[i].pos.z = sphere[j].z + (z * sphere[j].r) / distance;
                }
            }

            x = SQUARE(hair[i].pos.x - hair[i - 1].pos.x);
            y = hair[i].pos.y - hair[i - 1].pos.y;
            z = SQUARE(hair[i].pos.z - hair[i - 1].pos.z);
            distance = phd_sqrt(x + z);
            hair[i - 1].pos.y_rot = phd_atan(z, x);
            hair[i - 1].pos.x_rot = -phd_atan(distance, y);

            phd_PushUnitMatrix();
            phd_mxptr->m03 = hair[i - 1].pos.x << W2V_SHIFT;
            phd_mxptr->m13 = hair[i - 1].pos.y << W2V_SHIFT;
            phd_mxptr->m23 = hair[i - 1].pos.z << W2V_SHIFT;
            phd_RotYXZ(hair[i - 1].pos.y_rot, hair[i - 1].pos.x_rot, 0);

            if (i == HAIR_SEGMENTS)
                phd_TranslateRel(*(bone-1), *(bone-2), *(bone-3));
            else
                phd_TranslateRel(*(bone+1), *(bone+2), *(bone+3));

            hair[i].pos.x = phd_mxptr->m03 >> W2V_SHIFT;
            hair[i].pos.y = phd_mxptr->m13 >> W2V_SHIFT;
            hair[i].pos.z = phd_mxptr->m23 >> W2V_SHIFT;

            // Calc velocity from change in position
            hair[i].vel.x = hair[i].pos.x - hair[0].vel.x;
            hair[i].vel.y = hair[i].pos.y - hair[0].vel.y;
            hair[i].vel.z = hair[i].pos.z - hair[0].vel.z;

            phd_PopMatrix();
        }
    }
}

/*
void DrawHair(void)
{
    
}
*/


#ifdef DLL_INJECT
void injector::inject_hair()
{
    this->inject(0x0041FDD0, InitialiseHair);
    this->inject(0x0041FE50, HairControl);
    //this->inject(0x00420920, DrawHair);
}
#endif