#pragma once

#include "IFEP.h"

#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <random>
#include <chrono>
#include <algorithm>

void MetaLearningAckley()
{
	printf("\n=== Metalearning Ackley ===\n");

	std::mt19937 rng{static_cast<unsigned int>(std::chrono::system_clock::now().time_since_epoch().count())};

	auto ackley = [](std::vector<double> x, size_t n, void* data)
	{
		const double pi = 3.141592653589793;
		const double e = 2.718281828459045;

		double firstSum = 0, secondSum = 0;
		for (size_t i = 0; i < n; i++)
		{
			firstSum += x[i] * x[i];
			secondSum += cos(2 * pi * x[i]);
		}
		return -20.0 * exp(-0.2 * sqrt(firstSum / n)) - exp(secondSum / n) + 20 + e;
	};


	// 35 samples equal 3 generations with 5 individuals
	const size_t samples = 35;
	const size_t generations = 3;
	const size_t population = 5;

	const size_t repetitions = 10000;
	std::vector<std::array<double, 2>> bounds = {{-1, 1},{-1,1}};


	// EVOLUTION SEARCH
	double fitness = 0;
	std::ofstream evolution_search_log("metalearning_ackley/evolution_search.log");
	std::ofstream dummy_log("metalearning_ackley/dummy_log.log");
	for (size_t r = 1; r < repetitions; r++)
	{
		IFEP evolution(population, generations);
		evolution.Evolve(ackley, bounds, nullptr, "");
		evolution_search_log << evolution.population[0].fitness << std::endl;
		fitness += (evolution.population[0].fitness - fitness) / r;
	}
	printf("Evolution fitness: %f\n", fitness);


	// RANDOM SEARCH
	fitness = 0;
	std::ofstream random_search_log("metalearning_ackley/random_search.log");
	for (size_t r = 1; r < repetitions; r++)
	{
		double max_fitness = 0;
		std::uniform_real<double> uniform0(bounds[0][0], bounds[0][1]);
		std::uniform_real<double> uniform1(bounds[1][0], bounds[1][1]);
		for (size_t i = 0; i < samples; i++)
		{
			std::vector<double> x = {uniform0(rng), uniform1(rng)};
			max_fitness = std::max(max_fitness, ackley(x, 2, nullptr));
		}
		random_search_log << max_fitness << std::endl;
		fitness += (max_fitness - fitness) / r;
	}
	printf("Random fitness: %f\n", fitness);


	// GRID SEARCH
	fitness = 0;
	std::ofstream grid_search_log("metalearning_ackley/grid_search.log");
	for (size_t r = 1; r < repetitions; r++)
	{
		double max_fitness = 0;
		double step0 = (bounds[0][1] - bounds[0][0]) / sqrt(samples);
		double step1 = (bounds[1][1] - bounds[1][0]) / sqrt(samples);
		for (double i = bounds[0][0]; i <= bounds[0][1]; i += step0)
		{
			for (double j = bounds[1][0]; j <= bounds[1][1]; j += step1)
			{
				std::vector<double> x = {i, j};
				max_fitness = std::max(max_fitness, ackley(x, 2, nullptr));
			}
		}
		grid_search_log << max_fitness << std::endl;
		fitness += (max_fitness - fitness) / r;
	}
	printf("Grid fitness: %f\n", fitness);
}
