#ifndef rng_H
#define rng_H

#include <random>

class RNG
{
	public:
		RNG();
		RNG(int rng_seed);

		double rand();
		double rand(double max);
		double rand(double min, double max);
		double gaussian(double mean, double variance);
		int poisson(double mean);

		int rand_int(int max);			// uniform 0 to max (inclusive)
		int rand_int(int min, int max);	// uniform min to max (inclusive)

		void shuffle(int array[], int n); // shuffle an array in place

		void seed(int rng_seed);

	private:
		std::mt19937 mt_;
};

void swap(int* a, int* b);

extern RNG rng;

#endif
