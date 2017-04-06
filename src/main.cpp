/*
This code is free to use, copy, distribute, and modify.
If you use this code or any modification of this code, we request that you reference both this code https://zenodo.org/record/438675 and the paper https://arxiv.org/abs/1703.09721.
*/

#include <iostream>
#include <cmath>

#include "Likelihood.h"
#include "Figures.h"
#include "Progress.h"
#include "rng.h"

int main()
{
	// init
//	Progress_Bar_visible = false;
	rng.seed(1888);

	// calculate and write to file, and then read in the galactic likelihood part of the integral
//	calc_L_gals(); // calculate and write to file, does not need to be rerun
	read_L_gals(); // reads in from file, needs to be rerun for any likelihood calculation

	// the best fit point with 1 sigma errorbars
	double hfgal = hat_f_gal();
	printf("hat_f_gal = %.2g_{-%.2g}^{+%.2g}\n", hfgal, hfgal - sigma_to_f_gal(1, true), sigma_to_f_gal(1, false) - hfgal);
	printf("f_gal = 0 allowed at %.1f\\sigma\n", sqrt(-2 * (logL(0) - logL(hfgal))));
	printf("f_gal = 1 allowed at %.1f\\sigma\n", sqrt(-2 * (logL(1) - logL(hfgal))));

	// generate the data for figures and tables
	Likelihood();
	Likelihood_Table();
//	IC_SkyMap();
//	MW_SkyMap();
//	MW_Visualization();
	vMF_test();
	Likelihood_CLs_Table();

	return 0;
}

