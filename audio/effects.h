#pragma once

#include <math.h>

static short NoiseGate(short sample)
{
	short s_threshold = 99;
	if (sample > s_threshold || sample < -s_threshold)
	{
		sample *= 2.25;
		if (sample > 32768) sample = 32768;
		if (sample < -32768) sample = -32768;

	}
	else sample = 0;
	return sample;
}

static short TanhDist(short sample, short gain)
{
	return gain * tanh((double)NoiseGate(sample) * gain);
}
