#include "framework.h"
#include "inventory_2d.h"
#include "3d_gen.h"
#include "3d_gen_a.h"
#include "draw.h"
#include "gameflow.h"
#include "lara.h"
#include "input.h"
#include "sound.h"
#include "savegame.h"
#include "text.h"
#include "output.h"
#include "libgpu.h"
#include "specific.h"

// TODO: gameflow is ignored !
// need to decompile DoGameflow() !!
/*
INVOBJ inventry_objects_list[MAX_INVOBJ] =
{
    { UZI_ITEM, 0x0FFFC, 1000, 0x4000, 0x12000, 0x4000, INV_RESIZE, STR_UZIS, ALL_MESH },
    { PISTOLS_ITEM, 6, 1000, 0x4000, 0xAD4C, 0xBD40, INV_RESIZE, STR_PISTOLS, ALL_MESH },
    { SHOTGUN_ITEM, 65530, 640, 0x8000, 0xC000, 0x2000, INV_RESIZE, STR_SHOTGUN, ALL_MESH },
    { REVOLVER_ITEM, 0, 800, 0x4000, 0x2AAA, 0x3BC2, INV_RESIZE, STR_REVOLVER, (0x1) },  // NORMAL
    { REVOLVER_ITEM, 0, 800, 0x4000, 0x2AAA, 0x3BC2, INV_RESIZE, STR_REVOLVER_LASERSIGHT, (0x1|0x2|0x8) }, // LASERSIGHT
    { CROSSBOW_ITEM, 0, 768, 0x2000, 0x1800, 0, INV_RESIZE, STR_CROSSBOW, (0x1) },          // NORMAL
    { CROSSBOW_ITEM, 0, 768, 0x2000, 0x1800, 0, INV_RESIZE, STR_CROSSBOW_LASERSIGHT, (0x1|0x2) }, // LASERSIGHT
    { GRENADE_GUN_ITEM, 0, 800, 0x4000, 0, 0x2ECC, INV_RESIZE, STR_GRENADEGUN, ALL_MESH },
    { SHOTGUN_AMMO1_ITEM, 0, 500, 0x4000, 0, 0, INV_RESIZE, STR_SHOTGUN_AMMO1, ALL_MESH },
    { SHOTGUN_AMMO2_ITEM, 0, 500, 0x4000, 0, 0, INV_RESIZE, STR_SHOTGUN_AMMO2, ALL_MESH },
    { GRENADE_GUN_AMMO1_ITEM, 3, 800, 0x4000, 0, 0, INV_RESIZE, STR_GRENADEGUN_AMMO1, ALL_MESH },
    { GRENADE_GUN_AMMO2_ITEM, 3, 800, 0x4000, 0, 0, INV_RESIZE, STR_GRENADEGUN_AMMO2, ALL_MESH },
    { GRENADE_GUN_AMMO3_ITEM, 3, 800, 0x4000, 0, 0, INV_RESIZE, STR_GRENADEGUN_AMMO3, ALL_MESH },
    { CROSSBOW_AMMO1_ITEM, 0, 1100, 0x4000, 0, 0, INV_RESIZE, STR_CROSSBOW_AMMO1, ALL_MESH },
    { CROSSBOW_AMMO2_ITEM, 0, 1100, 0x4000, 0, 0, INV_RESIZE, STR_CROSSBOW_AMMO2, ALL_MESH },
    { CROSSBOW_AMMO3_ITEM, 0, 1100, 0x4000, 0, 0, INV_RESIZE, STR_CROSSBOW_AMMO3, ALL_MESH },
    { REVOLVER_AMMO_ITEM, 0, 700, 0x4000, 0xF448, 0, INV_RESIZE, STR_REVOLVER_AMMO, ALL_MESH },
    { UZI_AMMO_ITEM, 5, 700, 0, 0x1508, 0, INV_RESIZE, STR_UZIS_AMMO, ALL_MESH },
    { PISTOLS_AMMO_ITEM, 4, 700, 0, 0x4000, 0, INV_RESIZE, STR_PISTOLS_AMMO, ALL_MESH },
    { LASERSIGHT_ITEM, 2, 700, 0x4000, 0x7D0, 0, INV_RESIZE, STR_LASERSIGHT, ALL_MESH },
    { BIGMEDI_ITEM, 2, 768, 0, 0, 0, INV_RESIZE, STR_LARGE_MEDIPACK, ALL_MESH },
    { SMALLMEDI_ITEM, 0, 768, 0, 0x5000, 0, INV_RESIZE, STR_SMALL_MEDIPACK, ALL_MESH },
    { BINOCULARS_ITEM, 65535, 768, 0x1000, 0x7D0, 0, INV_RESIZE, STR_BINOCULARS, ALL_MESH },
    { FLARE_INV_ITEM, 2, 0x44C, 0x4000, 0, 0, INV_RESIZE, STR_FLARES, ALL_MESH },
    { COMPASS_ITEM, 0x0FFF2, 0x258, 0, 0x36B0, 0, 0, STR_COMPASS, ALL_MESH },
    { MEMCARD_LOAD_INV_ITEM, 2, 0x0FA0, 0, 0, 0, INV_RESIZE, STR_LOAD, ALL_MESH },
    { MEMCARD_SAVE_INV_ITEM, 2, 0x0FA0, 0, 0, 0, INV_RESIZE, STR_SAVE, ALL_MESH },
    { WATERSKIN1_EMPTY, 2, 11200, 0, 0x0C950, 0, INV_RESIZE, STR_SMALL_WATERSKIN_EMPTY, ALL_MESH },
    { WATERSKIN1_1, 2, 11200, 0, 0x0C950, 0, INV_RESIZE, STR_SMALL_WATERSKIN_CONTAIN1L, ALL_MESH },
    { WATERSKIN1_2, 2, 11200, 0, 0x0C950, 0, INV_RESIZE, STR_SMALL_WATERSKIN_CONTAIN2L, ALL_MESH },
    { WATERSKIN1_3, 2, 11200, 0, 0x0C950, 0, INV_RESIZE, STR_SMALL_WATERSKIN_CONTAIN3L, ALL_MESH },
    { WATERSKIN2_EMPTY, 2, 11200, 0, 0x0C950, 0, INV_RESIZE, STR_LARGE_WATERSKIN_EMPTY, ALL_MESH },
    { WATERSKIN2_1, 2, 11200, 0, 0x0C950, 0, INV_RESIZE, STR_LARGE_WATERSKIN_CONTAIN1L, ALL_MESH },
    { WATERSKIN2_2, 2, 11200, 0, 0x0C950, 0, INV_RESIZE, STR_LARGE_WATERSKIN_CONTAIN2L, ALL_MESH },
    { WATERSKIN2_3, 2, 11200, 0, 0x0C950, 0, INV_RESIZE, STR_LARGE_WATERSKIN_CONTAIN3L, ALL_MESH },
    { WATERSKIN2_4, 2, 11200, 0, 0x0C950, 0, INV_RESIZE, STR_LARGE_WATERSKIN_CONTAIN4L, ALL_MESH },
    { WATERSKIN2_5, 2, 11200, 0, 0x0C950, 0, INV_RESIZE, STR_LARGE_WATERSKIN_CONTAIN5L, ALL_MESH },
    { PUZZLE_ITEM1, 14, 1200, 0, 0, 0, INV_RESIZE, STR_LOAD, ALL_MESH },
    { PUZZLE_ITEM2, 14, 1200, 0, 0x0C000, 0, INV_RESIZE, STR_LOAD, ALL_MESH },
    { PUZZLE_ITEM3, 8, 1200, 0, 0, 0, INV_RESIZE, STR_LOAD, ALL_MESH },
    { PUZZLE_ITEM4, 8, 1200, 0, 0, 0, INV_RESIZE, STR_LOAD, ALL_MESH },
    { PUZZLE_ITEM5, 8, 1200, 0, 0, 0, INV_RESIZE, STR_LOAD, ALL_MESH },
    { PUZZLE_ITEM6, 8, 1200, 0, 0, 0, INV_RESIZE, STR_LOAD, ALL_MESH },
    { PUZZLE_ITEM7, 8, 1200, 0, 0, 0, INV_RESIZE, STR_LOAD, ALL_MESH },
    { PUZZLE_ITEM8, 8, 1200, 0, 0, 0, INV_RESIZE, STR_LOAD, ALL_MESH },
    { PUZZLE_ITEM9, 8, 1200, 0, 0, 0, INV_RESIZE, STR_LOAD, ALL_MESH },
    { PUZZLE_ITEM10, 8, 1200, 0, 0, 0, INV_RESIZE, STR_LOAD, ALL_MESH },
    { PUZZLE_ITEM11, 8, 1200, 0, 0, 0, INV_RESIZE, STR_LOAD, ALL_MESH },
    { PUZZLE_ITEM12, 8, 1200, 0, 0, 0, INV_RESIZE, STR_LOAD, ALL_MESH },
    { PUZZLE_ITEM1_COMBO1, 18, 1200, 0, 0, 0, INV_RESIZE, STR_LOAD, ALL_MESH },
    { PUZZLE_ITEM1_COMBO2, 18, 1200, 0, 0, 0, INV_RESIZE, STR_LOAD, ALL_MESH },
    { PUZZLE_ITEM2_COMBO1, 8, 1200, 0, 0, 0, INV_RESIZE, STR_LOAD, ALL_MESH },
    { PUZZLE_ITEM2_COMBO2, 8, 1200, 0, 0, 0, INV_RESIZE, STR_LOAD, ALL_MESH },
    { PUZZLE_ITEM3_COMBO1, 8, 1200, 0, 0, 0, INV_RESIZE, STR_LOAD, ALL_MESH },
    { PUZZLE_ITEM3_COMBO2, 8, 1200, 0, 0, 0, INV_RESIZE, STR_LOAD, ALL_MESH },
    { PUZZLE_ITEM4_COMBO1, 8, 1200, 0, 0, 0, INV_RESIZE, STR_LOAD, ALL_MESH },
    { PUZZLE_ITEM4_COMBO2, 8, 1200, 0, 0, 0, INV_RESIZE, STR_LOAD, ALL_MESH },
    { PUZZLE_ITEM5_COMBO1, 8, 1200, 0, 0, 0, INV_RESIZE, STR_LOAD, ALL_MESH },
    { PUZZLE_ITEM5_COMBO2, 8, 1200, 0, 0, 0, INV_RESIZE, STR_LOAD, ALL_MESH },
    { PUZZLE_ITEM6_COMBO1, 8, 1200, 0, 0, 0, INV_RESIZE, STR_LOAD, ALL_MESH },
    { PUZZLE_ITEM6_COMBO2, 8, 1200, 0, 0, 0, INV_RESIZE, STR_LOAD, ALL_MESH },
    { PUZZLE_ITEM7_COMBO1, 8, 1200, 0, 0, 0, INV_RESIZE, STR_LOAD, ALL_MESH },
    { PUZZLE_ITEM7_COMBO2, 8, 1200, 0, 0, 0, INV_RESIZE, STR_LOAD, ALL_MESH },
    { PUZZLE_ITEM8_COMBO1, 8, 1200, 0, 0, 0, INV_RESIZE, STR_LOAD, ALL_MESH },
    { PUZZLE_ITEM8_COMBO2, 8, 1200, 0, 0, 0, INV_RESIZE, STR_LOAD, ALL_MESH },
    { KEY_ITEM1, 14, 1200, 0, 0, 0, INV_RESIZE, STR_LOAD, ALL_MESH },
    { KEY_ITEM2, 8, 1200, 0, 0, 0, INV_RESIZE, STR_LOAD, ALL_MESH },
    { KEY_ITEM3, 8, 1200, 0, 0, 0, INV_RESIZE, STR_LOAD, ALL_MESH },
    { KEY_ITEM4, 8, 1200, 0, 0, 0, INV_RESIZE, STR_LOAD, ALL_MESH },
    { KEY_ITEM5, 8, 1200, 0, 0, 0, INV_RESIZE, STR_LOAD, ALL_MESH },
    { KEY_ITEM6, 8, 1200, 0, 0, 0, INV_RESIZE, STR_LOAD, ALL_MESH },
    { KEY_ITEM7, 8, 1200, 0, 0, 0, INV_RESIZE, STR_LOAD, ALL_MESH },
    { KEY_ITEM8, 8, 1200, 0, 0, 0, INV_RESIZE, STR_LOAD, ALL_MESH },
    { KEY_ITEM9, 8, 1200, 0, 0, 0, INV_RESIZE, STR_LOAD, ALL_MESH },
    { KEY_ITEM10, 8, 1200, 0, 0, 0, INV_RESIZE, STR_LOAD, ALL_MESH },
    { KEY_ITEM11, 4, 1200, 0, 0x0C000, 0, INV_RESIZE, STR_LOAD, ALL_MESH },
    { KEY_ITEM12, 8, 1200, 0, 0x0C000, 0, INV_RESIZE, STR_LOAD, ALL_MESH },
    { KEY_ITEM1_COMBO1, 10, 1200, 0, 0, 0, INV_RESIZE, STR_LOAD, ALL_MESH },
    { KEY_ITEM1_COMBO2, 10, 1200, 0, 0, 0, INV_RESIZE, STR_LOAD, ALL_MESH },
    { KEY_ITEM2_COMBO1, 8, 1200, 0, 0, 0, INV_RESIZE, STR_LOAD, ALL_MESH },
    { KEY_ITEM2_COMBO2, 8, 1200, 0, 0, 0, INV_RESIZE, STR_LOAD, ALL_MESH },
    { KEY_ITEM3_COMBO1, 8, 1200, 0, 0, 0, INV_RESIZE, STR_LOAD, ALL_MESH },
    { KEY_ITEM3_COMBO2, 8, 1200, 0, 0, 0, INV_RESIZE, STR_LOAD, ALL_MESH },
    { KEY_ITEM4_COMBO1, 8, 1200, 0, 0, 0, INV_RESIZE, STR_LOAD, ALL_MESH },
    { KEY_ITEM4_COMBO2, 8, 1200, 0, 0, 0, INV_RESIZE, STR_LOAD, ALL_MESH },
    { KEY_ITEM5_COMBO1, 8, 1200, 0, 0, 0, INV_RESIZE, STR_LOAD, ALL_MESH },
    { KEY_ITEM5_COMBO2, 8, 1200, 0, 0, 0, INV_RESIZE, STR_LOAD, ALL_MESH },
    { KEY_ITEM6_COMBO1, 8, 1200, 0, 0, 0, INV_RESIZE, STR_LOAD, ALL_MESH },
    { KEY_ITEM6_COMBO2, 8, 1200, 0, 0, 0, INV_RESIZE, STR_LOAD, ALL_MESH },
    { KEY_ITEM7_COMBO1, 8, 1200, 0, 0, 0, INV_RESIZE, STR_LOAD, ALL_MESH },
    { KEY_ITEM7_COMBO2, 8, 1200, 0, 0, 0, INV_RESIZE, STR_LOAD, ALL_MESH },
    { KEY_ITEM8_COMBO1, 8, 1200, 0, 0, 0, INV_RESIZE, STR_LOAD, ALL_MESH },
    { KEY_ITEM8_COMBO2, 8, 1200, 0, 0, 0, INV_RESIZE, STR_LOAD, ALL_MESH },
    { PICKUP_ITEM1, 14, 1200, 0, 0, 0, INV_RESIZE, STR_LOAD, ALL_MESH },
    { PICKUP_ITEM2, 8, 1200, 0, 0, 0, INV_RESIZE, STR_LOAD, ALL_MESH },
    { PICKUP_ITEM3, 8, 1200, 0, 0, 0, INV_RESIZE, STR_LOAD, ALL_MESH },
    { PICKUP_ITEM4, 8, 1200, 0, 0, 0, INV_RESIZE, STR_LOAD, ALL_MESH },
    { PICKUP_ITEM1_COMBO1, 14, 1200, 0, 0, 0, INV_RESIZE, STR_LOAD, ALL_MESH },
    { PICKUP_ITEM1_COMBO2, 14, 1200, 0, 0, 0, INV_RESIZE, STR_LOAD, ALL_MESH },
    { PICKUP_ITEM2_COMBO1, 14, 1200, 0, 0, 0, INV_RESIZE, STR_LOAD, ALL_MESH },
    { PICKUP_ITEM2_COMBO2, 14, 1200, 0, 0, 0, INV_RESIZE, STR_LOAD, ALL_MESH },
    { PICKUP_ITEM3_COMBO1, 14, 1200, 0, 0, 0, INV_RESIZE, STR_LOAD, ALL_MESH },
    { PICKUP_ITEM3_COMBO2, 14, 1200, 0, 0, 0, INV_RESIZE, STR_LOAD, ALL_MESH },
    { PICKUP_ITEM4_COMBO1, 14, 1200, 0, 0, 0, INV_RESIZE, STR_LOAD, ALL_MESH },
    { PICKUP_ITEM4_COMBO2, 14, 1200, 0, 0, 0, INV_RESIZE, STR_LOAD, ALL_MESH },
    { QUEST_ITEM1, 0, 1200, 0x8000, 0x0C000, 0, INV_RESIZE, STR_QUEST1, ALL_MESH },
    { QUEST_ITEM2, 0, 1024, 0x8000, 0, 0, INV_RESIZE, STR_QUEST2, ALL_MESH },
    { QUEST_ITEM3, 0, 1280, 0x0C000, 0, 0x0C000, INV_RESIZE, STR_QUEST3, ALL_MESH },
    { QUEST_ITEM4, 0, 1280, 0x4000, 0, 0x0C000, INV_RESIZE, STR_QUEST4, ALL_MESH },
    { QUEST_ITEM5, 0, 768, 0, 0, 0x2000, INV_RESIZE, STR_QUEST5, ALL_MESH },
    { QUEST_ITEM6, 0, 768, 0x8000, 0, 0x2000, INV_RESIZE, STR_QUEST6, ALL_MESH },
    { BURNING_TORCH_ITEM, 14, 1200, 0, 0x4000, 0, INV_RESIZE, STR_LOAD, ALL_MESH },
    { CROWBAR_ITEM, 4, 1200, 0, 0x4000, 0, INV_RESIZE, STR_LOAD, ALL_MESH },
    { CLOCKWORK_BEETLE, 14, 1200, 0x4000, 0, 0, INV_RESIZE, STR_MECHANICAL_SCARAB_WITH_KEY, ALL_MESH },
    { CLOCKWORK_BEETLE_COMBO1, 18, 700, 0x4000, 0, 0, INV_RESIZE, STR_WINDING_KEY, ALL_MESH },
    { CLOCKWORK_BEETLE_COMBO2, 14, 700, 0x4000, 0, 0, INV_RESIZE, STR_MECHANICAL_SCARAB, ALL_MESH },
    { EXAMINE1, 4, 1300, 0, 0x4000, 0, INV_RESIZE, STR_LOAD, ALL_MESH },
    { EXAMINE2, 14, 1200, 0, 0x4000, 0, INV_RESIZE, STR_LOAD, ALL_MESH },
    { EXAMINE3, 14, 1200, 0, 0x4000, 0, INV_RESIZE, STR_LOAD, ALL_MESH },
};
*/

INV_OPTION options_table[MAX_INVOBJ] =
{
    { false, true,  false, false, false, false, false, false, OPTION_UZIS },        // INV_UZI
    { false, true,  false, false, false, false, false, false, OPTION_PISTOLS },     // INV_PISTOLS
    { false, true,  true,  false, false, false, false, false, OPTION_SHOTGUN },     // INV_SHOTGUN
    { false, true,  false, false, false, false, true,  false, OPTION_REVOLVER },    // INV_REVOLVER
    { false, true,  false, false, false, false, false, true,  OPTION_REVOLVER },    // INV_REVOLVER_LASER
    { false, true,  true,  false, false, false, true,  false, OPTION_CROSSBOW },    // INV_CROSSBOW
    { false, true,  true,  false, false, false, false, true,  OPTION_CROSSBOW },    // INV_CROSSBOW_LASER
    { false, true,  true,  false, false, false, false, false, OPTION_GRENADEGUN },  // INV_GRENADEGUN
    { false, false, false, false, false, false, false, false, OPTION_AMMO },        // INV_SHOTGUN_AMMO1
    { false, false, false, false, false, false, false, false, OPTION_AMMO },        // INV_SHOTGUN_AMMO2
    { false, false, false, false, false, false, false, false, OPTION_AMMO },        // INV_GRENADEGUN_AMMO1
    { false, false, false, false, false, false, false, false, OPTION_AMMO },        // INV_GRENADEGUN_AMMO2
    { false, false, false, false, false, false, false, false, OPTION_AMMO },        // INV_GRENADEGUN_AMMO3
    { false, false, false, false, false, false, false, false, OPTION_AMMO },        // INV_CROSSBOW_AMMO1
    { false, false, false, false, false, false, false, false, OPTION_AMMO },        // INV_CROSSBOW_AMMO2
    { false, false, false, false, false, false, false, false, OPTION_AMMO },        // INV_CROSSBOW_AMMO3
    { false, false, false, false, false, false, false, false, OPTION_AMMO },        // INV_REVOLVER_AMMO
    { false, false, false, false, false, false, false, false, OPTION_AMMO },        // INV_UZI_AMMO
    { false, false, false, false, false, false, false, false, OPTION_AMMO },        // INV_PISTOLS_AMMO
    { false, false, false, false, false, false, true,  false, OPTION_NONE },        // INV_LASERSIGHT
    { true,  false, false, false, false, false, false, false, OPTION_MEDIKIT },     // INV_BIGMEDI
    { true,  false, false, false, false, false, false, false, OPTION_MEDIKIT },     // INV_SMALLMEDI
    { true,  false, false, false, false, false, false, false, OPTION_NONE },        // INV_BINOCULARS
    { true,  false, false, false, false, false, false, false, OPTION_NONE },        // INV_FLARE
    { false, false, false, false, false, false, false, false, OPTION_NONE },        // INV_COMPASS
    { false, false, false, false, true,  false, false, false, OPTION_NONE },        // INV_MEMCARD_LOAD
    { false, false, false, false, false, true,  false, false, OPTION_NONE },        // INV_MEMCARD_SAVE
    { true,  false, false, false, false, false, true,  false, OPTION_NONE },        // INV_WATERSKIN_EMPTY
    { true,  false, false, false, false, false, false, false, OPTION_NONE },        // INV_WATERSKIN1_1
    { true,  false, false, false, false, false, false, false, OPTION_NONE },        // INV_WATERSKIN1_2
    { true,  false, false, false, false, false, false, false, OPTION_NONE },        // INV_WATERSKIN1_3
    { true,  false, false, false, false, false, true,  false, OPTION_NONE },        // INV_WATERSKIN2_EMPTY
    { true,  false, false, false, false, false, false, false, OPTION_NONE },        // INV_WATERSKIN2_1
    { true,  false, false, false, false, false, false, false, OPTION_NONE },        // INV_WATERSKIN2_2
    { true,  false, false, false, false, false, false, false, OPTION_NONE },        // INV_WATERSKIN2_3
    { true,  false, false, false, false, false, false, false, OPTION_NONE },        // INV_WATERSKIN2_4
    { true,  false, false, false, false, false, false, false, OPTION_NONE },        // INV_WATERSKIN2_5
    { true,  false, false, false, false, false, false, false, OPTION_NONE },        // INV_PUZZLE1
    { true,  false, false, false, false, false, false, false, OPTION_NONE },        // INV_PUZZLE2
    { true,  false, false, false, false, false, false, false, OPTION_NONE },        // INV_PUZZLE3
    { true,  false, false, false, false, false, false, false, OPTION_NONE },        // INV_PUZZLE4
    { true,  false, false, false, false, false, false, false, OPTION_NONE },        // INV_PUZZLE5
    { true,  false, false, false, false, false, false, false, OPTION_NONE },        // INV_PUZZLE6
    { true,  false, false, false, false, false, false, false, OPTION_NONE },        // INV_PUZZLE7
    { true,  false, false, false, false, false, false, false, OPTION_NONE },        // INV_PUZZLE8
    { true,  false, false, false, false, false, false, false, OPTION_NONE },        // INV_PUZZLE9
    { true,  false, false, false, false, false, false, false, OPTION_NONE },        // INV_PUZZLE10
    { true,  false, false, false, false, false, false, false, OPTION_NONE },        // INV_PUZZLE11
    { true,  false, false, false, false, false, false, false, OPTION_NONE },        // INV_PUZZLE12
    { false, false, false, false, false, false, true,  false, OPTION_NONE },        // INV_PUZZLE1_COMBO1
    { false, false, false, false, false, false, true,  false, OPTION_NONE },        // INV_PUZZLE1_COMBO2
    { false, false, false, false, false, false, true,  false, OPTION_NONE },        // INV_PUZZLE2_COMBO1
    { false, false, false, false, false, false, true,  false, OPTION_NONE },        // INV_PUZZLE2_COMBO2
    { false, false, false, false, false, false, true,  false, OPTION_NONE },        // INV_PUZZLE3_COMBO1
    { false, false, false, false, false, false, true,  false, OPTION_NONE },        // INV_PUZZLE3_COMBO2
    { false, false, false, false, false, false, true,  false, OPTION_NONE },        // INV_PUZZLE4_COMBO1
    { false, false, false, false, false, false, true,  false, OPTION_NONE },        // INV_PUZZLE4_COMBO2
    { false, false, false, false, false, false, true,  false, OPTION_NONE },        // INV_PUZZLE5_COMBO1
    { false, false, false, false, false, false, true,  false, OPTION_NONE },        // INV_PUZZLE5_COMBO2
    { false, false, false, false, false, false, true,  false, OPTION_NONE },        // INV_PUZZLE6_COMBO1
    { false, false, false, false, false, false, true,  false, OPTION_NONE },        // INV_PUZZLE6_COMBO2
    { false, false, false, false, false, false, true,  false, OPTION_NONE },        // INV_PUZZLE7_COMBO1
    { false, false, false, false, false, false, true,  false, OPTION_NONE },        // INV_PUZZLE7_COMBO2
    { false, false, false, false, false, false, true,  false, OPTION_NONE },        // INV_PUZZLE8_COMBO1
    { false, false, false, false, false, false, true,  false, OPTION_NONE },        // INV_PUZZLE8_COMBO2
    { true,  false, false, false, false, false, false, false, OPTION_NONE },        // INV_KEY1
    { true,  false, false, false, false, false, false, false, OPTION_NONE },        // INV_KEY2
    { true,  false, false, false, false, false, false, false, OPTION_NONE },        // INV_KEY3
    { true,  false, false, false, false, false, false, false, OPTION_NONE },        // INV_KEY4
    { true,  false, false, false, false, false, false, false, OPTION_NONE },        // INV_KEY5
    { true,  false, false, false, false, false, false, false, OPTION_NONE },        // INV_KEY6
    { true,  false, false, false, false, false, false, false, OPTION_NONE },        // INV_KEY7
    { true,  false, false, false, false, false, false, false, OPTION_NONE },        // INV_KEY8
    { true,  false, false, false, false, false, false, false, OPTION_NONE },        // INV_KEY9
    { true,  false, false, false, false, false, false, false, OPTION_NONE },        // INV_KEY10
    { true,  false, false, false, false, false, false, false, OPTION_NONE },        // INV_KEY11
    { true,  false, false, false, false, false, false, false, OPTION_NONE },        // INV_KEY12
    { false, false, false, false, false, false, true,  false, OPTION_NONE },        // INV_KEY1_COMBO1
    { false, false, false, false, false, false, true,  false, OPTION_NONE },        // INV_KEY1_COMBO2
    { false, false, false, false, false, false, true,  false, OPTION_NONE },        // INV_KEY2_COMBO1
    { false, false, false, false, false, false, true,  false, OPTION_NONE },        // INV_KEY2_COMBO2
    { false, false, false, false, false, false, true,  false, OPTION_NONE },        // INV_KEY3_COMBO1
    { false, false, false, false, false, false, true,  false, OPTION_NONE },        // INV_KEY3_COMBO2
    { false, false, false, false, false, false, true,  false, OPTION_NONE },        // INV_KEY4_COMBO1
    { false, false, false, false, false, false, true,  false, OPTION_NONE },        // INV_KEY4_COMBO2
    { false, false, false, false, false, false, true,  false, OPTION_NONE },        // INV_KEY5_COMBO1
    { false, false, false, false, false, false, true,  false, OPTION_NONE },        // INV_KEY5_COMBO2
    { false, false, false, false, false, false, true,  false, OPTION_NONE },        // INV_KEY6_COMBO1
    { false, false, false, false, false, false, true,  false, OPTION_NONE },        // INV_KEY6_COMBO2
    { false, false, false, false, false, false, true,  false, OPTION_NONE },        // INV_KEY7_COMBO1
    { false, false, false, false, false, false, true,  false, OPTION_NONE },        // INV_KEY7_COMBO2
    { false, false, false, false, false, false, true,  false, OPTION_NONE },        // INV_KEY8_COMBO1
    { false, false, false, false, false, false, true,  false, OPTION_NONE },        // INV_KEY8_COMBO2
    { true,  false, false, false, false, false, false, false, OPTION_NONE },        // INV_PICKUP1
    { true,  false, false, false, false, false, false, false, OPTION_NONE },        // INV_PICKUP2
    { true,  false, false, false, false, false, false, false, OPTION_NONE },        // INV_PICKUP3
    { true,  false, false, false, false, false, false, false, OPTION_NONE },        // INV_PICKUP4
    { false, false, false, false, false, false, true,  false, OPTION_NONE },        // INV_PICKUP1_COMBO1
    { false, false, false, false, false, false, true,  false, OPTION_NONE },        // INV_PICKUP1_COMBO2
    { false, false, false, false, false, false, true,  false, OPTION_NONE },        // INV_PICKUP2_COMBO1
    { false, false, false, false, false, false, true,  false, OPTION_NONE },        // INV_PICKUP2_COMBO2
    { false, false, false, false, false, false, true,  false, OPTION_NONE },        // INV_PICKUP3_COMBO1
    { false, false, false, false, false, false, true,  false, OPTION_NONE },        // INV_PICKUP3_COMBO2
    { false, false, false, false, false, false, true,  false, OPTION_NONE },        // INV_PICKUP4_COMBO1
    { false, false, false, false, false, false, true,  false, OPTION_NONE },        // INV_PICKUP4_COMBO2
    { true,  false, false, false, false, false, false, false, OPTION_NONE },        // INV_QUEST1
    { true,  false, false, false, false, false, false, false, OPTION_NONE },        // INV_QUEST2
    { true,  false, false, false, false, false, false, false, OPTION_NONE },        // INV_QUEST3
    { true,  false, false, false, false, false, false, false, OPTION_NONE },        // INV_QUEST4
    { true,  false, false, false, false, false, false, false, OPTION_NONE },        // INV_QUEST5
    { true,  false, false, false, false, false, false, false, OPTION_NONE },        // INV_QUEST6
    { true,  false, false, false, false, false, false, false, OPTION_NONE },        // INV_BURNING_TORCH
    { true,  false, false, false, false, false, false, false, OPTION_NONE },        // INV_CROWBAR
    { true,  false, false, false, false, false, false, false, OPTION_NONE },        // INV_CLOCKWORK_BEETLE
    { false, false, false, false, false, false, true,  false, OPTION_NONE },        // INV_CLOCKWORK_BEETLE_COMBO1
    { false, false, false, false, false, false, true,  false, OPTION_NONE },        // INV_CLOCKWORK_BEETLE_COMBO2
    { false, false, false, true,  false, false, false, false, OPTION_NONE },        // INV_EXAMINE1
    { false, false, false, true,  false, false, false, false, OPTION_NONE },        // INV_EXAMINE2
    { false, false, false, true,  false, false, false, false, OPTION_NONE },        // INV_EXAMINE3
};

COMBINELIST combine_table[MAX_COMBINE] =
{
    { combine_revolver, INV_REVOLVER, INV_LASERSIGHT, INV_REVOLVER_LASER },
    { combine_crossbow, INV_CROSSBOW, INV_LASERSIGHT, INV_CROSSBOW_LASER },
    { combine_puzzle_item1, INV_PUZZLE1_COMBO1, INV_PUZZLE1_COMBO2, INV_PUZZLE1 },
    { combine_puzzle_item2, INV_PUZZLE2_COMBO1, INV_PUZZLE2_COMBO2, INV_PUZZLE2 },
    { combine_puzzle_item3, INV_PUZZLE3_COMBO1, INV_PUZZLE3_COMBO2, INV_PUZZLE3 },
    { combine_puzzle_item4, INV_PUZZLE4_COMBO1, INV_PUZZLE4_COMBO2, INV_PUZZLE4 },
    { combine_puzzle_item5, INV_PUZZLE5_COMBO1, INV_PUZZLE5_COMBO2, INV_PUZZLE5 },
    { combine_puzzle_item6, INV_PUZZLE6_COMBO1, INV_PUZZLE6_COMBO2, INV_PUZZLE6 },
    { combine_puzzle_item7, INV_PUZZLE7_COMBO1, INV_PUZZLE7_COMBO2, INV_PUZZLE7 },
    { combine_puzzle_item8, INV_PUZZLE8_COMBO1, INV_PUZZLE8_COMBO2, INV_PUZZLE8 },
    { combine_key_item1, INV_KEY1_COMBO1, INV_KEY1_COMBO2, INV_KEY1 },
    { combine_key_item2, INV_KEY2_COMBO1, INV_KEY2_COMBO2, INV_KEY2 },
    { combine_key_item3, INV_KEY3_COMBO1, INV_KEY3_COMBO2, INV_KEY3 },
    { combine_key_item4, INV_KEY4_COMBO1, INV_KEY4_COMBO2, INV_KEY4 },
    { combine_key_item5, INV_KEY5_COMBO1, INV_KEY5_COMBO2, INV_KEY5 },
    { combine_key_item6, INV_KEY6_COMBO1, INV_KEY6_COMBO2, INV_KEY6 },
    { combine_key_item7, INV_KEY7_COMBO1, INV_KEY7_COMBO2, INV_KEY7 },
    { combine_key_item8, INV_KEY8_COMBO1, INV_KEY8_COMBO2, INV_KEY8 },
    { combine_pickup_item1, INV_PICKUP1_COMBO1, INV_PICKUP1_COMBO2, INV_PICKUP1 },
    { combine_pickup_item2, INV_PICKUP2_COMBO1, INV_PICKUP2_COMBO2, INV_PICKUP2 },
    { combine_pickup_item3, INV_PICKUP3_COMBO1, INV_PICKUP3_COMBO2, INV_PICKUP3 },
    { combine_pickup_item4, INV_PICKUP4_COMBO1, INV_PICKUP4_COMBO2, INV_PICKUP4 },
    { combine_clockwork, INV_CLOCKWORK_BEETLE_COMBO1, INV_CLOCKWORK_BEETLE_COMBO2, INV_CLOCKWORK_BEETLE }
};

RING2D* ring_2d[MAX_RING];
AMMOLIST ammo_object_list[MAX_AMMO];
MENU current_options[MAX_MENU];
DWORD inventory_light = RGB(127, 127, 127);
float inventory_drawX;
float inventory_drawY;
int inventory_xpos = 0;
int inventory_ypos = 0;
int compass_needle_angle;
int objlist_spacing;
int combine_obj1;
int combine_obj2;
short normal_ring_fade_val;
short normal_ring_fade_dir;
short ammo_selector_fade_val;
short ammo_selector_fade_dir;
short combine_ring_fade_val;
short combine_ring_fade_dir;
short examine_mode = 0;
short stats_mode;
short inv_pistols_ammo_count = 0;
short inv_shotgun_ammo1_count = 0;
short inv_shotgun_ammo2_count = 0;
short inv_uzi_ammo_count = 0;
short inv_grenadegun_ammo1_count = 0;
short inv_grenadegun_ammo2_count = 0;
short inv_grenadegun_ammo3_count = 0;
short inv_crossbow_ammo1_count = 0;
short inv_crossbow_ammo2_count = 0;
short inv_crossbow_ammo3_count = 0;
short inv_revolver_ammo_count = 0;
char current_selected_option;
char num_ammo_slots;
char current_ammo_type;
char combine_type_flag;
char ammo_selector_flag;
char seperate_type_flag;
char old_lara_busy;
char left_repeat;
char right_repeat;
char loading_or_saving;
char current_pistols_ammo_type = 0;
char current_shotgun_ammo_type = 0;
char current_uzi_ammo_type = 0;
char current_grenadegun_ammo_type = 0;
char current_crossbow_ammo_type = 0;
char current_revolver_ammo_type = 0;
char save_current_selection = 0;
char save_pistols_ammo_type = 0;
char save_shotgun_ammo_type = 0;
char save_uzi_ammo_type = 0;
char save_grenadegun_ammo_type = 0;
char save_crossbow_ammo_type = 0;
char save_revolver_ammo_type = 0;
bool menu_active;
bool ammo_active;
bool left_debounce;
bool right_debounce;
bool up_debounce;
bool down_debounce;
bool go_left;
bool go_right;
bool go_up;
bool go_down;
bool select_debounce;
bool deselect_debounce;
bool go_select;
bool go_deselect;
bool use_items;
bool friggrimmer1;
bool friggrimmer2;

int display_inventory_2d(void)
{
    short flag;
    int end, val;

    old_lara_busy = Lara.busy;
    friggrimmer1 = false;
    if (CHK_EXI(TrInput, IN_SELECT))
        friggrimmer1 = true;

    ring_2d[RING_INVENTORY] = &GadwPolygonBuffers_RingNormal;
    ring_2d[RING_COMBINE] = &GadwPolygonBuffers_RingCombine;
    CreateMonoScreen();
    in_inventory = TRUE;
    construct_inventory_2d();
    camera.number_frames = 2;
    flag = 0;
    end = 0;

    do
    {
        if (reset_flag)
            break;
        objlist_spacing = phd_centerx >> 1;
        if (compass_needle_angle != WALL_L)
            compass_needle_angle -= 32;

        GPU_BeginScene();
        SetDebounce = 1;
        S_UpdateInput();
        TrInput = TrInputDB;
        UpdatePulseColour();
        GameTimer++;

        if (CHK_EXI(dbinput, IN_OPTION))
        {
            SoundEffect(SFX_MENU_SELECT, nullptr, SFX_ALWAYS);
            flag = 1;
        }

        if (thread_started)
            return thread_started;

        draw_outlines();
        do_debounced_input();

        if (examine_mode)
        {
            do_examine_mode();
        }
        else
        {
            draw_current_object_list(RING_INVENTORY);
            handle_inventry_menu();
            if (ring_2d[RING_COMBINE]->active)
                draw_current_object_list(RING_COMBINE);
            draw_ammo_selector();
            fade_ammo_selector();
            do_compass_mode();
        }

        if (use_items && TrInput == IN_NONE)
            flag = 1;

        S_OutputPolyList();
        camera.number_frames = S_DumpScreen();

        if (loading_or_saving)
        {
            do
            {
                GPU_BeginScene();
                val = 0;
                SetDebounce = 1;
                S_UpdateInput();
                TrInput = TrInputDB;
                UpdatePulseColour();

                if (loading_or_saving == 1)
                {
                    val = go_and_load_game();
                }
                else
                {
                    if (go_and_save_game())
                        val = 1;
                }
            } while (val == 0);

            if (val == 1 && loading_or_saving == val)
            {
                end = 1;
                flag = 1;
            }

            friggrimmer2 = true;
            friggrimmer1 = true;
            deselect_debounce = false;
            go_deselect = false;
            loading_or_saving = 0;
        }
    }
    while (!flag);

    ///InitialisePickUpDisplay(); // it reset the pickup display !
    GLOBAL_invitemlastchosen = ring_2d[RING_INVENTORY]->current_obj_list[ring_2d[RING_INVENTORY]->cur_obj_in_list].inv_item;
    update_laras_weapons_status();
    if (use_items)
        use_current_item();
    RemoveMonoScreen();
    in_inventory = false;
    Lara.busy = old_lara_busy;

    return end;
}

void construct_inventory_2d(void)
{
    compass_needle_angle = SECTOR(4);
    examine_mode = 0;
    stats_mode = 0;

    AlterFOV(DEFAULT_FOV);
    Lara.busy = FALSE;

    GLOBAL_invitemchosen = NO_ITEM;
    GLOBAL_invitemlastchosen = NO_ITEM;
    left_debounce = false;
    right_debounce = false;
    up_debounce = false;
    down_debounce = false;
    go_left = false;
    go_right = false;
    go_up = false;
    go_down = false;
    select_debounce = false;
    deselect_debounce = false;
    go_select = false;
    go_deselect = false;
    left_repeat = false;
    right_repeat = false;
    loading_or_saving = 0;
    use_items = false;

    inv_pistols_ammo_count = Lara.pistol_ammo_count;
    inv_shotgun_ammo1_count = (Lara.shotgun_ammo1_count == INFINITE_AMMO) ? (INFINITE_AMMO) : (Lara.shotgun_ammo1_count / 6);
    inv_shotgun_ammo2_count = (Lara.shotgun_ammo2_count == INFINITE_AMMO) ? (INFINITE_AMMO) : (Lara.shotgun_ammo2_count / 6);
    inv_uzi_ammo_count = Lara.uzi_ammo_count;
    inv_grenadegun_ammo1_count = Lara.grenade_ammo1_count;
    inv_grenadegun_ammo2_count = Lara.grenade_ammo2_count;
    inv_grenadegun_ammo3_count = Lara.grenade_ammo3_count;
    inv_crossbow_ammo1_count = Lara.crossbow_ammo1_count;
    inv_crossbow_ammo2_count = Lara.crossbow_ammo2_count;
    inv_crossbow_ammo3_count = Lara.crossbow_ammo3_count;
    inv_revolver_ammo_count = Lara.revolver_ammo_count;
    
    construct_object_list();

    if (GLOBAL_invEnter == NO_ITEM)
    {
        if (GLOBAL_invitemlastchosen != NO_ITEM)
        {
            if (have_i_got_item(GLOBAL_invitemlastchosen))
            {
                setup_objectlist_startposition_invitem(GLOBAL_invitemlastchosen);
            }
            else
            {
                if (GLOBAL_invitemlastchosen >= INV_WATERSKIN_EMPTY && GLOBAL_invitemlastchosen <= INV_WATERSKIN1_3)
                {
                    for (int i = INV_WATERSKIN_EMPTY; i <= INV_WATERSKIN1_3; i++)
                    {
                        if (have_i_got_item(i))
                            setup_objectlist_startposition_invitem(i);
                    }
                }
                else if (GLOBAL_invitemlastchosen >= INV_WATERSKIN2_EMPTY && GLOBAL_invitemlastchosen <= INV_WATERSKIN2_5)
                {
                    for (int i = INV_WATERSKIN2_EMPTY; i <= INV_WATERSKIN2_5; i++)
                    {
                        if (have_i_got_item(i))
                            setup_objectlist_startposition_invitem(i);
                    }
                }
            }

            GLOBAL_invitemlastchosen = NO_ITEM;
        }
    }
    else
    {
        if (have_i_got_object(GLOBAL_invEnter))
            setup_objectlist_startposition_objnumber(GLOBAL_invEnter);
        GLOBAL_invEnter = NO_ITEM;
    }

    ammo_selector_fade_val = 0;
    ammo_selector_fade_dir = 0;
    combine_ring_fade_val = 0;
    combine_ring_fade_dir = 0;
    combine_type_flag = 0;
    seperate_type_flag = 0;
    combine_obj1 = 0;
    combine_obj2 = 0;
    normal_ring_fade_val = 128;
    normal_ring_fade_dir = 0;
    handle_object_changeover();
}

void do_debounced_input(void)
{
    go_left = false;
    go_right = false;
    go_up = false;
    go_down = false;
    go_select = false;
    go_deselect = false;

    /// CHECK FOR LEFT AND RIGHT
    if (CHK_EXI(TrInput, IN_LEFT))
    {
        if (left_repeat >= INVENTORY_INPUT_REPEAT)
            go_left = true;
        else
            ++left_repeat;
        if (!left_debounce)
            go_left = true;
        left_debounce = true;
    }
    else
    {
        left_debounce = false;
        left_repeat = 0;
    }

    if (CHK_EXI(TrInput, IN_RIGHT))
    {
        if (right_repeat >= INVENTORY_INPUT_REPEAT)
            go_right = true;
        else
            ++right_repeat;
        if (!right_debounce)
            go_right = true;
        right_debounce = true;
    }
    else
    {
        right_debounce = false;
        right_repeat = 0;
    }

    /// NOW CHECK FOR UPWARD AND BACKWARD
    if (CHK_EXI(TrInput, IN_FORWARD))
    {
        if (!up_debounce)
            go_up = true;
        up_debounce = true;
    }
    else
    {
        up_debounce = false;
    }

    if (CHK_EXI(TrInput, IN_BACK))
    {
        if (!down_debounce)
            go_down = true;
        down_debounce = true;
    }
    else
    {
        down_debounce = false;
    }

    /// NOW CHECK FOR ACTION/SELECT
    if (CHK_EXI(TrInput, IN_ACTION) || CHK_EXI(TrInput, IN_SELECT))
    {
        select_debounce = true;
    }
    else
    {
        if (select_debounce && !friggrimmer1)
            go_select = true;
        select_debounce = false;
        friggrimmer1 = false;
    }

    /// NOW CHECK ESCAPE
    if (CHK_EXI(TrInput, IN_DESELECT))
    {
        deselect_debounce = true;
    }
    else
    {
        if (deselect_debounce && !friggrimmer2)
            go_deselect = true;
        deselect_debounce = false;
        friggrimmer2 = false;
    }
}

void DrawThreeDeeObject2D(int x, int y, int num, int shade, int xrot, int yrot, int zrot, COLORREF color, int overlay)
{
    ITEM_INFO item;
    INVOBJ* obj = &inventry_objects_list[num];

    item.objectNumber = obj->object_number;
    item.pos.xRot = xrot + obj->xrot;
    item.pos.yRot = yrot + obj->yrot;
    item.pos.zRot = zrot + obj->zrot;

    phd_LookAt(NULL, WALL_L, NULL, NULL, NULL, NULL, NULL);

    if (color != 0)
    {
        if (color == 1)
            inventory_light = RGB(47, 47, 47);
        else
            inventory_light = RGB(color, color, color);
    }
    else
    {
        inventory_light = RGB(127, 127, 127);
    }

    SetD3DViewMatrix();
    phd_PushUnitMatrix();
    phd_TranslateRel(0, 0, obj->scale);

    inventory_drawX = float(x);
    inventory_drawY = float(y + obj->yoff);
    
    item.animNumber = Objects[item.objectNumber].animIndex;
    item.meshBits = obj->meshbits;
    item.shade = -1;
    item.pos.xPos = 0;
    item.pos.yPos = 0;
    item.pos.zPos = 0;
    item.roomNumber = 0;
    item.il.room_ambient.r = 127;
    item.il.room_ambient.g = 127;
    item.il.room_ambient.b = 127;

    DrawInventoryItem(&item, shade, overlay, (obj->flags & 8) != 0);
    phd_PopMatrix();

    inventory_drawX = float(phd_centerx);
    inventory_drawY = float(phd_centery);
}

void DrawInventoryItem(ITEM_INFO* item, int shade, int overlay, BOOL shade_flags)
{
    ANIM_STRUCT* anim;
    OBJECT_INFO* obj;
    BONE_STRUCT* bone;
    short* rotation;
    short** mesh;
    int mesh_bits;

    phd_PushMatrix();
    phd_TranslateRel(item->pos.xPos, item->pos.yPos, item->pos.zPos);
    phd_RotYXZ(item->pos.yRot, item->pos.xRot, item->pos.zRot);

    if (item->objectNumber >= EXAMINE1 && item->objectNumber <= EXAMINE3 && examine_mode)
    {
        PHD_VECTOR examine_shift;

        switch (item->objectNumber)
        {
            case EXAMINE1:
                examine_shift.x = 0x1C00;
                examine_shift.y = 0x1200;
                examine_shift.z = 0x1000;
                ScaleCurrentMatrix(&examine_shift);
                break;
            case EXAMINE2:
                examine_shift.x = 0x1680;
                examine_shift.y = 0x1400;
                examine_shift.z = 0x1000;
                ScaleCurrentMatrix(&examine_shift);
                break;
        }
    }

    obj = &Objects[item->objectNumber];
    anim = &Anims[item->animNumber];
    mesh = &Meshes[obj->meshIndex];
    bone = (BONE_STRUCT*)&Bones[obj->boneIndex];
    mesh_bits = 1;

    if (!shade_flags)
        phd_TranslateRel((int)*(anim->framePtr + 6), (int)*(anim->framePtr + 7), (int)*(anim->framePtr + 8));
    rotation = anim->framePtr + 9;
    gar_RotYXZsuperpack(&rotation, 0);

    if (item->meshBits & 1) // BODY
    {
        if (overlay)
        {
            phd_PutPolygons_inv(*mesh, inventory_drawX, inventory_drawY, inventory_light);
        }
        else
        {
            DWORD item_backup = item_color;
            if (shade <= 127)
                shade *= 2;
            else
                shade = 255;
            item_color = shade << 24;
            phd_PutPolygons_inv(*mesh, inventory_drawX, inventory_drawY, inventory_light);
            item_color = item_backup;
        }
    }

    for (int i = obj->nMeshes - 1; i > 0; bone++, i--)
    {
        mesh += 2;
        mesh_bits *= 2;

        DWORD flags = bone->flags;
        if (flags & BT_POP)
            phd_PopMatrix();
        if (flags & BT_PUSH)
            phd_PushMatrix();

        phd_TranslateRel(bone->x, bone->y, bone->z);
        gar_RotYXZsuperpack(&rotation, 0);

        /// DELETED COMPASS CHEAT !!!
        /// DONT NEED COMPASS CHEAT AGAIN !
        if (item->objectNumber == COMPASS_ITEM)
        {
            short compass_speed = (short)(compass_needle_angle * 4 * rcossin_tbl[(item_rotation & 63) << 10 >> 3] >> W2V_SHIFT);
            short compass_angle = (LaraItem->pos.yRot + compass_speed) - 0x8000;
            phd_RotY(compass_angle);
        }

        if (mesh_bits & item->meshBits)
        {
            if (overlay)
            {
                phd_PutPolygons_inv(*mesh, inventory_drawX, inventory_drawY, inventory_light);
            }
            else
            {
                DWORD item_backup = item_color;
                if (shade <= 127)
                    shade *= 2;
                else
                    shade = 255;
                item_color = shade << 25;
                phd_PutPolygons_inv(*mesh, inventory_drawX, inventory_drawY, inventory_light);
                item_color = item_backup;
            }
        }
    }

    phd_PopMatrix();
}

int go_and_load_game(void)
{
    return LoadGameInfo();
}

int go_and_save_game(void)
{
    return SaveGameInfo();
}

void construct_combine_object_list(void)
{
    ring_2d[RING_COMBINE]->num_obj_in_list = NULL;
    for (int i = 0; i < MAX_INVOBJ; i++)
        ring_2d[RING_COMBINE]->current_obj_list[i].inv_item = NO_ITEM;

    if (CHK_NOP(gfLevelFlags, SLEV_YOUNG_LARA))
    {
        if (CHK_EXI(Lara.revolver_type_carried, CR_PRESENT))
        {
            if (CHK_EXI(Lara.revolver_type_carried, CR_LASERSIGHT))
                insert_object_into_list_combine(INV_REVOLVER_LASER);
            else
                insert_object_into_list_combine(INV_REVOLVER);
        }

        if (CHK_EXI(Lara.crossbow_type_carried, CR_PRESENT))
        {
            if (CHK_EXI(Lara.crossbow_type_carried, CR_LASERSIGHT))
                insert_object_into_list_combine(INV_CROSSBOW_LASER);
            else
                insert_object_into_list_combine(INV_CROSSBOW);
        }

        if (Lara.lasersight)
            insert_object_into_list_combine(INV_LASERSIGHT);
    }

    if (Lara.water_skin_empty1)
        insert_object_into_list_combine(Lara.water_skin_empty1 + (INV_WATERSKIN_EMPTY - 1));
    if (Lara.water_skin_empty2)
        insert_object_into_list_combine(Lara.water_skin_empty2 + (INV_WATERSKIN_EMPTY - 1));

    for (int i = 0; i < 16; i++)
    {
        if (Lara.puzzleitemscombo & (1 << i))
            insert_object_into_list_combine(INV_PUZZLE1_COMBO1 + i);
    }

    for (int i = 0; i < 16; i++)
    {
        if (Lara.keyitemscombo & (1 << i))
            insert_object_into_list_combine(INV_KEY1_COMBO1 + i);
    }

    for (int i = 0; i < 8; i++)
    {
        if (Lara.pickupitemscombo & (1 << i))
            insert_object_into_list_combine(INV_PICKUP1_COMBO1 + i);
    }

    if (Lara.clockwork_beetle & CLOCK_COMBO1_PRESENT)
        insert_object_into_list_combine(INV_CLOCKWORK_BEETLE_COMBO1);
    if (Lara.clockwork_beetle & CLOCK_COMBO2_PRESENT)
        insert_object_into_list_combine(INV_CLOCKWORK_BEETLE_COMBO2);

    ring_2d[RING_COMBINE]->obj_list_movement = 0;
    ring_2d[RING_COMBINE]->cur_obj_in_list = 0;
    ring_2d[RING_COMBINE]->active = FALSE;
}

void insert_object_into_list_combine(short inv_item)
{
    if (options_table[inv_item].can_combine)
    {
        if (ring_2d[RING_INVENTORY]->current_obj_list[ring_2d[RING_INVENTORY]->cur_obj_in_list].inv_item != inv_item)
        {
            ring_2d[RING_COMBINE]->current_obj_list[ring_2d[RING_COMBINE]->num_obj_in_list].inv_item = inv_item;
            ring_2d[RING_COMBINE]->current_obj_list[ring_2d[RING_COMBINE]->num_obj_in_list].yrot = 0;
            ring_2d[RING_COMBINE]->current_obj_list[ring_2d[RING_COMBINE]->num_obj_in_list].bright = 32; // RGBA_TO_DWORD(32, 32, 32, 0), in DrawInventoryItem()
            ring_2d[RING_COMBINE]->num_obj_in_list++;
        }
    }
}

void construct_object_list(void)
{
    ring_2d[RING_INVENTORY]->num_obj_in_list = NULL;
    for (int i = 0; i < MAX_INVOBJ; i++)
        ring_2d[RING_INVENTORY]->current_obj_list[i].inv_item = NO_ITEM;

    current_pistols_ammo_type = AINV_AMMO1;
    current_uzi_ammo_type = AINV_AMMO1;
    current_revolver_ammo_type = AINV_AMMO1;
    current_shotgun_ammo_type = AINV_AMMO1;
    current_grenadegun_ammo_type = AINV_AMMO1;
    current_crossbow_ammo_type = AINV_AMMO1;

    if (CHK_NOP(gfLevelFlags, SLEV_YOUNG_LARA))
    {
        /// PISTOLS
        if (CHK_EXI(Lara.pistols_type_carried, CR_PRESENT))
            insert_object_into_list_inventory(INV_PISTOLS);

        /// UZI
        if (CHK_EXI(Lara.uzi_type_carried, CR_PRESENT))
            insert_object_into_list_inventory(INV_UZI);
        else if (inv_uzi_ammo_count)
            insert_object_into_list_inventory(INV_UZI_AMMO);

        /// REVOLVER
        if (CHK_EXI(Lara.revolver_type_carried, CR_PRESENT))
        {
            if (CHK_EXI(Lara.revolver_type_carried, CR_LASERSIGHT))
                insert_object_into_list_inventory(INV_REVOLVER_LASER);
            else
                insert_object_into_list_inventory(INV_REVOLVER);
        }
        else if (inv_revolver_ammo_count)
        {
            insert_object_into_list_inventory(INV_REVOLVER_AMMO);
        }

        /// SHOTGUN
        if (CHK_EXI(Lara.shotgun_type_carried, CR_PRESENT))
        {
            insert_object_into_list_inventory(INV_SHOTGUN);
            if (CHK_EXI(Lara.shotgun_type_carried, CR_AMMO2))
                current_shotgun_ammo_type = AINV_AMMO2;
        }
        else
        {
            if (inv_shotgun_ammo1_count)
                insert_object_into_list_inventory(INV_SHOTGUN_AMMO1);
            if (inv_shotgun_ammo2_count)
                insert_object_into_list_inventory(INV_SHOTGUN_AMMO2);
        }

        /// GRENADEGUN
        if (CHK_EXI(Lara.grenadegun_type_carried, CR_PRESENT))
        {
            insert_object_into_list_inventory(INV_GRENADEGUN);

            if (CHK_EXI(Lara.grenadegun_type_carried, CR_AMMO2))
                current_grenadegun_ammo_type = AINV_AMMO2;
            else if (CHK_EXI(Lara.grenadegun_type_carried, CR_AMMO3))
                current_grenadegun_ammo_type = AINV_AMMO3;
        }
        else
        {
            if (inv_grenadegun_ammo1_count)
                insert_object_into_list_inventory(INV_GRENADEGUN_AMMO1);
            if (inv_grenadegun_ammo2_count)
                insert_object_into_list_inventory(INV_GRENADEGUN_AMMO2);
            if (inv_grenadegun_ammo3_count)
                insert_object_into_list_inventory(INV_GRENADEGUN_AMMO3);
        }

        /// CROSSBOW
        if (CHK_EXI(Lara.crossbow_type_carried, CR_PRESENT))
        {
            if (CHK_EXI(Lara.crossbow_type_carried, CR_LASERSIGHT))
                insert_object_into_list_inventory(INV_CROSSBOW_LASER);
            else
                insert_object_into_list_inventory(INV_CROSSBOW);

            if (CHK_EXI(Lara.crossbow_type_carried, CR_AMMO2))
                current_crossbow_ammo_type = AINV_AMMO2;
            else if (CHK_EXI(Lara.crossbow_type_carried, CR_AMMO3))
                current_crossbow_ammo_type = AINV_AMMO3;
        }
        else
        {
            if (inv_crossbow_ammo1_count)
                insert_object_into_list_inventory(INV_CROSSBOW_AMMO1);
            if (inv_crossbow_ammo2_count)
                insert_object_into_list_inventory(INV_CROSSBOW_AMMO2);
            if (inv_crossbow_ammo3_count)
                insert_object_into_list_inventory(INV_CROSSBOW_AMMO3);
        }

        /// OTHER
        if (Lara.lasersight)
            insert_object_into_list_inventory(INV_LASERSIGHT);
        if (Lara.binoculars)
            insert_object_into_list_inventory(INV_BINOCULARS);
        if (Lara.flare_count)
            insert_object_into_list_inventory(INV_FLARE);
    }

    if (Lara.small_medipack_count)
        insert_object_into_list_inventory(INV_SMALLMEDI);
    if (Lara.large_medipack_count)
        insert_object_into_list_inventory(INV_BIGMEDI);

    if (Lara.water_skin_empty1)
        insert_object_into_list_inventory(Lara.water_skin_empty1 + (INV_WATERSKIN_EMPTY - 1));
    if (Lara.water_skin_empty2)
        insert_object_into_list_inventory(Lara.water_skin_empty2 + (INV_WATERSKIN2_EMPTY - 1));

    if (Lara.crowbar)
        insert_object_into_list_inventory(INV_CROWBAR);

    if (Lara.clockwork_beetle & CLOCK_PRESENT)
        insert_object_into_list_inventory(INV_CLOCKWORK_BEETLE);
    if (Lara.clockwork_beetle & CLOCK_COMBO1_PRESENT)
        insert_object_into_list_inventory(INV_CLOCKWORK_BEETLE_COMBO1);
    if (Lara.clockwork_beetle & CLOCK_COMBO2_PRESENT)
        insert_object_into_list_inventory(INV_CLOCKWORK_BEETLE_COMBO2);

    for (int i = 0; i < 12; i++)
    {
        if (Lara.puzzleitems[i])
            insert_object_into_list_inventory(INV_PUZZLE1 + i);
    }

    for (int i = 0; i < 8; i++)
    {
        if (Lara.puzzleitemscombo & (1 << i))
            insert_object_into_list_inventory(INV_PUZZLE1_COMBO1 + i);
    }

    for (int i = 0; i < 12; i++)
    {
        if (Lara.keyitems & (1 << i))
            insert_object_into_list_inventory(INV_KEY1 + i);
    }

    for (int i = 0; i < 8; i++)
    {
        if (Lara.keyitemscombo & (1 << i))
            insert_object_into_list_inventory(INV_KEY1_COMBO1 + i);
    }

    for (int i = 0; i < 4; i++)
    {
        if (Lara.pickupitems & (1 << i))
            insert_object_into_list_inventory(INV_PICKUP1 + i);
    }

    for (int i = 0; i < 8; i++)
    {
        if (Lara.pickupitemscombo & (1 << i))
            insert_object_into_list_inventory(INV_PICKUP1_COMBO1 + i);
    }

    for (int i = 0; i < 6; i++)
    {
        if (Lara.questitems & (1 << i))
            insert_object_into_list_inventory(INV_QUEST1 + i);
    }

    if (Lara.examine1)
        insert_object_into_list_inventory(INV_EXAMINE1);
    if (Lara.examine2)
        insert_object_into_list_inventory(INV_EXAMINE2);
    if (Lara.examine3)
        insert_object_into_list_inventory(INV_EXAMINE3);

    if (gameflow->load_save_enabled)
    {
        insert_object_into_list_inventory(INV_MEMCARD_LOAD);
        insert_object_into_list_inventory(INV_MEMCARD_SAVE);
    }

    ring_2d[RING_INVENTORY]->obj_list_movement = 0;
    ring_2d[RING_INVENTORY]->cur_obj_in_list = 0;
    ring_2d[RING_COMBINE]->obj_list_movement = 0;
    ring_2d[RING_COMBINE]->cur_obj_in_list = 0;
    handle_object_changeover();
    ring_2d[RING_INVENTORY]->active = TRUE;
    ring_2d[RING_COMBINE]->active = FALSE;
    ammo_active = false;
}

void insert_object_into_list_inventory(short num)
{
    RING2D* normal = ring_2d[RING_INVENTORY];
    INVLIST* list_inv = normal->current_obj_list;

    list_inv[normal->num_obj_in_list].inv_item = num;
    list_inv[normal->num_obj_in_list].yrot = 0;
    list_inv[normal->num_obj_in_list].bright = 32; // RGBA_TO_DWORD(32, 32, 32, 0), in DrawInventoryItem()
    normal->num_obj_in_list++;
}

void draw_current_object_list(int ring_type)
{
    if (ring_2d[ring_type]->num_obj_in_list <= 0)
        return;
    
    if (ring_type == RING_COMBINE)
    {
        ammo_selector_fade_val = 0;
        ammo_selector_fade_dir = 0;

        if (combine_ring_fade_dir == 1)
        {
            if (combine_ring_fade_val < 128)
                combine_ring_fade_val += 32;

            if (combine_ring_fade_val > 128)
            {
                combine_ring_fade_val = 128;
                combine_ring_fade_dir = 0;
            }
        }
        else if (combine_ring_fade_dir == 2)
        {
            if (combine_ring_fade_val <= 0 || (combine_ring_fade_val -= 32, combine_ring_fade_val <= 0))
            {
                combine_ring_fade_val = 0;
                combine_ring_fade_dir = 0;
                if (combine_type_flag)
                {
                    normal_ring_fade_dir = 2;
                }
                else
                {
                    ring_2d[RING_INVENTORY]->active = TRUE;
                    menu_active = true;
                    ring_2d[RING_COMBINE]->active = FALSE;
                    handle_object_changeover();
                }
                ring_2d[RING_COMBINE]->active = FALSE;
            }
        }
    }
    else if (normal_ring_fade_dir == 1)
    {
        if (normal_ring_fade_val < 128)
            normal_ring_fade_val += 32;

        if (normal_ring_fade_val > 128)
        {
            normal_ring_fade_val = 128;
            normal_ring_fade_dir = 0;
            ring_2d[RING_INVENTORY]->active = TRUE;
            menu_active = true;
        }
    }
    else if (normal_ring_fade_dir == 2)
    {
        if (normal_ring_fade_val > 0)
            normal_ring_fade_val -= 32;

        if (normal_ring_fade_val <= 0)
        {
            normal_ring_fade_val = 0;
            normal_ring_fade_dir = 1;
            if (combine_type_flag == 1)
            {
                combine_type_flag = 0;
                combine_these_two_objects(combine_obj1, combine_obj2);
            }
            else if (combine_type_flag == 2)
            {
                combine_type_flag = 0;
                construct_object_list();
                setup_objectlist_startposition_invitem(combine_obj1);
            }
            else if (seperate_type_flag)
            {
                seperate_object(ring_2d[RING_INVENTORY]->current_obj_list[ring_2d[RING_INVENTORY]->cur_obj_in_list].inv_item);
            }
            handle_object_changeover();
        }
    }

    RING2D* ring = ring_2d[ring_type];
    int count, shade;
    int minobj, maxobj;
    int n, current_obj;
    int xoff, list_num, i;
    int y, centerx, centery;
    int inventoryX, inventoryY;
    int moving_turn;
    unsigned short pos_y;
    char buffer[128];

    count = 0;
    i = 0;
    n = 0;
    minobj = 0;
    maxobj = 0;
    list_num = ring->num_obj_in_list;
    xoff = 0;

    if (list_num != 1)
        xoff = objlist_spacing * ring->obj_list_movement >> 16;

    if (list_num == 2)
    {
        minobj = -1;
        maxobj = 0;
        n = ring->cur_obj_in_list - 1;
    }

    if (list_num == 3 || list_num == 4)
    {
        minobj = -2;
        maxobj = 1;
        n = ring->cur_obj_in_list - 2;
    }

    if (list_num >= 5)
    {
        minobj = -3;
        maxobj = 2;
        n = ring->cur_obj_in_list - 3;
    }

    if (n < 0)
        n += list_num;

    if (ring->obj_list_movement < 0)
        maxobj++;

    i = minobj;
    if (minobj <= maxobj)
    {
        while (i <= maxobj)
        {
            if (i == minobj)
            {
                if (ring->obj_list_movement < 0)
                    shade = 0;
                else
                    shade = ring->obj_list_movement << 7 >> 16;
            }
            else if (i != minobj + 1 || maxobj == minobj + 1)
            {
                if (i != maxobj)
                {
                    shade = 128;
                }
                else
                {
                    if (ring->obj_list_movement < 0)
                        shade = -(128 * ring->obj_list_movement) >> 16;
                    else
                        shade = 128 - (ring->obj_list_movement << 7 >> 16);
                }
            }
            else
            {
                if (ring->obj_list_movement >= 0)
                    shade = 128;
                else
                    shade = 128 - (-128 * ring->obj_list_movement >> 16);
            }

            if (minobj == 0 && maxobj == 0)
                shade = 128;

            switch (ring_type)
            {
                case RING_INVENTORY:
                    if (normal_ring_fade_val < 128 && shade)
                        shade = normal_ring_fade_val;
                    break;
                case RING_COMBINE:
                    if (combine_ring_fade_val < 128 && shade)
                        shade = combine_ring_fade_val;
                    break;
            }

            INVOBJ* obj = &inventry_objects_list[ring->current_obj_list[n].inv_item];
            if (i == 0)
            {
                switch (obj->object_number)
                {
                    case BIGMEDI_ITEM:
                        count = Lara.large_medipack_count;
                        break;
                    case SMALLMEDI_ITEM:
                        count = Lara.small_medipack_count;
                        break;
                    case FLARE_INV_ITEM:
                        count = Lara.flare_count;
                        break;
                    case SHOTGUN_AMMO1_ITEM:
                        count = (Lara.shotgun_ammo1_count != INFINITE_AMMO) ? (Lara.shotgun_ammo1_count / 6) : (Lara.shotgun_ammo1_count);
                        break;
                    case SHOTGUN_AMMO2_ITEM:
                        count = (Lara.shotgun_ammo2_count != INFINITE_AMMO) ? (Lara.shotgun_ammo2_count / 6) : (Lara.shotgun_ammo2_count);
                        break;
                    case GRENADE_GUN_AMMO1_ITEM:
                        count = Lara.grenade_ammo1_count;
                        break;
                    case GRENADE_GUN_AMMO2_ITEM:
                        count = Lara.grenade_ammo2_count;
                        break;
                    case GRENADE_GUN_AMMO3_ITEM:
                        count = Lara.grenade_ammo3_count;
                        break;
                    case CROSSBOW_AMMO1_ITEM:
                        count = Lara.crossbow_ammo1_count;
                        break;
                    case CROSSBOW_AMMO2_ITEM:
                        count = Lara.crossbow_ammo2_count;
                        break;
                    case CROSSBOW_AMMO3_ITEM:
                        count = Lara.crossbow_ammo3_count;
                        break;
                    case REVOLVER_AMMO_ITEM:
                        count = Lara.revolver_ammo_count;
                        break;
                    case UZI_AMMO_ITEM:
                        count = Lara.uzi_ammo_count;
                        break;
                    default:
                        if ((obj->object_number - PUZZLE_ITEM1) < 12)
                            count = Lara.puzzleitems[obj->object_number - PUZZLE_ITEM1];
                        break;
                }

                if (count != 0)
                {
                    if (count == INFINITE_AMMO)
                        sprintf(buffer, &gfStringWad[gfStringOffset[STR_UNLIMITED]], &gfStringWad[gfStringOffset[obj->name]]);
                    else
                        sprintf(buffer, "%d x %s", count, &gfStringWad[gfStringOffset[obj->name]]);
                }
                else
                {
                    sprintf(buffer, &gfStringWad[gfStringOffset[obj->name]]);
                }

                if (ring_type == RING_COMBINE)
                    pos_y = (WORD)(phd_centery + (phd_winymax + 1) * 0.0625 * 3.0);
                else
                    pos_y = (WORD)(phd_centery - (phd_winymax + 1) * 0.0625 * 3.0);
                PrintString(phd_centerx, pos_y, 8, buffer, FF_CENTER);
            }

            if (i == 0 && ring->obj_list_movement == 0)
            {
                current_obj = n;
                if (inventry_objects_list[ring->current_obj_list[n].inv_item].flags & 2)
                    ring->current_obj_list[n].yrot += 1022;
            }
            else
            {
                current_obj = n;
                spinback(&ring->current_obj_list[n].yrot);
            }

            INVLIST* list = &ring->current_obj_list[current_obj];
            if (ring->obj_list_movement != 0)
                moving_turn = ring->obj_list_movement <= 0 ? 1 : -1;
            else
                moving_turn = 0;

            if (i == moving_turn)
            {
                if (list->bright < 160)
                    list->bright += 16;
                if (list->bright > 160)
                    list->bright = 160;
            }
            else
            {
                if (list->bright > 32)
                    list->bright -= 16;
                if (list->bright < 32)
                    list->bright = 32;
            }

            if (ring_type == RING_INVENTORY)
                y = 42;
            else
                y = 190;

            centerx = int(phd_centerx * 0.00390625   * STEP_L);
            centery = int(phd_centery * 0.0083333338 * y);
            inventoryX = inventory_xpos + centerx + (i * objlist_spacing) + xoff;
            inventoryY = inventory_ypos + centery;
            DrawThreeDeeObject2D(inventoryX, inventoryY, ring->current_obj_list[current_obj].inv_item, shade, NULL, ring->current_obj_list[current_obj].yrot, NULL, ring->current_obj_list[current_obj].bright, FALSE);
            
            ++n;
            if (n >= ring->num_obj_in_list)
                n = 0;
            ++i;
        }

        if (ring->active && ring->num_obj_in_list != 1 && (ring_type != RING_COMBINE || combine_ring_fade_val == 128))
        {
            if (ring->obj_list_movement > 0)
                ring->obj_list_movement += 0x2000;
            if (ring->obj_list_movement < 0)
                ring->obj_list_movement -= 0x2000;

            if (go_left && ring->obj_list_movement == 0)
            {
                SoundEffect(SFX_MENU_ROTATE, nullptr, SFX_ALWAYS);
                ring->obj_list_movement += 0x2000;
                if (ammo_selector_flag)
                    ammo_selector_fade_dir = 2;
            }

            if (go_right && ring->obj_list_movement == 0)
            {
                SoundEffect(SFX_MENU_ROTATE, nullptr, SFX_ALWAYS);
                ring->obj_list_movement -= 0x2000;
                if (ammo_selector_flag)
                    ammo_selector_fade_dir = 2;
            }

            if (ring->obj_list_movement < MAXWORD)
            {
                if (ring->obj_list_movement < -MAXWORD)
                {
                    ring->cur_obj_in_list++;

                    if (ring->cur_obj_in_list >= ring->num_obj_in_list)
                        ring->cur_obj_in_list = 0;
                    ring->obj_list_movement = 0;
                    if (!ring_type)
                        handle_object_changeover();
                }
            }
            else
            {
                ring->cur_obj_in_list--;

                if (ring->cur_obj_in_list < 0)
                    ring->cur_obj_in_list = ring->num_obj_in_list - 1;
                ring->obj_list_movement = 0;
                if (!ring_type)
                    handle_object_changeover();
            }
        }
    }
}

void setup_ammo_selector(void)
{
    INV_OPTION opts = options_table[ring_2d[RING_INVENTORY]->current_obj_list[ring_2d[RING_INVENTORY]->cur_obj_in_list].inv_item];
    ammo_selector_flag = 0;
    num_ammo_slots = 0;

    if (!ring_2d[RING_AMMO]->active)
    {
        ammo_object_list[AINV_AMMO1].yrot = 0;
        ammo_object_list[AINV_AMMO2].yrot = 0;
        ammo_object_list[AINV_AMMO3].yrot = 0;

        if (opts.can_equip)
        {
            ammo_selector_flag = 1;
            ammo_selector_fade_dir = 1;

            if (CHK_EXI(opts.flags, OPTION_SHOTGUN))
            {
                ammo_object_list[0].inv_item = INV_SHOTGUN_AMMO1;
                ammo_object_list[0].amount = inv_shotgun_ammo1_count;
                ammo_object_list[1].inv_item = INV_SHOTGUN_AMMO2;
                ammo_object_list[1].amount = inv_shotgun_ammo2_count;
                current_ammo_type = current_shotgun_ammo_type;
                num_ammo_slots = 2;
            }

            if (CHK_EXI(opts.flags, OPTION_CROSSBOW))
            {
                ammo_object_list[0].inv_item = INV_CROSSBOW_AMMO1;
                ammo_object_list[0].amount = inv_crossbow_ammo1_count;
                ammo_object_list[1].inv_item = INV_CROSSBOW_AMMO2;
                ammo_object_list[1].amount = inv_crossbow_ammo2_count;
                ammo_object_list[2].inv_item = INV_CROSSBOW_AMMO3;
                ammo_object_list[2].amount = inv_crossbow_ammo3_count;
                current_ammo_type = current_crossbow_ammo_type;
                num_ammo_slots = 3;
            }

            if (CHK_EXI(opts.flags, OPTION_GRENADEGUN))
            {
                ammo_object_list[0].inv_item = INV_GRENADEGUN_AMMO1;
                ammo_object_list[0].amount = inv_grenadegun_ammo1_count;
                ammo_object_list[1].inv_item = INV_GRENADEGUN_AMMO2;
                ammo_object_list[1].amount = inv_grenadegun_ammo2_count;
                ammo_object_list[2].inv_item = INV_GRENADEGUN_AMMO3;
                ammo_object_list[2].amount = inv_grenadegun_ammo3_count;
                current_ammo_type = current_grenadegun_ammo_type;
                num_ammo_slots = 3;
            }

            if (CHK_EXI(opts.flags, OPTION_UZIS))
            {
                ammo_object_list[0].inv_item = INV_UZI_AMMO;
                ammo_object_list[0].amount = inv_uzi_ammo_count;
                current_ammo_type = current_uzi_ammo_type;
                num_ammo_slots = 1;
            }

            if (CHK_EXI(opts.flags, OPTION_PISTOLS))
            {
                ammo_object_list[0].inv_item = INV_PISTOLS_AMMO;
                ammo_object_list[0].amount = inv_pistols_ammo_count; // normally INFINITE_AMMO...
                current_ammo_type = current_pistols_ammo_type;
                num_ammo_slots = 1;
            }

            if (CHK_EXI(opts.flags, OPTION_REVOLVER))
            {
                ammo_object_list[0].inv_item = INV_REVOLVER_AMMO;
                ammo_object_list[0].amount = inv_revolver_ammo_count;
                current_ammo_type = current_revolver_ammo_type;
                num_ammo_slots = 1;
            }
        }
    }
}

void handle_object_changeover(void)
{
    current_selected_option = 0;
    menu_active = true;
    setup_ammo_selector();
}

void handle_inventry_menu(void)
{
    RING2D* item;
    RING2D* ammo;
    INVOBJ* inv_ammo1, *inv_ammo2, *inv_ammo3;
    INV_OPTION opts;
    short inv_item;
    short inv_ammo;
    int posY;
    int num;

    item = ring_2d[RING_INVENTORY];
    ammo = ring_2d[RING_AMMO];
    inv_ammo1 = &inventry_objects_list[ammo_object_list[AINV_AMMO1].inv_item];
    inv_ammo2 = &inventry_objects_list[ammo_object_list[AINV_AMMO2].inv_item];
    inv_ammo3 = &inventry_objects_list[ammo_object_list[AINV_AMMO3].inv_item];
    inv_item = item->current_obj_list[item->cur_obj_in_list].inv_item;
    inv_ammo = ammo->current_obj_list[ammo->cur_obj_in_list].inv_item;

    if (ammo->active)
    {
        PrintString(phd_centerx, phd_centery, 1, &gfStringWad[gfStringOffset[STR_COMBINE_WITH]], FF_CENTER);

        if (item->obj_list_movement)
            return;
        if (ammo->obj_list_movement)
            return;

        if (go_select)
        {
            if (inv_item >= INV_WATERSKIN_EMPTY && inv_item <= INV_WATERSKIN1_3)
            {
                combine_waterskin(0);
                combine_type_flag = 2;
                combine_ring_fade_dir = 2;
                SoundEffect(SFX_MENU_COMBINE, nullptr, SFX_ALWAYS);
            }
            else if (inv_item >= INV_WATERSKIN2_1 && inv_item <= INV_WATERSKIN2_5)
            {
                combine_waterskin(1);
                combine_type_flag = 2;
                combine_ring_fade_dir = 2;
                SoundEffect(SFX_MENU_COMBINE, nullptr, SFX_ALWAYS);
            }
            else if (do_these_objects_combine(inv_item, inv_ammo))
            {
                combine_ring_fade_dir = 2;
                combine_type_flag = 1;
                combine_obj1 = inv_item;
                combine_obj2 = inv_ammo;
                SoundEffect(SFX_MENU_COMBINE, nullptr, SFX_ALWAYS);
            }
            else
            {
                combine_ring_fade_dir = 2;
                SayNo();
            }
        }

        if (go_deselect)
        {
            SoundEffect(SFX_MENU_SELECT, nullptr, SFX_ALWAYS);
            combine_ring_fade_dir = 2;
            go_deselect = false;
        }

        return;
    }
    else
    {
        for (int i = 0; i < 3; i++)
        {
            current_options[i].text = nullptr;
            current_options[i].type = T_NONE;
        }

        num = 0;
        opts = options_table[inv_item];
        if (ammo_active)
        {
            if (CHK_EXI(opts.flags, OPTION_SHOTGUN)
            ||  CHK_EXI(opts.flags, OPTION_CROSSBOW)
            ||  CHK_EXI(opts.flags, OPTION_GRENADEGUN)
            ||  CHK_EXI(opts.flags, OPTION_PISTOLS)
            ||  CHK_EXI(opts.flags, OPTION_REVOLVER)
            ||  CHK_EXI(opts.flags, OPTION_UZIS))
            {
                /// AMMO1 TEXT
                num = 1;
                current_options[MENU_1].type = T_INVAMMO1;
                current_options[MENU_1].text = &gfStringWad[gfStringOffset[inv_ammo1->name]];
            }
            
            if (CHK_EXI(opts.flags, OPTION_SHOTGUN)
            ||  CHK_EXI(opts.flags, OPTION_CROSSBOW)
            ||  CHK_EXI(opts.flags, OPTION_GRENADEGUN))
            {
                /// AMMO2 TEXT
                num = 2;
                current_options[MENU_2].type = T_INVAMMO2;
                current_options[MENU_2].text = &gfStringWad[gfStringOffset[inv_ammo2->name]];
            }

            if (CHK_EXI(opts.flags, OPTION_CROSSBOW)
            ||  CHK_EXI(opts.flags, OPTION_GRENADEGUN))
            {
                /// AMMO3 TEXT
                num = 3;
                current_options[MENU_3].type = T_INVAMMO3;
                current_options[MENU_3].text = &gfStringWad[gfStringOffset[inv_ammo3->name]];
            }

            current_selected_option = current_ammo_type;
        }
        else
        {
            if (opts.can_load)
            {
                num = 1;
                current_options[MENU_1].type = T_LOAD;
                current_options[MENU_1].text = &gfStringWad[gfStringOffset[STR_LOAD_GAME]];
            }
            
            if (opts.can_save)
            {
                current_options[num].type = T_SAVE;
                current_options[num++].text = &gfStringWad[gfStringOffset[STR_SAVE_GAME]];
            }
            
            if (opts.can_examine)
            {
                current_options[num].type = T_EXAMINE;
                current_options[num++].text = &gfStringWad[gfStringOffset[STR_EXAMINE]];
            }

            if (opts.can_use)
            {
                current_options[num].type = T_USE;
                current_options[num++].text = &gfStringWad[gfStringOffset[STR_USE]];
            }

            if (opts.can_equip)
            {
                current_options[num].type = T_EQUIP;
                current_options[num++].text = &gfStringWad[gfStringOffset[STR_EQUIP]];
            }

            if (opts.can_choose_ammo)
            {
                current_options[num].type = T_CHOOSE_AMMO;
                current_options[num++].text = &gfStringWad[gfStringOffset[STR_CHOOSE_AMMO]];
            }

            if (opts.can_combine && is_item_currently_combinable(inv_item))
            {
                current_options[num].type = T_COMBINE;
                current_options[num++].text = &gfStringWad[gfStringOffset[STR_COMBINE]];
            }

            if (opts.can_separate)
            {
                current_options[num].type = T_SEPARATE;
                current_options[num++].text = &gfStringWad[gfStringOffset[STR_SEPARATE]];
            }
        }

        switch (num)
        {
            case 1:
            case 2:
                posY = phd_centery - 20;
                break;
            case 3:
                posY = phd_centery - 40;
                break;
        }

        if (num > 0)
        {
            for (int i = 0; i < num; i++)
            {
                if (i == current_selected_option)
                {
                    PrintString(phd_centerx, posY, 1, current_options[i].text, FF_CENTER);
                    posY += 40;
                }
                else
                {
                    PrintString(phd_centerx, posY, 5, current_options[i].text, FF_CENTER);
                    posY += 40;
                }
            }
        }

        if (menu_active && !item->obj_list_movement && !ammo->obj_list_movement)
        {
            if (go_up && current_selected_option)
            {
                --current_selected_option;
                SoundEffect(SFX_MENU_SELECT, nullptr, SFX_ALWAYS);
            }
            else if (go_down && current_selected_option < num - 1)
            {
                ++current_selected_option;
                SoundEffect(SFX_MENU_SELECT, nullptr, SFX_ALWAYS);
            }

            if (ammo_active)
            {
                if (go_left && current_selected_option)
                {
                    --current_selected_option;
                    SoundEffect(SFX_MENU_SELECT, nullptr, SFX_ALWAYS);
                }

                if (go_right && current_selected_option < num - 1)
                {
                    ++current_selected_option;
                    SoundEffect(SFX_MENU_SELECT, nullptr, SFX_ALWAYS);
                }

                current_ammo_type = current_selected_option;
            }
            
            if (go_select)
            {
                MENU* menu = &current_options[current_selected_option];
                if (menu->type != 5 && menu->type != 1)
                    SoundEffect(SFX_MENU_CHOOSE, nullptr, SFX_ALWAYS);

                switch (menu->type)
                {
                    case T_CHOOSE_AMMO:
                        item->active = FALSE;
                        save_current_selection = current_selected_option;
                        save_pistols_ammo_type = current_pistols_ammo_type;
                        save_uzi_ammo_type = current_uzi_ammo_type;
                        save_revolver_ammo_type = current_revolver_ammo_type;
                        save_shotgun_ammo_type = current_shotgun_ammo_type;
                        save_grenadegun_ammo_type = current_grenadegun_ammo_type;
                        save_crossbow_ammo_type = current_crossbow_ammo_type;
                        ammo_active = true;
                        break;
                    case T_LOAD:
                        loading_or_saving = 1;
                        break;
                    case T_SAVE:
                        loading_or_saving = 2;
                        break;
                    case T_EXAMINE:
                        examine_mode = 1;
                        stats_mode = 0;
                        break;
                    case T_INVAMMO1:
                    case T_INVAMMO2:
                    case T_INVAMMO3:
                        ammo_active = false;
                        item->active = TRUE;
                        current_selected_option = 0;
                        break;
                    case T_COMBINE:
                        construct_combine_object_list();
                        item->active = FALSE;
                        ammo->active = TRUE;
                        ammo_selector_flag = 0;
                        menu_active = false;
                        combine_ring_fade_dir = 1;
                        break;
                    case T_SEPARATE:
                        seperate_type_flag = 1;
                        normal_ring_fade_dir = 2;
                        break;
                    case T_EQUIP:
                    case T_USE:
                        menu_active = false;
                        use_items = true;
                        break;
                }
            }
            else if (go_deselect && ammo_active)
            {
                SoundEffect(SFX_MENU_SELECT, nullptr, SFX_ALWAYS);
                go_deselect = false;
                ammo_active = false;
                item->active = true;
                current_pistols_ammo_type = save_pistols_ammo_type;
                current_uzi_ammo_type = save_uzi_ammo_type;
                current_revolver_ammo_type = save_revolver_ammo_type;
                current_shotgun_ammo_type = save_shotgun_ammo_type;
                current_grenadegun_ammo_type = save_grenadegun_ammo_type;
                current_crossbow_ammo_type = save_crossbow_ammo_type;
                current_selected_option = save_current_selection;
            }
        }
    }
}

void fade_ammo_selector(void)
{
    if (ring_2d[RING_INVENTORY]->active && (right_repeat >= INVENTORY_INPUT_REPEAT || left_repeat >= INVENTORY_INPUT_REPEAT))
    {
        ammo_selector_fade_val = 0;
    }
    else if (ammo_selector_fade_dir == 1)
    {
        if (ammo_selector_fade_val < 128)
            ammo_selector_fade_val += 32;

        if (ammo_selector_fade_val > 128)
        {
            ammo_selector_fade_val = 128;
            ammo_selector_fade_dir = 0;
        }
    }
    else if (ammo_selector_fade_dir == 2)
    {
        if (ammo_selector_fade_val > 0)
            ammo_selector_fade_val -= 32;

        if (ammo_selector_fade_val < 0)
        {
            ammo_selector_fade_val = 0;
            ammo_selector_fade_dir = 0;
        }
    }
}

void draw_ammo_selector(void)
{
    if (ammo_selector_flag)
    {
        AMMOLIST* ammo;
        int space;
        int i;
        char buffer[128];

        space = ((2 * phd_centerx) - objlist_spacing) >> 1;
        switch (num_ammo_slots)
        {
            case 2:
                space += objlist_spacing / -2;
                break;
            case 3:
                space -= objlist_spacing;
                break;
        }

        i = 0;
        if (num_ammo_slots)
        {
            for (i = 0; i < num_ammo_slots; i++)
            {
                ammo = &ammo_object_list[i];
                if (i == current_ammo_type)
                {
                    if (inventry_objects_list[ammo->inv_item].flags & 2)
                        ammo->yrot += 1022;
                }
                else
                {
                    spinback(&ammo->yrot);
                }

                if (i == current_ammo_type)
                {
                    if (ammo->amount == -1)
                        sprintf(buffer, &gfStringWad[gfStringOffset[STR_UNLIMITED]], &gfStringWad[gfStringOffset[inventry_objects_list[ammo->inv_item].name]]);
                    else
                        sprintf(buffer, "%d x %s", ammo->amount, &gfStringWad[gfStringOffset[inventry_objects_list[ammo->inv_item].name]]);

                    if (ammo_selector_fade_val)
                        PrintString(phd_centerx, font_height + phd_centery + 2 * font_height - 9, 8, buffer, FF_CENTER);
                }

                DrawThreeDeeObject2D(
                    int(inventory_xpos + (phd_centerx * 0.00390625   * 64.0 ) + space),
                    int(inventory_ypos + (phd_centery * 0.0083333338 * 190.0)),
                    ammo->inv_item,
                    ammo_selector_fade_val,
                    NULL,
                    ammo->yrot,
                    NULL,
                    i != current_ammo_type,
                    FALSE);
                space += objlist_spacing;
            }
        }
    }
}

void spinback(WORD* yangle)
{
    WORD sYAngle = *yangle;
    WORD save, posi, nega;
    WORD result;

    save = sYAngle;
    if (sYAngle)
    {
        posi = sYAngle;
        if (sYAngle <= 0x8000)
        {
            if (sYAngle < 1022)
                sYAngle = 1022;
            if (sYAngle > 0x4000)
                sYAngle = 0x4000;
            result = posi - (save >> 3);
            if (result > 0x8000)
                result = 0;
        }
        else
        {
            nega = -sYAngle;
            if (nega < 1022)
                posi = 1022;
            if (nega > 0x4000)
                nega = 0x4000;
            result = (nega >> 3) + posi;
            if (result < 0x8000)
            {
                *yangle = 0;
                return;
            }
        }

        *yangle = result;
    }
}

void update_laras_weapons_status(void)
{
    if (CHK_EXI(Lara.shotgun_type_carried, CR_PRESENT))
    {
        Lara.shotgun_type_carried &= ~(CR_AMMOMASK); // delete all the ammo flag

        switch (current_shotgun_ammo_type)
        {
            case AINV_AMMO1:
                Lara.shotgun_type_carried |= CR_AMMO1;
                break;
            case AINV_AMMO2:
                Lara.shotgun_type_carried |= CR_AMMO2;
                break;
        }
    }

    if (CHK_EXI(Lara.grenadegun_type_carried, CR_PRESENT))
    {
        Lara.grenadegun_type_carried &= ~(CR_AMMOMASK);

        switch (current_grenadegun_ammo_type)
        {
            case AINV_AMMO1:
                Lara.grenadegun_type_carried |= CR_AMMO1;
                break;
            case AINV_AMMO2:
                Lara.grenadegun_type_carried |= CR_AMMO2;
                break;
            case AINV_AMMO3:
                Lara.grenadegun_type_carried |= CR_AMMO3;
                break;
        }
    }

    if (CHK_EXI(Lara.crossbow_type_carried, CR_PRESENT))
    {
        Lara.crossbow_type_carried &= ~(CR_AMMOMASK);

        switch (current_crossbow_ammo_type)
        {
            case AINV_AMMO1:
                Lara.crossbow_type_carried |= CR_AMMO1;
                break;
            case AINV_AMMO2:
                Lara.crossbow_type_carried |= CR_AMMO2;
                break;
            case AINV_AMMO3:
                Lara.crossbow_type_carried |= CR_AMMO3;
                break;
        }
    }
}

BOOL is_item_currently_combinable(short inv_item)
{
    int i;
    if (inv_item < INV_WATERSKIN_EMPTY || inv_item > INV_WATERSKIN2_5)
    {
        for (i = 0; i < MAX_COMBINE; i++)
        {
            short item_a = combine_table[i].item_a;
            short item_b = combine_table[i].item_b;
            
            if (item_a == inv_item)
            {
                if (have_i_got_item(item_b))
                    return TRUE;
            }
            else if (item_b == inv_item)
            {
                if (have_i_got_item(item_a))
                    return TRUE;
            }
        }
    }
    else if (inv_item > INV_WATERSKIN1_3)
    {
        for (i = 0; i < 4; i++)
        {
            if (have_i_got_item(i + INV_WATERSKIN_EMPTY))
                return TRUE;
        }
    }
    else
    {
        for (i = 0; i < 6; i++)
        {
            if (have_i_got_item(i + INV_WATERSKIN2_EMPTY))
                return TRUE;
        }
    }
    return FALSE;
}

BOOL have_i_got_item(short inv_item)
{
    RING2D* inv = ring_2d[RING_INVENTORY];

    for (int i = 0; i < MAX_INVOBJ; i++)
    {
        if (inv_item == inv->current_obj_list[i].inv_item)
            return TRUE;
    }

    return FALSE;
}

BOOL do_these_objects_combine(short item_a, short item_b)
{
    COMBINELIST* list;

    for (int i = 0; i < MAX_COMBINE; i++)
    {
        list = &combine_table[i];
        if (list->item_a == item_a && list->item_b == item_b)        // item_a == TRUE
            return TRUE;
        else if (list->item_a == item_b && list->item_b == item_a)   // item_b == TRUE
            return TRUE;
    }

    return FALSE;
}

void combine_these_two_objects(short item_a, short item_b)
{
    int i;
    COMBINELIST* list;
    for (i = 0; i < MAX_COMBINE; i++)
    {
        list = &combine_table[i];
        if (list->item_a == item_a && list->item_b == item_b)
            break;
        else if (list->item_a == item_b && list->item_b == item_a)
            break;
    }

    if (list != nullptr) // check for nullptr !!
    {
        list->combine_routine(CONSTRUCT_ITEM);
        construct_object_list();
        setup_objectlist_startposition_invitem(list->result);
        handle_object_changeover();
    }
}

void seperate_object(short inv_item)
{
    int i;
    COMBINELIST* list;
    for (i = 0; i < MAX_COMBINE; i++)
    {
        list = &combine_table[i];
        if (list->result == inv_item)
            break;
    }

    if (list != nullptr) // check for nullptr !!
    {
        list->combine_routine(DECONSTRUCT_ITEM);
        construct_object_list();
        setup_objectlist_startposition_invitem(list->item_a);
    }
}

void combine_revolver(int flag)
{
    switch (flag)
    {
        case CONSTRUCT_ITEM:
            Lara.lasersight = FALSE;
            Lara.revolver_type_carried |= CR_LASERSIGHT;
            break;
        case DECONSTRUCT_ITEM:
            Lara.lasersight = TRUE;
            Lara.revolver_type_carried &= ~(CR_LASERSIGHT);
            break;
    }

    if (Lara.gunStatus != LG_UNARMED && Lara.gunType == LG_REVOLVER)
    {
        undraw_pistols_left(LG_REVOLVER);
        draw_pistol_meshes(LG_REVOLVER);
    }
}

void combine_crossbow(int flag)
{
    switch (flag)
    {
        case CONSTRUCT_ITEM:
            Lara.lasersight = FALSE;
            Lara.crossbow_type_carried |= CR_LASERSIGHT;
            break;
        case DECONSTRUCT_ITEM:
            Lara.lasersight = TRUE;
            Lara.crossbow_type_carried &= ~(CR_LASERSIGHT);
            break;
    }

    if (Lara.gunStatus != LG_UNARMED && Lara.gunType == LG_CROSSBOW)
    {
        undraw_shotgun_meshes(LG_CROSSBOW);
        draw_shotgun_meshes(LG_CROSSBOW);
    }
}

void combine_puzzle_item1(int flag)
{
    Lara.puzzleitemscombo &= ~(INV_COMBO1);
    Lara.puzzleitems[0] = 1;
}

void combine_puzzle_item2(int flag)
{
    Lara.puzzleitemscombo &= ~(INV_COMBO2);
    Lara.puzzleitems[1] = 1;
}

void combine_puzzle_item3(int flag)
{
    Lara.puzzleitemscombo &= ~(INV_COMBO3);
    Lara.puzzleitems[2] = 1;
}

void combine_puzzle_item4(int flag)
{
    Lara.puzzleitemscombo &= ~(INV_COMBO4);
    Lara.puzzleitems[3] = 1;
}

void combine_puzzle_item5(int flag)
{
    Lara.puzzleitemscombo &= ~(INV_COMBO5);
    Lara.puzzleitems[4] = 1;
}

void combine_puzzle_item6(int flag)
{
    Lara.puzzleitemscombo &= ~(INV_COMBO6);
    Lara.puzzleitems[5] = 1;
}

void combine_puzzle_item7(int flag)
{
    Lara.puzzleitemscombo &= ~(INV_COMBO7);
    Lara.puzzleitems[6] = 1;
}

void combine_puzzle_item8(int flag)
{
    Lara.puzzleitemscombo &= ~(INV_COMBO8);
    Lara.puzzleitems[7] = 1;
}

void combine_key_item1(int flag)
{
    Lara.keyitems |= 1;
    Lara.keyitemscombo &= ~(INV_COMBO1);
}

void combine_key_item2(int flag)
{
    Lara.keyitems |= 2;
    Lara.keyitemscombo &= ~(INV_COMBO2);
}

void combine_key_item3(int flag)
{
    Lara.keyitems |= 4;
    Lara.keyitemscombo &= ~(INV_COMBO3);
}

void combine_key_item4(int flag)
{
    Lara.keyitems |= 8;
    Lara.keyitemscombo &= ~(INV_COMBO4);
}

void combine_key_item5(int flag)
{
    Lara.keyitems |= 16;
    Lara.keyitemscombo &= ~(INV_COMBO5);
}

void combine_key_item6(int flag)
{
    Lara.keyitems |= 32;
    Lara.keyitemscombo &= ~(INV_COMBO6);
}

void combine_key_item7(int flag)
{
    Lara.keyitems |= 64;
    Lara.keyitemscombo &= ~(INV_COMBO7);
}

void combine_key_item8(int flag)
{
    Lara.keyitems |= 128;
    Lara.keyitemscombo &= ~(INV_COMBO8);
}

void combine_pickup_item1(int flag)
{
    Lara.pickupitems |= 1;
    Lara.pickupitemscombo &= ~(INV_COMBO1);
}

void combine_pickup_item2(int flag)
{
    Lara.pickupitems |= 2;
    Lara.pickupitemscombo &= ~(INV_COMBO2);
}

void combine_pickup_item3(int flag)
{
    Lara.pickupitems |= 4;
    Lara.pickupitemscombo &= ~(INV_COMBO3);
}

void combine_pickup_item4(int flag)
{
    Lara.pickupitems |= 8;
    Lara.pickupitemscombo &= ~(INV_COMBO4);
}

void combine_clockwork(int flag)
{
    Lara.clockwork_beetle = 1;
}

void combine_waterskin(int flag)
{
    int empty_count;
    int empty2_count;
    int slotcount_1;
    int slotcount_2;
    int i, j;

    empty_count = Lara.water_skin_empty1 - 1;
    empty2_count = Lara.water_skin_empty2 - 1;
    slotcount_1 = 3 - empty_count;
    slotcount_2 = 5 - empty2_count;

    if (flag)
    {
        if (Lara.water_skin_empty2 != 1 && empty_count != 3)
        {
            i = Lara.water_skin_empty2 - 1;
            do
            {
                if (slotcount_1)
                {
                    empty_count++;
                    empty2_count--;
                    --slotcount_1;
                }
                --i;
            } while (i);
            Lara.water_skin_empty1 = empty_count + 1;
            Lara.water_skin_empty2 = empty2_count + 1;
            combine_obj1 = (empty_count + 1) + (INV_WATERSKIN_EMPTY - 1);
        }
    }
    else if (Lara.water_skin_empty1 != 1 && empty2_count != 5)
    {
        j = Lara.water_skin_empty1 - 1;
        do
        {
            if (slotcount_2)
            {
                empty_count = empty_count - 1;
                empty2_count = empty2_count + 1;
                --slotcount_2;
            }
            --j;
        } while (j);
        Lara.water_skin_empty1 = empty_count + 1;
        Lara.water_skin_empty2 = empty2_count + 1;
        combine_obj1 = (empty2_count + 1) + (INV_WATERSKIN2_EMPTY - 1);
    }
}

void setup_objectlist_startposition_invitem(short inv_item)
{
    RING2D* inv = ring_2d[RING_INVENTORY];

    for (int i = 0; i < MAX_INVOBJ; i++)
    {
        if (inv_item == inv->current_obj_list[i].inv_item)
        {
            inv->cur_obj_in_list = i;
            inv = ring_2d[RING_INVENTORY]; // reset the old ring...
        }
    }
}

void setup_objectlist_startposition_objnumber(short object_number)
{
    RING2D* inv = ring_2d[RING_INVENTORY];

    for (int i = 0; i < MAX_INVOBJ; i++)
    {
        if (object_number == inventry_objects_list[inv->current_obj_list[i].inv_item].object_number)
        {
            inv->cur_obj_in_list = i;
            inv = ring_2d[RING_INVENTORY]; // reset the old ring...
        }
    }
}

void use_current_item(void)
{
    int rangeBackup = BinocularRange;
    BinocularRange = 0;
    old_lara_busy = false;
    LaraItem->meshBits = -1;

    short inv_item = ring_2d[RING_INVENTORY]->current_obj_list[ring_2d[RING_INVENTORY]->cur_obj_in_list].inv_item;
    short object_number = inventry_objects_list[inv_item].object_number;

    /// WEAPON
    if (Lara.waterStatus == LWS_ABOVEWATER || Lara.waterStatus == LWS_WADE)
    {
        switch (object_number)
        {
            case PISTOLS_ITEM:
                Lara.requestGunType = LG_PISTOLS;
                if (Lara.gunStatus == LHS_ARMLESS && Lara.gunType == LG_PISTOLS)
                    Lara.gunStatus = LHS_DRAW;
                break;
            case SHOTGUN_ITEM:
                Lara.requestGunType = LG_SHOTGUN;
                if (Lara.gunStatus == LHS_ARMLESS && Lara.gunType == LG_SHOTGUN)
                    Lara.gunStatus = LHS_DRAW;
                break;
            case UZI_ITEM:
                Lara.requestGunType = LG_UZIS;
                if (Lara.gunStatus == LHS_ARMLESS && Lara.gunType == LG_UZIS)
                    Lara.gunStatus = LHS_DRAW;
                break;
            case REVOLVER_ITEM:
                Lara.requestGunType = LG_REVOLVER;
                if (Lara.gunStatus == LHS_ARMLESS && Lara.gunType == LG_REVOLVER)
                    Lara.gunStatus = LHS_DRAW;
                break;
            case GRENADE_GUN_ITEM:
                Lara.requestGunType = LG_GRENADEGUN;
                if (Lara.gunStatus == LHS_ARMLESS && Lara.gunType == LG_GRENADEGUN)
                    Lara.gunStatus = LHS_DRAW;
                break;
            case CROSSBOW_ITEM:
                Lara.requestGunType = LG_CROSSBOW;
                if (Lara.gunStatus == LHS_ARMLESS && Lara.gunType == LG_CROSSBOW)
                    Lara.gunStatus = LHS_DRAW;
                break;
        }
    }

    /// FLARE
    if (object_number == FLARE_INV_ITEM)
    {
        if (Lara.gunStatus == LHS_ARMLESS)
        {
            if (LaraItem->currentAnimState != STATE_LARA_CRAWL_IDLE
            &&  LaraItem->currentAnimState != STATE_LARA_CRAWL_FORWARD
            &&  LaraItem->currentAnimState != STATE_LARA_CRAWL_TURN_LEFT
            &&  LaraItem->currentAnimState != STATE_LARA_CRAWL_TURN_RIGHT
            &&  LaraItem->currentAnimState != STATE_LARA_CRAWL_BACK
            &&  LaraItem->currentAnimState != STATE_LARA_CRAWL_TO_CLIMB)
            {
                if (Lara.gunType != LG_FLARE)
                {
                    TrInput = IN_FLARE;
                    LaraGun();
                    TrInput = IN_NONE;
                    GLOBAL_invitemchosen = object_number;
                }
                return;
            }
        }
        else
        {
            SayNo();
        }
    }

    /// MEDIKIT
    if (object_number == BIGMEDI_ITEM)
    {
        if ((LaraItem->hitPoints > 0 && LaraItem->hitPoints < LARA_HITPOINTS) || Lara.poisoned)
        {
            if (Lara.large_medipack_count != INFINITE_AMMO)
                Lara.large_medipack_count--;

            Lara.dpoisoned = 0;
            LaraItem->hitPoints += LARA_HITPOINTS;
            if (LaraItem->hitPoints > LARA_HITPOINTS)
                LaraItem->hitPoints = LARA_HITPOINTS;
            SoundEffect(SFX_MENU_MEDI, nullptr, SFX_ALWAYS);
            savegame_level.health_used++;
        }
        else
        {
            SayNo();
        }
    }

    if (object_number == SMALLMEDI_ITEM)
    {
        if ((LaraItem->hitPoints > 0 && LaraItem->hitPoints < LARA_HITPOINTS) || Lara.poisoned)
        {
            if (Lara.small_medipack_count != INFINITE_AMMO)
                Lara.small_medipack_count--;

            Lara.dpoisoned = 0;
            LaraItem->hitPoints += LARA_HITPOINTS / 2;
            if (LaraItem->hitPoints > LARA_HITPOINTS)
                LaraItem->hitPoints = LARA_HITPOINTS;
            SoundEffect(SFX_MENU_MEDI, nullptr, SFX_ALWAYS);
            savegame_level.health_used++;
        }
        else
        {
            SayNo();
        }
    }
    else if (object_number != BINOCULARS_ITEM)
    {
        GLOBAL_invitemchosen = object_number;
        return;
    }

    /// BINOCULARS
    if (object_number == BINOCULARS_ITEM)
    {
        if ((LaraItem->currentAnimState == STATE_LARA_IDLE && LaraItem->animNumber == ANIMATION_LARA_STAY_IDLE) || (Lara.isDucked && CHK_NOP(TrInput, IN_DUCK)))
        {
            old_lara_busy = true;
            BinocularRange = 128;
            if (Lara.gunStatus != LHS_ARMLESS)
                Lara.gunStatus = LHS_UNDRAW;
            GLOBAL_invitemchosen = object_number;
        }
    }

    if (rangeBackup)
        BinocularRange = rangeBackup;
    else
        BinocularOldCamera = camera.old_type;
}

void picked_up_object(short object_number)
{
    switch (object_number)
    {
        case WATERSKIN1_EMPTY:
            Lara.water_skin_empty1 = 1;
            break;
        case WATERSKIN2_EMPTY:
            Lara.water_skin_empty2 = 1;
            break;
        case UZI_ITEM:
            if (CHK_NOP(Lara.uzi_type_carried, CR_PRESENT))
                Lara.uzi_type_carried |= (CR_PRESENT | CR_AMMO1);
            if (Lara.uzi_ammo_count != INFINITE_AMMO)
                Lara.uzi_ammo_count += 30;
            break;
        case PISTOLS_ITEM:
            if (CHK_NOP(Lara.pistols_type_carried, CR_PRESENT))
                Lara.pistols_type_carried |= (CR_PRESENT | CR_AMMO1);
            Lara.pistol_ammo_count = INFINITE_AMMO;
            return;
        case SHOTGUN_ITEM:
            if (CHK_NOP(Lara.shotgun_type_carried, CR_PRESENT))
                Lara.shotgun_type_carried |= (CR_PRESENT | CR_AMMO1);
            if (Lara.shotgun_ammo1_count != INFINITE_AMMO)
                Lara.shotgun_ammo1_count += (6 * 6);
            break;
        case REVOLVER_ITEM:
            if (CHK_NOP(Lara.revolver_type_carried, CR_PRESENT))
                Lara.revolver_type_carried |= (CR_PRESENT | CR_AMMO1);
            if (Lara.revolver_ammo_count != INFINITE_AMMO)
                Lara.revolver_ammo_count += 6;
            break;
        case CROSSBOW_ITEM:
            if (CHK_NOP(Lara.crossbow_type_carried, CR_PRESENT))
                Lara.crossbow_type_carried |= (CR_PRESENT | CR_AMMO1);
            if (Lara.crossbow_ammo1_count != INFINITE_AMMO)
                Lara.crossbow_ammo1_count += 10;
            break;
        case GRENADE_GUN_ITEM:
            if (CHK_NOP(Lara.grenadegun_type_carried, CR_PRESENT))
                Lara.grenadegun_type_carried |= (CR_PRESENT | CR_AMMO1);
            if (Lara.grenade_ammo1_count != INFINITE_AMMO)
                Lara.grenade_ammo1_count += 10;
            break;
        case SHOTGUN_AMMO1_ITEM:
            if (Lara.shotgun_ammo1_count != INFINITE_AMMO)
                Lara.shotgun_ammo1_count += (6 * 6);
            break;
        case SHOTGUN_AMMO2_ITEM:
            if (Lara.shotgun_ammo2_count != INFINITE_AMMO)
                Lara.shotgun_ammo2_count += (6 * 6);
            break;
        case GRENADE_GUN_AMMO1_ITEM:
            if (Lara.grenade_ammo1_count != INFINITE_AMMO)
                Lara.grenade_ammo1_count += 4;
            break;
        case GRENADE_GUN_AMMO2_ITEM:
            if (Lara.grenade_ammo2_count != INFINITE_AMMO)
                Lara.grenade_ammo2_count += 4;
            break;
        case GRENADE_GUN_AMMO3_ITEM:
            if (Lara.grenade_ammo3_count != INFINITE_AMMO)
                Lara.grenade_ammo3_count += 4;
            break;
        case CROSSBOW_AMMO1_ITEM:
            if (Lara.crossbow_ammo1_count != INFINITE_AMMO)
                Lara.crossbow_ammo1_count += 10;
            break;
        case CROSSBOW_AMMO2_ITEM:
            if (Lara.crossbow_ammo2_count != INFINITE_AMMO)
                Lara.crossbow_ammo2_count += 10;
            break;
        case CROSSBOW_AMMO3_ITEM:
            if (Lara.crossbow_ammo3_count != INFINITE_AMMO)
                Lara.crossbow_ammo3_count += 10;
            break;
        case REVOLVER_AMMO_ITEM:
            if (Lara.revolver_ammo_count != INFINITE_AMMO)
                Lara.revolver_ammo_count += 6;
            break;
        case UZI_AMMO_ITEM:
            if (Lara.uzi_ammo_count != INFINITE_AMMO)
                Lara.uzi_ammo_count += 30;
            break;
        case FLARE_INV_ITEM:
            if (Lara.flare_count != INFINITE_AMMO)
                Lara.flare_count += 12;
            break;
        case LASERSIGHT_ITEM:
            if (CHK_NOP(Lara.pistols_type_carried, CR_LASERSIGHT)
            ||  CHK_NOP(Lara.uzi_type_carried, CR_LASERSIGHT)
            ||  CHK_NOP(Lara.shotgun_type_carried, CR_LASERSIGHT)
            ||  CHK_NOP(Lara.grenadegun_type_carried, CR_LASERSIGHT)
            ||  CHK_NOP(Lara.crossbow_type_carried, CR_LASERSIGHT)
            ||  CHK_NOP(Lara.revolver_type_carried, CR_LASERSIGHT))
            {
                Lara.lasersight = 1;
            }
            break;
        case BIGMEDI_ITEM:
            if (Lara.large_medipack_count != INFINITE_AMMO)
                Lara.large_medipack_count++;
            break;
        case SMALLMEDI_ITEM:
            if (Lara.small_medipack_count != INFINITE_AMMO)
                Lara.small_medipack_count++;
            break;
        case BINOCULARS_ITEM:
            Lara.binoculars = 1;
            break;
        case CROWBAR_ITEM:
            Lara.crowbar = 1;
            break;
        case CLOCKWORK_BEETLE:
            Lara.clockwork_beetle |= 1;
            break;
        case CLOCKWORK_BEETLE_COMBO1:
            Lara.clockwork_beetle |= 2;
            break;
        case CLOCKWORK_BEETLE_COMBO2:
            Lara.clockwork_beetle |= 4;
            break;
        case EXAMINE1:
            Lara.examine1 = 1;
            break;
        case EXAMINE2:
            Lara.examine2 = 1;
            break;
        case EXAMINE3:
            Lara.examine3 = 1;
            break;
        case PUZZLE_ITEM1:
        case PUZZLE_ITEM2:
        case PUZZLE_ITEM3:
        case PUZZLE_ITEM4:
        case PUZZLE_ITEM5:
        case PUZZLE_ITEM6:
        case PUZZLE_ITEM7:
        case PUZZLE_ITEM8:
        case PUZZLE_ITEM9:
        case PUZZLE_ITEM10:
        case PUZZLE_ITEM11:
        case PUZZLE_ITEM12:
            Lara.puzzleitems[object_number - PUZZLE_ITEM1]++;
            break;
        case PUZZLE_ITEM1_COMBO1:
        case PUZZLE_ITEM1_COMBO2:
        case PUZZLE_ITEM2_COMBO1:
        case PUZZLE_ITEM2_COMBO2:
        case PUZZLE_ITEM3_COMBO1:
        case PUZZLE_ITEM3_COMBO2:
        case PUZZLE_ITEM4_COMBO1:
        case PUZZLE_ITEM4_COMBO2:
        case PUZZLE_ITEM5_COMBO1:
        case PUZZLE_ITEM5_COMBO2:
        case PUZZLE_ITEM6_COMBO1:
        case PUZZLE_ITEM6_COMBO2:
        case PUZZLE_ITEM8_COMBO1:
        case PUZZLE_ITEM8_COMBO2:
            Lara.puzzleitemscombo |= (1 << (object_number + 69));
            break;
        case KEY_ITEM1:
        case KEY_ITEM2:
        case KEY_ITEM3:
        case KEY_ITEM4:
        case KEY_ITEM5:
        case KEY_ITEM6:
        case KEY_ITEM7:
        case KEY_ITEM8:
        case KEY_ITEM9:
        case KEY_ITEM10:
        case KEY_ITEM11:
        case KEY_ITEM12:
            Lara.keyitems |= (1 << (object_number + 53));
            break;
        case KEY_ITEM1_COMBO1:
        case KEY_ITEM1_COMBO2:
        case KEY_ITEM2_COMBO1:
        case KEY_ITEM2_COMBO2:
        case KEY_ITEM3_COMBO1:
        case KEY_ITEM3_COMBO2:
        case KEY_ITEM4_COMBO1:
        case KEY_ITEM4_COMBO2:
        case KEY_ITEM5_COMBO1:
        case KEY_ITEM5_COMBO2:
        case KEY_ITEM6_COMBO1:
        case KEY_ITEM6_COMBO2:
        case KEY_ITEM7_COMBO1:
        case KEY_ITEM7_COMBO2:
        case KEY_ITEM8_COMBO1:
        case KEY_ITEM8_COMBO2:
            Lara.keyitemscombo |= (1 << (object_number + 41));
            break;
        case PICKUP_ITEM1:
        case PICKUP_ITEM2:
        case PICKUP_ITEM3:
        case PICKUP_ITEM4:
            Lara.pickupitems |= (1 << (object_number + 25));
            break;
        case PICKUP_ITEM1_COMBO1:
        case PICKUP_ITEM1_COMBO2:
        case PICKUP_ITEM2_COMBO1:
        case PICKUP_ITEM2_COMBO2:
        case PICKUP_ITEM3_COMBO1:
        case PICKUP_ITEM3_COMBO2:
        case PICKUP_ITEM4_COMBO1:
        case PICKUP_ITEM4_COMBO2:
            Lara.pickupitemscombo |= 1 << (object_number + 21);
            break;
        case QUEST_ITEM1:
        case QUEST_ITEM2:
        case QUEST_ITEM3:
        case QUEST_ITEM4:
        case QUEST_ITEM5:
        case QUEST_ITEM6:
            Lara.questitems |= (1 << (object_number + 4));
            break;
        default:
            S_Log(LT_Function, "not found any variables to store this object_number: %d", false, object_number); // just in case i missed something or missed implemented a new item...
            break;
    }
}

BOOL have_i_got_object(short object_number)
{
    if (object_number >= PUZZLE_ITEM1_COMBO1 && object_number <= PUZZLE_ITEM8_COMBO2)
        return (Lara.puzzleitemscombo >> (object_number + 69)) & 1;
    else if (object_number >= PUZZLE_ITEM1 && object_number <= PUZZLE_ITEM12)
        return (Lara.puzzleitems[object_number - PUZZLE_ITEM1]);
    else if (object_number >= KEY_ITEM1_COMBO1 && object_number <= KEY_ITEM8_COMBO2)
        return (Lara.keyitemscombo >> (object_number + 41)) & 1;
    else if (object_number >= KEY_ITEM1 && object_number <= KEY_ITEM12)
        return (Lara.keyitems >> (object_number + 53)) & 1;
    else if (object_number >= PICKUP_ITEM1_COMBO1 && object_number <= PICKUP_ITEM4_COMBO2)
        return (Lara.pickupitemscombo >> (object_number + 21)) & 1;
    else if (object_number >= PICKUP_ITEM1 && object_number <= PICKUP_ITEM4)
        return (Lara.pickupitems >> (object_number + 25)) & 1;
    else if (object_number >= QUEST_ITEM1 && object_number <= QUEST_ITEM6)
        return (Lara.questitems >> (object_number + 4)) & 1;
    else if (object_number == CROWBAR_ITEM)
        return (Lara.crowbar);
    else
        return 0;
}

int convert_obj_to_invobj(short object_number)
{
    for (int i = 0; i < MAX_INVOBJ; i++)
    {
        INVOBJ* list = &inventry_objects_list[i];
        if (object_number == list->object_number)
            return i;
    }

    return NO_ITEM;
}

void do_compass_mode(void)
{
    DrawThreeDeeObject2D(
        int(inventory_xpos + (phd_centerx * 0.00390625 * 60.0)),
        int(inventory_ypos + (phd_centery)),
        INV_COMPASS,
        128,
        0,
        0,
        0,
        0,
        FALSE);
}

void do_examine_mode(void)
{
    static int examine2_y = font_height;
    static int examine3_y = font_height * 5;
    short saved_scale;
    short inv_item = ring_2d[RING_INVENTORY]->current_obj_list[ring_2d[RING_INVENTORY]->cur_obj_in_list].inv_item;
    INVOBJ* obj = &inventry_objects_list[inv_item];


    if (examine_mode < 128)
        examine_mode += 8;
    if (examine_mode > 128)
        examine_mode = 128;

    switch (inv_item)
    {
        case INV_EXAMINE1:
            saved_scale = obj->scale;
            obj->scale = 300;

            DrawThreeDeeObject2D(
                int(inventory_xpos + (phd_centerx * 0.00390625   * STEP_L)),
                int(inventory_ypos + (phd_centery * 0.0083333338 * STEP_L) / 2),
                INV_EXAMINE1,
                examine_mode,
                0x8000,
                0x4000,
                0x4000,
                96,
                FALSE);

            obj->scale = saved_scale;
            break;
        case INV_EXAMINE2:
            saved_scale = obj->scale;
            obj->scale = 300;

            DrawThreeDeeObject2D(
                int(inventory_xpos + (phd_centerx * 0.00390625   * STEP_L)),
                int(inventory_ypos + (phd_centery * 0.0083333338 * STEP_L) / 2),
                INV_EXAMINE2,
                examine_mode,
                0,
                0,
                0,
                0,
                0);

            obj->scale = saved_scale;

            PrintString(phd_centerx, examine2_y,                     5, &gfStringWad[gfStringOffset[201]], FF_CENTER);
            PrintString(phd_centerx, examine2_y + phd_winheight / 2, 5, &gfStringWad[gfStringOffset[202]], FF_CENTER);
            break;
        case INV_EXAMINE3:
            saved_scale = obj->scale;
            obj->scale = 400;

            DrawThreeDeeObject2D(
                int(inventory_xpos + (phd_centerx * 0.00390625   * STEP_L)),
                int(inventory_ypos + (phd_centery * 0.0083333338 * STEP_L) / 2 - 8),
                INV_EXAMINE3,
                examine_mode,
                0x8000,
                0x4000,
                0x4000,
                96,
                0);

            obj->scale = saved_scale;

            PrintString(phd_centerx, examine3_y, 8, &gfStringWad[gfStringOffset[203]], FF_CENTER);
            break;
    }

    if (go_deselect)
    {
        SoundEffect(SFX_MENU_SELECT, nullptr, SFX_ALWAYS);
        go_deselect = false;
        examine_mode = 0;
    }
}

void load_inventory_gameflow(BYTE* data, int item_first, int item_last, int space)
{
    if (data[0] >= item_first && data[0] <= item_last)
    {
        short inv_item = data[0] - space;
        if (inv_item >= INV_START_ITEM && inv_item <= MAX_INVOBJ)
        {
            inventry_objects_list[inv_item].name  = data[0];
            inventry_objects_list[inv_item].yoff  = data[1];
            inventry_objects_list[inv_item].scale = data[2];
            inventry_objects_list[inv_item].yrot  = data[3];
            inventry_objects_list[inv_item].xrot  = data[4];
            inventry_objects_list[inv_item].zrot  = data[5];
            inventry_objects_list[inv_item].flags = data[6];
        }
    }
}

void ChooseInventory(void)
{
    display_inventory_2d();
}

#ifdef DLL_INJECT
void injector::f_game::inject_inventory()
{
    inject(0x0043B760, ChooseInventory);
    inject(0x0043B9B0, construct_inventory_2d);
    inject(0x0043BC30, do_debounced_input);
    inject(0x0043BD80, DrawThreeDeeObject2D);
    inject(0x0043BF10, DrawInventoryItem);
    inject(0x0043C400, go_and_load_game);
    inject(0x0043C410, go_and_save_game);
    inject(0x0043C420, construct_combine_object_list);
    inject(0x0043C5A0, insert_object_into_list_combine);
    inject(0x0043C610, construct_object_list);
    inject(0x0043CA90, insert_object_into_list_inventory);
    inject(0x0043CAE0, draw_current_object_list);
    inject(0x0043D450, handle_object_changeover);
    inject(0x0043D470, handle_inventry_menu);
    inject(0x0043DE30, fade_ammo_selector);
    inject(0x0043DED0, draw_ammo_selector);
    inject(0x0043E140, spinback);
    inject(0x0043E1B0, update_laras_weapons_status);
    inject(0x0043E250, is_item_currently_combinable);
    inject(0x0043E2F0, have_i_got_item);
    inject(0x0043E320, do_these_objects_combine);
    inject(0x0043E360, combine_these_two_objects);
    inject(0x0043E3D0, seperate_object);
    inject(0x0043E420, combine_revolver);
    inject(0x0043E480, combine_crossbow);
    inject(0x0043E4E0, combine_puzzle_item1);
    inject(0x0043E500, combine_puzzle_item2);
    inject(0x0043E520, combine_puzzle_item3);
    inject(0x0043E540, combine_puzzle_item4);
    inject(0x0043E560, combine_puzzle_item5);
    inject(0x0043E580, combine_puzzle_item6);
    inject(0x0043E5A0, combine_puzzle_item7);
    inject(0x0043E5C0, combine_puzzle_item8);
    inject(0x0043E5E0, combine_key_item1);
    inject(0x0043E600, combine_key_item2);
    inject(0x0043E620, combine_key_item3);
    inject(0x0043E640, combine_key_item4);
    inject(0x0043E660, combine_key_item5);
    inject(0x0043E680, combine_key_item6);
    inject(0x0043E6A0, combine_key_item7);
    inject(0x0043E6C0, combine_key_item8);
    inject(0x0043E6E0, combine_pickup_item1);
    inject(0x0043E700, combine_pickup_item2);
    inject(0x0043E720, combine_pickup_item3);
    inject(0x0043E740, combine_pickup_item4);
    inject(0x0043E760, combine_clockwork);
    inject(0x0043E770, combine_waterskin);
    inject(0x0043E830, setup_objectlist_startposition_invitem);
    inject(0x0043E860, setup_objectlist_startposition_objnumber);
    inject(0x0043E8A0, use_current_item);
    inject(0x0043EB80, picked_up_object);
    inject(0x0043EF60, have_i_got_object);
    inject(0x0043F150, convert_obj_to_invobj);
    inject(0x0043F180, do_compass_mode);
    inject(0x0043F1E0, do_examine_mode);
}
#endif
