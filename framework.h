#pragma once

#define DLL_NAME "Tomb Raider IV - DLL"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <windowsx.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <iostream>
#include <ostream>
#include <sstream>
#include <fstream>

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
#include <zlib.h>

#include "global/injector.h"
#include "global/objects.h"
#include "global/enums.h"
#include "global/constants.h"
#include "global/types.h"
#include "global/vars.h"
#include "global/macro.h"

using namespace std;