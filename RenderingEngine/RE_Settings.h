#pragma once

#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#ifdef _WIN32
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#else
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#endif

#include <cassert>
#include <stdlib.h>
#include <string.h>
#include <string>

int RE_Init(Uint32 sdlFlags, Uint32 imgFlags);
void RE_Quit();

//.................................................................................................
// Debug et informations

#define RE_AssertNew(ptr) { if (ptr == NULL) { assert(false); abort(); }}


