/*
This code is free to use, copy, distribute, and modify.
If you use this code or any modification of this code, we request that you reference both this code https://zenodo.org/record/x and the paper https://arxiv.org/abs/17xx.xxxxx.
*/

#include <fstream>
#include <iomanip>
#include <string>
#include <cmath>
#include <iostream>
#include <algorithm>

#include "Figures.h"
#include "Likelihood.h"
#include "ICEvent.h"
#include "Backgrounds.h"
#include "Progress.h"
#include "vMF.h"
#include "MWDisks.h"

void Likelihood()
{
	std::ofstream data("data/Likelihood.txt");

	int N_Steps;
	double f_gal, f_gal_min, f_gal_max;

	N_Steps = 1e3;
	f_gal_min = 0;
	f_gal_max = 1;

	data << std::setprecision(10);
	for (int i = 0; i <= N_Steps; i++)
	{
		f_gal = f_gal_min + i * (f_gal_max - f_gal_min) / N_Steps;
		data << f_gal << " " << logL(f_gal) << std::endl;
	} // i, N_Steps
	data.close();
}

// generates a LaTeX friendly table
std::string p_to_tex(double p)
{
	char tmp[100];
	int power;
	double base;

	if (p > 1e-3)
		sprintf(tmp, "$%.2g$" , p);
	else if (p == 0)
		sprintf(tmp, "$0$");
	else
	{
		power = log10(p) - 1;
		base = p / pow(10, power);
		sprintf(tmp, "$%.2g\\e{%i}$", base, power);
	}
	return std::string(tmp);
}

std::string E_to_tex(double E)
{
	char tmp[100];
	sprintf(tmp, "$%i$", (int)E);
	return std::string(tmp);
}

bool E_sorter(ICEvent event_i, ICEvent event_j)
{
	return event_i.E > event_j.E;
}

void Likelihood_Table()
{
	std::ofstream data("data/Likelihood_Table.txt");

	double Lbkg, Lastro, Lgal, Lexgal, pbkg, pastro, pgal, pexgal, sum_gal, sum_exgal, sum_bkg, hfgal;
	std::vector<ICEvent> events_sorted = events;
	std::sort(events_sorted.begin(), events_sorted.end(), E_sorter);

	sum_gal = 0;
	sum_exgal = 0;
	sum_bkg = 0;

	hfgal = hat_f_gal();

	data << hfgal << std::endl;
	data << std::setprecision(2);
	for (unsigned int i = 0; i < events_sorted.size(); i++)
	{
		Lbkg = L_bkg(events_sorted[i]);
		Lastro = L_astro(events_sorted[i]);
		Lgal = L_gal(events_sorted[i], hfgal);
		Lexgal = L_exgal(hfgal);

		pbkg = Lbkg / (Lbkg + Lastro);
		pastro = Lastro / (Lbkg + Lastro);
		pgal = Lgal / (Lgal + Lexgal) * pastro;
		pexgal = Lexgal / (Lgal + Lexgal) * pastro;

		sum_gal += pgal;
		sum_exgal += pexgal;
		sum_bkg += pbkg;

		data << E_to_tex(events_sorted[i].E) << " & ";
		data << events_sorted[i].id << " & ";
		data << p_to_tex(pgal) << " & ";
		data << p_to_tex(pexgal) << " & ";
		data << p_to_tex(pbkg) << "\\\\" << std::endl;
	} // i, events_sorted

	data << sum_gal << " " << sum_exgal << " " << sum_bkg << std::endl;
	data.close();
}

void SkyMap()
{
	std::cout << "Generating SkyMap..." << std::endl;

	std::ofstream data("data/SkyMap.txt");
	coord2D coord_gal, coord_gal_smeared;

	int k, l; // indices for the grid
	const int N_thetas = 5e2;
	const int N_phis = 5e2;
	int N_Repeat = 1e9;
	double theta;

	data << N_thetas << " " << N_phis << " " << N_Repeat << std::endl;

	int *grid = new int[N_thetas * N_phis];

	Progress_Bar *pbar = new Progress_Bar();
	pbar->update(0);
	// populates the grid
	for (unsigned int i = 0; i < events.size(); i++)
	{
		coord_gal = eq_to_gal(events[i].coord_eq);
		for (int j = 0; j < N_Repeat; j++)
		{
			coord_gal_smeared = vMF_smear(coord_gal, events[i].sigma_direction);
			k = (N_thetas - 1) * fmod(coord_gal_smeared.theta, M_PI) / M_PI;
			l = (N_phis - 1) * fmod(11 * M_PI + coord_gal_smeared.phi, 2 * M_PI) / (2 * M_PI);
			grid[k * N_phis + l]++;

			if (j % 10000 == 0)
				pbar->update(0, events.size() * N_Repeat, i * N_Repeat + j, true);
		} // j, N_Repeat
	} // i, events
	delete pbar;

	// writes the grid to file
	for (int i = 0; i < N_thetas * N_phis; i++)
	{
		k = int(1.0 * i / (N_phis));
		theta = M_PI * k / (N_phis - 1);
		if (k == 0 or k == N_phis - 1)
			data << log(grid[i] / N_Repeat) << std::endl;
		else
			data << log(grid[i] / sin(theta) / N_Repeat) << std::endl;
	}
	data.close();

	delete[] grid;

	std::cout << "Done." << std::endl;
}

void MW_Visualization()
{
	std::ofstream data("data/MW_Visualization.txt");
	int N_Repeat;
	coord_cart coord_c;
	N_Repeat = 1e7;

	Progress_Bar *pbar = new Progress_Bar();

	pbar->update(0);
	for (int i = 0; i < N_Repeat; i++)
	{
		coord_c = sph_to_cart(sun_to_gal(MW_sph(false))); // rsq = false
		data << coord_c.x << " " << coord_c.y << " " << coord_c.z << std::endl;
		pbar->update(0, N_Repeat, i, true);
	} // i, N_Repeat
	delete pbar;
	data.close();
}

void vMF_test()
{
	std::ofstream data("data/vMF_test.txt");
	double alpha50, sigma_direction, rangle_count, cos_theta;
	int N_Repeat;

	alpha50 = 15 * M_PI / 180;
	N_Repeat = 1e5;

	sigma_direction = sigma_direction_vMF(alpha50);

	data << alpha50 << " " << sigma_direction << std::endl; // alpha50 in radians

	rangle_count = 0;
	for (int i = 0; i < N_Repeat; i++)
	{
		cos_theta = cos_theta_vMF(sigma_direction);
		if (acos(cos_theta) < alpha50)
			rangle_count++;
		data << cos_theta << std::endl;
	}
	data.close();

	std::cout << "Should be 0.5: " << 1.0 * rangle_count / N_Repeat << std::endl;
}

