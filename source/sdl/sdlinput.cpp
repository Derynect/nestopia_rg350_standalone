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

#include <stdio.h>

#include <SDL/SDL.h>
#ifdef _GTK
#include "gtkui/gtkui.h"
#endif

#include "nstcommon.h"
#include "video.h"
#include "input.h"

#include "ini.h"

#include "sdlmain.h"
#include "sdlvideo.h"
#include "sdlinput.h"

//static SDL_Joystick *joystick;
//gamepad_t player[NUMGAMEPADS];
//static uiinput_t ui;
static inputsettings_t inputconf;
static char inputconfpath[256];
extern int drawtext;

static unsigned char nescodes[TOTALBUTTONS] = {
	Input::Controllers::Pad::UP,
	Input::Controllers::Pad::DOWN,
	Input::Controllers::Pad::LEFT,
	Input::Controllers::Pad::RIGHT,
	Input::Controllers::Pad::SELECT,
	Input::Controllers::Pad::START,
	Input::Controllers::Pad::A,
	Input::Controllers::Pad::B,
	Input::Controllers::Pad::A,
	Input::Controllers::Pad::B,
	Input::Controllers::Pad::UP,
	Input::Controllers::Pad::DOWN,
	Input::Controllers::Pad::LEFT,
	Input::Controllers::Pad::RIGHT,
	Input::Controllers::Pad::SELECT,
	Input::Controllers::Pad::START,
	Input::Controllers::Pad::A,
	Input::Controllers::Pad::B,
	Input::Controllers::Pad::A,
	Input::Controllers::Pad::B
};

extern Emulator emulator;
extern nstpaths_t nstpaths;

void nstsdl_input_joysticks_detect() {
}

void nstsdl_input_joysticks_close() {
}

#define DINGOO_UP       SDLK_UP
#define DINGOO_DOWN     SDLK_DOWN
#define DINGOO_LEFT     SDLK_LEFT
#define DINGOO_RIGHT    SDLK_RIGHT
#define DINGOO_A        SDLK_LCTRL
#define DINGOO_B        SDLK_LALT
#define DINGOO_X        SDLK_SPACE
#define DINGOO_Y        SDLK_LSHIFT
#define DINGOO_L        SDLK_TAB
#define DINGOO_R        SDLK_BACKSPACE
#define DINGOO_L2       SDLK_PAGEUP
#define DINGOO_R2       SDLK_PAGEDOWN
#define DINGOO_L3       SDLK_KP_DIVIDE
#define DINGOO_R3       SDLK_KP_PERIOD
#define DINGOO_START    SDLK_RETURN
#define DINGOO_SELECT   SDLK_ESCAPE
#define DINGOO_MENU     SDLK_HOME

void nstsdl_input_match_keyboard(Input::Controllers *controllers, SDL_Event event) {
	nesinput_t input;

	input.nescode = 0x00;
	input.player = 0;
	input.pressed = event.type == SDL_KEYDOWN;
	input.turboa = 0;
	input.turbob = 0;

    //printf("scancode %d mod %d unicode %d sym %d\n",
    //        event.key.keysym.scancode, (int)event.key.keysym.mod, (int)event.key.keysym.unicode, event.key.keysym.sym);

	for (int i = 0; i < 1; i++) {
	    switch (event.key.keysym.sym)
        {
        case DINGOO_UP:
            input.nescode = Input::Controllers::Pad::UP;
            input.player = i;
            break;
        case DINGOO_DOWN:
            input.nescode = Input::Controllers::Pad::DOWN;
            input.player = i;
            break;
        case DINGOO_LEFT:
            input.nescode = Input::Controllers::Pad::LEFT;
            input.player = i;
            break;
        case DINGOO_RIGHT:
            input.nescode = Input::Controllers::Pad::RIGHT;
            input.player = i;
            break;
        case DINGOO_SELECT:
            input.nescode = Input::Controllers::Pad::SELECT;
            input.player = i;
            break;
        case DINGOO_START:
            input.nescode = Input::Controllers::Pad::START;
            input.player = i;
            break;
        case DINGOO_A:
            input.nescode = Input::Controllers::Pad::A;
            input.player = i;
            break;
        case DINGOO_B:
            input.nescode = Input::Controllers::Pad::B;
            input.player = i;
            break;
        case DINGOO_X:
            input.nescode = Input::Controllers::Pad::A;
            input.player = i;
            input.turboa = 1;
            break;
        case DINGOO_Y:
            input.nescode = Input::Controllers::Pad::B;
            input.player = i;
            input.turbob = 1;
            break;
        case DINGOO_MENU:
            if (event.type == SDL_KEYDOWN)
                nst_schedule_quit();
            break;
        }
    }

	nst_input_inject(controllers, input);
	
	if (event.key.keysym.sym == DINGOO_L && event.type == SDL_KEYDOWN) {
	    nst_timing_set_ffspeed();
    }
	if (event.key.keysym.sym == DINGOO_L && event.type == SDL_KEYUP) {
	    nst_timing_set_default();
    }
}

void nstsdl_input_match_mouse(Input::Controllers *controllers, SDL_Event event) {
}

int nstsdl_input_checksign(int axisvalue) {
    return 0;
}

void nstsdl_input_match_joystick(Input::Controllers *controllers, SDL_Event event) {
}

void nstsdl_input_conf_defaults() {
}

void nstsdl_input_conf_set(SDL_Event event, int type, int pnum, int counter) {
}

static int nstsdl_input_config_match(void* user, const char* section, const char* name, const char* value) {
    return 0;
}

void nstsdl_input_conf_read() {
}

void nstsdl_input_conf_write() {
}

void nstsdl_input_process(Input::Controllers *controllers, SDL_Event event) {
	// Process input events
	switch(event.type) {
		case SDL_KEYUP:
		case SDL_KEYDOWN:
			nstsdl_input_match_keyboard(controllers, event);
			break;
			
		default: break;
	}
}

void nstsdl_input_conf(int type, int pnum) {
}

void nstsdl_input_conf_button(int pnum, int bnum) {
}
