/*
This code is free to use, copy, distribute, and modify.
If you use this code or any modification of this code, we request that you reference both this code https://zenodo.org/record/438675 and the paper https://arxiv.org/abs/17xx.xxxxx.
*/

#ifndef Coordinates_H
#define Coordinates_H

class coord_sph;

class coord2D
{
	public:
		double theta, phi;
		coord2D() {};
		coord2D(double theta_, double phi_)
		{
			theta = theta_;
			phi = phi_;
		}
		coord2D(coord_sph coord);

};

class coord_sph : public coord2D
{
	public:
		double r, theta, phi;
		coord_sph() {};
		coord_sph(double r_, double theta_, double phi_)
		{
			r = r_;
			theta = theta_;
			phi = phi_;
		}
		coord_sph(coord2D coord)
		{
			r = 1; // unit vector
			theta = coord.theta;
			phi = coord.phi;
		}

};

class coord_cart
{
	public:
		double x, y, z;
		coord_cart() {};
		coord_cart(double x_, double y_, double z_)
		{
			x = x_;
			y = y_;
			z = z_;
		}

};

// converts between the sun's reference frame and the galaxy's
coord_cart sun_to_gal(coord_cart coord);
coord_cart gal_to_sun(coord_cart coord);

coord_cart sph_to_cart(coord_sph coord);
coord_sph cart_to_sph(coord_cart coord);

coord_sph sun_to_gal(coord_sph coord);
coord_sph gal_to_sun(coord_sph coord);

// converts between galactic and equatorial (J2000) coordinates
coord2D gal_to_eq(coord2D coord);
coord2D eq_to_gal(coord2D coord);

// costheta of angle between
double cos_theta(coord2D coord1, coord2D coord2);

#endif
