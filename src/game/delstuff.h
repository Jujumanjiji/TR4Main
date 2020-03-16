#pragma once

extern void CalcLaraMatrices(short pprot);
extern void DEL_CalcLaraMatrices_Normal(short *frame, int *bone, short pprot);
extern void DEL_CalcLaraMatrices_Interpolate(short *frame1, short *frame2, int frac, int rate, int *bone, short pprot);