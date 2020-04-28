#pragma once

#ifdef DLL_INJECT
#define GLOBAL_cutpos            VAR_U_(0x007E7230, int*)
#define actor_pnodes             VAR_U_(0x007E7200, short*)
#define actor_frame              ARRAY_(0x007E7240, short, [6])
#define cutseq_num               VAR_I_(0x004BFC78, int, 0)
#define frig_shadow_bbox         ARRAY_(0x004B01A0, short, [6])
#define frig2_shadow_bbox        ARRAY_(0x004AECF4, short, [6])
#define DEL_playingamefmv        VAR_I_(0x004BF65C, bool, false)

#define DrawCutSeqActors ((void(__cdecl*)(void)) 0x0046B2B0)
#define UpdateAnimFrame ((void(__cdecl*)(short* actor, int nodes, short* frame)) 0x0046B220)
#define handle_cutseq_triggering ((void(__cdecl*)(void)) 0x0046A6D0)
#define trigger_title_spotcam ((void(__cdecl*)(int num)) 0x00460420)
#define PrepareCutscene ((void(__cdecl*)(void)) 0x00480570)
#define MoveLaraForFMV ((void(__cdecl*)(int x, int y, int z, short angle)) 0x0046D040)
#endif

extern void frigup_lara(void);