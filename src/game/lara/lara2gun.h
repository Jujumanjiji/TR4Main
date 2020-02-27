#pragma once

extern int weapon_holsters(int weapon_type);
extern void set_arm_info(LARA_ARM* arm, short frame);
extern void draw_pistol_meshes(int weapon_type);
extern void undraw_pistols_left(int weapon_type);
extern void undraw_pistols_right(int weapon_type);
extern void draw_pistols(int weapon_type);
extern void undraw_pistols(int weapon_type);
extern void ready_pistols(int weapon_type);
extern void pistol_handler(int weapon_type);
extern void animate_pistols(int weapon_type);

#define legacy_set_arm_info             _FUNCV(0x0042B7C0, (LARA_ARM* arm, short frame), set_arm_info)
#define legacy_draw_pistol_meshes       _FUNCV(0x0042BB20, (int weapon_type), draw_pistol_meshes)
#define legacy_undraw_pistols_left      _FUNCV(0x0042BB70, (int weapon_type), undraw_pistols_left)
#define legacy_undraw_pistols_right     _FUNCV(0x0042BBB0, (int weapon_type), undraw_pistols_right)
#define legacy_draw_pistols             _FUNCV(0x0042B720, (int weapon_type), draw_pistols)
#define legacy_undraw_pistols           _FUNCV(0x0042B840, (int weapon_type), undraw_pistols)
#define legacy_ready_pistols            _FUNCV(0x0042BAB0, (int weapon_type), ready_pistols)
#define legacy_pistol_handler           _FUNCV(0x0042BBF0, (int weapon_type), pistol_handler)
#define legacy_animate_pistols          _FUNCV(0x0042BE40, (int weapon_type), animate_pistols)