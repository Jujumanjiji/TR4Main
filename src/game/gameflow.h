#pragma once

#define MAX_LEVEL 255
#define CUTSCENE_TITLE 1

extern void DoBootScreen(uint32 language_id);
extern void DoLevel(unsigned char name, unsigned char audio);
extern void DoTitle(unsigned char name, unsigned char audio);

#ifdef DLL_INJECT
#define gameflow                            VAR_U_(0x007FD278, GAMEFLOW*)
#define gfCurrentLevel                      VAR_U_(0x007FD290, unsigned char)
#define gfLevelName                         ARRAY_(0x007FD2C0, char, [40])
#define gfInitialiseGame                    VAR_I_(0x004AF452, unsigned char, 1)
#define gfRequiredStartPos                  VAR_U_(0x007FD270, char)
#define gfStatus                            VAR_I_(0x004BF650, int, 0)
#define gfLegend                            VAR_U_(0x007FD2E8, unsigned char)
#define gfLegendTime                        VAR_I_(0x004BF668, unsigned char, 0)
#define gfLevelStart                        VAR_U_(0x007FD2B0, unsigned char)
#define gfLevelFlags                        VAR_U_(0x007FD260, unsigned short)
#define gfGameMode                          VAR_I_(0x004AF451, unsigned char, 1)
#define gfCutsceneID                        VAR_I_(0x004BF66A, unsigned char, 0)
#define gfLensFlare                         VAR_U_(0x007FD2A0, PHD_VECTOR)
#define gfLensFlareColor                    VAR_U_(0x007FD2B4, CVECTOR)
#define gfNumMips                           VAR_U_(0x004BF66B, unsigned char)
#define gfMips                              ARRAY_(0x007FD280, unsigned char, [8])
#define gfMirrorRoom                        VAR_U_(0x007FD250, unsigned char)
#define gfMirrorZPlane                      VAR_U_(0x007FDF64, long)
#define gfResidentCut                       ARRAY_(0x007FD254, unsigned char, [4])
#define gfResetHub                          VAR_U_(0x007FD288, unsigned char)
#define gfResetHubEnabled                   VAR_I_(0x004BF3CE, bool, false)
#define gfUVRotate                          VAR_U_(0x007FD2E9, unsigned char)
#define gfLayer1Vel                         VAR_U_(0x007FD252, char)
#define gfLayer2Vel                         VAR_U_(0x007FD251, char)
#define gfLayer1Color                       VAR_U_(0x007FD264, CVECTOR)
#define gfLayer2Color                       VAR_U_(0x007FD25C, CVECTOR)
#define gfFogColor                          VAR_U_(0x004BF664, CVECTOR)
#define gfFogColor2                         VAR_U_(0x007F7884, CVECTOR)
#define LightningRGB                        ARRAY_(0x007FD2F8, unsigned short, [3])
#define LightningRGBs                       ARRAY_(0x007FE130, unsigned short, [3])
#define gfStringWad                         VAR_U_(0x007FD2B8, char*)
#define gfStringOffset                      VAR_U_(0x007FD26C, unsigned short*)
#define gfStringOffsetReal                  VAR_U_(0x007FD274, unsigned short*)
#define gfScriptWad                         VAR_I_(0x004BF658, unsigned char*, nullptr)
#define gfScriptOffset                      VAR_I_(0x004BF64C, unsigned short*, nullptr)
#define CutSceneTriggered                   VAR_U_(0x007FE7E4, unsigned int)
#define FmvSceneTriggered                   VAR_U_(0x007FE210, unsigned int)
#define byte_5358AC                         VAR_U_(0x005358AC, char)
#define dword_672E60                        VAR_U_(0x00672E60, int)
#define bDoCredits                          VAR_U_(0x004BF669, bool)
#define bCreditDone                         VAR_U_(0x007FE114, bool)

#define DoGameflow ((void(__cdecl*)(void)) 0x00451770)
///#define DoBootScreen ((void(__cdecl*)(int language_id)) 0x004808E0)
///#define DoLevel ((void(__cdecl*)(unsigned char name, unsigned char audio)) 0x00452710)
///#define DoTitle ((void(__cdecl*)(unsigned char name, unsigned char audio)) 0x00451E30)
#endif