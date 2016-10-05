#pragma once

#include "Utilities.h"
#include "IFEP.h"
#include "EpizodicLearning.h"

#include <map>
#include <stack>
#include <vector>
#include <functional>
#include <fstream>
#include <string>

template <class Agent, class World, class... AgentParams>
IFEP HyperparameterEvolution(World& _world, size_t population, size_t generations,
	size_t epizodes, size_t limit, std::tuple<AgentParams...> agent_params, std::string log_file_name = "", std::ios_base::openmode mode = std::ios_base::out)
{
	//printf("\n=== Hyperparameter Evolution ===\n");
	std::ofstream log_file(log_file_name.c_str());

	using State = World::state_type;
	using Perception = World::perception_type;

	IWorld<Perception, State>& world = _world;
	IAgent<Perception>& agent = Agent(world.Actions(), agent_params);

	auto Fitness = [&](std::vector<double> params, size_t N, void* constants)
	{
		Agent agent(world.Actions(), agent_params);

		size_t offset = 0;
		agent.DistributeParamValues(params, offset);

		double fitness = EpizodicLearning(world, agent, epizodes, limit, "");

		for (auto param : params)
			log_file << param << " ";
		log_file << fitness << std::endl;

		return fitness;
	};

	std::vector<std::array<double, 2>> bounds;
	agent.CollectParamBounds(bounds);

	IFEP evolution(population, generations);
	evolution.Evolve(Fitness, bounds, nullptr, "");

	log_file.close();

	return evolution;
}
