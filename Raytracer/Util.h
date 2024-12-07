#pragma once
#include <stdint.h>
#include <math.h>

struct SRGB
{
	float r;
	float g;
	float b;
};

// *Really* minimal PCG32 code / (c) 2014 M.E. O'Neill / pcg-random.org
// Licensed under Apache License 2.0 (NO WARRANTY, etc. see website)
typedef struct { uint64_t state;  uint64_t inc; } pcg32_random_t;
#pragma warning(disable:4146)
uint32_t pcg32_random_r(pcg32_random_t* rng)
{
	uint64_t oldstate = rng->state;
	// Advance internal state
	rng->state = oldstate * 6364136223846793005ULL + (rng->inc | 1);
	// Calculate output function (XSH RR), uses old state for max ILP
	uint32_t xorshifted = (uint32_t)(((oldstate >> 18u) ^ oldstate) >> 27u);
	uint32_t rot = oldstate >> 59u;
	return (xorshifted >> rot) | (xorshifted << ((-rot) & 31));
}

// Returns a random float between 0.0 and 1.0
float RandomFloat()
{
	thread_local pcg32_random_t state;
	return (float)ldexp(pcg32_random_r(&state), -32);
}

// Convert linear rgb values to sRGB
float LinearToSrgb(float x)
{
	if (x >= 0.0031308f)
		return (1.055f) * powf(x, (1.0f / 2.4f)) - 0.055f;

	return 12.92f * x;
}

// Curve shaped to better deal with very bright colors
float ACESFilm(float x)
{
	float a = 2.51f;
	float b = 0.03f;
	float c = 2.43f;
	float d = 0.59f;
	float e = 0.14f;
	return fmax(fmin((x * (a * x + b)) / (x * (c * x + d) + e), 1.f), 0.f);
}

// takes a linear sRGB color and does a nice looking remapping to deal with overexposed colors
SRGB ToneMap(SRGB c)
{
	// S gamut cine
	float r = c.r * 0.6456794776f + c.g * 0.2591145470f + c.b * 0.0952059754f;
	float g = c.r * 0.0875299915f + c.g * 0.7596995626f + c.b * 0.1527704459f;
	float b = c.r * 0.0369574199f + c.g * 0.1292809048f + c.b * 0.8337616753f;

	r = ACESFilm(r);
	g = ACESFilm(g);
	b = ACESFilm(b);

	SRGB result;

	// S gamut cine
	result.r = r * 1.6269474099f - g * 0.5401385388f - b * 0.0868088707f;
	result.g = r * -0.1785155272f + g * 1.4179409274f - b * 0.2394254004f;
	result.b = r * 0.0444361150f - g * 0.1959199662f + b * 1.2403560812f;

	result.r = fmax(fmin(result.r, 1.f), 0.f);
	result.g = fmax(fmin(result.g, 1.f), 0.f);
	result.b = fmax(fmin(result.b, 1.f), 0.f);

	return result;
}