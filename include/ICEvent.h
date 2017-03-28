/*
This code is free to use, copy, distribute, and modify.
If you use this code or any modification of this code, we request that you reference both this code https://zenodo.org/record/438675 and the paper https://arxiv.org/abs/17xx.xxxxx.
*/

#ifndef ICEvent_H
#define ICEvent_H

#include <vector>
#include <string>

#include "Coordinates.h"

extern const double ICEmin;

class ICEvent
{
	public:
		ICEvent();
		// dec_, RA_ in degrees
		ICEvent(int id_, double Edep_, double Em_, double Ep_, double time_, double dec_, double RA_, double alpha50_, bool is_shower_);

		int id;
		double Edep, Em, Ep; // TeV, E (-Em, +Ep)
		double E; // same for showers, adjusted for tracks
		double time; // MJD
		coord2D coord_eq; // direction in equatorial coordinates
		double alpha50; // degress of 50% containment
		double sigma_direction; // corresponds to vmf
		double kappa; // corresponds to vmf
		bool is_shower; // true if is shower, false if is track
};

// for tracks only
// from 1611.07905
double Edep2Enu(double Edep);

std::vector<ICEvent> read_IC(int n, std::string fname);
std::vector<ICEvent> read_IC54();

extern std::vector<ICEvent> events;

#endif
