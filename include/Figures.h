/*
This code is free to use, copy, distribute, and modify.
If you use this code or any modification of this code, we request that you reference both this code https://zenodo.org/record/x and the paper https://arxiv.org/abs/17xx.xxxxx.
*/

#ifndef Figures_H
#define Figures_H

#include <string>

class ICEvent;

// likelihood scan over f_gal
void Likelihood();

// generates a LaTeX friendly table
std::string p_to_tex(double p);
std::string E_to_tex(double E);
bool E_sorter(ICEvent event_i, ICEvent event_j);
void Likelihood_Table();

// visualizations
void IC_SkyMap();
void MW_SkyMap();

// testing
void vMF_test();

#endif
