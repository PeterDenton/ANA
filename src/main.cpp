#include <iostream>

#include "rng.h"
#include "Likelihood.h"
#include "Figures.h"
#include "Progress.h"
#include "Test.h"

bool test = false; // run the real thing

int main()
{
	// init
	rng.seed(1888);
//	Progress_Bar_visible = false;

	calc_L_gals();
	read_L_gals();

	std::cout << "hat_f_gal = " << hat_f_gal() << std::endl;

	Likelihood();
	Likelihood_Table();
//	SkyMap();
//	MW_Visualization();
//	vMF_test();

	return 0;
}

