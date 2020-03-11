/**************************************************************************************/
/*                                                                                    */
/* Available object types: this file is used by room, anim and sprite editors         */
/*                                                                                    */
/**************************************************************************************/

/* Version number MUST be increased whenever the object lists are changed */
#define OBJECT_VERSION 129

enum object_types {
    LARA, PISTOLS_ANIM, UZI_ANIM, SHOTGUN_ANIM, CROSSBOW_ANIM, GRENADE_GUN_ANIM, REVOLVER_ANIM, FLARE_ANIM,
    LARA_SKIN, LARA_SKIN_JOINTS, LARA_SCREAM,
    LARA_CROSSBOW_LASER,
    LARA_REVOLVER_LASER,
    LARA_HOLSTERS, LARA_HOLSTERS_PISTOLS, LARA_HOLSTERS_UZIS, LARA_HOLSTERS_REVOLVER,
    LARA_SPEECH_HEAD1, LARA_SPEECH_HEAD2, LARA_SPEECH_HEAD3, LARA_SPEECH_HEAD4,
    ACTOR1_SPEECH_HEAD1, ACTOR1_SPEECH_HEAD2,
    ACTOR2_SPEECH_HEAD1, ACTOR2_SPEECH_HEAD2,
    LARA_WATER_MESH, LARA_PETROL_MESH, LARA_DIRT_MESH,
    CROWBAR_ANIM, TORCH_ANIM,
    HAIR,
    MOTORBIKE, JEEP, VEHICLE_EXTRA,

    ENEMY_JEEP,

    SKELETON, SKELETON_MIP,
    GUIDE, GUIDE_MIP,
    VON_CROY, VON_CROY_MIP,
    BADDY_1, BADDY_1_MIP,
    BADDY_2, BADDY_2_MIP,
    SETHA, SETHA_MIP,

    MUMMY, MUMMY_MIP,
    SPHINX, SPHINX_MIP,
    CROCODILE, CROCODILE_MIP,
    HORSEMAN, HORSEMAN_MIP,
    SCORPION, SCORPION_MIP,
    JEAN_YVES, JEAN_YVES_MIP,
    TROOPS, TROOPS_MIP,
    KNIGHTS_TEMPLAR, KNIGHTS_TEMPLAR_MIP,
    MUTANT, MUTANT_MIP,
    HORSE, HORSE_MIP,
    BABOON_NORMAL,    BABOON_NORMAL_MIP,
    BABOON_INV, BABOON_INV_MIP,
    BABOON_SILENT, BABOON_SILENT_MIP,
    WILD_BOAR, WILD_BOAR_MIP,
    HARPY, HARPY_MIP,
    DEMIGOD1, DEMIGOD1_MIP,
    DEMIGOD2, DEMIGOD2_MIP,
    DEMIGOD3, DEMIGOD3_MIP,
    LITTLE_BEETLE,
    BIG_BEETLE, BIG_BEETLE_MIP,
    WRAITH1,WRAITH2,WRAITH3,WRAITH4,
    BAT,
    DOG, DOG_MIP,
    HAMMERHEAD,HAMMERHEAD_MIP,
    SAS,SAS_MIP,
    SAS_DYING,SAS_DYING_MIP,
    SAS_CAPTAIN,SAS_CAPTAIN_MIP,
    SAS_DRAG_BLOKE,
    AHMET,AHMET_MIP,
    LARA_DOUBLE,LARA_DOUBLE_MIP,
    SMALL_SCORPION,
    FISH,

    GAME_PIECE1, GAME_PIECE2, GAME_PIECE3, ENEMY_PIECE, WHEEL_OF_FORTUNE,
    SCALES,

    DARTS, DART_EMITTER, HOMING_DART_EMITTER,
    FALLING_CEILING, FALLING_BLOCK, FALLING_BLOCK2,
    SMASHABLE_BIKE_WALL,
    SMASHABLE_BIKE_FLOOR,
    TRAPDOOR1, TRAPDOOR2, TRAPDOOR3,
    FLOOR_TRAPDOOR1, FLOOR_TRAPDOOR2,
    CEILING_TRAPDOOR1, CEILING_TRAPDOOR2,
    SCALING_TRAPDOOR,
    ROLLINGBALL,
    SPIKEY_FLOOR,TEETH_SPIKES,JOBY_SPIKES,SLICER_DICER,
    CHAIN, PLOUGH, STARGATE, HAMMER, BURNING_FLOOR, COG, SPIKEBALL,
    FLAME, FLAME_EMITTER, FLAME_EMITTER2, FLAME_EMITTER3,
    ROPE, FIREROPE, POLEROPE,
    ONEBLOCK_PLATFORM, TWOBLOCK_PLATFORM,
    RAISING_BLOCK1, RAISING_BLOCK2, EXPANDING_PLATFORM,
    SQUISHY_BLOCK1, SQUISHY_BLOCK2,
    PUSHABLE_OBJECT1, PUSHABLE_OBJECT2, PUSHABLE_OBJECT3, PUSHABLE_OBJECT4, PUSHABLE_OBJECT5,
    TRIPWIRE, SENTRY_GUN, MINE,
    MAPPER, OBELISK,
    FLOOR_4BLADE, ROOF_4BLADE, BIRD_BLADE, CATWALK_BLADE, MOVING_BLADE, PLINTH_BLADE, SETH_BLADE,
    LIGHTNING_CONDUCTOR,
    ELEMENT_PUZZLE,

    PUZZLE_ITEM1, PUZZLE_ITEM2, PUZZLE_ITEM3, PUZZLE_ITEM4,
    PUZZLE_ITEM5, PUZZLE_ITEM6, PUZZLE_ITEM7, PUZZLE_ITEM8,
    PUZZLE_ITEM9, PUZZLE_ITEM10, PUZZLE_ITEM11, PUZZLE_ITEM12,

    PUZZLE_ITEM1_COMBO1, PUZZLE_ITEM1_COMBO2,
    PUZZLE_ITEM2_COMBO1, PUZZLE_ITEM2_COMBO2,
    PUZZLE_ITEM3_COMBO1, PUZZLE_ITEM3_COMBO2,
    PUZZLE_ITEM4_COMBO1, PUZZLE_ITEM4_COMBO2,
    PUZZLE_ITEM5_COMBO1, PUZZLE_ITEM5_COMBO2,
    PUZZLE_ITEM6_COMBO1, PUZZLE_ITEM6_COMBO2,
    PUZZLE_ITEM7_COMBO1, PUZZLE_ITEM7_COMBO2,
    PUZZLE_ITEM8_COMBO1, PUZZLE_ITEM8_COMBO2,

    KEY_ITEM1, KEY_ITEM2, KEY_ITEM3, KEY_ITEM4,
    KEY_ITEM5, KEY_ITEM6, KEY_ITEM7, KEY_ITEM8,
    KEY_ITEM9, KEY_ITEM10, KEY_ITEM11, KEY_ITEM12,

    KEY_ITEM1_COMBO1, KEY_ITEM1_COMBO2,
    KEY_ITEM2_COMBO1, KEY_ITEM2_COMBO2,
    KEY_ITEM3_COMBO1, KEY_ITEM3_COMBO2,
    KEY_ITEM4_COMBO1, KEY_ITEM4_COMBO2,
    KEY_ITEM5_COMBO1, KEY_ITEM5_COMBO2,
    KEY_ITEM6_COMBO1, KEY_ITEM6_COMBO2,
    KEY_ITEM7_COMBO1, KEY_ITEM7_COMBO2,
    KEY_ITEM8_COMBO1, KEY_ITEM8_COMBO2,

    PICKUP_ITEM1, PICKUP_ITEM2, PICKUP_ITEM3, PICKUP_ITEM4,

    PICKUP_ITEM1_COMBO1, PICKUP_ITEM1_COMBO2,
    PICKUP_ITEM2_COMBO1, PICKUP_ITEM2_COMBO2,
    PICKUP_ITEM3_COMBO1, PICKUP_ITEM3_COMBO2,
    PICKUP_ITEM4_COMBO1, PICKUP_ITEM4_COMBO2,

    EXAMINE1,EXAMINE2,EXAMINE3,

    CROWBAR_ITEM,
    BURNING_TORCH_ITEM,
    CLOCKWORK_BEETLE, CLOCKWORK_BEETLE_COMBO1, CLOCKWORK_BEETLE_COMBO2,
    MINE_DETECTOR,

    QUEST_ITEM1, QUEST_ITEM2, QUEST_ITEM3, QUEST_ITEM4, QUEST_ITEM5, QUEST_ITEM6,
    MAP, SECRET_MAP,
    PUZZLE_HOLE1, PUZZLE_HOLE2, PUZZLE_HOLE3, PUZZLE_HOLE4,
    PUZZLE_HOLE5, PUZZLE_HOLE6, PUZZLE_HOLE7, PUZZLE_HOLE8,
    PUZZLE_HOLE9, PUZZLE_HOLE10, PUZZLE_HOLE11, PUZZLE_HOLE12,
    PUZZLE_DONE1, PUZZLE_DONE2, PUZZLE_DONE3, PUZZLE_DONE4,
    PUZZLE_DONE5, PUZZLE_DONE6, PUZZLE_DONE7, PUZZLE_DONE8,
    PUZZLE_DONE9, PUZZLE_DONE10, PUZZLE_DONE11, PUZZLE_DONE12,
    KEY_HOLE1, KEY_HOLE2, KEY_HOLE3, KEY_HOLE4,
    KEY_HOLE5, KEY_HOLE6, KEY_HOLE7, KEY_HOLE8,
    KEY_HOLE9, KEY_HOLE10, KEY_HOLE11, KEY_HOLE12,

    WATERSKIN1_EMPTY,WATERSKIN1_1,WATERSKIN1_2,WATERSKIN1_3,
    WATERSKIN2_EMPTY,WATERSKIN2_1,WATERSKIN2_2,WATERSKIN2_3,WATERSKIN2_4,WATERSKIN2_5,

    SWITCH_TYPE1, SWITCH_TYPE2, SWITCH_TYPE3, SWITCH_TYPE4, SWITCH_TYPE5, SWITCH_TYPE6, SWITCH_TYPE7, SWITCH_TYPE8,
    UNDERWATER_SWITCH1,UNDERWATER_SWITCH2,
    TURN_SWITCH,
    COG_SWITCH,
    LEVER_SWITCH,
    JUMP_SWITCH,
    CROWBAR_SWITCH,
    PULLEY,
    DOOR_TYPE1, DOOR_TYPE2, DOOR_TYPE3, DOOR_TYPE4, DOOR_TYPE5, DOOR_TYPE6, DOOR_TYPE7, DOOR_TYPE8,
    PUSHPULL_DOOR1, PUSHPULL_DOOR2,
    KICK_DOOR1, KICK_DOOR2,
    UNDERWATER_DOOR,
    DOUBLE_DOORS,
    BRIDGE_FLAT, BRIDGE_TILT1, BRIDGE_TILT2,

    SARCOPHAGUS,

    SEQUENCE_DOOR1,
    SEQUENCE_SWITCH1, SEQUENCE_SWITCH2, SEQUENCE_SWITCH3,

    SARCOPHAGUS_CUT,
    HORUS_STATUE,
    GOD_HEAD,
    SETH_DOOR,
    STATUE_PLINTH,

    PISTOLS_ITEM,
    PISTOLS_AMMO_ITEM,
    UZI_ITEM,
    UZI_AMMO_ITEM,
    SHOTGUN_ITEM,
    SHOTGUN_AMMO1_ITEM,
    SHOTGUN_AMMO2_ITEM,
    CROSSBOW_ITEM,
    CROSSBOW_AMMO1_ITEM,
    CROSSBOW_AMMO2_ITEM,
    CROSSBOW_AMMO3_ITEM,
    CROSSBOW_BOLT,
    GRENADE_GUN_ITEM,
    GRENADE_GUN_AMMO1_ITEM,
    GRENADE_GUN_AMMO2_ITEM,
    GRENADE_GUN_AMMO3_ITEM,
    GRENADE,
    SIXSHOOTER_ITEM,
    SIXSHOOTER_AMMO_ITEM,
    BIGMEDI_ITEM, SMALLMEDI_ITEM,
    LASERSIGHT_ITEM, BINOCULARS_ITEM,
    FLARE_ITEM, FLARE_INV_ITEM,
    DIARY_ITEM, COMPASS_ITEM,
    MEMCARD_LOAD_INV_ITEM,
    MEMCARD_SAVE_INV_ITEM,
    PC_LOAD_INV_ITEM,
    PC_SAVE_INV_ITEM,

    SMOKE_EMITTER_WHITE, SMOKE_EMITTER_BLACK, STEAM_EMITTER,
    EARTHQUAKE,
    BUBBLES,
    WATERFALLMIST,
    GUNSHELL, SHOTGUNSHELL,
    GUN_FLASH,
    BUTTERFLY,
    SPRINKLER,

    RED_LIGHT, GREEN_LIGHT, BLUE_LIGHT, AMBER_LIGHT, WHITE_LIGHT, BLINKING_LIGHT,
    LENS_FLARE,

    AI_GUARD, AI_AMBUSH, AI_PATROL1, AI_MODIFY, AI_FOLLOW, AI_PATROL2, AI_X1, AI_X2, LARA_START_POS,

    KILL_ALL_TRIGGERS,
    TRIGGER_TRIGGERER,
    SMASH_OBJECT1, SMASH_OBJECT2, SMASH_OBJECT3,    SMASH_OBJECT4, SMASH_OBJECT5, SMASH_OBJECT6, SMASH_OBJECT7, SMASH_OBJECT8,
    MESHSWAP1, MESHSWAP2, MESHSWAP3,
    DEATH_SLIDE,
    BODY_PART, CAMERA_TARGET,
    WATERFALL1, WATERFALL2, WATERFALL3,
    PLANET_EFFECT,
    ANIMATING1, ANIMATING1_MIP, ANIMATING2, ANIMATING2_MIP, ANIMATING3, ANIMATING3_MIP, ANIMATING4, ANIMATING4_MIP,
    ANIMATING5, ANIMATING5_MIP, ANIMATING6, ANIMATING6_MIP,  ANIMATING7, ANIMATING7_MIP,  ANIMATING8, ANIMATING8_MIP,
    ANIMATING9, ANIMATING9_MIP, ANIMATING10, ANIMATING10_MIP, ANIMATING11, ANIMATING11_MIP, ANIMATING12, ANIMATING12_MIP,
    ANIMATING13, ANIMATING13_MIP, ANIMATING14, ANIMATING14_MIP, ANIMATING15, ANIMATING15_MIP, ANIMATING16, ANIMATING16_MIP,
    HORIZON, SKY_GRAPHICS, BINOCULAR_GRAPHICS, TARGET_GRAPHICS,

    DEFAULT_SPRITES,
    MISC_SPRITES,

    NUMBER_OBJECTS
};

enum static_types {
    PLANT0, PLANT1, PLANT2, PLANT3, PLANT4, PLANT5, PLANT6, PLANT7, PLANT8, PLANT9,
    FURNITURE0, FURNITURE1, FURNITURE2, FURNITURE3, FURNITURE4,
    FURNITURE5, FURNITURE6, FURNITURE7, FURNITURE8, FURNITURE9,
    ROCK0, ROCK1, ROCK2, ROCK3, ROCK4, ROCK5, ROCK6, ROCK7, ROCK8, ROCK9,
    ARCHITECTURE0, ARCHITECTURE1, ARCHITECTURE2, ARCHITECTURE3, ARCHITECTURE4,
    ARCHITECTURE5, ARCHITECTURE6, ARCHITECTURE7, ARCHITECTURE8, ARCHITECTURE9,
    DEBRIS0, DEBRIS1, DEBRIS2, DEBRIS3, DEBRIS4, DEBRIS5, DEBRIS6, DEBRIS7, DEBRIS8, DEBRIS9,
    SHATTER0, SHATTER1, SHATTER2, SHATTER3, SHATTER4, SHATTER5, SHATTER6, SHATTER7, SHATTER8, SHATTER9,
    NUMBER_STATIC_OBJECTS
};

enum floor_types {
    FLOOR_TYPE, DOOR_TYPE, TILT_TYPE, ROOF_TYPE, TRIGGER_TYPE, LAVA_TYPE, CLIMB_TYPE, SPLIT1, SPLIT2, SPLIT3, SPLIT4,
    NOCOLF1T, NOCOLF1B, NOCOLF2T, NOCOLF2B, NOCOLC1T, NOCOLC1B, NOCOLC2T, NOCOLC2B,
    MONKEY_TYPE, TRIGTRIGGER_TYPE, MINER_TYPE
};

enum trigger_types {
    TRIGGER, PAD, SWITCH, KEY, PICKUP, HEAVY, ANTIPAD, COMBAT, DUMMY, ANTITRIGGER, HEAVYSWITCH, HEAVYANTITRIGGER, MONKEY
};

enum trigobj_types {
    TO_OBJECT, TO_CAMERA, TO_SINK, TO_FLIPMAP, TO_FLIPON, TO_FLIPOFF, TO_TARGET, TO_FINISH, TO_CD, TO_FLIPEFFECT, TO_SECRET, TO_BODYBAG, TO_FLYBY, TO_CUTSCENE
};
