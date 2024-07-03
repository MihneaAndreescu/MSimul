#include "Rng.h"
#include <random>

std::mt19937 rng(228);

int getRandom()
{
	return rng();
}