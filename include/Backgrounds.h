#ifndef Backgrounds_H
#define Backgrounds_H

class ICEvent;

extern const double Gamma_atmospherics, Gamma_astro;

double N_bkg(ICEvent event);
double N_astro(ICEvent event);
double Phi_bkg(ICEvent event);
double Phi_astro(ICEvent event);
double L_bkg(ICEvent event);
double L_astro(ICEvent event);

#endif