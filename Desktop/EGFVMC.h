#pragma once

#include "IExactQAgent.h"

template <class Perception>
class EGFVMC : public IExactQAgent<Perception>
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

	EGFVMC(size_t actions, std::tuple<> params)
		: EGFVMC{actions}
	{}

	EGFVMC(size_t actions)
		: IExactQAgent{actions}
	{}

	std::vector<double> Policy(Perception& perception) override
	{
		std::vector<double>& action_values = ActionValues(perception);
		std::vector<double> policy(action_values.size(), 0.0);
		auto max = std::max_element(action_values.begin(), action_values.end());
		auto max_predicate = [max](auto x)->bool { return x == (*max); };
		size_t max_count = std::count_if(action_values.begin(), action_values.end(), max_predicate);
		size_t remaining_count = action_values.size() - max_count;
		for (size_t i = 0; i < action_values.size(); i++)
		{
			if (max_predicate(action_values[i]))
				policy[i] = (1 - epsilon) / max_count;
			else // other than max
				policy[i] = epsilon / remaining_count;
		}
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
