/*
This code is free to use, copy, distribute, and modify.
If you use this code or any modification of this code, we request that you reference both this code https://zenodo.org/record/x and the paper https://arxiv.org/abs/17xx.xxxxx.
*/

#ifndef MWDisks_H
#define MWDisks_H

#include <cmath>

#include "Coordinates.h"

// soft disk
class MCMC_point
{
	public:
		MCMC_point() {};
		MCMC_point(double x, double y, double z, int section);
		double x, y, z;
		double Phi;
		int section; // 0: bulge, 1: thin, 2: thick
		int parameter; // which parameter to MH next, 0: x, 1: y, 2: z
};

extern MCMC_point MCMC_bulge, MCMC_thick, MCMC_thin;

extern const double d_min;
extern const double Sigma_d_thin, R_d_thin, Sigma_d_thick, R_d_thick, rho_b, R_0, M_b, alpha, r_0, r_cut, q, M_thin, M_thick, z_d_thin, z_d_thick;

// rsq: should it include the 1/r^2 part
void Calc_Phi(MCMC_point* p, bool rsq = true);
MCMC_point jump(MCMC_point p0, bool rsq = true);
// metropolis hastings
void MH(MCMC_point* p, bool rsq = true);
coord_sph MW_sph(bool rsq = true);
coord2D MW(bool rsq = true);

#endif
