/*
This code is free to use, copy, distribute, and modify.
If you use this code or any modification of this code, we request that you reference both this code https://zenodo.org/record/x and the paper https://arxiv.org/abs/17xx.xxxxx.
*/

#include <gsl/gsl_min.h>
#include <gsl/gsl_errno.h>

#include <cmath>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>

#include "Likelihood.h"
#include "MWDisks.h"
#include "ICEvent.h"
#include "Coordinates.h"
#include "vMF.h"
#include "Backgrounds.h"
#include "Progress.h"

double L_gals[54];

void calc_L_gals()
{
	std::cout << "Calculating L_gals..." << std::endl;

	std::ofstream data("data/L_gals.txt");

	int N_Repeat;
	coord2D coord_gal_event, coord_gal_MW;
	double L;
	bool HE_Cut;

	HE_Cut = false; // a cut on galactic energy at 900 TeV
	N_Repeat = 1e6;

	Progress_Bar *pbar = new Progress_Bar();
	pbar->update(0);
	for (unsigned int i = 0; i < events.size(); i++)
	{
		if (events[i].E > 900 and HE_Cut)
			data << i << " " << events[i].id << " " << 0 << std::endl;
		else
		{
			coord_gal_event = eq_to_gal(events[i].coord_eq);

			L = 0;
			for (int j = 0; j < N_Repeat; j++)
			{
				coord_gal_MW = MW(true);
				L += f_vMF(cos_theta(coord_gal_event, coord_gal_MW), events[i].kappa);
			} // j, N_Repeat
			L /= N_Repeat;

			data << i << " " << events[i].id << " " << L << std::endl;
		} // E < 900 TeV
		pbar->update(0, events.size(), i, true);
	} // i, events
	delete pbar;
	data.close();

	std::cout << "Done." << std::endl;
}

void read_L_gals()
{
	std::ifstream data("data/L_gals.txt");

	std::string line;
	std::stringstream ss;

	int i, id;
	double L;

	while (getline(data, line))
	{
		std::stringstream ss(line);
		ss >> i >> id >> L;
		L_gals[id] = L;
	} // while, data, line

	data.close();
}

double L_gal(ICEvent event, double f_gal)
{
	return f_gal * L_gals[event.id];
}

double L_exgal(double f_gal)
{
	return (1 - f_gal) / (4 * M_PI);
}

double L(ICEvent event, double f_gal)
{
	return (L_gal(event, f_gal) + L_exgal(f_gal)) * L_astro(event) + L_bkg(event);
}

double logL(double f_gal)
{
	double logL;

	logL = 0;
	for (unsigned int i = 0; i < events.size(); i++)
	{
		logL += log(L(events[i], f_gal));
	} // i, events

	return logL;
}

// for the minimizer
double minus_logL(double f_gal, void *params)
{
	return -logL(f_gal);
}

// finds the best fit f_gal
double hat_f_gal()
{
	int status;
	double minimum;
	const gsl_min_fminimizer_type *T;
	gsl_min_fminimizer *s;
	gsl_function F;

	F.function = &minus_logL;
	T = gsl_min_fminimizer_goldensection;
	s = gsl_min_fminimizer_alloc(T);

	gsl_min_fminimizer_set(s, &F, 0.1, 0, 1.);

	status = GSL_CONTINUE;
	while (status == GSL_CONTINUE)
	{
		gsl_min_fminimizer_iterate(s);
		status = gsl_min_test_interval(gsl_min_fminimizer_x_lower(s), gsl_min_fminimizer_x_upper(s), 1e-5, 0);
	} // while continuing
	minimum = gsl_min_fminimizer_x_minimum(s);

	gsl_min_fminimizer_free(s);
	return minimum;
}

