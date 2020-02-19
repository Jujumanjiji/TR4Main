#include "framework.h"
#include "file.h"
#include "init.h"
#include "setup.h"
#include "utils/file_utils.h"

/*
void LoadObjects(void)
{
    Log(LT_Enter, "DLL - Entered LoadObjects()");
    int i, size;
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

    memset(objects, 0, sizeof(objects));
    memset(static_objects, 0, sizeof(static_objects));

    // mesh base
    number_meshbase = ReadDword();
    AllocReadT(mesh_base, short, number_meshbase);
    
    // meshes
    number_meshes = ReadDword();
    AllocT(meshes, short*, sizeof(short) * number_meshes);
    OnlyReadT(meshes, short*, number_meshes);

    for (i = 0; i < number_meshes; i++)
        meshes[i] = &mesh_base[(int)meshes[i] / 2];
    saved_mesh_number = number_meshes;

    // anims
    number_anims = ReadDword();
    AllocRead(anims, ANIM_STRUCT, number_anims);

    // changes
    number_changes = ReadDword();
    AllocRead(changes, CHANGE_STRUCT, number_changes);

    // ranges
    number_ranges = ReadDword();
    AllocRead(ranges, RANGE_STRUCT, number_ranges);

    // commands
    number_commands = ReadDword();
    AllocReadT(commands, short, number_commands);

    // bones
    number_bones = ReadDword();
    AllocReadT(bones, int, number_bones);

    // frames
    number_frames = ReadDword();
    AllocReadT(frames, short, number_frames);

    // frames assigned to anims
    for (i = 0; i < number_anims; i++)
        AddPtr(anims[i].frame_ptr, short, frames);

    // objects
    number_objects = ReadDword();
    for (i = 0; i < number_objects; i++)
    {
        object_number = ReadDword();
        objects[object_number].nmeshes = ReadWord();
        objects[object_number].mesh_index = ReadWord();
        objects[object_number].bone_index = ReadDword();
        objects[object_number].frame_base = (short*)ReadDword();
        objects[object_number].anim_index = ReadWord();
        objects[object_number].loaded = TRUE;
    }

    // skinning data
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
    number_static = ReadDword();
    for (i = 0; i < number_static; i++)
    {
        static_number = ReadDword();
        static_objects[static_number].mesh_number = ReadWord();
        static_objects[static_number].x_minp = ReadWord();
        static_objects[static_number].x_maxp = ReadWord();
        static_objects[static_number].y_minp = ReadWord();
        static_objects[static_number].y_maxp = ReadWord();
        static_objects[static_number].z_minp = ReadWord();
        static_objects[static_number].z_maxp = ReadWord();
        static_objects[static_number].x_minc = ReadWord();
        static_objects[static_number].x_maxc = ReadWord();
        static_objects[static_number].y_minc = ReadWord();
        static_objects[static_number].y_maxc = ReadWord();
        static_objects[static_number].z_minc = ReadWord();
        static_objects[static_number].z_maxc = ReadWord();
        static_objects[static_number].flags = ReadWord();
    }

    for (i = 0; i < NUMBER_STATIC_OBJECTS; i++)
        static_objects[i].mesh_number *= 2;

    // process mesh data
    ProcessMeshData(2 * saved_mesh_number);
}*/

void injector::inject_file()
{
    //this->inject(0x00473F20, LoadTextures); // (int roomtex, int objtex, int dumptex)
    //this->inject(0x004749C0, LoadRooms);
    //this->inject(0x00474E10, LoadObjects);  // OK
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