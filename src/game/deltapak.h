#pragma once

#ifdef DLL_INJECT
#define DrawCutSeqActors ((void(__cdecl*)(void)) 0x0046B2B0)
#define UpdateAnimFrame ((void(__cdecl*)(short* actor, int nodes, short* frame)) 0x0046B220)

#define GLOBAL_cutpos            VAR_U_(0x007E7230, int*)
#define actor_pnodes             VAR_U_(0x007E7200, short*)
#define actor_frame              ARRAY_(0x007E7240, short, [6])
#define cutseq_num               VAR_I_(0x004BFC78, int, 0)
#define frig_shadow_bbox         ARRAY_(0x004B01A0, short, [6])
#define frig2_shadow_bbox        ARRAY_(0x004AECF4, short, [6])
#endif

extern void frigup_lara(void);