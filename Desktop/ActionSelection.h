#pragma once

#include "HyperparameterEvolution.h"
#include "EpizodicLearning.h"

#include "RU.h"
#include "EGEVMC.h"
#include "EGFVMC.h"
#include "GEVMC.h"
#include "GFVMC.h"
#include "OSEGEVMC.h"
#include "OSEGFVMC.h"
#include "OSGEVMC.h"
#include "OSGFVMC.h"

#include <fstream>
#include <string>

template <template<class> class Agent, class World>
void EvolveAndEvaluate(World& _world, size_t population, size_t generations, size_t epizodes, size_t limit, size_t averaging, std::string log_file_path, std::string name)
{
	std::cout << std::string("\n=== ") + name + std::string(" Agent ===\n") << std::endl;

	using State = World::state_type;
	using Perception = World::perception_type;

	IWorld<Perception, State>& world = _world;

	std::string evolution_file_name = log_file_path + name + std::string("_evolution.log");
	std::string evaluation_file_name = log_file_path + name + std::string("_evaluation.log");

	IFEP evolution = HyperparameterEvolution<Agent<Perception>>(world, population, generations, epizodes, limit, std::tuple<>(), evolution_file_name);

	for (size_t i = 0; i < averaging; i++)
	{
		Agent<Perception> agent(world.Actions());

		size_t offset = 0;
		agent.DistributeParamValues(evolution.population[0].x, offset);

		EpizodicLearning(world, agent, epizodes, limit, evaluation_file_name, std::ios::app);
	}
}

template <class World>
void ActionSelection(World& world, size_t population, size_t generations, size_t epizodes, size_t limit, size_t averaging, std::string log_file_dir)
{
	EvolveAndEvaluate<RU>(world, population, generations, epizodes, limit, averaging, log_file_dir, "ru");
	EvolveAndEvaluate<EGEVMC>(world, population, generations, epizodes, limit, averaging, log_file_dir, "egevmc");
	EvolveAndEvaluate<EGFVMC>(world, population, generations, epizodes, limit, averaging, log_file_dir, "egfvmc");
	EvolveAndEvaluate<GEVMC>(world, population, generations, epizodes, limit, averaging, log_file_dir, "gevmc");
	EvolveAndEvaluate<GFVMC>(world, population, generations, epizodes, limit, averaging, log_file_dir, "gfvmc");
	EvolveAndEvaluate<OSEGEVMC>(world, population, generations, epizodes, limit, averaging, log_file_dir, "osegevmc");
	EvolveAndEvaluate<OSEGFVMC>(world, population, generations, epizodes, limit, averaging, log_file_dir, "osegfvmc");
	EvolveAndEvaluate<OSGEVMC>(world, population, generations, epizodes, limit, averaging, log_file_dir, "osgevmc");
	EvolveAndEvaluate<OSGFVMC>(world, population, generations, epizodes, limit, averaging, log_file_dir, "osgfvmc");
}
