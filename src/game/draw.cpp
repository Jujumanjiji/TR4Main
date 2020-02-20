#include "framework.h"
#include "draw.h"
#include "3dsystem/3d_gen.h"
#include "3dsystem/3d_gen_a.h"
#include "game/control.h"
#include "game/delstuff.h"
#include "game/deltapak.h"
#include "game/effects.h"
#include "game/items.h"
#include "game/health.h"
#include "game/tomb4fx.h"
#include "specific/drawprimitive.h"
#include "specific/output.h"
#include "specific/specific.h"
#include "utils/utils.h"

DOOR_VBUF vbuf[4];

// TODO: found what are these !!
#define Draw403C10 ((void(__cdecl*)(void)) 0x00403C10)
#define Draw48D9B0 ((void(__cdecl*)(void)) 0x0048D9B0)

/*
int DrawPhaseGame(void)
{
    CalcLaraMatrices(FALSE);
    phd_PushUnitMatrix();
    CalcLaraMatrices(TRUE);
    phd_PopMatrix();

    if (GnPlayingCutseq)
        frigup_lara();

    SetLaraUnderwaterNodes();
    DrawRooms(camera.src.room_number);
    DrawGameInfo(TRUE);
     
    S_OutputPolyList();
    camera.number_frames = S_DumpScreen();
    S_AnimateTextures(camera.number_frames);
    return camera.number_frames;
}

void DrawRooms(short current_room)
{
    ROOM_INFO* r;
    int bakx, baky, bakz, bakroom;
    
    CurrentRoom = int(current_room);
    r = &rooms[current_room];
    r->test_left = 0;
    r->test_top = 0;
    r->test_right = phd_winxmax;
    r->test_bottom = phd_winymax;
    phd_left = 0;
    phd_top = 0;
    phd_right = int(phd_winxmax);
    phd_bottom = int(phd_winymax);

    outside = r->flags & ROOM_HORIZON;
    underwater = r->flags & ROOM_WATER;

    r->bound_active = 2;

    bound_list[0] = current_room;
    bound_start = 0;
    bound_end = 1;
    draw_rooms_number = 0;

    if (outside)
    {
        outside_top = 0;
        outside_left = 0;
        outside_right = int(phd_winxmax);
        outside_bottom = int(phd_winymax);
    }
    else
    {
        outside_top = int(phd_winymax);
        outside_left = int(phd_winxmax);
        outside_right = 0;
        outside_bottom = 0;
    }
    
    GetRoomBounds();
    InitialiseFogBulbs();
    CreateFXBulbs();

    // draw horizon, do lightning.
    if (outside)
    {
        // only if it's loaded !
        if (objects[HORIZON].loaded)
        {
            if (BinocularRange)
                AlterFOV(DEFAULT_FOV - BinocularRange);

            phd_PushMatrix();
            phd_TranslateAbs(camera.src.x, camera.src.y, camera.src.z);

            // draw lightning
            if (CHK_ANY(gf_level_flags, SLEV_LIGHTNING))
            {
                // is drawed or random ?
                if (LightningDraw || LightningRandom)
                {
                    UpdateLightning();
                    LightningTimer--;
                    if (LightningTimer > -1)
                        LightningTimer--;
                    if (!LightningTimer)
                        SoundEffect(182, nullptr, NULL); // TODO: SFX_LIGHTNING
                }
                // start a new lightning randomly
                else if (!(GetRandomDraw() & 0x7F))
                {
                    LightningDraw = short((GetRandomDraw() & 0x1F) + 16);
                    LightningStart = short(GetRandomDraw() + 256);
                    LightningTimer = short((GetRandomDraw() & 3) + 12);
                }
            }

            RenderTypeDebug = RENDER_HORIZON_AND_LAYER;
            phd_PushMatrix();

            // draw layer
            if (CHK_ANY(gf_level_flags, (SLEV_LAYER1 | SLEV_LAYER2)))
            {
                // draw layer 1
                if (CHK_ANY(gf_level_flags, SLEV_LAYER1))
                {
                    phd_RotY(0x7FF8);
                    if (CHK_ANY(gf_level_flags, SLEV_LIGHTNING))
                        DrawHorizonLayer(gfLayer1Blue | ((gfLayer1Green | (unsigned short)((gfLayer1Red | 0x2C00) << 8)) << 8), gfLayer1Color2, -1536, 4);
                    else
                        DrawHorizonLayer(gfLayer1Color, gfLayer1Color2, -1536, 4);
                }

                // draw layer 2
                if (CHK_ANY(gf_level_flags, SLEV_LAYER2))
                {
                    DrawHorizonLayer(gfLayer2Color, gfLayer2Color2, -1536, 2);
                }

                // calc layer first (else it will be drawed throught the level)
                CalcHorizonAndLayer(); // else it will draw in the level geometry
            }

            phd_PopMatrix();

            // draw horizon
            if (CHK_ANY(gf_level_flags, SLEV_HORIZON))
            {
                DrawHorizon(meshes[objects[HORIZON].mesh_index]);
                // calc the horizon secondly (else the layer will cause wreid render with horizon)
                CalcHorizonAndLayer();
            }

            phd_PopMatrix();

            if (BinocularRange)
                AlterFOV(DEFAULT_FOV - BinocularRange);
        }
        else
        {
            outside = NO_HORIZON;
        }
    }

    // lara is loaded ? draw it !
    if (objects[LARA].loaded)
    {
        if (!(lara_item->flags & FITEM_INVISIBLE))
        {
            RenderTypeDebug = RENDER_LARA;
            if (lara_item->mesh_bits) // lara have mesh ?
            {
                DrawLara(lara_item, 0);

                phd_PushMatrix();

                // set left gunflash
                if (lara.left_arm.flash_gun)
                {
                    phd_SwapMatrix(phd_mxptr, phd_laralefthand);
                    SetGunFlash_Left(lara.gun_type);
                }

                // set right gunflash
                if (lara.right_arm.flash_gun)
                {
                    phd_SwapMatrix(phd_mxptr, phd_lararighthand);
                    SetGunFlash_Right(lara.gun_type);
                }

                phd_PopMatrix();

                DrawGunFlashes();
            }

            if (CHK_ANY(gf_level_flags, SLEV_MIRROR) && lara_item->room_number == gf_mirror_room)
                DrawLara_Mirror();
        }
    }

    InitDynamicLighting();

    RenderTypeDebug = RENDER_DYNAMICS;
    for (int i = 0; i < MAX_DYNAMICS; i++)
    {
        if (dynamics[i].on)
        {
            if (dynamics[i].x < 0)
                dynamics[i].x = 0;
            if (dynamics[i].z < 0)
                dynamics[i].z = 0;
        }
    }

    for (int i = 0; i < draw_rooms_number; i++)
        PrintRooms(draw_rooms[i]);

    if (CHK_ANY(gf_level_flags, SLEV_TRAIN))
        DrawTrainEffect(); // needed to draw the desert railroad level correctly ! else no floor texture or horizon & layer

    DrawGunShells();
    if (GnPlayingCutseq)
        DrawCutSeqActors();

    DrawRope();
    DrawSparks();

    bakx = lara_item->pos.x;
    baky = lara_item->pos.y;
    bakz = lara_item->pos.z;
    bakroom = lara_item->room_number;
    lara_item->pos.x = camera.src.x;
    lara_item->pos.y = camera.src.y;
    lara_item->pos.z = camera.src.z;
    lara_item->room_number = camera.src.room_number;

    RenderTypeDebug = RENDER_EFFECT;
    DrawWeather();
    DrawFires();
    DrawSmoke();
    DrawSplashes();
    DrawBubbles();
    DrawDebris();
    DrawBlood();
    DrawDrips();
    DrawShockwaves();
    DrawLightning();
    Draw403C10();
    Draw48D9B0();

    lara_item->pos.x = bakx;
    lara_item->pos.y = baky;
    lara_item->pos.z = bakz;
    lara_item->room_number = bakroom;

    if (CHK_ANY(gf_level_flags, SLEV_LENS_FLARE))
        SetUpLensFlare(gf_lens_flare.x, gf_lens_flare.y - 0x1000, gf_lens_flare.z, nullptr);

    if (LaserSightActive)
        DrawLaserSightSprite();

    for (int j = 0; j < draw_rooms_number; j++)
        PrintObjects(draw_rooms[j]);
}

void PrintRooms(short roomNumber)
{
    CurrentRoom = roomNumber;
    ROOM_INFO* r;

    r = &rooms[roomNumber];
    phd_TranslateAbs(0, 0, 0);
    phd_left = r->left;
    phd_right = r->right;
    phd_top = r->top;
    phd_bottom = r->bottom;
    S_InsertRoom(r);
}

void PrintObjects(short roomNumber)
{
    ROOM_INFO* r;
    ITEM_INFO* item;
    MESH_INFO* mesh;
    LPDRAW draw;
    LPDRAWEXTRA draw_extra;
    int clip;

    CurrentRoom = roomNumber;
    r = &rooms[roomNumber];
    r->bound_active = false;

    phd_PushMatrix();
    phd_TranslateAbs(r->x, r->y, r->z);
    phd_left = r->left;
    phd_right = r->right;
    phd_top = r->top;
    phd_bottom = r->bottom;

    if (CHK_ANY(gf_level_flags, SLEV_TRAIN))
    {
        phd_left = 0;
        phd_top = 0;
        phd_right = phd_winxmax + 1;
        phd_bottom = phd_winymax + 1;
    }

    RenderTypeDebug = RENDER_STATIC;
    mesh = r->mesh;
    for (short i = 0; i < r->num_meshes; i++)
    {
        if (mesh->ocb & 1)
        {
            phd_PushMatrix();
            phd_TranslateAbs(mesh->x, mesh->y, mesh->z);
            phd_RotY(mesh->y_rot);

            clip = S_GetObjectBounds(&static_objects[mesh->static_number].x_minp);
            if (clip)
            {
                S_CalculateStaticMeshLight(mesh->x, mesh->y, mesh->z, mesh->shade, r);
                phd_PutPolygons(meshes[static_objects[mesh->static_number].mesh_number], clip);
            }

            phd_PopMatrix();
        }

        mesh++; // next static mesh
    }

    RenderTypeDebug = RENDER_OBJECTS;
    ///not needed since it's used by the train but it's set upward ! (no change when added) (added by default)
    phd_left = 0;
    phd_top = 0;
    phd_right = phd_winxmax + 1;
    phd_bottom = phd_winymax + 1;
    for (short j = r->item_number; j != NO_ITEM; j = items[j].next_item)
    {
        CurrentDeathItemRoom = roomNumber;

        item = &items[j];
        if (item->status != FITEM_INVISIBLE)
        {
            if (item->after_death)
                CurrentDeathItemColor = 0xFE000000 * item->after_death;

            // draw classic render
            draw = objects[item->object_number].draw_routine;
            if (draw)
                draw(item);

            // draw extra render
            draw_extra = objects[item->object_number].draw_routine_extra;
            if (draw_extra)
                draw_extra(item);

            CurrentDeathItemColor = 0xFF000000; // default
        }

        // smoothly fade away the item and destroy it from the render
        if (item->after_death > 0)
        {
            if (item->after_death < 128)
                item->after_death++;
            if (item->after_death >= 128)
                KillItem(j);
        }
    }

    RenderTypeDebug = RENDER_EFFECT;
    for (short k = r->fx_number; k != NO_ITEM; k = effects[k].next_fx)
    {
        DrawEffect(k);
    }

    phd_PopMatrix();

    r->left = phd_winxmax;
    r->top = phd_winymax;
    r->right = 0;
    r->bottom = 0;
}
*/
/*
void GetRoomBounds(void)
{
    ROOM_INFO* r;
    short* door;
    int i, room_number;

    while (bound_start != bound_end)
    {
        room_number = bound_list[(bound_start++) % 128];
        r = &rooms[room_number];
        r->bound_active -= 2;

        ENLARGEDOWN(r->test_left, r->left);
        ENLARGEDOWN(r->test_top, r->top);
        ENLARGEUP(r->test_right, r->right);
        ENLARGEUP(r->test_bottom, r->bottom);

        if (!(r->bound_active & 1))
        {
            draw_rooms[draw_rooms_number++] = room_number;
            r->bound_active |= 1;

            if (r->flags & ROOM_HORIZON)
                outside = ROOM_HORIZON;
        }

        if (r->flags & ROOM_HORIZON)
        {
            ENLARGEDOWN(r->left, outside_left);
            ENLARGEUP(r->right, outside_right);
            ENLARGEDOWN(r->top, outside_top);
            ENLARGEUP(r->bottom, outside_bottom);
        }

        phd_PushMatrix();
        phd_TranslateAbs(r->x, r->y, r->z);

        if ((door = r->door))
        {
            for (i = *(door++); i > 0; i--, door += 15)
            {
                room_number = *(door++);
                if (((int)(*(door + 0) * (r->x + *(door + 3) - w2v_matrix[M03])) +
                     (int)(*(door + 1) * (r->y + *(door + 4) - w2v_matrix[M13])) +
                     (int)(*(door + 2) * (r->z + *(door + 5) - w2v_matrix[M23]))) >= 0)
                    continue;

                SetRoomBounds(door, room_number, r);
            }
        }

        phd_PopMatrix();
    }
}

void SetRoomBounds(short* door, int roomNumber, ROOM_INFO* parent)
{
    ROOM_INFO* r;
    DOOR_VBUF* dest, *last;
    int* mptr;
    int z_behing, z_toofar;
    int left, right, top, bottom;
    int xv, yv, zv, xs, ys;
    int i;

    r = &rooms[roomNumber];
    if (r->left <= parent->test_left && r->right >= parent->test_right && r->top <= parent->test_top && r->bottom >= parent->test_bottom)
        return;

    left = parent->test_left;
    right = parent->test_right;
    top = parent->test_top;
    bottom = parent->test_bottom;
    mptr = phd_mxptr;
    dest = vbuf;
    door += 3;
    z_behing = 0;
    z_toofar = 0;

    for (i = 0; i < 4; i++, dest++, door += 3)
    {
        dest->xv = xv = (door[0] * mptr[M00] + door[1] * mptr[M01] + door[2] * mptr[M02]) + mptr[M03];
        dest->yv = yv = (door[0] * mptr[M10] + door[1] * mptr[M11] + door[2] * mptr[M12]) + mptr[M13];
        dest->zv = zv = (door[0] * mptr[M20] + door[1] * mptr[M21] + door[2] * mptr[M22]) + mptr[M23];

        if (zv > 0)
        {
            if (zv > phd_zfar)
                z_toofar++;

            zv /= phd_persp;
            if (zv)
            {
                xs = xv / zv + phd_centerx;
                ys = yv / zv + phd_centery;
            }
            else
            {
                xs = (xv >= 0) ? phd_right : phd_left;
                ys = (yv >= 0) ? phd_bottom : phd_top;
            }

            if ((xs - 1) < left)
                left = xs - 1;
            if ((xs + 1) > right)
                right = xs + 1;

            if ((ys - 1) < top)
                top = ys - 1;
            if ((ys + 1) > bottom)
                bottom = ys + 1;
        }
        else
        {
            z_behing++;
        }
    }

    if (z_behing == 4)
        return;
    if (z_toofar == 4 && outside)
        return;

    if (z_behing > 0)
    {
        dest = vbuf;
        last = dest + 3;

        for (i = 0; i < 4; i++, last = dest, dest++)
        {
            if ((dest->zv <= 0) ^ (last->xv <= 0))
            {
                if (dest->xv < 0 && last->xv < 0)
                {
                    left = 0;
                }
                else if (dest->xv > 0 && last->xv > 0)
                {
                    right = phd_winxmax;
                }
                else
                {
                    left = 0;
                    right = phd_winxmax;
                }

                if (dest->zv < 0 && last->yv < 0)
                {
                    top = 0;
                }
                else if (dest->yv > 0 && last->yv > 0)
                {
                    bottom = phd_winymax;
                }
                else
                {
                    top = 0;
                    bottom = phd_winymax;
                }
            }
        }
    }

    CLIPDOWN(left, parent->test_left);
    CLIPUP(right, parent->test_right);
    CLIPDOWN(top, parent->test_top);
    CLIPUP(bottom, parent->test_bottom);

    if (left >= right || top >= bottom)
        return;

    if (r->bound_active & 2)
    {
        ENLARGEDOWN2(left, r->test_left);
        ENLARGEDOWN2(top, r->test_top);
        ENLARGEUP2(right, r->test_right);
        ENLARGEUP2(bottom, r->test_bottom);
    }
    else
    {
        bound_list[(bound_end++) % 128] = roomNumber;
        r->bound_active |= 2;
        r->test_left = (short)left;
        r->test_right = (short)right;
        r->test_top = (short)top;
        r->test_bottom = (short)bottom;
    }
}


void DrawEffect(short fxNumber)
{
    FX_INFO* fx;
    OBJECT_INFO* obj;

    fx = &effects[fxNumber];
    obj = &objects[fx->object_number];

    if (obj->loaded && obj->draw_routine)
    {
        phd_PushMatrix();
        phd_TranslateAbs(fx->pos.x, fx->pos.y, fx->pos.z);

        if (phd_mxptr[M23] > phd_znear && phd_mxptr[M23] < phd_zfar)
        {
            phd_RotYXZ(fx->pos.y_rot, fx->pos.x_rot, fx->pos.z_rot);
            if (obj->nmeshes)
                phd_PutPolygons(meshes[obj->mesh_index], NO_CLIP);
            else
                phd_PutPolygons(meshes[fx->current_frame], NO_CLIP);
        }

        phd_PopMatrix();
    }
}
*/
/*
void DrawAnimatingItem(ITEM_INFO* item)
{
    OBJECT_INFO* obj;
    short *frame[2];
    short *rotation, *rotationW;
    short *extra_rotation;
    short **mesh;
    int frac, rate;
    int clip;
    int *bone;
    int meshBits, meshSwapBits;

    frac = GetFrames(item, frame, &rate);
    obj = &objects[item->object_number];

    if (obj->shadow_size)
        S_PrintShadow(obj->shadow_size, frame[0], item);

    phd_PushMatrix();
    phd_TranslateAbs(item->pos.x, item->pos.y, item->pos.z);
    phd_RotYXZ(item->pos.y_rot, item->pos.x_rot, item->pos.z_rot);

    if (item->object_number < ENEMY_JEEP || item->object_number > SETHA_MIP)
    {
        DrawMovingItem(item, frame[0]);
    }
    else
    {
        phd_left = 0;
        phd_right = phd_winwidth;
        phd_top = 0;
        phd_bottom = phd_winheight;
    }

    clip = S_GetObjectBounds(frame[0]);
    if (clip) // when drawed
    {
        CalculateObjectLighting(item, frame[0]);

        if (!item->data)
            extra_rotation = dummy_rotation;
        else
            extra_rotation = (short*)item->data;

        mesh = &meshes[obj->mesh_index];
        bone = &bones[obj->bone_index];
        meshBits = 1;

        // normal
        if (!frac)
        {
            phd_TranslateRel((int)*(frame[0] + 6), (int)*(frame[0] + 7), (int)*(frame[0] + 8));
            rotation = (short*)(frame[0] + 9);
            gar_RotYXZsuperpack(&rotation, 0);

            if (item->mesh_bits & 1)
            {
                if (item->meshswap_meshbits & 1)
                    phd_PutPolygons(mesh[1], clip);
                else
                    phd_PutPolygons(mesh[0], clip);
            }

            for (int i = 0; i < obj->nmeshes; i++, mesh += 2)
            {
                if (i > 0)
                {
                    if (bone[BT_FLAG] & 1)
                        phd_PopMatrix();
                    if (bone[BT_FLAG] & 2)
                        phd_PushMatrix();

                    phd_TranslateRel(*(bone + 1), *(bone + 2), *(bone + 3));
                    gar_RotYXZsuperpack(&rotation, 0);

                    if (extra_rotation && CHK_ANY(bone[BT_FLAG], (ROT_Y | ROT_X | ROT_Z)))
                    {
                        if (bone[BT_FLAG] & ROT_Y)
                            phd_RotY(*(extra_rotation++));

                        if (bone[BT_FLAG] & ROT_X)
                            phd_RotX(*(extra_rotation++));

                        if (bone[BT_FLAG] & ROT_Z)
                            phd_RotZ(*(extra_rotation++));
                    }

                    meshSwapBits = meshBits * 2;
                    meshBits *= 2;
                    if (meshSwapBits & item->mesh_bits)
                    {
                        if (item->meshswap_meshbits & meshSwapBits)
                            phd_PutPolygons(mesh[1], clip);
                        else
                            phd_PutPolygons(mesh[0], clip);
                    }

                    if (item->fired_weapon) // TODO: check EnemyOffset !!
                    {

                    }

                    ///meshBits <<= 1;
                    bone += 4;
                }
            }
        }
        // interpolate
        else
        {
            InitInterpolate(frac, rate);
            phd_TranslateRel_ID((int)*(frame[0] + 6), (int)*(frame[0] + 7), (int)*(frame[0] + 8),
                                (int)*(frame[1] + 6), (int)*(frame[1] + 7), (int)*(frame[1] + 8));

            rotation = (short*)(frame[0] + 9);
            rotationW = (short*)(frame[1] + 9);
            gar_RotYXZsuperpack_I(&rotation, &rotationW, 0);

            if (item->mesh_bits & 1)
            {
                if (item->meshswap_meshbits & 1)
                    phd_PutPolygons_I(mesh[1], clip);
                else
                    phd_PutPolygons_I(mesh[0], clip);
            }

            for (int i = 0; i < obj->nmeshes; i++, mesh += 2)
            {
                if (i > 0)
                {
                    if (bone[BT_FLAG] & 1)
                        phd_PopMatrix_I();

                    if (bone[BT_FLAG] & 2)
                        phd_PushMatrix_I();

                    phd_TranslateRel_I(*(bone + 1), *(bone + 2), *(bone + 3));
                    gar_RotYXZsuperpack_I(&rotation, &rotationW, 0);

                    if (extra_rotation && CHK_ANY(bone[BT_FLAG], (ROT_Y | ROT_X | ROT_Z)))
                    {
                        if (bone[BT_FLAG] & ROT_Y)
                            phd_RotY_I(*(extra_rotation++));

                        if (bone[BT_FLAG] & ROT_X)
                            phd_RotX_I(*(extra_rotation++));

                        if (bone[BT_FLAG] & ROT_Z)
                            phd_RotZ_I(*(extra_rotation++));
                    }

                    meshSwapBits = meshBits * 2;
                    meshBits *= 2;
                    if (meshSwapBits & item->mesh_bits)
                    {
                        if (item->meshswap_meshbits & meshSwapBits)
                            phd_PutPolygons_I(mesh[1], clip);
                        else
                            phd_PutPolygons_I(mesh[0], clip);
                    }

                    ///meshBits <<= 1;
                    bone += 4;
                }
            }
        }
    }

    phd_left = 0;
    phd_top = 0;
    phd_right = phd_winwidth;
    phd_bottom = phd_winheight;
    phd_PopMatrix();
}

void gar_RotYXZsuperpack(short** pprot, int skip)
{
    short v2;
    WORD *prot;
    WORD shift;
    int alt;

    while (skip)
    {
        prot = (WORD*)*pprot; // get the current value.
        if (*prot & (3 << 14))
            (*pprot) += 1;
        else
            (*pprot) += 2;
        --skip;
        //*pprot = (short*)prot; // assign the new value.
    }

    prot = (WORD*)*pprot;
    shift = (*prot >> W2V_SHIFT);
    switch (shift)
    {
        case 0:
            phd_RotYXZpack((*prot << 16) + *(prot + 1));
            (*pprot) += 2;
            return;
        case 1:
            phd_RotX(short(*prot) * 16);
            break;
        case 2:
            phd_RotY(short(*prot) * 16);
            break;
        case 3:
            phd_RotZ(short(*prot) * 16);
            break;
    }

    (*pprot) += 1;
}*/

void injector::inject_draw()
{
    //this->inject(0x0044EBA0, DrawPhaseGame);
    //this->inject(0x0044EC10, DrawRooms);
    //this->inject(0x0044F2D0, PrintRooms);
    //this->inject(0x0044F330, PrintObjects);
    //this->inject(0x0044F5D0, GetRoomBounds);
    //this->inject(0x0044F790, SetRoomBounds);
    //this->inject(0x0044FB10, DrawEffect);
    //this->inject(0x0044FC00, DrawMovingItem);
    //this->inject(0x0044FF60, DrawAnimatingItem);
    //this->inject(0x00450520, InitInterpolate);
    //this->inject(0x00450560, phd_PopMatrix_I);
    //this->inject(0x00450590, phd_PushMatrix_I);
    //this->inject(0x004505C0, phd_RotY_I);
    //this->inject(0x004505F0, phd_RotX_I);
    //this->inject(0x00450620, phd_RotZ_I);
    //this->inject(0x00450650, phd_TranslateRel_I);
    //this->inject(0x00450690, phd_TranslateRel_ID);
    //this->inject(0x004506E0, phd_RotYXZ_I);
    //this->inject(0x00450720, gar_RotYXZsuperpack_I);
    //this->inject(0x00450760, gar_RotYXZsuperpack);
    //this->inject(0x00450810, phd_PutPolygons_I);
    //this->inject(0x00450840, InterpolateMatrix);
    //this->inject(0x00450AB0, InterpolateArmMatrix);
    //this->inject(0x00450BB0, CalculateObjectLighting);
    //this->inject(0x00450CB0, CalculateObjectLightingLara);
    //this->inject(0x00450DC0, GetFrames);
    //this->inject(0x00450E60, GetBoundsAccurate);
    //this->inject(0x00450EE0, GetBestFrame);
    
}