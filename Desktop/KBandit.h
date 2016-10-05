#pragma once

#include "IWorld.h"
#include "IState.h"

#include <vector>
#include <random>
#include <chrono>

class KBandit : public IWorld<DummyState, DummyState>
{
public:
	int lever_count;
	std::vector<std::normal_distribution<double>> levers;
	std::mt19937 rng{static_cast<unsigned int>(std::chrono::high_resolution_clock::now().time_since_epoch().count())};
	std::uniform_real_distribution<double> zero_to_one{0, 1};
	std::normal_distribution<double> normal{0, 1};

	KBandit(int lever_count, bool initialize = true);
	~KBandit() override {};

	void InitDefault() override;
	void InitRandom() override;

	DummyState DefaultState() override;
	DummyState RandomState() override;

	//std::vector<DummyState> AllStates() override;

	double Transition(DummyState& state, size_t action) override;
	size_t Actions() { return lever_count; };

	DummyState Observation(DummyState& state) override;
	bool Terminal(DummyState& state) override;

	void Print(DummyState& state) override {}
};

KBandit::KBandit(int lever_count, bool initialize)
	: lever_count{lever_count}
{
	if (initialize)
		InitDefault();
}

void KBandit::InitDefault()
{
	InitRandom();
}

void KBandit::InitRandom()
{
	double min = static_cast<double>(-lever_count);
	double max = static_cast<double>(lever_count);
	std::uniform_real_distribution<double> reward_mean_distribution{min, max};

	levers.resize(lever_count);
	for (size_t i = 0; i < lever_count; i++)
	{
		double mean = reward_mean_distribution(rng);
		levers[i] = std::normal_distribution<double>(mean, 1);
	}
}

DummyState KBandit::DefaultState()
{
	return DummyState();
}

DummyState KBandit::RandomState()
{
	return DummyState();
}

double KBandit::Transition(DummyState& state, size_t action)
{
	return levers[action](rng);
}

DummyState KBandit::Observation(DummyState& state)
{
	return DummyState();
}

bool KBandit::Terminal(DummyState& state)
{
	return false;
}
