#include "Rng.h"
#include <random>

std::mt19937 rng(228);

int getRandom()
{
	int x = rng();
	if (x < 0)
	{
		x *= -1;
	}
	return x;
}