/*-
 * Copyright (c) 2020 Hans Petter Selasky. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <assert.h>
#include <math.h>

#include "protocol.h"

/* https://en.wikipedia.org/wiki/M-law_algorithm */

static int
audio_encode(float value, int max)
{
	float divisor = logf(1.0f + 255.0f);

	if (value == 0.0f)
		return (0);
	else if (value < 0.0f)
		return - (logf(1.0f - 255.0f * value) / divisor) * max;
	else
		return (logf(1.0f + 255.0f * value) / divisor) * max;
}

static float
audio_decode(int input, int max)
{
	constexpr float multiplier = (1.0f / 255.0f);
	float value = (float)input / (float)max;

	if (value == 0.0f)
		return (0);
	else if (value < 0.0f)
		return - multiplier * (powf(1.0f + 255.0f, -value) - 1.0f);
	else
		return multiplier * (powf(1.0f + 255.0f, value) - 1.0f);
}

size_t
hpsjam_packet::get8Bit2ChSample(float *left, float *right) const
{
	size_t samples = length * 2;

	for (size_t x = 0; x != samples; x++) {
		left[x] = audio_decode(getS8(x * 2), 127);
		right[x] = audio_decode(getS8(x * 2 + 1), 127);
	}
	return (samples);
}

size_t
hpsjam_packet::get16Bit2ChSample(float *left, float *right) const
{
	size_t samples = length;

	for (size_t x = 0; x != samples; x++) {
		left[x] = audio_decode(getS16(x * 4), 0x7fff);
		right[x] = audio_decode(getS16(x * 4 + 2), 0x7fff);
	}
	return (samples);
}

size_t
hpsjam_packet::get24Bit2ChSample(float *left, float *right) const
{
	size_t samples = (length * 4) / 6;

	for (size_t x = 0; x != samples; x++) {
		left[x] = audio_decode(getS24(x * 6), 0x7fffff);
		right[x] = audio_decode(getS24(x * 6 + 3), 0x7fffff);
	}
	return (samples);
}

size_t
hpsjam_packet::get32Bit2ChSample(float *left, float *right) const
{
	size_t samples = length / 2;

	for (size_t x = 0; x != samples; x++) {
		left[x] = audio_decode(getS32(x * 8), 0x7fffffff);
		right[x] = audio_decode(getS32(x * 8 + 4), 0x7fffffff);
	}
	return (samples);
}

size_t
hpsjam_packet::get8Bit1ChSample(float *left) const
{
	size_t samples = length * 4;

	for (size_t x = 0; x != samples; x++) {
		left[x] = audio_decode(getS8(x), 127);
	}
	return (samples);
}

size_t
hpsjam_packet::get16Bit1ChSample(float *left) const
{
	size_t samples = length * 2;

	for (size_t x = 0; x != samples; x++) {
		left[x] = audio_decode(getS16(2 * x), 0x7fff);
	}
	return (samples);
}

size_t
hpsjam_packet::get24Bit1ChSample(float *left) const
{
	size_t samples = (length * 4) / 3;

	for (size_t x = 0; x != samples; x++) {
		left[x] = audio_decode(getS24(3 * x), 0x7fffff);
	}
	return (samples);
}

size_t
hpsjam_packet::get32Bit1ChSample(float *left) const
{
	size_t samples = length;

	for (size_t x = 0; x != samples; x++) {
		left[x] = audio_decode(getS32(4 * x), 0x7fffffff);
	}
	return (samples);
}

void
hpsjam_packet::put8Bit2ChSample(float *left, float *right, size_t samples)
{
	assert((samples % 2) == 0);

	length = 1 + samples / 2;
	type = HPSJAM_TYPE_AUDIO_8_BIT_2CH;
	sequence[0] = 0;
	sequence[1] = 0;

	for (size_t x = 0; x != samples; x++) {
		putS8(x * 2, audio_encode(left[x], 127));
		putS8(x * 2 + 1, audio_encode(right[x], 127));
	}
}

void
hpsjam_packet::put16Bit2ChSample(float *left, float *right, size_t samples)
{
	length = 1 + samples;
	type = HPSJAM_TYPE_AUDIO_16_BIT_2CH;
	sequence[0] = 0;
	sequence[1] = 0;

	for (size_t x = 0; x != samples; x++) {
		putS16(x * 4, audio_encode(left[x], 0x7fff));
		putS16(x * 4 + 1, audio_encode(right[x], 0x7fff));
	}
}

void
hpsjam_packet::put24Bit2ChSample(float *left, float *right, size_t samples)
{
	length = 1 + (samples * 6 + 3) / 4;
	type = HPSJAM_TYPE_AUDIO_24_BIT_2CH;
	sequence[0] = 0;
	sequence[1] = 0;

	for (size_t x = 0; x != samples; x++) {
		putS24(x * 6, audio_encode(left[x], 0x7fffff));
		putS24(x * 6 + 3, audio_encode(right[x], 0x7fffff));
	}
}

void
hpsjam_packet::put32Bit2ChSample(float *left, float *right, size_t samples)
{
	length = 1 + (samples * 2);
	type = HPSJAM_TYPE_AUDIO_32_BIT_2CH;
	sequence[0] = 0;
	sequence[1] = 0;

	for (size_t x = 0; x != samples; x++) {
		putS32(x * 8, audio_encode(left[x], 0x7fffffff));
		putS32(x * 8 + 4, audio_encode(right[x], 0x7fffffff));
	}
}

void
hpsjam_packet::put8Bit1ChSample(float *left, size_t samples)
{
	assert((samples % 4) == 0);

	length = 1 + samples / 4;
	type = HPSJAM_TYPE_AUDIO_8_BIT_1CH;
	sequence[0] = 0;
	sequence[1] = 0;

	for (size_t x = 0; x != samples; x++) {
		putS8(x, audio_encode(left[x], 127));
	}
}

void
hpsjam_packet::put16Bit1ChSample(float *left, size_t samples)
{
	assert((samples % 2) == 0);

	length = 1 + samples / 2;
	type = HPSJAM_TYPE_AUDIO_16_BIT_1CH;
	sequence[0] = 0;
	sequence[1] = 0;

	for (size_t x = 0; x != samples; x++) {
		putS16(2 * x, audio_encode(left[x], 0x7fff));
	}
}

void
hpsjam_packet::put24Bit1ChSample(float *left, size_t samples)
{
	length = 1 + (samples * 3 + 3) / 4;
	type = HPSJAM_TYPE_AUDIO_24_BIT_1CH;
	sequence[0] = 0;
	sequence[1] = 0;

	for (size_t x = 0; x != samples; x++) {
		putS24(3 * x, audio_encode(left[x], 0x7fffff));
	}
}

void
hpsjam_packet::put32Bit1ChSample(float *left, size_t samples)
{
	length = 1 + samples;
	type = HPSJAM_TYPE_AUDIO_32_BIT_1CH;
	sequence[0] = 0;
	sequence[1] = 0;

	for (size_t x = 0; x != samples; x++) {
		putS32(4 * x, audio_encode(left[x], 0x7fffffff));
	}
}
