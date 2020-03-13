#pragma once

#ifdef DLL_INJECT
#define ExplodeItemNode ((void(__cdecl*)(ITEM_INFO* item, short meshNumber, BOOL noXZVel, int num_shatter)) 0x0044DE50)
#define ShatterObject ((void(__cdecl*)(SHATTER_ITEM *shatter_item, MESH_INFO *mesh_info, int num_shatter, short roomNumber, BOOL noXZvel)) 0x00433410)
#endif