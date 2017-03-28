/*
This code is free to use, copy, distribute, and modify.
If you use this code or any modification of this code, we request that you reference both this code https://zenodo.org/record/438675 and the paper https://arxiv.org/abs/17xx.xxxxx.
*/

#include <gsl/gsl_roots.h>
#include <gsl/gsl_errno.h>

#include <cmath>

#include "vMF.h"
#include "rng.h"
#include "Coordinates.h"

double sigma_direction_vMF_helper(double sigma_direction, void *params)
{
	double *alpha50 = (double*) params;
	return 1 + pow(sigma_direction, 2) * log(1 - 0.5 * (1 - exp(-2 / pow(sigma_direction, 2)))) - cos(*alpha50);
}

// alpha50 in rad
// returns sigma
double sigma_direction_vMF(double alpha50)
{
	const gsl_root_fsolver_type *T;
	gsl_root_fsolver *s;
	gsl_function F;
	F.function = &sigma_direction_vMF_helper;
	F.params = &alpha50;
	T = gsl_root_fsolver_brent;
	s = gsl_root_fsolver_alloc(T);
	double x_lo, x_hi;
	x_lo = 1e-3;
	x_hi = 100;
	gsl_root_fsolver_set(s, &F, x_lo, x_hi);

	int status = GSL_CONTINUE;
	double r;
	while (status == GSL_CONTINUE)
	{
		gsl_root_fsolver_iterate(s);
		r = gsl_root_fsolver_root(s);
		x_lo = gsl_root_fsolver_x_lower(s);
		x_hi = gsl_root_fsolver_x_upper(s);
		status = gsl_root_test_interval(x_lo, x_hi, 0, 1e-3);
	}
	gsl_root_fsolver_free(s);
	return r;
}

double kappa_vMF_helper(double kappa, void *params)
{
	double *sigma_direction = (double*) params;
	return 1 - kappa * pow(*sigma_direction, 2) - exp(-2 * kappa);
}

// returns kappa, the concentration
double kappa_vMF(double sigma_direction)
{
	const gsl_root_fsolver_type *T;
	gsl_root_fsolver *s;
	gsl_function F;
	F.function = &kappa_vMF_helper;
	F.params = &sigma_direction;
	T = gsl_root_fsolver_brent;
	s = gsl_root_fsolver_alloc(T);
	double x_lo, x_hi;
	x_lo = 0.1 / pow(sigma_direction, 2);
	x_hi = 10 / pow(sigma_direction, 2);
	gsl_root_fsolver_set(s, &F, x_lo, x_hi);

	int status = GSL_CONTINUE;
	double r;
	while (status == GSL_CONTINUE)
	{
		gsl_root_fsolver_iterate(s);
		r = gsl_root_fsolver_root(s);
		x_lo = gsl_root_fsolver_x_lower(s);
		x_hi = gsl_root_fsolver_x_upper(s);
		status = gsl_root_test_interval(x_lo, x_hi, 0, 1e-3);
	}
	gsl_root_fsolver_free(s);
	return r;
}

// returns a random angle (costheta) distributed according to a given sigma value
double cos_theta_vMF(double sigma_direction)
{
	double u;
	u = rng.rand();
	return 1 + pow(sigma_direction, 2) * log(1 - u * (1 - exp(-2 / pow(sigma_direction, 2))));
}

// generates a random direction spread around a given direction, with a given sigma
coord2D vMF_smear(coord2D coord, double sigma_direction)
{
	double stheta1, ctheta1, rangle, sr, cr, psi, cpsi, ctheta2, theta2, stheta2, cDphi, Dphi, phi2;
	rangle = acos(cos_theta_vMF(sigma_direction));

	stheta1 = sin(coord.theta);
	ctheta1 = cos(coord.theta);

	sr = sin(rangle);
	cr = cos(rangle);

	psi = 2 * M_PI * rng.rand();
	cpsi = cos(psi);

	ctheta2 = ctheta1 * cr + stheta1 * sr * cpsi;
	theta2 = acos(ctheta2);
	stheta2 = sin(theta2);

	cDphi = (cr - ctheta1 * ctheta2) / (stheta1 * stheta2);
	cDphi = (cDphi > 1) ? 1 : cDphi;
	cDphi = (cDphi < -1) ? -1 : cDphi;
	if (psi > 0)
		Dphi = acos(cDphi);
	else
		Dphi = 0;
	if (psi < M_PI)
		phi2 = coord.phi + Dphi;
	else
		phi2 = coord.phi - Dphi;
	return coord2D(theta2, phi2);
}

// the pdf
double f_vMF(double cos_theta, double kappa)
{
	if (kappa > 10) // for numerical stability
		return exp(kappa * (cos_theta - 1)) * kappa / (2 * M_PI);
	return exp(kappa * cos_theta) * kappa / (4 * M_PI * sinh(kappa));
}

