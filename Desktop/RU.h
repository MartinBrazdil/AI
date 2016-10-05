#pragma once

#include "IExactQAgent.h"

#include <vector>

template <class Perception>
class RU : public IAgent<Perception>
{
public:
	RU(size_t actions, std::tuple<> params)
		: RU{actions}
	{}

	RU(size_t actions)
		: IAgent{actions}
	{}

	std::vector<double> dummy;
	std::vector<double>& ActionValues(Perception& perception) override
	{
		return dummy;
	}

	std::vector<double> Policy(Perception& perception) override
	{
		return std::vector<double>(actions, 1.0 / actions);
	}

	void LearnEpisode() override
	{}
};
