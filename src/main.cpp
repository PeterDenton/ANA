/*
This code is free to use, copy, distribute, and modify.
If you use this code or any modification of this code, we request that you reference both this code https://zenodo.org/record/438675 and the paper https://arxiv.org/abs/1703.09721.
*/

#include <iostream>

#include "Likelihood.h"
#include "Figures.h"
#include "Progress.h"

int main()
{
	// init
//	Progress_Bar_visible = false;

	// calculate and write to file, and then read in the galactic likelihood part of the integral
	calc_L_gals(); // calculate and write to file, does not need to be rerun
	read_L_gals(); // reads in from file, needs to be rerun for any likelihood calculation

	// the best fit point
	std::cout << "hat_f_gal = " << hat_f_gal() << std::endl;

	// generate the data for figures and tables
	Likelihood();
	Likelihood_Table();
//	IC_SkyMap();
//	MW_SkyMap();
//	MW_Visualization();
	vMF_test();

	return 0;
}

