/*
 * FMWaveGenerator.cc
 */

#include "FMWaveGenerator.h"
#include "globals.h"
#include "EnvelopeGenerator.h"
#include <math.h>
#include <iostream>

FMWaveGenerator::FMWaveGenerator()
{
	for (int i = 0; i < 4; i++)
		_eg[i] = std::make_shared<ADSREnvelopeGenerator>();
	_feedback = 0.;
	_algorithm = 4;
}

static const double s_feedback_values[] {
  0, Pi/16., Pi/8., Pi/4., Pi/2., Pi, 2.*Pi, 4.*Pi
};

void FMWaveGenerator::setup(const FMInstrument& inst)
{

	_algorithm = inst.al;
	_feedback = inst.fb;
	for (int i = 0; i < 4; i++) {
		_eg[i]->set_attack_time(inst.op[i].at);
		_eg[i]->set_decay_rate(inst.op[i].dr);
		_eg[i]->set_sustain_level(inst.op[i].sl);
		_eg[i]->set_sustain_rate(inst.op[i].sr);
		_eg[i]->set_release_rate(inst.op[i].rr);
		_detune[i] = inst.op[i].dt;
		_beta[i]=inst.op[i].beta;
		_multiplier[i]=inst.op[i].multiplier;
		/*_multiplier[i]=i+1;
		_beta[i]=i*Pi;*/
	}
}

double FMWaveGenerator::next_sample()
{
	double result;
	double Pi=acos(-1);
double a1 = _eg[0]->next_sample();
double a2 = _eg[1]->next_sample();
double a3 = _eg[2]->next_sample();
double a4 = _eg[3]->next_sample();

double w1 = _freq * _multiplier[0] + _detune[0];
double w2 = _freq * _multiplier[1] + _detune[1];
double w3 = _freq * _multiplier[2] + _detune[2];
double w4 = _freq * _multiplier[3] + _detune[3];
static double t = 0.;
t += 1. / AUDIO_SAMPLING_FREQUENCY;
	switch (_algorithm) {
	case 0:
		result =
				a4 *_beta[3]* sin(
					2*Pi*w4*t +
					a3*_beta[2] * sin(
							2*Pi* w3*t+
							a2*_beta[1]*sin(
									2*Pi*w2*t+
									a1*_beta[0]*sin(
											2*Pi*w1*t+
											_feedback*a1*_beta[0]*sin(2*Pi*w1*t)
									)
							)
					)
				);
		break;
	case 1:
		result =
			a4*_beta[3]*sin(
				2*Pi*w4*t+
				a3* _beta[2]*sin(
						2*Pi*w3*t+
						a2*_beta[1]* sin(2*Pi*w2*t)+
						a1*_beta[0]*sin(2*Pi*w1*t+
								_feedback*a1*_beta[0]*sin(2*Pi*w1*t)
						)
				)
			);
		break;
	case 2:
		result=
				a4*_beta[3]*sin(
						2*Pi*w4*t
						+a1*_beta[0]*sin(
								2*Pi*w1*t+
								_feedback*a1*_beta[0]*sin(2*Pi*w1*t))+a3*_beta[2]*sin(
											2*Pi*w3*t+a2*_beta[1]*sin(
													2*Pi*w2*t)));
		break;
	case 3:
		result=
				a4*_beta[3]*sin(2*Pi*w4*t+
						a3*_beta[2]*sin(
								2*Pi*w3*t)+
				a2*_beta[1]*sin(
						2*Pi*w2*t+
						a1*_beta[0]*sin(
								2*Pi*w1*t+
								_feedback*a1*_beta[0]*sin(2*Pi*w1*t))));
		break;
	case 4:
		result=
				a4*_beta[3]*sin(2*Pi*w4*t+
						a3*_beta[2]*sin(
								2*Pi*w3*t))+
								a2*_beta[1]*sin(
										2*Pi*w2*t+
										a1*_beta[0]*sin(
												2*Pi*w1*t+
												_feedback*a1*_beta[0]*sin(2*Pi*w1*t)));
		break;
	case 5:
		result=
				a4*_beta[3]*sin(2*Pi*w4*t+a1*_beta[0]*sin(
						2*Pi*w1*t+
						_feedback*a1*_beta[0]*sin(2*Pi*w1*t)))+
				a3*_beta[2]*sin(2*Pi*w3*t+a1*_beta[0]*sin(
						2*Pi*w1*t+
						_feedback*a1*_beta[0]*sin(2*Pi*w1*t)))+
				a2*_beta[1]*sin(2*Pi*w3*t+a1*_beta[0]*sin(
						2*Pi*w1*t+
						_feedback*a1*_beta[0]*sin(2*Pi*w1*t)));
		break;
	case 6:
		result=
				a4*_beta[3]*sin(2*Pi*w4*t)+
				a3*_beta[2]*sin(2*Pi*w3*t)+
				a2*_beta[1]*sin(2*Pi*w3*t+a1*_beta[0]*sin(
						2*Pi*w1*t+
						_feedback*a1*_beta[0]*sin(2*Pi*w1*t)));
		break;
	case 7:
		result=
				a4*_beta[3]*sin(2*Pi*w4*t)+
				a3*_beta[2]*sin(2*Pi*w3*t)+
				a2*_beta[1]*sin(2*Pi*w3*t)+
				a1*_beta[0]*sin(
						2*Pi*w1*t+
						_feedback*a1*_beta[0]*sin(2*Pi*w1*t));
		break;
	default:
		throw std::runtime_error("WTF");
	}
	WaveGenerator::next_sample();
	///////////
	if (result > 1) result =1;
	if (result < -1) result =-1;
	//////////////////
	return result;
}




