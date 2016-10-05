#pragma once

#include <random>
#include <ctime>

extern std::mt19937 rng;
extern std::uniform_real_distribution<double> uniform;
extern std::normal_distribution<double> normal;

double Normal(double mu = 0, double sigma = 1);
double Uniform(double low = 0, double high = 1);
