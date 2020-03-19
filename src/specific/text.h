#pragma once

#define STR_UNLIMITED 39

#ifdef DLL_INJECT
#define draw_outlines ((void(__cdecl*)(void)) 0x0047A500)
#define UpdatePulseColour ((void(__cdecl*)(void)) 0x00463930)

#define PrintString ((void(__cdecl*)(WORD x, WORD y, BYTE colourFlag, char* string, WORD flag)) 0x00463B50)
#endif
