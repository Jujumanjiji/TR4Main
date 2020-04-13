#pragma once

extern void CalcLaraMatrices(short pprot);
extern void CalcLaraMatrices_Cutscene(short *frame, BONE_STRUCT *bone, short pprot);
extern void DEL_CalcLaraMatrices_Interpolate(short *frame1, short *frame2, int frac, int rate, BONE_STRUCT *bone, short pprot);