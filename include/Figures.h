/*
This code is free to use, copy, distribute, and modify.
If you use this code or any modification of this code, we request that you reference both this code zenodo.org/record/x and the paper https://arxiv.org/abs/17xx.xxxxx.
*/

#ifndef Figures_H
#define Figures_H

#include <string>

// likelihood scan over f_gal
void Likelihood();

// generates a LaTeX friendly table
std::string p_to_tex(double p);
void Likelihood_Table();

// visualizations
void SkyMap();
void MW_Visualization();

// testing
void vMF_test();

#endif
