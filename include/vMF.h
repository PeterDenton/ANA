#ifndef vMF_H
#define vMF_H

class coord2D;

double sigma_direction_vMF_helper(double sigma_direction, void *params);
double sigma_direction_vMF(double alpha50); // in radians

double kappa_vMF_helper(double kappa, void *params);
double kappa_vMF(double sigma_direction);

double cos_theta_vMF(double sigma_direction);
coord2D vMF_smear(coord2D coord, double sigma_direction);

double f_vMF(double cos_theta, double kappa);

#endif
