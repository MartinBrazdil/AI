#include "Random.h"

#include <random>
#include <ctime>
using namespace std;

std::mt19937 rng = std::mt19937((int)time(NULL));
std::uniform_real_distribution<double> uniform = std::uniform_real_distribution<double>(0, 1);
std::normal_distribution<double> normal = std::normal_distribution<double>(0, 1);

double Normal(double mu /*= 0*/, double sigma /*= 1*/)
{
	return mu + sigma * normal(rng);
}

double Uniform(double low /*= 0*/, double high /*= 1*/)
{
	return low + uniform(rng) * (high - low);
}
