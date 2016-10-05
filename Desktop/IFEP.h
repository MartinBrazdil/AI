#pragma once

#include <string>
#include <vector>
#include <array>
#include <random>
#include <chrono>
#include <algorithm>
#include <functional>
#include <string>
#include <fstream>

using Fitness = std::function<double(std::vector<double>, size_t, void*)>;

class IFEP
{
public:
	std::mt19937 rng{static_cast<unsigned int>(std::chrono::system_clock::now().time_since_epoch().count())};

	std::uniform_real_distribution<double> uniform{0, 1};
	std::normal_distribution<double> normal{0, 1};
	std::cauchy_distribution<double> cauchy{0, 1};

	const double PI = 3.141592653589793;

	size_t dimensions;
	double tau1, tau2;

	class Individual
	{
	public:
		double fitness;
		std::vector<double> x, eta;
		Individual() {}
		Individual(size_t s) { x.resize(s); eta.resize(s); };
	};

	size_t population_size, generations;
	std::vector<Individual> population;

	IFEP::IFEP(size_t population_size, size_t generations)
		: population_size(population_size), generations(generations)
	{}

	IFEP::~IFEP() {}

	void IFEP::Evolve(Fitness fitness, std::vector<std::array<double, 2>> bounds, void* constants, std::string log_file_name)
	{
		std::fstream log_file(log_file_name.c_str(), std::fstream::out);
		auto compare = [](Individual& a, Individual& b) {return a.fitness > b.fitness; };

		dimensions = bounds.size();
		tau1 = pow(sqrt(2 * dimensions), -1);
		tau2 = pow(sqrt(2 * sqrt(dimensions)), -1);

		population.resize(population_size, {dimensions});

		for (auto& individual : population)
		{
			for (int i = 0; i < dimensions; i++)
			{
				std::uniform_real_distribution<double> objective_distribution(bounds[i][0], bounds[i][1]);
				double dispersion = (bounds[i][1] - bounds[i][0]) / 5;

				individual.x[i] = objective_distribution(rng);
				individual.eta[i] = dispersion * uniform(rng);
			}
			individual.fitness = fitness(individual.x, dimensions, constants);
		}
		for (int g = 0; g < generations; g++)
		{
			for (size_t i = 0; i < population_size; i++)
			{
				Individual& parent = population[i];
				std::vector<Individual> offsprings = Reproduce(fitness, bounds, constants, parent);

				population.push_back(offsprings[0]);
				population.push_back(offsprings[1]);

				PrintIndividual(offsprings[0], log_file);
				PrintIndividual(offsprings[1], log_file);
			}
			sort(population.begin(), population.end(), compare);
			population.resize(population_size);
		}
		log_file.close();
	}

	std::vector<Individual> IFEP::Reproduce(Fitness fitness, std::vector<std::array<double, 2>> bounds, void* constants, const Individual& parent)
	{
		std::vector<Individual> offsprings(2, {dimensions});

		double N = normal(rng);
		for (int i = 0; i < dimensions; i++)
		{
			offsprings[0].x[i] = parent.x[i] + parent.eta[i] * normal(rng);
			offsprings[1].x[i] = parent.x[i] + parent.eta[i] * cauchy_quantile(uniform(rng));
			if (!IsValid(offsprings[0].x[i], offsprings[1].x[i], bounds[i])) { --i; continue; }
			offsprings[0].eta[i] = offsprings[1].eta[i] = parent.eta[i] * exp(tau1 * N + tau2 * normal(rng));
		}

		offsprings[0].fitness = fitness(offsprings[0].x, dimensions, constants);
		offsprings[1].fitness = fitness(offsprings[1].x, dimensions, constants);

		return offsprings;
	}

	bool IFEP::IsValid(double x0, double x1, std::array<double, 2> bound)
	{
		return bound[0] < x0 && x0 < bound[1] && bound[0] < x1 && x1 < bound[1];
	}

	void IFEP::PrintIndividual(const Individual& individual, std::fstream& log_file)
	{
		for (int i = 0; i < dimensions; i++)
			log_file << individual.x[i] << " ";
		log_file << individual.fitness << std::endl;
	}

	double IFEP::cauchy_quantile(double probability)
	{
		return tan(PI * (probability - 1.0 / 2.0));
	}
};
