#pragma once

#define STR_UNLIMITED 39

#define STR_BUTTON_USE 120
#define STR_BUTTON_CHOOSE_AMMO 121
#define STR_BUTTON_COMBINE 122
#define STR_BUTTON_SEPARATE 123
#define STR_BUTTON_EQUIP 124
#define STR_BUTTON_LOADGAME 126
#define STR_BUTTON_SAVEGAME 127
#define STR_BUTTON_EXAMINE 128


#define STR_SAVETEXT 179
#define STR_LOADTEXT 173

#ifdef DLL_INJECT
#define draw_outlines ((void(__cdecl*)(void)) 0x0047A500)
#define UpdatePulseColour ((void(__cdecl*)(void)) 0x00463930)

#define PrintString ((void(__cdecl*)(WORD x, WORD y, BYTE colourFlag, char* string, WORD flag)) 0x00463B50)
#endif
