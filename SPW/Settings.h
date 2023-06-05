#pragma once

#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "../RenderingEngine/RE.h"
#include "../PlatformerEngine/PE.h"

#ifdef _WIN32
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "SDL_mixer.h"
#else
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#endif

#include <algorithm>
#include <array>
#include <iostream>
#include <vector>
#include <list>
#include <set>
#include <map>
#include <cassert>
#include <cstdint>

#include <cstdio>
#include <cstdlib>
#include <cstring>

#define AssertNew(ptr) { if (ptr == NULL) { assert(false); abort(); } }

#define HD_WIDTH 1280
#define HD_HEIGHT 720

#define FHD_WIDTH 1920
#define FHD_HEIGHT 1080
