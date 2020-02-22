#pragma once

///#define set_arm_info ((void(__cdecl*)(LARA_ARM* arm, short frame)) 0x0042B7C0)
///#define draw_pistol_meshes ((void(__cdecl*)(int weapon_type)) 0x0042BB20)
///#define undraw_pistols_left ((void(__cdecl*)(int weapon_type)) 0x0042BB70)
///#define undraw_pistols_right ((void(__cdecl*)(int weapon_type)) 0x0042BBB0)
///#define draw_pistols ((void(__cdecl*)(int weapon_type)) 0x0042B720)
///#define undraw_pistols ((void(__cdecl*)(int weapon_type)) 0x0042B840)
///#define ready_pistols ((void(__cdecl*)(int weapon_type)) 0x0042BAB0)
///#define pistol_handler ((void(__cdecl*)(int weapon_type)) 0x0042BBF0)
///#define animate_pistols ((void(__cdecl*)(int weapon_type)) 0x0042BE40)

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