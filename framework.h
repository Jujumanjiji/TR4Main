#pragma once

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <sstream>

#ifdef _WIN32
#include <direct.h>
#endif

#define DLL_NAME "Tomb Raider IV - DLL"

#include "global/injector.h"
#include "global/objects.h"
#include "global/enums.h"
#include "global/constants.h"
#include "global/types.h"
#include "global/vars.h"
#include "global/macro.h"

using namespace std;