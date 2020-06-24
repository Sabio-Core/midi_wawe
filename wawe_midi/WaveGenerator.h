/*
 * WaweGenerator.h
 */

#ifndef WAVEGENERATOR_H_
#define WAVEGENERATOR_H_

#include <algorithm>

#include "globals.h"

class WaveGenerator
{
protected:
	double _freq { 0. };
	double _period_counter { 0. };
public:
	WaveGenerator() = default;
	virtual ~WaveGenerator() = default;

	void set_freq(double freq) { //выбор частоты волны
		_freq = std::max(0., std::min(freq, AUDIO_SAMPLING_FREQUENCY/2.));
	}

	virtual double next_sample();

	void restart() { _period_counter = 0.; }
};

class SquareWaveGenerator: public WaveGenerator // меандр
{
public:
	SquareWaveGenerator() = default;
	virtual ~SquareWaveGenerator() = default;

	virtual double next_sample();
};

class SawtoothWaveGenerator: public WaveGenerator //пилообразна€ волна
{
public:
	SawtoothWaveGenerator() = default;
	virtual ~SawtoothWaveGenerator() = default;

	virtual double next_sample();
};

class TriangleWaveGenerator: public WaveGenerator //треугольник волна
{
public:
	TriangleWaveGenerator() = default;
	virtual ~TriangleWaveGenerator() = default;

	virtual double next_sample();
};

class SineWaveGenerator: public WaveGenerator //синусоидальна€ волна
{
public:
	SineWaveGenerator() = default;
	virtual ~SineWaveGenerator() = default;

	virtual double next_sample();
};



#endif /* WAVEGENERATOR_H_ */
