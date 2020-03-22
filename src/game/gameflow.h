#pragma once

#define gameflow                            VAR_U_(0x007FD278, GAMEFLOW*)
#define gfCurrentLevel                      VAR_U_(0x007FD290, BYTE)
#define gfLegend                            VAR_U_(0x007FD2E8, BYTE)
#define gfLevelComplete                     VAR_U_(0x007FD2B0, char)
#define gfLevelFlags                        VAR_U_(0x007FD260, WORD)
#define gfGameMode                          VAR_U_(0x004AF451, char, 1)
#define gfLensFlare                         VAR_U_(0x007FD2A0, PHD_VECTOR)
#define gfLensFlareColor                    VAR_U_(0x007FD2B4, CVECTOR)
#define gfNumMips                           VAR_U_(0x004BF66B, BYTE)
#define gfMips                              ARRAY_(0x007FD280, BYTE, [])
#define gfMirrorRoom                        VAR_U_(0x007FD250, BYTE)
#define gfMirrorZPlane                      VAR_U_(0x007FDF64, long)
#define gfResidentCut                       ARRAY_(0x007FD254, BYTE, [4])
#define gfResetHub                          VAR_U_(0x007FD288, BYTE)
#define gfLayer1Vel                         VAR_U_(0x007FD252, char)
#define gfLayer2Vel                         VAR_U_(0x007FD251, char)
#define gfLayer1Color                       VAR_U_(0x007FD264, CVECTOR)
#define gfLayer2Color                       VAR_U_(0x007FD25C, CVECTOR)
#define gfStringWad                         VAR_U_(0x007FD2B8, char*)
#define gfStringOffset                      VAR_U_(0x007FD274, WORD*)

#ifdef DLL_INJECT
#define DoGameflow ((void(__cdecl*)(void)) 0x00451770)
#endif