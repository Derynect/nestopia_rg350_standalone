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

dimensions_t nstsdl_video_get_dimensions() {
    dimensions_t scrsize;
    scrsize.w = 320;
    scrsize.h = 240;
    return scrsize;
}

void nstsdl_video_resize() {
	dimensions_t rendersize = nst_video_get_dimensions_render();
    SDL_SetVideoMode(320, 240, 16, flags);
}

void nstsdl_video_set_cursor() {
}

void nstsdl_video_set_title(const char *title) {
}

uint32_t lastNs = 0;
void nstsdl_video_swapbuffers() {
    //SDL_Flip(screen);
#if 0
    uint32_t curNs = getUs();
    if (lastNs != 0)
        printf("%u\n", abs(curNs - lastNs));
    lastNs = curNs;
#endif
}

void nstsdl_video_toggle_fullscreen() {
}

void nstsdl_video_toggle_filter() {
	video_toggle_filter();
	nst_video_set_dimensions_screen(nstsdl_video_get_dimensions());
	video_init();
	nstsdl_video_resize();
}

void nstsdl_video_toggle_scale() {
	video_toggle_scalefactor();
	nst_video_set_dimensions_screen(nstsdl_video_get_dimensions());
	video_init();
	nstsdl_video_resize();
}
