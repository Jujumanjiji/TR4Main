#pragma once

#define STR_UNLIMITED 39

#ifdef DLL_INJECT
#define PrintString ((void(__cdecl*)(WORD x, WORD y, BYTE colourFlag, char* string, WORD flag)) 0x00463B50)
#endif
