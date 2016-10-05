#pragma once

#include "IExactQAgent.h"

#include <map>

template <class Perception>
class GFVMC : public IExactQAgent<Perception>
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

	GFVMC(size_t actions, std::tuple<> params)
		: GFVMC{actions}
	{}

	GFVMC(size_t actions)
		: IExactQAgent{actions}
	{}

	std::vector<double> Policy(Perception& perception)
	{
		return IAgent<Perception>::PolicyGreedy(perception);
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
