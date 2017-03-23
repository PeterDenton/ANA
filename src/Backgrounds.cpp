/*
This code is free to use, copy, distribute, and modify.
If you use this code or any modification of this code, we request that you reference both this code zenodo.org/record/x and the paper https://arxiv.org/abs/17xx.xxxxx.
*/

#include <cmath>

#include "Backgrounds.h"
#include "ICEvent.h"

// spectral indices
const double Gamma_atmospherics = 3.7;
const double Gamma_astro = 2.58;

double N_bkg(ICEvent event)
{
	double x_mu, phi_mu_3, x_atm, phi_atm_3, sin_dec, scale;
	if (event.is_shower)
	{
		x_mu = 0.1;
		x_atm = 0.29;
	}
	else
	{
		x_mu = 0.9;
		x_atm = 0.71;
	}

	sin_dec = sin(M_PI / 2 - event.coord_eq.theta);
	scale = 1347. / 988; // converts three years to four

	// digitized from 1405.5303, supplementary figure 5, E_dep>60 TeV
	if (sin_dec < -0.8)			{phi_mu_3 = 0;		phi_atm_3 = 0;}
	else if (sin_dec < -0.6)	{phi_mu_3 = 0.039;	phi_atm_3 = 0;}
	else if (sin_dec < -0.4)	{phi_mu_3 = 0.057;	phi_atm_3 = 0.057;}
	else if (sin_dec < -0.2)	{phi_mu_3 = 0.032;	phi_atm_3 = 0.18;}
	else if (sin_dec < 0.)		{phi_mu_3 = 0;		phi_atm_3 = 0.44;}
	else if (sin_dec < 0.2)		{phi_mu_3 = 0;		phi_atm_3 = 0.57;}
	else if (sin_dec < 0.4)		{phi_mu_3 = 0;		phi_atm_3 = 0.28;}
	else if (sin_dec < 0.6)		{phi_mu_3 = 0;		phi_atm_3 = 0.15;}
	else if (sin_dec < 0.8)		{phi_mu_3 = 0;		phi_atm_3 = 0.11;}
	else						{phi_mu_3 = 0;		phi_atm_3 = 0.04;}

	return (x_mu * phi_mu_3 + x_atm * phi_atm_3) * scale;
}

double N_astro(ICEvent event)
{
	double sin_dec, scale, phi_astro_3;

	sin_dec = sin(M_PI / 2 - event.coord_eq.theta);
	scale = 1347. / 988; // converts three years to four

	// digitized from 1405.5303, supplementary figure 5, E_dep>60 TeV
	if (sin_dec < -0.8)			{phi_astro_3 = 2.828;}
	else if (sin_dec < -0.6)	{phi_astro_3 = 2.605;}
	else if (sin_dec < -0.4)	{phi_astro_3 = 2.727;}
	else if (sin_dec < -0.2)	{phi_astro_3 = 2.611;}
	else if (sin_dec < 0.)		{phi_astro_3 = 2.034;}
	else if (sin_dec < 0.2)		{phi_astro_3 = 2.105;}
	else if (sin_dec < 0.4)		{phi_astro_3 = 1.886;}
	else if (sin_dec < 0.6)		{phi_astro_3 = 1.459;}
	else if (sin_dec < 0.8)		{phi_astro_3 = 1.290;}
	else						{phi_astro_3 = 0.713;}

	return phi_astro_3 * scale;
}

double Phi_bkg(ICEvent event)
{
	return ((Gamma_atmospherics - 1) / ICEmin) * pow(event.Edep / ICEmin, -Gamma_atmospherics);
}

double Phi_astro(ICEvent event)
{
	// Phi_astro1 is the unbroken power law
	// Phi_astro2 is the broken power law
	return Phi_astro1(event);
}

double Phi_astro1(ICEvent event)
{
	return ((Gamma_astro - 1) / ICEmin) * pow(event.Edep / ICEmin, -Gamma_astro);
}

double Phi_astro2(ICEvent event)
{
	// see eq 27 from 1611.07905 (and eq 12)
	// the fit for eq 27 uses the glashow, showers, and tracks
	// E0 is from after eq 7
	double gamma1, gamma2, s, E0, N, Ninv;
/*
	// eq 14
	gamma1 = 3.63;
	gamma2 = 2.43;
	s = 0.0827;

	// eq 19
	gamma1 = 3.47;
	gamma2 = 2.62;
	s = 0.17;
*/
	// eq 27
	gamma1 = 3.5;
	gamma2 = 2.3;
	s = 0.04;

	E0 = 33.11;

	// normalization
	Ninv = ICEmin * (pow(ICEmin / E0, -gamma1) / (gamma1 - 1) + s * pow(ICEmin / E0, -gamma2) / (gamma2 - 1));
	N = 1. / Ninv;

	return N * (pow(event.E / E0, -gamma1) + s * pow(event.E / E0, -gamma2));
}

double L_bkg(ICEvent event)
{
	if (event.id == 28 or event.id == 32)
		return 1;
	if (event.id == 45)
		return 0;

	double f_bkg;

	f_bkg = N_bkg(event) / (N_bkg(event) + N_astro(event));

	return f_bkg * Phi_bkg(event);
}

double L_astro(ICEvent event)
{
	if (event.id == 28 or event.id == 32)
		return 0;
	if (event.id == 45)
		return 1;

	double f_astro;

	f_astro = N_astro(event) / (N_bkg(event) + N_astro(event));
	return f_astro * Phi_astro(event);
}

