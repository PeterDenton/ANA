/*
This code is free to use, copy, distribute, and modify.
If you use this code or any modification of this code, we request that you reference both this code https://zenodo.org/record/438675 and the paper https://arxiv.org/abs/1703.09721.
*/

#include <gsl/gsl_min.h>
#include <gsl/gsl_roots.h>
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

	HE_Cut = false; // a cut on galactic energy at 900 TeV. Default: false
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
	double l;
	l = (L_gal(event, f_gal) + L_exgal(f_gal)) * L_astro(event) + L_bkg(event);
	if (l == 0)
		return 1e-50;
	return l;
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

// for the minimizer/root finder
double sigma_minus_2logL(double f_gal, void *params)
{
	double *p = (double *)params;
	double sigma, logL_hfgal;

	sigma = p[0];
	logL_hfgal = p[1];

	return -pow(sigma, 2) - 2 * (logL(f_gal) - logL_hfgal);
}

// finds the best fit f_gal
double hat_f_gal()
{
	int status;
	double minimum, init, F_min, F_init, F_max;
	const gsl_min_fminimizer_type *T;
	gsl_min_fminimizer *s;
	gsl_function F;
	double params[2] = {0, 0};

	init = 0.1;

	// check that there is a minimum near init
	F_min = sigma_minus_2logL(0, params);
	F_init = sigma_minus_2logL(init, params);
	F_max = sigma_minus_2logL(1, params);
	if (F_min < F_init and F_init < F_max)	return 0;
	if (F_min > F_init and F_init > F_max)	return 1;

	F.function = &sigma_minus_2logL;
	F.params = params;

	T = gsl_min_fminimizer_brent;
	s = gsl_min_fminimizer_alloc(T);

	gsl_min_fminimizer_set(s, &F, init, 0, 1.);

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

double sigma_to_f_gal(double sigma, bool lower)
{
	int status;
	double f_gal, hfgal, min, max;
	const gsl_root_fsolver_type *T;
	gsl_root_fsolver *s;
	gsl_function F;
	hfgal = hat_f_gal();
	double params[2] = {sigma, logL(hfgal)};

	if (lower)
	{
		min = 0;
		max = hfgal;
	}
	else
	{
		min = hfgal;
		max = 1;
	}

	// if they have the same sign:
	if (sigma_minus_2logL(min, params) * sigma_minus_2logL(max, params) > 0)
		return -1;

	F.function = &sigma_minus_2logL;
	F.params = params;
	T = gsl_root_fsolver_brent;
	s = gsl_root_fsolver_alloc(T);

	gsl_root_fsolver_set(s, &F, min, max);

	status = GSL_CONTINUE;
	while (status == GSL_CONTINUE)
	{
		gsl_root_fsolver_iterate(s);
		status = gsl_root_test_interval(gsl_root_fsolver_x_lower(s), gsl_root_fsolver_x_upper(s), 1e-5, 0);
	} // while continuing
	f_gal = gsl_root_fsolver_root(s);

	gsl_root_fsolver_free(s);

	return f_gal;
}

