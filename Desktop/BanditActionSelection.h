#pragma once

#include "KBandit.h"
#include "HyperparameterEvolution.h"
#include "GreedyEvaluation.h"

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
void EvolveAndEvaluate(World& _world, size_t epizodes, size_t averaging, std::string name)
{
	std::cout << std::string("\n=== Bandit ") + name + std::string(" Agent ===\n") << std::endl;

	using State = World::state_type;
	using Perception = World::perception_type;

	IWorld<Perception, State>& world = _world;

	std::string evolution_file_name = std::string("bandit_action_selection/") + name + std::string("_evolution.log");
	std::string evaluation_file_name = std::string("bandit_action_selection/") + name + std::string("_evaluation.log");

	IFEP evolution = HyperparameterEvolution<Agent<DummyState>>(world, 10, 10, epizodes, 1, std::tuple<>(), evolution_file_name);

	for (size_t i = 0; i < averaging; i++)
	{
		Agent<DummyState> agent(world.Actions());

		size_t offset = 0;
		agent.DistributeParamValues(evolution.population[0].x, offset);

		EpizodicLearning(world, agent, epizodes, 1, evaluation_file_name, std::ios::app);
	}
}

void BanditActionSelection()
{
	size_t epizodes = 100;
	size_t averaging = 1000;

	KBandit kbandit(10);

	EvolveAndEvaluate<RU>(kbandit, epizodes, averaging, "ru");
	EvolveAndEvaluate<EGEVMC>(kbandit, epizodes, averaging, "egevmc");
	EvolveAndEvaluate<EGFVMC>(kbandit, epizodes, averaging, "egfvmc");
	EvolveAndEvaluate<GEVMC>(kbandit, epizodes, averaging, "gevmc");
	EvolveAndEvaluate<GFVMC>(kbandit, epizodes, averaging, "gfvmc");
	EvolveAndEvaluate<OSEGEVMC>(kbandit, epizodes, averaging, "osegevmc");
	EvolveAndEvaluate<OSEGFVMC>(kbandit, epizodes, averaging, "osegfvmc");
	EvolveAndEvaluate<OSGEVMC>(kbandit, epizodes, averaging, "osgevmc");
	EvolveAndEvaluate<OSGFVMC>(kbandit, epizodes, averaging, "osgfvmc");
}
