#include <cmath>

#include <iostream>

#include "Coordinates.h"
#include "rng.h"
#include "MWDisks.h"
#include "Matrix.h"
#include "rng.h"

coord2D::coord2D(coord_sph coord)
{
	theta = coord.theta;
	phi = coord.phi;
}

coord2D random_coord2D()
{
	double x, theta, phi;

	phi = 2 * M_PI * rng.rand();
	x = rng.rand(-1, 1);
	theta = acos(x);

	return coord2D(theta, phi);
}

coord_cart sun_to_gal(coord_cart coord)
{
	coord.x -= R_0;

	return coord;
}

coord_cart gal_to_sun(coord_cart coord)
{
	coord.x += R_0;

	return coord;
}

coord_cart sph_to_cart(coord_sph coord)
{
	double x, y, z;

	x = coord.r * sin(coord.theta) * cos(coord.phi);
	y = coord.r * sin(coord.theta) * sin(coord.phi);
	z = coord.r * cos(coord.theta);

	return coord_cart(x, y, z);
}

coord_sph cart_to_sph(coord_cart coord)
{
	double r, theta, phi;

	r = sqrt(pow(coord.x, 2) + pow(coord.y, 2) + pow(coord.z, 2));
	if (r == 0)
		theta = M_PI / 2; // arbitrary choice
	else
		theta = acos(coord.z / r);
	phi = atan2(coord.y, coord.x);

	return coord_sph(r, theta, phi);
}

coord_sph sun_to_gal(coord_sph coord)
{
	coord_cart coord_tmp = sph_to_cart(coord); // in the sun's frame
	coord_tmp = sun_to_gal(coord_tmp); // in the gal's frame
	return cart_to_sph(coord_tmp);
}

coord_sph gal_to_sun(coord_sph coord)
{
	coord_cart coord_tmp = sph_to_cart(coord); // in the gal's frame
	coord_tmp = gal_to_sun(coord_tmp); // in the sun's frame
	return cart_to_sph(coord_tmp);
}

// from https://www.rssd.esa.int/SA/HIPPARCOS/docs/vol1_all.pdf, eq 1.5.11
double Ag[3][3] = {
	{-0.0548755604, +0.4941094279, -0.8676661490},
	{-0.8734370902, -0.4448296300, -0.1980763734},
	{-0.4838350155, +0.7469822445, +0.4559837762}
};
// calculated from python, inverse of above
double Aginv[3][3] = {
	{-0.05487556, -0.87343709, -0.48383502},
	{+0.49410943, -0.44482963, +0.74698224},
	{-0.86766615, -0.19807637, +0.45598378}
};

// hipparcos, fairly accurate
coord2D gal_to_eq(coord2D coord)
{
	double v_gal[3], v_eq[3];
	coord_cart coord_c = sph_to_cart(coord_sph(coord));
	v_gal[0] = coord_c.x;
	v_gal[1] = coord_c.y;
	v_gal[2] = coord_c.z;
	vector_dot_matrix(Aginv, v_gal, v_eq);
	coord_c.x = v_eq[0];
	coord_c.y = v_eq[1];
	coord_c.z = v_eq[2];
	return coord2D(cart_to_sph(coord_c));
}

coord2D eq_to_gal(coord2D coord)
{
	double v_gal[3], v_eq[3];
	coord_cart coord_c = sph_to_cart(coord_sph(coord));
	v_eq[0] = coord_c.x;
	v_eq[1] = coord_c.y;
	v_eq[2] = coord_c.z;
	vector_dot_matrix(Ag, v_eq, v_gal);
	coord_c.x = v_gal[0];
	coord_c.y = v_gal[1];
	coord_c.z = v_gal[2];
	return coord2D(cart_to_sph(coord_c));
}

// sigtheta in degrees (from IC)
// blah TODO change from gaussian to von mises
coord2D smear(coord2D coord, double sigtheta)
{
	// see Berlind's makeCRmock.c
	double stheta1, ctheta1, rangle, sr, cr, psi, cpsi, ctheta2, theta2, stheta2, cDphi, Dphi, phi2;

	stheta1 = sin(coord.theta);
	ctheta1 = cos(coord.theta);

	rangle = (M_PI / 180) * rng.gaussian(0, sigtheta);
	if (rangle < 0)
		rangle *= -1;

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

double cos_theta(coord2D coord1, coord2D coord2)
{
	return cos(coord1.theta) * cos(coord2.theta) + cos(coord1.phi - coord2.phi) * sin(coord1.theta) * sin(coord2.theta);
}
