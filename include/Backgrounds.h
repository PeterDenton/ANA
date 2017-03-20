/*
This code is free to use, copy, distribute, and modify.
If you use this code or any modification of this code, we request that you reference both this code zenodo.org/record/x and the paper https://arxiv.org/abs/17xx.xxxxx.
*/

#ifndef Backgrounds_H
#define Backgrounds_H

class ICEvent;

// spectral indices
extern const double Gamma_atmospherics, Gamma_astro;

double N_bkg(ICEvent event);
double N_astro(ICEvent event);
double Phi_bkg(ICEvent event);
double Phi_astro(ICEvent event);
double L_bkg(ICEvent event);
double L_astro(ICEvent event);

#endif
