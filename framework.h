#pragma once
#pragma warning(disable: 4554)

#define DLL_EXPORT __declspec(dllexport)
#define NOT_INJECT 0x00000000
#define DLL_INJECT
#define DLL_NAME "Tomb Raider IV - DLL"
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#include <windowsx.h>
#include <algorithm>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <cstdlib>
#include <iostream>
#include <ostream>
#include <sstream>
#include <fstream>
#include <time.h>
#include <ctime>

#ifdef _WIN32
#include <direct.h>
#endif

#define DIRECTDRAW_VERSION 0x700
#define DIRECT3D_VERSION 0x700
#define DIRECTINPUT_VERSION 0x700
#define DIRECTSOUND_VERSION 0x700

#include <ddraw.h>
#include <d3d.h>
#include <dinput.h>
#include <mmreg.h>
#include <dsound.h>

#include "features.h"
#include "injector.h"
#include "objects.h"
#include "phd_math.h"
#include "enums.h"
#include "constants.h"
#include "types.h"
#include "vars.h"
#include "macro.h"
#include "utils.h"

using namespace std;