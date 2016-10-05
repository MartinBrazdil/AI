#pragma once

#include <map>

#include "IAgent.h"

template <class Perception>
class IExactQAgent : public IAgent<Perception>
{
public:
	std::map<Perception, std::vector<double>> state_action_values;
	std::map<Perception, std::vector<size_t>> state_action_frequencies;

	IExactQAgent(size_t actions) : IAgent{actions} {}

	std::vector<double>& ActionValues(Perception& perception) override
	{
		auto& action_values = state_action_values[perception];
		if (action_values.empty())
			action_values.resize(actions);
		return action_values;
	}

	std::vector<size_t>& Frequencies(const Perception& perception)
	{
		auto& frequencies = state_action_frequencies[perception];
		if (frequencies.empty())
			frequencies.resize(actions);
		return frequencies;
	}
};
