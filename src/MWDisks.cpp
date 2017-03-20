/*
This code is free to use, copy, distribute, and modify.
If you use this code or any modification of this code, we request that you reference both this code zenodo.org/record/x and the paper https://arxiv.org/abs/17xx.xxxxx.
*/

#include <math.h>
#include <cmath>
#include <assert.h>

#include "MWDisks.h"
#include "Coordinates.h"
#include "rng.h"

// soft disk model
// initialize the MCMC points
MCMC_point::MCMC_point(double x, double y, double z, int section)
: x(x), y(y), z(z), section(section)
{
	Calc_Phi(this);
}

// the MCMC points
MCMC_point MCMC_bulge(0, 0, 0, 0);
MCMC_point MCMC_thin(0, 0, 0, 1);
MCMC_point MCMC_thick(0, 0, 0, 2);

// 5pc minimum distance cutoff
const double d_min = 5;

// from 1102.4340
// See best fit in table 2
// in pc, solar masses
const double Sigma_d_thin = 816.6;
const double R_d_thin = 2.9e3;
const double Sigma_d_thick = 209.5;
const double R_d_thick = 3.31e3;
const double rho_b = 95.6;
const double R_0 = 8.29e3; // distance from sun to GC

// from below eq 1
const double M_b = 8.9e9;
const double alpha = 1.8;
const double r_0 = 0.075e3;
const double r_cut = 2.1e3;
const double q = 0.5;

// from below eq 3
const double M_thin = 2 * M_PI * Sigma_d_thin * pow(R_d_thin, 2);
const double M_thick = 2 * M_PI * Sigma_d_thick * pow(R_d_thick, 2);
const double z_d_thin = 300;
const double z_d_thick = 900;

void Calc_Phi(MCMC_point* p, bool rsq)
{
	double R, r_prime, d, Phi;
	R = sqrt(pow(p->x, 2) + pow(p->y, 2));
	if (rsq)
		d = sqrt(pow(p->x - R_0, 2) + pow(p->y, 2) + pow(p->z, 2)); // distance from point to earth
	else
		d = 1 + d_min; // for testing, compare to the function uncorrected by distance

	// cutoff at sources too close to the earth
	if (d < d_min)
	{
		p->Phi = 0;
		return;
	}

	switch (p->section)
	{
		case 0: // bulge
			r_prime = sqrt(pow(R, 2) + pow(p->z / q, 2));
			Phi = exp(-pow(r_prime / r_cut, 2)) / pow(1 + r_prime / r_0, alpha);
			break;
		case 1: // thin
			Phi = (Sigma_d_thin / (2 * z_d_thin)) * exp(-std::abs(p->z) / z_d_thin - R / R_d_thin);
			break;
		case 2: // thick
			Phi = (Sigma_d_thick / (2 * z_d_thick)) * exp(-std::abs(p->z) / z_d_thick - R / R_d_thick);
			break;
		default:
			Phi = 0;
	}
	p->Phi = Phi / pow(d, 2);
}

MCMC_point jump(MCMC_point p0, bool rsq)
{
	MCMC_point p1 = p0;

	switch (p0.section)
	{
		case 0: // bulge
			double sigma;
			sigma = r_0; // scale of bulge
			sigma = R_0; // earth to bulge
			switch (p0.parameter)
			{
				case 0: // x
					p1.x = rng.gaussian(p1.x, sigma);
					break;
				case 1: // y
					p1.y = rng.gaussian(p1.y, sigma);
					break;
				case 2: // z
					p1.z = rng.gaussian(p1.z, sigma);
			}
			break;
		case 1: // thin
			switch (p0.parameter)
			{
				case 0: // x
					p1.x = rng.gaussian(p0.x, R_d_thin);
					break;
				case 1: // y
					p1.y = rng.gaussian(p0.y, R_d_thin);
					break;
				case 2: // z
					p1.z = rng.gaussian(p0.z, z_d_thin);
			}
			break;
		case 2: // thick
			switch (p0.parameter)
			{
				case 0: // x
					p1.x = rng.gaussian(p0.x, R_d_thick);
					break;
				case 1: // y
					p1.y = rng.gaussian(p0.y, R_d_thick);
					break;
				case 2: // z
					p1.z = rng.gaussian(p0.z, z_d_thick);
			}
			break;
	}
	p1.section = p0.section;
	Calc_Phi(&p1, rsq);
	return p1;
}

// metropolis hastings
void MH(MCMC_point* p, bool rsq)
{
	MCMC_point jumped;

	jumped = jump(*p, rsq);
	p->parameter = (p->parameter + 1) % 3;
	if (jumped.Phi > p->Phi)
	{
		*p = jumped;
		return;
	} // new point is better than the first: automatically accept
	double r = rng.rand();
	if (r < jumped.Phi / p->Phi)
	{
		*p = jumped;
		return;
	} // new point isn't too much worse, accept anyways

	// keep the old point
	// do nothing
}

coord_sph spherical(MCMC_point p)
{
	coord_cart coord_c;

	coord_c.x = p.x;
	coord_c.y = p.y;
	coord_c.z = p.z;

	return cart_to_sph(coord_c);
}

coord_sph MW_sph(bool rsq)
{
	// determine if from bulge, thin disk, or thick disk based on the total mass in each region
	double M_rand;
	coord_sph coord_s;

 	M_rand = rng.rand(M_b + M_thin + M_thick);
	if (M_rand < M_b)
	{
		MH(&MCMC_bulge, rsq);
		coord_s = spherical(MCMC_bulge);
	} // from bulge
	else if (M_rand < M_b + M_thin)
	{
		MH(&MCMC_thin, rsq);
		coord_s = spherical(MCMC_thin);
	} // from thin disk
	else
	{
		MH(&MCMC_thick, rsq);
		coord_s = spherical(MCMC_thick);
	} // from thick disk

	coord_s = gal_to_sun(coord_s); // back to centered on the sun
	return coord_s;
}

coord2D MW(bool rsq)
{
	coord_sph coord_s = MW_sph(rsq);
	return coord2D(coord_s.theta, coord_s.phi);
}

