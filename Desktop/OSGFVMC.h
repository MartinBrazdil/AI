#pragma once

#include "IExactQAgent.h"

template <class Perception>
class OSGFVMC : public IExactQAgent<Perception>
{
public:
	double gamma = 0.9;
	std::array<double, 2> gamma_bounds{0, 1};

	void CollectParamValues(std::vector<double>& params) override
	{
		params.push_back(gamma);
	}
	void CollectParamBounds(std::vector<std::array<double, 2>>& bounds) override
	{
		bounds.push_back(gamma_bounds);
	}
	void DistributeParamValues(std::vector<double>& params, size_t& offset) override
	{
		gamma = params[offset++];
	}
	void DistributeParamBounds(std::vector<std::array<double, 2>>& bounds, size_t& offset) override
	{
		gamma_bounds = bounds[offset++];
	}

	OSGFVMC(size_t actions, std::tuple<> params)
		: OSGFVMC{actions}
	{}

	OSGFVMC(size_t actions) 
		: IExactQAgent{actions}
	{}

	std::vector<double> Policy(Perception& perception)
	{
		std::vector<double>& action_values = ActionValues(perception);
		std::vector<size_t>& frequencies = Frequencies(perception);
		std::vector<double> policy(action_values.size(), 0.0);

		double max_value = *(std::max_element(action_values.begin(), action_values.end()));

		auto max_or_unexplored = [&](size_t i) {
			return action_values[i] == max_value || frequencies[i] == 0;
		};

		size_t maxOrUnexploredCount = 0;
		for (size_t i = 0; i < action_values.size(); i++)
			if (max_or_unexplored(i))
				maxOrUnexploredCount++;

		for (size_t i = 0; i < action_values.size(); i++)
			if (max_or_unexplored(i))
				policy[i] = 1.0 / maxOrUnexploredCount;

		return policy;
	}

	void LearnEpisode() override
	{
		double reward_return = 0;
		std::map<Perception, std::pair<size_t, double>> epizode_perc_act_ret;
		for (auto expIt = experiences.rbegin(); expIt != experiences.rend(); expIt++)
		{
			reward_return = expIt->reward + gamma * reward_return;
			epizode_perc_act_ret[expIt->perception] = {expIt->action, reward_return};
		}
		experiences.clear();

		for (auto& par : epizode_perc_act_ret)
		{
			const Perception perception = par.first;
			size_t action = par.second.first;
			double reward_return = par.second.second;

			double& value = ActionValues(const_cast<Perception&>(perception))[action];
			size_t& frequency = Frequencies(const_cast<Perception&>(perception))[action];

			value += (reward_return - value) / ++frequency;
		}
	}
};
