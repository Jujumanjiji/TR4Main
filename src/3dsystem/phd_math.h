#pragma once

constexpr auto RADIAN = 0.01745329252f;
constexpr auto WALL_L = 1024;
constexpr auto STEP_L = 256;
constexpr short ONE_DEGREE = 182;
constexpr short GAME_FOV = 80;
constexpr short DEFAULT_FOV = GAME_FOV * ONE_DEGREE;

extern short ANGLE(short angle);
extern float to_degrees(short angle);
extern float to_radians(short angle);

extern long mGetAngle(long srcX, long srcZ, long targetX, long targetZ);
extern int phd_atan(int x, int y);
extern int phd_sqrt(int x);
extern int phd_sin(short angle);
extern int phd_cos(short angle);

#ifndef is
#define is &
#endif

#ifndef or
#define or ||
#endif

#ifndef not
#define not !
#endif

#ifndef and
#define and &&
#endif