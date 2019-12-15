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
    printf("%s\n", __func__);
}

void nstsdl_input_joysticks_close() {
    printf("%s\n", __func__);
}

#define DINGOO_UP SDLK_g
#define DINGOO_DOWN SDLK_l
#define DINGOO_LEFT SDLK_i
#define DINGOO_RIGHT SDLK_j
#define DINGOO_A 29
#define DINGOO_B 56
#define DINGOO_X 57
#define DINGOO_Y 42
#define DINGOO_L 15
#define DINGOO_R SDLK_BACKSPACE
#define DINGOO_START 28
#define DINGOO_SELECT 1
#define DINGOO_MENU SDLK_HOME

void nstsdl_input_match_keyboard(Input::Controllers *controllers, SDL_Event event) {
	nesinput_t input;

	input.nescode = 0x00;
	input.player = 0;
	input.pressed = 0;
	input.turboa = 0;
	input.turbob = 0;

	if (event.type == SDL_KEYDOWN) { input.pressed = 1; }

	for (int i = 0; i < NUMGAMEPADS; i++) {
        if (DINGOO_UP == event.key.keysym.scancode) {
            input.nescode = Input::Controllers::Pad::UP;
            input.player = i;
        }
        else if (DINGOO_DOWN == event.key.keysym.scancode) {
            input.nescode = Input::Controllers::Pad::DOWN;
            input.player = i;
        }
        else if (DINGOO_LEFT == event.key.keysym.scancode) {
            input.nescode = Input::Controllers::Pad::LEFT;
            input.player = i;
        }
        else if (DINGOO_RIGHT == event.key.keysym.scancode) {
            input.nescode = Input::Controllers::Pad::RIGHT;
            input.player = i;
        }
        else if (DINGOO_SELECT == event.key.keysym.scancode) {
            input.nescode = Input::Controllers::Pad::SELECT;
            input.player = i;
        }
        else if (DINGOO_START == event.key.keysym.scancode) {
            input.nescode = Input::Controllers::Pad::START;
            input.player = i;
        }
        else if (DINGOO_A == event.key.keysym.scancode) {
            input.nescode = Input::Controllers::Pad::A;
            input.player = i;
        }
        else if (DINGOO_B == event.key.keysym.scancode) {
            input.nescode = Input::Controllers::Pad::B;
            input.player = i;
        }
        else if (DINGOO_X == event.key.keysym.scancode) {
            input.nescode = Input::Controllers::Pad::A;
            input.turboa = 1;
            input.player = i;
        }
        else if (DINGOO_Y == event.key.keysym.scancode) {
            input.nescode = Input::Controllers::Pad::B;
            input.turbob = 1;
            input.player = i;
        }
        else if (DINGOO_MENU == event.key.keysym.scancode) {
            nst_schedule_quit();
        }
    }

	nst_input_inject(controllers, input);
	
	if (event.key.keysym.scancode == DINGOO_L && event.type == SDL_KEYDOWN) { nst_timing_set_ffspeed(); }
	if (event.key.keysym.scancode == DINGOO_L && event.type == SDL_KEYUP) { nst_timing_set_default(); }
}

void nstsdl_input_match_mouse(Input::Controllers *controllers, SDL_Event event) {
    printf("%s\n", __func__);
}

int nstsdl_input_checksign(int axisvalue) {
    printf("%s\n", __func__);
    return 0;
}

void nstsdl_input_match_joystick(Input::Controllers *controllers, SDL_Event event) {
    printf("%s\n", __func__);
}

void nstsdl_input_conf_defaults() {
    printf("%s\n", __func__);
}

void nstsdl_input_conf_set(SDL_Event event, int type, int pnum, int counter) {
    printf("%s\n", __func__);
}

static int nstsdl_input_config_match(void* user, const char* section, const char* name, const char* value) {
    printf("%s\n", __func__);
    return 0;
}

void nstsdl_input_conf_read() {
    printf("%s\n", __func__);
}

void nstsdl_input_conf_write() {
    printf("%s\n", __func__);
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
    printf("%s\n", __func__);
}

void nstsdl_input_conf_button(int pnum, int bnum) {
    printf("%s\n", __func__);
}
