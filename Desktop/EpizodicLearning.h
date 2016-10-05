#pragma once

#include "IWorld.h"
#include "IAgent.h"
#include "Utilities.h"

#include <fstream>
#include <string>

template <class Agent, class World>
double EpizodicLearning(World& _world, Agent& _agent, size_t epizodes, size_t limit, std::string log_file_name = "", std::ios_base::openmode mode = std::ios_base::out)
{
	std::ofstream log_file(log_file_name.c_str(), mode);

	using State = World::state_type;
	using Perception = World::perception_type;

	IWorld<Perception, State>& world = _world;
	IAgent<Perception>& agent = _agent;

	size_t counter = 0;
	double total_reward = 0;
	for (size_t e = 0; e < epizodes; e++)
	{
		double epizode_reward = 0;
		State state = world.RandomState();
		for (size_t i = 0; i < limit && !world.Terminal(state); i++)
		{
			Perception perception = world.Observation(state);
			size_t chosen_action = agent.Choose(perception);
			double reward = world.Transition(state, chosen_action);
			agent.Feedback(std::move(perception), chosen_action, reward);
			epizode_reward += reward;
		}
		total_reward += epizode_reward;
		agent.LearnEpisode();

		log_file << counter++ << " " << epizode_reward << std::endl;

		//printf("Epizode Reward: %f\n", epizode_reward);
		//printf("Total Reward: %f\n", total_reward);
	}
	log_file.close();
	return total_reward;
}
