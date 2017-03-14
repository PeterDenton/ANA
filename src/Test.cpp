#include "Test.h"
#include "Likelihood.h"
#include "Progress.h"

bool test = true; // singify that this is the test, cut down on N_Repeat

int main()
{
	Progress_Bar_visible = false;

	calc_L_gals();

	return 0;
}

