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

// for the minimizer
double minus_logL(double f_gal);
double hat_f_gal();

#endif
