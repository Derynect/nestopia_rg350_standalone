/*
 * Nestopia UE
 * 
 * Copyright (C) 2012-2017 R. Danbrook
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
#include <stdlib.h>
#include <string.h>

#include "nstcommon.h"
#include "config.h"
#include "audio.h"

extern Emulator emulator;

static SDL_AudioSpec spec;

static int framerate, channels;

static bool paused = false;

#define OUTBUFSZ 8192
static int16_t outbuf[OUTBUFSZ];
static uint32_t woffset = 0;
static uint32_t roffset = 0;
static int16_t* inbuf = 0;

void (*audio_deinit)();

void KillSound(void) {
    SDL_CloseAudio();
    SDL_QuitSubSystem(SDL_INIT_AUDIO);
}

void audio_deinit_sdl() {
    KillSound();
}

#undef FFF
void audio_play() {
	if (paused) { return; }

    // add channels?
    int i = 0;
    while (i < spec.samples && woffset < roffset + OUTBUFSZ)
    {
        outbuf[woffset++ % OUTBUFSZ] = inbuf[i++];
    }

#ifdef FFF
    printf("audio_play   woffset %d roffset %d spec.samples %d outbufsize %d wrote %d %s\n",
            woffset, roffset, spec.samples, (int)sizeof(outbuf), i,
            i < spec.samples ? "UNDERRUN" : "");

    if (i < spec.samples)
        printf("!!! overrun\n");
#endif
}

void audio_cb_sdl(void *data, uint8_t *stream, int len) {
    len /= 2;
    int16_t* ptr = (int16_t*)stream;
    int writecount = 0;
    for (int i = 0; i < len; i++) {
        if (roffset < woffset) {
            writecount++;
            ptr[i] = outbuf[roffset % OUTBUFSZ];
            roffset++;
        }
        else
            ptr[i] = 0;
    }
#ifdef FFF
    printf("audio_cb_sdl woffset %d roffset %d spec.samples %d outbufsize %d len %d writecount %d\n",
            woffset, roffset, spec.samples, OUTBUFSZ, len, writecount);
#endif

    return;
    /*
    len /= 2;
    int16_t* ptr = (int16_t*)stream;

    printf("audio_cb_sdl len %d outbufw %d outbufr %d spec.samples %d\n",
            len, outbufw, outbufr, spec.samples);

    for (int i = 0; i < len; i++) {
        if (i <= outbufw)
            ptr[i] = outbuf[i];
        else
            ptr[i] = 0;
    }

    if (outbufw > len) {
        memmove(outbuf, outbuf + len, outbufw - len);
        outbufw = 0;
    }

    return;
    int32_t *tmps = (int32_t*)stream;
    len >>= 2;

    // debug code
    //printf("s_BufferIn: %i s_BufferWrite = %i s_BufferRead = %i s_BufferSize = %i\n",
    //    s_BufferIn, s_BufferWrite, s_BufferRead, s_BufferSize);

    */
#if 0
    while (len) {
        int32_t sample = 0;
        if (s_BufferIn) {
            sample = audiobuf[s_BufferRead] & 0xFFFF;
            s_BufferRead++;
            s_BufferIn--;
            //sample |= (sample << 16);
//            sample |= (sample << 16);
        } else {
            sample = 0;
        }

        *tmps = sample;
        tmps++;
        len--; 
    }
#endif
}

void audio_init_sdl() {
	spec.freq = conf.audio_sample_rate;
	spec.format = AUDIO_S16SYS;
	spec.channels = 2;
	spec.silence = 0;
	spec.samples = 512;
	spec.userdata = 0;
	spec.callback = audio_cb_sdl;

    spec.samples = conf.audio_sample_rate / framerate;
    spec.samples *= 2;
//    spec.samples = 1024;
//	while(spec.samples < (conf.audio_sample_rate / 60) * 1)
//	    spec.samples <<= 1;

    conf.audio_stereo = 1;
    inbuf = (int16_t*)malloc(sizeof(int16_t) * spec.samples * 2);
	printf("spec.samples %d conf.audio_sample_rate %d framerate %d outbufsize %d stereo %d pal %d\n",
	        (int)spec.samples, conf.audio_sample_rate, framerate, (int)sizeof(outbuf), conf.audio_stereo, nst_pal());

	if (SDL_InitSubSystem(SDL_INIT_AUDIO) < 0)
	    printf("Error initing audio\n");

    if(SDL_OpenAudio(&spec, 0) < 0) {
        puts(SDL_GetError());
        KillSound();
        exit(0);
    }

    SDL_PauseAudio(0);
}

void audio_set_funcs() {
    audio_deinit = &audio_deinit_sdl;
}

void audio_init() {
	// Initialize audio device
	
	// Set the framerate based on the region. For PAL: (60 / 6) * 5 = 50
	framerate = nst_pal() ? (conf.timing_speed / 6) * 5 : conf.timing_speed;
	channels = conf.audio_stereo ? 2 : 1;
	roffset = woffset = 0;
	
	audio_set_funcs();
	
	audio_init_sdl();
	
	paused = false;
}

void audio_pause() {
    SDL_PauseAudio(1);
	paused = true;
}

void audio_unpause() {
    SDL_PauseAudio(0);
	paused = false;
}

void audio_set_params(Sound::Output *soundoutput) {
	// Set audio parameters
	Sound sound(emulator);
	
	sound.SetSampleBits(16);
	sound.SetSampleRate(conf.audio_sample_rate);
	
	sound.SetSpeaker(conf.audio_stereo ? Sound::SPEAKER_STEREO : Sound::SPEAKER_MONO);
	sound.SetSpeed(Sound::DEFAULT_SPEED);
	
	audio_adj_volume();
	
	soundoutput->samples[0] = inbuf;
	soundoutput->length[0] = spec.samples / 2;
	soundoutput->samples[1] = NULL;
	soundoutput->length[1] = 0;
}

void audio_adj_volume() {
	// Adjust the audio volume to the current settings
	Sound sound(emulator);
	sound.SetVolume(Sound::ALL_CHANNELS, conf.audio_volume);
	sound.SetVolume(Sound::CHANNEL_SQUARE1, conf.audio_vol_sq1);
	sound.SetVolume(Sound::CHANNEL_SQUARE2, conf.audio_vol_sq2);
	sound.SetVolume(Sound::CHANNEL_TRIANGLE, conf.audio_vol_tri);
	sound.SetVolume(Sound::CHANNEL_NOISE, conf.audio_vol_noise);
	sound.SetVolume(Sound::CHANNEL_DPCM, conf.audio_vol_dpcm);
	sound.SetVolume(Sound::CHANNEL_FDS, conf.audio_vol_fds);
	sound.SetVolume(Sound::CHANNEL_MMC5, conf.audio_vol_mmc5);
	sound.SetVolume(Sound::CHANNEL_VRC6, conf.audio_vol_vrc6);
	sound.SetVolume(Sound::CHANNEL_VRC7, conf.audio_vol_vrc7);
	sound.SetVolume(Sound::CHANNEL_N163, conf.audio_vol_n163);
	sound.SetVolume(Sound::CHANNEL_S5B, conf.audio_vol_s5b);
	
	if (conf.audio_volume == 0) { woffset = roffset = 0; }
}
