#include <cmath>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <cassert>

#include "ICEvent.h"
#include "vMF.h"

const double ICEmin = 60;

ICEvent::ICEvent(int id_, double Edep_, double Em_, double Ep_, double time_, double dec_, double RA_, double alpha50_, bool is_shower_)
{
	id = id_;
	Edep = Edep_;
	Em = Em_;
	Ep = Ep_;
	E = (is_shower) ? Edep : Edep2Enu(Edep);
	time = time_; // MJD
	coord_eq = coord2D((90 - dec_) * M_PI / 180., RA_ * M_PI / 180);
	alpha50 = alpha50_; // degress
	sigma_direction = sigma_direction_vMF(alpha50 * M_PI / 180); // sigma_direction_vMF takes radians
	kappa = kappa_vMF(sigma_direction);
	is_shower = is_shower_; // true if is shower, false if is track
}

std::vector<ICEvent> read_IC(int n, std::string fname)
{
	std::vector<ICEvent> IC_data;

	int id;
	double Edep, Em, Ep, time, dec, RA, angular_resolution;
	std::string shower_track;
	bool is_shower;

	std::ifstream event_stream (fname);
	std::string line;
	std::stringstream ss;

	while (getline(event_stream, line))
	{
		if (line[0] == '#')
			continue;
		std::stringstream ss(line);
		ss >> id >> Edep >> Em >> Ep >> time >> dec >> RA >> angular_resolution >> shower_track;
		if (id == 32) continue; // 32 is almost certainly a bkg
		is_shower = (shower_track[0] == 'S');
		if (Edep >= ICEmin)
		{
// source all events at the GC
// dec = -29.0078;
// RA = 266.4167;
			IC_data.push_back(ICEvent(id, Edep, Em, Ep, time, dec, RA, angular_resolution, is_shower));
		}
	}
	event_stream.close();

	return IC_data;
}

// for tracks only
double Edep2Enu(double Edep)
{
	double y, b, lmax, Edep0, Edep1, y0, y1, tmp, m;
	b = 0.28; // km^-1
	lmax = 1; // km (size of IC)

	std::ifstream data_stream("inputs/Edep2Enu.txt");
	std::string line;

	Edep0 = 0;
	Edep1 = 0;
	y0 = 0;
	y1 = 0;
	while (getline(data_stream, line) and Edep1 < Edep)
	{
		Edep0 = Edep1;
		y0 = y1;

		std::stringstream ss(line);
		ss >> Edep1 >> y1 >> tmp;
	}

	assert (Edep0 < Edep and Edep < Edep1);

	m = (y1 - y0) / log(Edep1 / Edep0);
	y = m * log(Edep / Edep0) + y0;

	data_stream.close();

	return Edep / (y + (1 - y) * b * lmax);
}

std::vector<ICEvent> read_IC54()
{
	return read_IC(54, "inputs/IC54.txt");
}

std::vector<ICEvent> events = read_IC54();

