/*
 * main.cc
 */


#include <iostream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_main.h>

#include "AudioDriver.h"
#include "SoundSorce.h"
#include "FMWaveGenerator.h"

int main(int, char**)
{
	SDL_Init(SDL_INIT_EVERYTHING);

		FMInstrument inst
		{ 4, 2., {
		//	at, dr, sl, sr, rr, dt, beta, multiplier
			{5., 0.001, 0.5, 0.0001, 0.001, 0., 0.1 , 0.05},
			{10., 0.1, 0.98, 0.000005, 0.0001, 0., 0.5, 1.},
			{5.0, 0.001, 0.5, 0.0001, 0.001, 0., 0.1 , 0.05},
			{1., 0.1, 0.98, 0.000005, 0.0001, 0., 0.5, 0.5} } };
/*
 * 	FMInstrument inst
	{ 4, 2, {
			// at,dr,sl,sr,rr,dt,beta, multiplier
		{1., 0.001, 0.9, 0.0001, 0.001, 0., 3 , 2.},
		{10., 0.1, 0.98, 0.000005, 0.0001, 0., 0.5, 1.},
		{1., 0.001, 0.9, 0.0001, 0.001, 0., 3, 2.},
		{1., 0.1, 0.98, 0.000005, 0.0001, 0., 0.5, 0.5} } };
 */
		AudioDriver drv;

		auto src = std::make_shared<SoundSource>();
		auto wg = std::make_shared<SineWaveGenerator>();
		auto eg = std::make_shared<ConstantEnvelopeGenerator>(1.);
		auto fm = std::make_shared<FMWaveGenerator>();

		fm->set_freq(440.);
		fm->setup(inst);

		src->set_envelope_generator(eg);
		src->set_wave_generator(fm);

		drv.set_sound_source(src);
		drv.unpause();
		drv.set_master_volume(4000);
		fm->key_on();

		auto win = std::shared_ptr<SDL_Window>(
				SDL_CreateWindow("", 50, 50, 800, 600, SDL_WINDOW_SHOWN),
				SDL_DestroyWindow);
		SDL_Event ev;
		for (;;) {
			while (SDL_PollEvent(&ev)) {
				if (ev.type == SDL_QUIT) goto the_exit;
			}

			SDL_Delay(10);
		}

	the_exit:
	return 0;
	}

/*
	SDL_Init(SDL_INIT_EVERYTHING);

	FMInstrument inst
			{ 5, true, {
			  { 1., 0.1, 0.9, 0.000001, 1.0, 0.},
			  { 1., 0.1, 0.9, 0.000001, 1.0, 0. },
			  { 1., 0.1, 0.9, 0.000001, 1.0, 0.},
			  { 1., 0.1, 0.9, 0.000001, 1.0, 0.} } };


	AudioDriver drv;
	drv.unpause();

	auto src = std::make_shared<SoundSource>();
	auto wg = std::make_shared<FMWaveGenerator>();
	auto eg = std::make_shared<ConstantEnvelopeGenerator>(1.);


	wg->set_freq(440.);

	wg->setup(inst);
	src->set_envelope_generator(eg);
	src->set_wave_generator(wg);

	drv.set_sound_source(src);
	drv.set_master_volume(4000); //не менять
	wg->key_on(); //.90

	auto win = std::shared_ptr<SDL_Window>(
			SDL_CreateWindow("", 50, 50, 800, 600, SDL_WINDOW_SHOWN),
			SDL_DestroyWindow);
	SDL_Event ev;
	for (;;) {
		while (SDL_PollEvent(&ev)) {
			if (ev.type == SDL_QUIT) goto the_exit;
		}

		SDL_Delay(10);
	}

	the_exit:
	return 0;
*/
