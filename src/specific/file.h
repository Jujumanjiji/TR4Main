#pragma once

#define number_meshes VAR_U_(0x00533A50, int)
#define number_anims VAR_U_(0x00533A60, int)

///#define LoadObjects ((void(__cdecl*)(void)) 0x00474E10)

#define CreateSkinningData ((void(__cdecl*)(void)) 0x00430F70)
#define ProcessMeshData ((void(__cdecl*)(int nmeshes)) 0x00471040)