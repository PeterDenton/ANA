#ifndef Figures_H
#define Figures_H

// likelihood scan over f_gal
void Likelihood();

// generates a latex friendly table
std::string p_to_tex(double p);
void Likelihood_Table();

// visualizations
void SkyMap();
void MW_Visualization();

// testing
void vMF_test();

#endif
