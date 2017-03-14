#ifndef ICEvent_H
#define ICEvent_H

#include <vector>
#include <string>

#include "Coordinates.h"

extern const double ICEmin;

class ICEvent
{
	public:
		int id;
		double Edep, Em, Ep; // TeV, E (-Em, +Ep)
		double E; // same for showers, adjusted for tracks
		double time; // MJD
		coord2D coord_eq; // direction in equatorial coordinates
		double alpha50; // degress of 50% containment
		double sigma_direction; // corresponds to vmf
		double kappa; // corresponds to vmf
		bool is_shower; // true if is shower, false if is track

		ICEvent() {};

		// dec_, RA_ in degrees
		ICEvent(int id_, double Edep_, double Em_, double Ep_, double time_, double dec_, double RA_, double alpha50_, bool is_shower_);
};

// for tracks only
// from 1611.07905
double Edep2Enu(double Edep);

std::vector<ICEvent> read_IC(int n, std::string fname);
std::vector<ICEvent> read_IC54();

extern std::vector<ICEvent> events;

#endif