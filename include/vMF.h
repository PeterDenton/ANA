/*
This code is free to use, copy, distribute, and modify.
If you use this code or any modification of this code, we request that you reference both this code https://zenodo.org/record/438675 and the paper https://arxiv.org/abs/1703.09721.
*/

#ifndef vMF_H
#define vMF_H

class coord2D;

double sigma_direction_vMF_helper(double sigma_direction, void *params);
// alpha50 in rad
// returns sigma
double sigma_direction_vMF(double alpha50);

double kappa_vMF_helper(double kappa, void *params);
// returns kappa, the concentration
double kappa_vMF(double sigma_direction);

// returns a random angle (costheta) distributed according to a given sigma value
double cos_theta_vMF(double sigma_direction);
// generates a random direction spread around a given direction, with a given sigma
coord2D vMF_smear(coord2D coord, double sigma_direction);

// the pdf
double f_vMF(double cos_theta, double kappa);

#endif
