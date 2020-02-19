#include "framework.h"
#include "utils_render.h"
#include "utils.h"
#include "3dsystem/3d_gen.h"
#include "3dsystem/3d_gen_a.h"
#include "game/control.h"
#include "game/draw.h"
#include "game/sphere.h"
#include "specific/output.h"
#include "specific/specific.h"

Baddy1Shape baddy1shape;

void Baddy1Shape::assign_item(ITEM_INFO* item)
{
    this->item = item;
}

void Baddy1Shape::assign_object(short objNumber)
{
    this->obj = &objects[objNumber];
}

void Baddy1Shape::assign_object_second(short objNumber)
{
    this->obj_sec = &objects[objNumber];
}

void Baddy1Shape::create_interpolate()
{
    this->frac = GetFrames(item, frame, &rate);
}

void Baddy1Shape::create_shadow()
{
    if (obj->shadow_size)
        S_PrintShadow(obj->shadow_size, frame[0], item);
}

void Baddy1Shape::create_rotation()
{
    rot = (OBJ_ROTATION*)item->data;
    if (rot == NULL)
    {
        rot = (OBJ_ROTATION*)malloc(sizeof(OBJ_ROTATION));
        rot->head_x = 0;
        rot->head_y = 0;
        rot->torso_x = 0;
        rot->torso_y = 0;
    }
}

void Baddy1Shape::set_world()
{
    phd_PushMatrix(); // world
    phd_TranslateAbs(item->pos.x, item->pos.y, item->pos.z);
    phd_RotYXZ(item->pos.y_rot, item->pos.x_rot, item->pos.z_rot);
}

void Baddy1Shape::set_clipping()
{
    clip = S_GetObjectBounds(frame[0]);
}

void Baddy1Shape::set_light()
{
    CalculateObjectLighting(item, frame[0]);
}

void Baddy1Shape::assign_bone()
{
    bone = &bones[obj->bone_index];
}

void Baddy1Shape::set_list()
{
    phd_PushMatrix();
}

void Baddy1Shape::assign_rotation()
{
    rotation1 = (short*)(frame[0] + 9);
    rotation2 = (short*)(frame[1] + 9);
}

void Baddy1Shape::add_mesh(RENDER human)
{
    if (human.start)
    {
        if (frac)
            InitInterpolate(frac, rate);
        phd_SwapTranslateRel(frac, frame[0], frame[1]);
        phd_SwapGarYXZsuperpack(frac, &rotation1, &rotation2);
        mesh = &meshes[MESH(obj, human.meshID)];
        phd_SwapPutPolygons(frac, clip, mesh);
    }
    else
    {
        lpAddons addon;

        if (human.push)
            phd_SwapPushMatrix(frac);

        phd_SwapTranslateRel(frac, BONE(bone, human.boneID));

        if (human.rotTorso)
        {
            phd_SwapRotY(frac, rot->torso_y);
            phd_SwapRotX(frac, rot->torso_x);
        }

        if (human.rotHead)
        {
            phd_SwapRotY(frac, rot->head_y);
            phd_SwapRotX(frac, rot->head_x);
        }

        phd_SwapGarYXZsuperpack(frac, &rotation1, &rotation2);

        addon = human.function;
        if (addon != NULL)
            mesh = addon(item, obj, obj_sec, human.meshID);
        else
            mesh = &meshes[MESH(obj, human.meshID)];
        
        phd_SwapPutPolygons(frac, clip, mesh);

        if (human.pop)
            phd_SwapPopMatrix(frac);
    }
}

void Baddy1Shape::end_list()
{
    phd_PopMatrix();
}

void Baddy1Shape::end_world()
{
    phd_PopMatrix();
}

void Baddy1Shape::add_flash(BITE_INFO* bite, bool popEnd)
{
    if (item->fired_weapon)
    {
        phd_SwapPushMatrix(frac);
        phd_SwapTranslateRel(frac, BONE(bite->x, bite->y, bite->z));
        phd_SwapRotX(frac, -ANGLE(90));
        phd_SwapRotZ(frac, (GetRandomDraw() << 14) + (GetRandomDraw() >> 2) - 4096);
        phd_SwapPutPolygons(frac, clip, &meshes[objects[GUN_FLASH].mesh_index]);
        phd_SwapPopMatrix(frac);
        --item->fired_weapon;
    }

    if (popEnd)
        phd_SwapPopMatrix(frac);
}