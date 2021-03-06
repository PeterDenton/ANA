/*
This code is free to use, copy, distribute, and modify.
If you use this code or any modification of this code, we request that you reference both this code https://zenodo.org/record/438675 and the paper https://arxiv.org/abs/1703.09721.
*/

#ifndef Likelihood_H
#define Likelihood_H

class ICEvent;

extern double L_gals[54];
void calc_L_gals();
void read_L_gals();

double L_gal(ICEvent event, double f_gal);
double L_exgal(double f_gal);

double L(ICEvent event, double f_gal);
double logL(double f_gal);

// for the minimizer/root finder
double sigma_minus_2logL(double f_gal, void *params);
// finds the best fit f_gal
double hat_f_gal();
// finds the f_gals for a given sigma, lower => starting at f_gal=0 or at f_gal=1
double sigma_to_f_gal(double sigma, bool lower);

#endif
