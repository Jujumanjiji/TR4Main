#pragma once
#ifdef DLL_INJECT

#define VAR_U_(address, type)            (*(type*)(address))                // uninitialized variable
#define VAR_I_(address, type, value)    (*(type*)(address))                // initialized variable (value is just for info)
#define ARRAY_(address, type, length)    (*(type(*)length)(address))        // array (can be multidimensional)


/// official app address
#define App                                VAR_U_(0x00753980, WINAPP)

///===============================///
///             MATRIX            ///
///===============================///

#define phd_mxptr                           VAR_U_(0x007E71D8, PHD_MATRIX*)
#define w2v_matrix                          ARRAY_(0x00753CC8, int, [12])
#define IM_ptr                              VAR_U_(0x007FE124, PHD_MATRIX*)
#define IM_rate                             VAR_U_(0x007FE120, int)
#define IM_frac                             VAR_U_(0x007FDF54, int)
#define IM_stack                            ARRAY_(0x007FD320, PHD_MATRIX, [64])

///===============================///
///             OTHER             ///
///===============================///

#define lara_matrix_normal_type2            ARRAY_(0x0080EA00, int, [12][12])
#define lara_matrix_holsters_type2          ARRAY_(0x0080E720, int, [12][12]) // first [12] = joint, second [12] = matrix
#define lara_matrix_normal                  VAR_U_(0x0080EA00, LARA_MATRIX)
#define lara_matrix_holsters                VAR_U_(0x0080E720, LARA_MATRIX)
#define phd_laralefthand                    ARRAY_(0x0080E9C0, int, [12])
#define phd_lararighthand                   ARRAY_(0x0080E930, int, [12])
#define phd_left                            VAR_U_(0x00753C78, int)
#define phd_top                             VAR_U_(0x00753C30, int)
#define phd_right                           VAR_U_(0x007540D0, int)
#define phd_bottom                          VAR_U_(0x00753C70, int)
#define phd_winxmax                         VAR_U_(0x00753C84, short)
#define phd_winymax                         VAR_U_(0x00753CA8, short)
#define phd_persp                           VAR_U_(0x00753C74, int)
#define phd_znear                           VAR_U_(0x00753C88, int)
#define phd_zfar                            VAR_U_(0x00753C60, int)
#define phd_centerx                         VAR_U_(0x00753C4C, int)
#define phd_centery                         VAR_U_(0x00753C50, int)
#define phd_winwidth                        VAR_U_(0x00753CF8, int)
#define phd_winheight                       VAR_U_(0x00753C48, int)
#define rcossin_tbl                         ARRAY_(0x004B56A4, short, [8192])
#define CurrentRoom                         VAR_U_(0x007FDF6C, int)
#define invnframes                          VAR_U_(0x007FE734, int)
#define NumRPickups                         VAR_U_(0x007F5708, int)
#define byte_7FD1DC                         VAR_U_(0x007FD1DC, char)
#define byte_7F5700                         VAR_U_(0x007F5700, char)
#define SequenceUsed                        VAR_U_(0x007F570C, short)
#define CurrentSequence                     VAR_I_(0x007F5700, BYTE, 0)
#define Sequences                           ARRAY_(0x007F573C, BYTE, [3])
#define SequenceResult                      ARRAY_(0x007F5720, BYTE, [3][3][3])
#define LevelNow                            VAR_U_(0x007FD290, char)
#define level_items                         VAR_I_(0x004BF68C, int, 0)
#define nAIObjects                          VAR_U_(0x007FD200, short)
#define AIObjects                           VAR_U_(0x007FD204, AIOBJECT*)
#define GnPlayingCutseq                     VAR_I_(0x004BFC80, int, 0)
#define bDisableLaraControl                 VAR_I_(0x004BF6F0, int, 0)
#define camera_number_frames                VAR_U_(0x007FE854, int)
#define outside                             VAR_U_(0x007FD300, int)
#define outside_top                         VAR_U_(0x007FE150, int)
#define outside_left                        VAR_U_(0x007FE118, int)
#define outside_right                       VAR_U_(0x007FE138, int)
#define outside_bottom                      VAR_U_(0x007FDF58, int)
#define underwater                          VAR_U_(0x007FE12C, int)
#define bound_list                          ARRAY_(0x004BF424, int, [128])
#define bound_start                         VAR_I_(0x004BF628, int, 0)
#define bound_end                           VAR_I_(0x004BF62C, int, 0)
#define draw_rooms                          ARRAY_(0x007FDF80, short, [200])
#define draw_rooms_number                   VAR_U_(0x007FD304, int)
#define RenderTypeDebug                     VAR_U_(0x007E71E0, int)
#define BinocularRange                      VAR_I_(0x004BF3E4, int, 0)
#define BinocularOldCamera                  VAR_I_(0x004BF3EC, int, CHASE_CAMERA)
#define LaserSight                          VAR_I_(0x004BF3F0, int, 0)
#define LaserSightActive                    VAR_I_(0x004BF37E, int, 0)
#define LaserSightPos                       VAR_U_(0x00800160, PHD_VECTOR)
#define Lightning                           ARRAY_(0x007FE130, unsigned short, [])
#define LightningDraw                       VAR_U_(0x007FDF68, short)
#define LightningRandom                     VAR_U_(0x007FDF5C, short)
#define LightningStart                      VAR_U_(0x007FE14C, short)
#define LightningTimer                      VAR_I_(0x004BF624, short, 0)
#define item_room_number                    VAR_U_(0x007FDF50, short)
#define item_color                          VAR_I_(0x004AD7D4, DWORD, RGBA_VECTORGET(0, 0, 0, 255)) /// argb
#define TargetType                          VAR_U_(0x007FE9B8, short) // use TARGET_TYPE enum
#define vbufdoor                            ARRAY_(0x007FDF20, DOOR_VBUF, [4])
#define GunFlashColor                       VAR_U_(0x005C047C, unsigned int)
#define next_firesparks                     VAR_I_(0x004ADF5C, int, 0)
#define next_smokesparks                    VAR_I_(0x004BF390, int, 0)
#define next_gunshell                       VAR_I_(0x004BF398, int, 0)
#define next_bubbles                        VAR_I_(0x004BF39C, int, 0)
#define next_drip                           VAR_I_(0x004BF3A0, int, 0)
#define next_debris                         VAR_I_(0x004BF358, int, 0)
#define next_blood                          VAR_I_(0x004BF394, int, 0)
#define dummy_rotation                      ARRAY_(0x004BF630, short, [12])
#define Setting_MusicVolume                 VAR_U_(0x004B38AC, int)
#define Setting_SFXVolume                   VAR_U_(0x004B38B0, int)
#define force_fixed_camera                  VAR_U_(0x007FE8C0, bool)
#define baddie_slots                        VAR_U_(0x007FD1FC, CREATURE_INFO*)
#define trigger_index                       VAR_U_(0x007FE248, short*)
#define number_boxes                        VAR_U_(0x007FE9B4, int)
#define SmokeCountL                         VAR_U_(0x00804E68, int)
#define SmokeCountR                         VAR_U_(0x00804E6C, int)
#define SmokeWeapon                         VAR_U_(0x00804F20, int)
#define TrInput                             VAR_U_(0x00535704, int)
#define TrInputDB                           VAR_U_(0x00535600, int)
#define dbinput                             VAR_U_(0x005358AC, int)
#define reset_flag                          VAR_I_(0x004BF414, int, 0)
///#define EntityBlindTimer                    VAR_U_(0x0080E136, char)
#define EnemyOffset                         ARRAY_(0x004AED00, BITE_INFO, [11])
#define level_data                          VAR_U_(0x005333A4, char*)
#define fp_level                            VAR_U_(0x00533A74, LPFILE)
#define height_type                         VAR_U_(0x007FE290, int) // height_types enum
#define interpolated_bounds                 ARRAY_(0x007FE140, short, [6])
#define LeftClimbTab                        ARRAY_(0x004AD800, short, [4])
#define RightClimbTab                       ARRAY_(0x004AD808, short, [4])
#define LeftIntRightExtTab                  ARRAY_(0x004AD838, short, [4])
#define LeftExtRightIntTab                  ARRAY_(0x004AD840, short, [4])
#define SelectedObject                      VAR_I_(0x004AE06C, int, NO_ITEM)
#define current_pendulum                    VAR_U_(0x007FD1A0, PENDULUM)
#define FXType                              VAR_U_(0x007FE7E0, short)
#define number_los_rooms                    VAR_I_(0x004BF418, int, 0)
#define los_rooms                           ARRAY_(0x007FE260, short, [20])
#define ClosestItem                         VAR_U_(0x007FE2E0, int)
#define ClosestDist                         VAR_U_(0x007FE380, int)
#define ClosestCoord                        VAR_U_(0x007FE330, PHD_VECTOR)
#define SmashedMeshCount                    VAR_I_(0x004BF3FE, short, 0)
#define SmashedMeshRoom                     ARRAY_(0x007FE360, short, [16])
#define SmashedMesh                         ARRAY_(0x007FE7A0, GAME_VECTOR*, [4])
#define ShatterItem                         VAR_U_(0x007FE960, SHATTER_ITEM)
#define lara_shadow_bbox                    VAR_U_(0x0080E710, short*)
#define vertex_buffer                       VAR_U_(0x00753910, LPDIRECT3DVERTEXBUFFER)
#define font_height                         VAR_U_(0x007F5050, int)
#define thread_started                      VAR_U_(0x00533B10, int)
#define GameTimer                           VAR_U_(0x007FD258, int)
#define SetDebounce                         VAR_U_(0x007FE160, int)

#define wibble                              VAR_I_(0x004BF360, int, 0)
#define fire_bounds                         ARRAY_(0x0080D7C0, short, [6])
#define dynamics                            ARRAY_(0x00804860, DYNAMIC, [MAX_DYNAMICS])
#define lara                                VAR_U_(0x0080DFE0, LARA_INFO)
#define lara_item                           VAR_U_(0x0080E13C, ITEM_INFO*)
#define lara_coll                           VAR_U_(0x004AD9C4, COLL_INFO*)
#define items                               VAR_U_(0x007FE28C, ITEM_INFO*)
#define objects                             ARRAY_(0x0052B850, OBJECT_INFO, [NUMBER_OBJECTS])
#define static_objects                      ARRAY_(0x005333C0, STATIC_INFO, [NUMBER_STATIC_OBJECTS])
#define mesh_base                           VAR_U_(0x00533A7C, short*)
#define meshes                              VAR_U_(0x00533A80, short**)
#define rooms                               VAR_U_(0x00533A64, ROOM_INFO*)
#define anims                               VAR_U_(0x00533A68, ANIM_STRUCT*)
#define changes                             VAR_U_(0x00533A6C, CHANGE_STRUCT*)
#define ranges                              VAR_U_(0x00533A70, RANGE_STRUCT*)
#define commands                            VAR_U_(0x00533A8C, short*)
#define bones                               VAR_U_(0x00533A88, int*)
#define frames                              VAR_U_(0x00533A84, short*)
#define overlaps                            VAR_U_(0x007FE9A8, short*)
#define boxes                               VAR_U_(0x007FE9B0, BOX_INFO*)
#define floor_data                          VAR_U_(0x00533A78, short*)
#define camera                              VAR_U_(0x007FE820, CAMERA_INFO)
#define effects                             VAR_U_(0x008012E0, FX_INFO*)
#define flip_status                         VAR_U_(0x007FE218, int)
///#define weapons                          ARRAY_(0x004AD848, WEAPON_INFO, [8])
///#define pistols_table                    ARRAY_(0x004AD820, PISTOL_DEF, [4])
#define ground_zone                         ARRAY_(0x007FE980, short*, [5][2]) // [ZONE_TYPE][flip_status] (return short*)
#define gunshells                           ARRAY_(0x007FF680, GUNSHELL_STRUCT, [MAX_GUNSHELLS])
#define sparks                              ARRAY_(0x00801400, SPARKS, [MAX_SPARKS])
#define gunflashes                          ARRAY_(0x00800CE0, GUNFLASH_STRUCT, [MAX_GUNFLASH])
#define firesparks                          ARRAY_(0x00800600, FIRE_SPARKS, [MAX_FIRE_SPARKS])
#define smokesparks                         ARRAY_(0x007FF100, SMOKE_SPARKS, [MAX_SMOKESPARKS])
#define debris                              ARRAY_(0x00808FA0, DEBRIS_STRUCT, [MAX_DEBRIS])
#define blood                               ARRAY_(0x00800180, BLOOD_STRUCT, [MAX_BLOOD])
#define splashes                            ARRAY_(0x00804E80, SPLASH_STRUCT, [MAX_SPLASHES])
#define ripples                             ARRAY_(0x00801300, RIPPLE_STRUCT, [MAX_RIPPLES])
#define bubbles                             ARRAY_(0x00800920, BUBBLE_STRUCT, [MAX_BUBBLES])
#define drips                               ARRAY_(0x00800DC0, DRIP_STRUCT, [MAX_DRIPS])
#define shockwave                           ARRAY_(0x007FF960, SHOCKWAVE_STRUCT, [MAX_SHOCKWAVE])
#define firelists                           ARRAY_(0x007FFF60, FIRE_LIST, [MAX_FIRE_LIST])
#define ropes                               ARRAY_(0x007FB4C0, ROPE_STRUCT, [MAX_ROPES])
#define collide_item                        ARRAY_(0x00804F80, ITEM_INFO*, [256]) // not sure for the size !
#define collide_meshes                      ARRAY_(0x00805380, MESH_INFO*, [256]) // not sure for the size !
#define effect_routines                     ARRAY_(0x004ADE8C, EFFECT_ROUTINES_STRUCT, [47])
#define savegame_level                      VAR_U_(0x007F7852, STATS)
#define savegame_game                       VAR_U_(0x007F7866, STATS)
#endif
