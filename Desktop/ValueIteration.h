#pragma once

#include "Utilities.h"

#include <fstream>
#include <string>

template <class World>
static void ValueIteration(World& world, double gamma, double epsilon, size_t limit, std::string log_file_name = "")
{
	//printf("\n=== Value Iteration ===\n");
	std::ofstream log_file(log_file_name.c_str());

	using State = World::state_type;
	using Perception = World::perception_type;

	std::set<State> all_states = world.AllStates();

	std::map<Perception, double> state_values;

	size_t counter = 0;
	double max_delta;
	do {
		max_delta = 0;
		for (auto state : all_states)
		{
			Perception perception = world.Observation(state);

			std::vector<double> state_value_buffer;
			for (size_t action = 0; action < world.Actions(); action++)
			{
				State next_state(state);
				double reward = world.Transition(next_state, action);
				Perception next_perception = world.Observation(next_state);

				double state_value = reward + gamma * state_values[next_perception];
				state_value_buffer.push_back(state_value);
			}
			auto max_value = *std::max_element(state_value_buffer.begin(), state_value_buffer.end());
			max_delta = std::max(max_delta, std::abs(max_value - state_values[perception]));
			state_values[perception] = max_value;
		}
		printf("Max delta: %f\n", max_delta);
		log_file << counter++ << " " << max_delta << std::endl;
	} while (epsilon < max_delta && 0 < --limit);
}
