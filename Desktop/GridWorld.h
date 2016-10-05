#pragma once

#include "IWorld.h"
#include "IState.h"
#include "IPerception.h"

#include <iostream>
#include <algorithm>
#include <random>
#include <chrono>
#include <memory>
#include <vector>
#include <stack>
#include <iomanip>

class GridCoords : public IPerception
{
public:
	size_t x, y;

	GridCoords() : x{0}, y{0} {}
	GridCoords(size_t x, size_t y) : x{x}, y{y} {}

	bool operator<(const IPerception& rhs) const override
	{
		const GridCoords* rhsPtr = dynamic_cast<const GridCoords*>(&rhs);
		if (y != rhsPtr->y)
			return y < rhsPtr->y;
		return x < rhsPtr->x;
	}

	bool operator==(const IPerception& rhs) const override
	{
		const GridCoords* rhsPtr = dynamic_cast<const GridCoords*>(&rhs);
		return x == rhsPtr->x && y == rhsPtr->y;
	}
};

class GridState : public IState
{
public:
	GridCoords coords;

	GridState() {}
	GridState(size_t x, size_t y) : coords{x, y} {}
	~GridState() override {}

	bool operator<(const GridState& rhs) const
	{
		return coords < rhs.coords;
	}
};

template <class Perception>
class GridWorld : public IWorld<Perception, GridState>
{
protected:
	std::mt19937 rng{static_cast<unsigned int>(std::chrono::system_clock::now().time_since_epoch().count())};

	int maxWidth, maxHeight;
	int width, height;

	std::uniform_int_distribution<int> widthSampler;
	std::uniform_int_distribution<int> heightSampler;

public:
	GridWorld(int maxWidth, int maxHeight, bool initialize = true);
	~GridWorld() override {};

	void InitDefault() override;
	void InitRandom() override;

	GridState DefaultState() override;
	GridState RandomState() override;
				
	//std::vector<GridState> AllStates() override;

	size_t Actions() override { return 4; };
	double Transition(GridState& state, size_t action) override;
				
	Perception Observation(GridState& state) override;
	bool Terminal(GridState& state) override;

	void Print(GridState& state) override;
};

template <>
GridCoords GridWorld<GridCoords>::Observation(GridState& state)
{
	GridCoords perception = state.coords;

	if (state.coords.x == 1 && state.coords.y == 0)
	{
		state.coords.y = 4;
		perception.y = 4;
	}

	if (state.coords.x == 3 && state.coords.y == 0)
	{
		state.coords.y = 2;
		perception.y = 2;
	}

	return perception;
}

template <class Perception>
GridWorld<Perception>::GridWorld(int maxWidth, int maxHeight, bool initialize)
	: maxWidth{maxWidth}, maxHeight{maxHeight}
{
	widthSampler = std::uniform_int_distribution<int>(0, maxWidth - 1);
	heightSampler = std::uniform_int_distribution<int>(0, maxHeight - 1);

	if (initialize)
		InitDefault();
}

template <class Perception>
void GridWorld<Perception>::InitDefault()
{
	width = 5;
	height = 5;
}

template <class Perception>
void GridWorld<Perception>::InitRandom()
{
	width = widthSampler(rng);
	height = heightSampler(rng);
}

template <class Perception>
GridState GridWorld<Perception>::DefaultState()
{
	int x = width / 2;
	int y = height / 2;
	return GridState(x, y);
}

template <class Perception>
GridState GridWorld<Perception>::RandomState()
{
	int x = widthSampler(rng);
	int y = heightSampler(rng);
	return GridState(x, y);
}

template <class Perception>
double GridWorld<Perception>::Transition(GridState& state, size_t action)
{
	if (action == 0)
		if (state.coords.x == 0)
			return -1;
		else
			state.coords.x--;
	if (action == 1)
		if (state.coords.x == width - 1)
			return -1;
		else
			state.coords.x++;
	if (action == 2)
		if (state.coords.y == 0)
			return -1;
		else
			state.coords.y--;
	if (action == 3)
		if (state.coords.y == height - 1)
			return -1;
		else
			state.coords.y++;

	if (state.coords.x == 1 && state.coords.y == 0)
		return 10;

	if (state.coords.x == 3 && state.coords.y == 0)
		return 5;

	return 0;
}

template <>
std::vector<uint64_t> GridWorld<std::vector<uint64_t>>::Observation(GridState& state)
{
	std::vector<uint64_t> perception;

	if (state.coords.x == 1 && state.coords.y == 0)
		state.coords.y = 4;

	if (state.coords.x == 3 && state.coords.y == 0)
		state.coords.y = 2;

	size_t bitsize = 8 * sizeof(uint64_t);
	perception.resize(static_cast<size_t>(ceil(width * height / static_cast<double>(bitsize))), 0);

	size_t idx = state.coords.y * width + state.coords.x;
	perception[idx / 64] |= uint64_t(1) << (idx % bitsize);

	return perception;
}

template<>
bool GridWorld<GridCoords>::Terminal(GridState& state)
{
	return false;
}

template<>
void GridWorld<GridCoords>::Print(GridState & state)
{
	std::cout << std::setprecision(2);
	for (size_t h = 0; h < height; h++)
	{
		for (size_t w = 0; w < width; w++)
		{
			if (h == state.coords.y && w == state.coords.x)
				std::cout  << "A";
			else std::cout  << ".";
		}
		std::cout << std::endl;
	}
	for (size_t i = 0; i < width; i++)
	{
		std::cout << "-";
	}
	printf("\n\n");
}
