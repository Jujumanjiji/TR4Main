#pragma once
#include "lara_constants.h"

///#define NEW_ROUTINES

#define InitialiseLaraLoad INITIALISE(0x00430EB0)
#define FlareItemControl CONTROL(0x0042FF90)
#define DrawFlareItem DRAW(0x0042F7B0)

#define LARA_ROUTINES_ARGS ITEM_INFO* item, COLL_INFO* coll
#define LARA_FUNC(name) extern void name(LARA_ROUTINES_ARGS)

#define LARA_ROUTINESNEW(address, newAddr) _FUNCV(address, (LARA_ROUTINES_ARGS), newAddr)
#ifdef NEW_ROUTINES
#define LARA_ROUTINES(address) __FUNCV(address, LARA_ROUTINES_ARGS)
#else
#define LARA_ROUTINES(address) ((void(__cdecl*)(LARA_ROUTINES_ARGS)) address)
#endif

