#pragma once

extern void InitialiseMutant(short itemNumber);

#ifdef DLL_INJECT
//#define InitialiseMutant ((void(__cdecl*)(short itemNumber)) 0x00403ED0)
#define MutantControl ((void(__cdecl*)(short itemNumber)) 0x00403F30)
#endif
