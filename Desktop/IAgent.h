#pragma once

#include <vector>
#include <random>
#include <chrono>

template <class Perception>
class IAgent
{
public:
	std::mt19937 rng{static_cast<unsigned int>(std::chrono::system_clock::now().time_since_epoch().count())};

	struct Experience
	{
		Perception perception;
		size_t action;
		double reward;

		Experience(const Experience& rhs)
			: perception(perception), action(action), reward(reward) {}

		Experience(Perception&& perception, size_t action, double reward)
			: perception(std::move(perception)), action(action), reward(reward) {}

		Experience(Experience&& rhs)
			: perception(std::move(rhs.perception)), action(rhs.action), reward(rhs.reward) {}
	};
	std::vector<Experience> experiences;

	const size_t actions;
	IAgent(size_t actions) : actions{actions} {}
	virtual ~IAgent() {}

	virtual void CollectParamValues(std::vector<double>& params) {}
	virtual void CollectParamBounds(std::vector<std::array<double, 2>>& bounds) {}
	virtual void DistributeParamValues(std::vector<double>& params, size_t& offset) {}
	virtual void DistributeParamBounds(std::vector<std::array<double, 2>>& bounds, size_t& offset) {}

	virtual std::vector<double>& ActionValues(Perception& perception) = 0;
	virtual std::vector<double> Policy(Perception& perception) = 0;
	virtual void LearnEpisode() = 0;

	void Feedback(Perception&& perception, size_t action, double reward)
	{
		experiences.push_back({std::move(perception), action, reward});
	}

	size_t Choose(Perception& perception)
	{
		std::vector<double> policy = Policy(perception);
		std::discrete_distribution<size_t> policy_distribution(policy.begin(), policy.end());
		return policy_distribution(rng);
	}

	std::vector<double> PolicyGreedy(Perception& perception)
	{
		std::vector<double> action_values = ActionValues(perception);
		std::vector<double> policy(action_values.size(), 0.0);
		auto max = std::max_element(action_values.begin(), action_values.end());
		auto max_predicate = [max](double& x) { return x == (*max); };
		size_t max_count = std::count_if(action_values.begin(), action_values.end(), max_predicate);
		for (size_t i = 0; i < action_values.size(); i++)
			if (max_predicate(action_values[i]))
				policy[i] = 1.0 / max_count;
		return policy;
	}

	size_t ChooseGreedy(Perception& perception)
	{
		std::vector<double> policy = PolicyGreedy(perception);
		auto max = std::max_element(policy.begin(), policy.end());
		size_t max_idx = std::distance(policy.begin(), max);
		//printf("action: %lli\n", max_idx);
		return max_idx;
	}
};
