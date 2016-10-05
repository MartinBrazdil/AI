#pragma once

#include "IExactQAgent.h"

template <class Perception>
class EGEVMC : public IExactQAgent<Perception>
{
public:
	double gamma = 0.9;
	std::array<double, 2> gamma_bounds{0, 1};

	double epsilon = 0.1;
	std::array<double, 2> epsilon_bounds{0, 1};

	void CollectParamValues(std::vector<double>& params) override
	{
		params.push_back(gamma);
		params.push_back(epsilon);
	}
	void CollectParamBounds(std::vector<std::array<double, 2>>& bounds) override
	{
		bounds.push_back(gamma_bounds);
		bounds.push_back(epsilon_bounds);
	}
	void DistributeParamValues(std::vector<double>& params, size_t& offset) override
	{
		gamma = params[offset++];
		epsilon = params[offset++];
	}
	void DistributeParamBounds(std::vector<std::array<double, 2>>& bounds, size_t& offset) override
	{
		gamma_bounds = bounds[offset++];
		epsilon_bounds = bounds[offset++];
	}
	
	EGEVMC(size_t actions, std::tuple<> params)
		: EGEVMC{actions}
	{}

	EGEVMC(size_t actions)
		: IExactQAgent{actions}
	{}

	std::vector<double> Policy(Perception& perception) override
	{
		std::vector<double>& action_values = ActionValues(perception);
		std::vector<double> policy(action_values.size(), 0.0);
		auto max = std::max_element(action_values.begin(), action_values.end());
		auto maxPredicate = [max](auto x)->bool { return x == (*max); };
		size_t maxCount = std::count_if(action_values.begin(), action_values.end(), maxPredicate);
		size_t remainingCount = action_values.size() - maxCount;
		for (size_t i = 0; i < action_values.size(); i++)
		{
			if (maxPredicate(action_values[i]))
				policy[i] = (1 - epsilon) / maxCount;
			else // other than max
				policy[i] = epsilon / remainingCount;
		}
		return policy;
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
