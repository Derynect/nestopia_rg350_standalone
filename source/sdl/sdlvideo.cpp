/*
 * Nestopia UE
 * 
 * Copyright (C) 2012-2018 R. Danbrook
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 */

#include <SDL/SDL.h>
#include <stdlib.h>
//#include <SDL/SDL_video.h>

// Nst Common
#include "nstcommon.h"
#include "config.h"
#include "video.h"
#include "input.h"
#include <time.h>

// Nst SDL
#include "cursor.h"
#include "sdlvideo.h"

extern nstpaths_t nstpaths;
extern Emulator emulator;

SDL_Surface *screen = NULL;
static Uint32 flags = SDL_SWSURFACE | SDL_TRIPLEBUF | SDL_ASYNCBLIT;

void nstsdl_video_create() {
    screen = SDL_SetVideoMode(320, 240, 16, flags);

    if (!screen) {
        fprintf(stderr, "Error creating screen: %s\n", SDL_GetError());
        exit(1);
    }

    SDL_ShowCursor(false);
}

void nstsdl_video_destroy() {
    SDL_QuitSubSystem(SDL_INIT_VIDEO);
}

