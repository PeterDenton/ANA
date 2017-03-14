#include <random>

#include "rng.h"

RNG::RNG()
{
	std::random_device rd;
	mt_.seed(rd());
}

RNG::RNG(int rng_seed)
{
	mt_.seed(rng_seed);
}

double RNG::rand()
{
	std::uniform_real_distribution<double> distribution(0, 1);
	return distribution(mt_);
}

double RNG::rand(double max)
{
	std::uniform_real_distribution<double> distribution(0, max);
	return distribution(mt_);
}

double RNG::rand(double min, double max)
{
	std::uniform_real_distribution<double> distribution(min, max);
	return distribution(mt_);
}

double RNG::gaussian(double mean, double variance)
{
	std::normal_distribution<double> distribution(mean, variance);
	return distribution(mt_);
}

int RNG::poisson(double mean)
{
	std::poisson_distribution<int> distribution(mean);
	return distribution(mt_);
}

int RNG::rand_int(int max)
{
	std::uniform_int_distribution<int> distribution(0, max);
	return distribution(mt_);
}

int RNG::rand_int(int min, int max)
{
	std::uniform_int_distribution<int> distribution(min, max);
	return distribution(mt_);
}

void RNG::shuffle(int array[], int n)
{
	for (int i = 0; i < n; i++)
		swap(&array[i], &array[this->rand_int(n - 1)]);
}

void RNG::seed(int seed)
{
	mt_.seed(seed);	
}

void swap(int* a, int* b)
{
	int tmp = *a;
	*a = *b;
	*b = tmp;
}


RNG rng;

