#include "framework.h"
#include "file.h"
#include "init.h"
#include "setup.h"
#include "utils.h"
#include "file_utils.h"

#define number_fogbulbs         VAR_U_(0x0052A348, int)
#define number_meshes           VAR_U_(0x00533A50, int)
#define number_rooms            VAR_U_(0x00533A5C, short)
#define number_anims            VAR_U_(0x00533A60, int)
#define number_roomlight        VAR_U_(0x00536F04, int)

void LoadRooms(void)
{
    ROOM_INFO* r;
    MESH_INFO* mesh;
    WORD number_doors;
    int size;

    S_Log(LT_Enter, "LoadRooms()", false);

    wibble = 0;
    number_roomlight = 0;
    number_fogbulbs = 0;

    ReadSDword();
    number_rooms = ReadSWord();
    Alloc(rooms, ROOM_INFO, number_rooms);

    // load all rooms:
    for (int i = 0; i < number_rooms; i++)
    {
        r = &rooms[i];
        r->x = ReadSDword();
        r->y = 0;
        r->z = ReadSDword();
        r->minfloor = ReadSDword();
        r->maxceiling = ReadSDword();

        size = ReadSDword();
        AllocReadT(r->data, short, size);

        // read doors
        number_doors = ReadUWord();
        if (number_doors)
        {
            size = sizeof(short) * 16 * number_doors; // sizeof(DOORS); but the DOORS size is wrong !
            r->door = (short*)game_malloc(size);
            r->door[0] = number_doors;
            memcpy(r->door + 1, level_data, size);
            level_data += size;
        }
        else
        {
            r->door = NULL;
        }

        r->x_size = ReadSWord();
        r->y_size = ReadSWord();

        size = (int)(r->x_size * r->y_size);
        AllocReadT(r->floor, FLOOR_INFO, size);

        r->ambiant = ReadUDword();
        r->num_light = ReadSWord();
        if (r->num_light)
        {
            AllocReadT(r->light, LIGHT_INFO, r->num_light);
        }
        else
        {
            r->light = NULL;
        }

        r->num_meshes = ReadSWord();
        if (r->num_meshes)
        {
            AllocReadT(r->mesh, MESH_INFO, r->num_meshes);

            for (int m = 0; m < r->num_meshes; m++)
            {
                mesh = &r->mesh[m];
                mesh->ocb = 1; // draw mesh :)
            }
        }
        else
        {
            r->mesh = NULL;
        }

        r->flipped_room = ReadSWord();
        r->flags = ReadUWord();
        r->mesh_effect = ReadSByte();
        r->reverb_type = ReadUByte();
        r->flip_number = ReadUByte();
        r->bound_active = 0;
        r->left = phd_winxmax;
        r->top = phd_winymax;
        r->right = 0;
        r->bottom = 0;
        r->item_number = NO_ITEM;
        r->fx_number = NO_ITEM;
        FixUpRoom(r);
    }

    BuildOutsideRoom();

    size = ReadSDword();
    AllocReadT(floor_data, short, size);
}

void LoadObjects(void)
{
    int i;
    int object_number;
    int static_number;
    int saved_mesh_number;
    int number_meshbase;
    int number_changes;
    int number_ranges;
    int number_commands;
    int number_bones;
    int number_frames;
    int number_objects;
    int number_static;

    S_Log(LT_Enter, "LoadObjects()", false);
    ZeroMemory(objects, sizeof(OBJECT_INFO[NUMBER_OBJECTS]));
    ZeroMemory(static_objects, sizeof(STATIC_INFO[NUMBER_STATIC_OBJECTS]));

    // mesh base
    number_meshbase = ReadSDword();
    AllocReadT(mesh_base, short, number_meshbase);
    
    // meshes
    number_meshes = ReadSDword();
    AllocT(meshes, short*, sizeof(short) * number_meshes);
    OnlyReadT(meshes, short*, number_meshes);

    for (i = 0; i < number_meshes; i++)
        meshes[i] = &mesh_base[(int)meshes[i] / 2];
    saved_mesh_number = number_meshes;

    // anims
    number_anims = ReadSDword();
    AllocRead(anims, ANIM_STRUCT, number_anims);

    // changes
    number_changes = ReadSDword();
    AllocRead(changes, CHANGE_STRUCT, number_changes);

    // ranges
    number_ranges = ReadSDword();
    AllocRead(ranges, RANGE_STRUCT, number_ranges);

    // commands
    number_commands = ReadSDword();
    AllocReadT(commands, short, number_commands);

    // bones
    number_bones = ReadSDword();
    AllocReadT(bones, int, number_bones);

    // frames
    number_frames = ReadUDword();
    AllocReadT(frames, short, number_frames);

    // frames assigned to anims
    for (i = 0; i < number_anims; i++)
        AddPtr(anims[i].frame_ptr, short, frames);

    // objects
    number_objects = ReadSDword();
    for (i = 0; i < number_objects; i++)
    {
        object_number = ReadSDword();
        objects[object_number].nmeshes = ReadSWord();
        objects[object_number].mesh_index = ReadSWord();
        objects[object_number].bone_index = ReadSDword();
        objects[object_number].frame_base = (short*)ReadSDword();
        objects[object_number].anim_index = ReadSWord();
        objects[object_number].loaded = TRUE;
    }

    // skinning data
    S_Log(LT_Function, "Assigning LARA_SKIN_JOINT to LARA", true);
    CreateSkinningData();

    // double the mesh index & duplicate item
    for (i = 0; i < NUMBER_OBJECTS; i++)
        objects[i].mesh_index *= sizeof(short);

    memcpy(&meshes[saved_mesh_number], &meshes[0], sizeof(short*) * saved_mesh_number);

    for (i = 0; i < saved_mesh_number; i++)
    {
        meshes[2 * i] = meshes[saved_mesh_number + i];
        meshes[2 * i + 1] = meshes[saved_mesh_number + i];
    }

    // initialise objects
    InitialiseObjects();

    // static objects
    number_static = ReadSDword();
    for (i = 0; i < number_static; i++)
    {
        static_number = ReadSDword();
        static_objects[static_number].mesh_number = ReadSWord();
        static_objects[static_number].x_minp = ReadSWord();
        static_objects[static_number].x_maxp = ReadSWord();
        static_objects[static_number].y_minp = ReadSWord();
        static_objects[static_number].y_maxp = ReadSWord();
        static_objects[static_number].z_minp = ReadSWord();
        static_objects[static_number].z_maxp = ReadSWord();
        static_objects[static_number].x_minc = ReadSWord();
        static_objects[static_number].x_maxc = ReadSWord();
        static_objects[static_number].y_minc = ReadSWord();
        static_objects[static_number].y_maxc = ReadSWord();
        static_objects[static_number].z_minc = ReadSWord();
        static_objects[static_number].z_maxc = ReadSWord();
        static_objects[static_number].flags = ReadSWord();
    }

    for (i = 0; i < NUMBER_STATIC_OBJECTS; i++)
        static_objects[i].mesh_number *= 2;

    // process mesh data
    ProcessMeshData(2 * saved_mesh_number);
}

#ifdef DLL_INJECT
void injector::inject_file()
{
    //this->inject(0x00473F20, LoadTextures); // (int roomtex, int objtex, int dumptex)
    this->inject(ADDRESS_STRUCT(0x004749C0, LoadRooms));
    this->inject(ADDRESS_STRUCT(0x00474E10, LoadObjects));    // OK
    //this->inject(0x004752A0, AdjustUV);
    //this->inject(0x00475730, LoadSprites);
    //this->inject(0x00475970, LoadCameras);
    //this->inject(0x00475A30, LoadSoundEffects);
    //this->inject(0x00475AC0, LoadBoxes);
    //this->inject(0x00475C70, LoadAnimatedTextures);
    //this->inject(0x00475CE0, LoadTextureInfos);
    //this->inject(0x00475EE0, LoadItems);
    //this->inject(0x004761E0, LoadDemoData);
    //this->inject(0x004761F0, LoadAIObjects);
    //this->inject(0x00476260, LoadSamples);
    //this->inject(0x00476410, LoadSomething);
    //this->inject(0x00476470, LoadLevel);
    //this->inject(0x00476790, FreeLevel);
    //this->inject(0x004768C0, S_LoadLevelFile); // corrupted !!!
}
#endif