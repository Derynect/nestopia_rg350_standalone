#include "scaler.h"
#include <stdio.h>

/*
    Borrowed from fceux and credits to Nebuleon <nebuleon.fumika@gmail.com>
	Upscale 256x224 -> 320x240

	Horizontal upscale:
		320/256=1.25  --  do some horizontal interpolation
		8p -> 10p
		4dw -> 5dw

		coarse interpolation:
		[ab][cd][ef][gh] -> [ab][(bc)c][de][f(fg)][gh]

		fine interpolation
		[ab][cd][ef][gh] -> [a(0.25a+0.75b)][(0.5b+0.5c)(0.75c+0.25d)][de][(0.25e+0.75f)(0.5f+0.5g)][(0.75g+0.25h)h]


	Vertical upscale:
		Bresenham algo with simple interpolation

	Parameters:
	uint32 *dst - pointer to 320x240x16bpp buffer
	uint8 *src - pointer to 256x224x8bpp buffer
	palette is taken from palettetranslate[]
	no pitch corrections are made!
*/

#define AVERAGE(z, x) ((((z) & 0x00FEFEFE) >> 1) + (((x) & 0x00FEFEFE) >> 1))

// AARRGGBB
void upscale_320x240(uint32_t* dst, uint32_t* src)
{
	int midh = 240 * 3 / 4;
	int Eh = 0;
	int source = 0;
	int dh = 0;
	int y, x;

	for (y = 0; y < 240; y++)
	{
		source = dh * 256;

		for (x = 0; x < 320/10; x++)
		{
			register uint32_t a, b, c, d, e, f, g, h;

			__builtin_prefetch(dst + 8, 1);
			__builtin_prefetch(src + source + 8, 0);
			__builtin_prefetch(src + source + 256 + 8, 0);

            //fine interpolation
            //[ab][cd][ef][gh] -> [a(0.25a+0.75b)] [(0.5b+0.5c)(0.75c+0.25d)] [de] [(0.25e+0.75f)(0.5f+0.5g)] [(0.75g+0.25h)h]
		    // coarse interpolation:
		    // [ab][cd][ef][gh] -> [ab][(bc)c][de][f(fg)][gh]
			a = src[source];
			b = src[source + 1];
			c = src[source + 2];
			d = src[source + 3];
			e = src[source + 4];
			f = src[source + 5];
			g = src[source + 6];
			h = src[source + 7];

			if(Eh >= midh) { // average + 256
			    a = AVERAGE(a, src[source + 256]);
			    b = AVERAGE(b, src[source + 256 + 1]);
			    c = AVERAGE(c, src[source + 256 + 2]);
			    d = AVERAGE(d, src[source + 256 + 3]);
			    e = AVERAGE(e, src[source + 256 + 4]);
			    f = AVERAGE(f, src[source + 256 + 5]);
			    g = AVERAGE(g, src[source + 256 + 6]);
			    h = AVERAGE(h, src[source + 256 + 7]);
			}

            //uint32_t xx = SCALEP(a, 0.5);
            //printf("%08X %08X\n", a, xx);
			*dst++ = a;
			*dst++ = b;
			*dst++ = AVERAGE(b, c);
			*dst++ = c;
			*dst++ = d;
			*dst++ = e;
			*dst++ = f;
			*dst++ = AVERAGE(f, g);
			*dst++ = g;
			*dst++ = h;

			source += 8;

		}
		Eh += 224; if(Eh >= 240) { Eh -= 240; dh++; }
	}
}
