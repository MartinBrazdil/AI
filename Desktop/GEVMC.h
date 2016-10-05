#pragma once

#include "IExactQAgent.h"

#include <vector>
#include <array>

template <class Perception>
class GEVMC : public IExactQAgent<Perception>
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

	GEVMC(size_t actions, std::tuple<> params)
		: GEVMC{actions}
	{}

	GEVMC(size_t actions)
		: IExactQAgent{actions}
	{}

	std::vector<double> Policy(Perception& perception)
	{
		return IAgent<Perception>::PolicyGreedy(perception);
	}

	void LearnEpisode() override
	{
		double reward_return = 0;
		for (auto exp_it = experiences.rbegin(); exp_it != experiences.rend(); exp_it++)
		{
			const Perception& perception = exp_it->perception;
			size_t action = exp_it->action;

			double& value = ActionValues(const_cast<Perception&>(perception))[action];
			size_t& frequency = Frequencies(const_cast<Perception&>(perception))[action];

			reward_return = exp_it->reward + gamma * reward_return;
			value += (reward_return - value) / ++frequency;
		}
		experiences.clear();
	}
};
