#pragma once

#include "GridWorld.h"
#include "KBandit.h"
#include "BreakOut.h"

#include <iostream>
#include <iomanip>
#include <map>

/* PRINT STATE ACTION VALUE OVERLOADS */

void PrintActionValues(std::map<GridCoords, std::vector<double>>& state_action_values)
{
	using GridCoords = GridCoords;

	std::cout << "-------------------------\n";
	for (auto coordsValuesPair : state_action_values)
	{
		const GridCoords& coords = coordsValuesPair.first;
		printf("Coords: x = %llu, y = %llu:\n", coords.x, coords.y);
		for (auto& value : coordsValuesPair.second)
		{
			//printf("Mean: %f\n", value);
			std::cout << value << std::endl;
		}
	}
	std::cout << "\n-------------------------\n";
}

void PrintActionValues(std::map<DummyState, std::vector<double>>& state_action_values)
{
	std::cout << "-------------------------\n";
	for (auto dummyValuesPair : state_action_values)
	{
		for (auto& value : dummyValuesPair.second)
		{
			//printf("Mean: %f, Frequency: %f\n", value.mean, value.frequency);
			std::cout << value << std::endl;
		}
	}
	std::cout << "\n-------------------------\n";
}

void PrintActionValues(std::map<PaddleAndBall, std::vector<double>>& state_action_values)
{
	std::cout << "-------------------------\n";
	for (auto paddleAndBallValuesPair : state_action_values)
	{
		PaddleAndBall paddleAndBall = paddleAndBallValuesPair.first;
		int ballX = paddleAndBall.params[paddleAndBall.BX];
		int ballY = paddleAndBall.params[paddleAndBall.BY];
		int paddleX = paddleAndBall.params[paddleAndBall.PX];
		printf("Paddle: %i, Ball: [%i, %i]:\n", paddleX, ballX, ballY);
		for (auto& value : paddleAndBallValuesPair.second)
		{
			//printf("Mean: %f, Frequency: %f\n", value.mean, value.frequency);
			std::cout << value << std::endl;
		}
	}
	std::cout << "\n-------------------------\n";
}

/* PRINT STATE VALUE OVERLOADS */

void PrintStateValues(std::map<PaddleAndBall, double>& state_values)
{
}

void PrintStateValues(std::map<GridCoords, double>& state_values)
{
	if (!state_values.empty())
	{
		using GridCoords = GridCoords;

		size_t x = state_values.begin()->first.x, y = state_values.begin()->first.y;
		std::cout << std::setprecision(2);

		std::cout << "-------------------------\n";

		for (auto coordsValuePair : state_values)
		{
			const GridCoords& gridState = coordsValuePair.first;
			double value = coordsValuePair.second;

			if (y < gridState.y)
			{
				while (y < gridState.y)
				{
					std::cout << std::endl;
					y++;
				}
				x = 0;
			}

			if (x <= gridState.x)
			{
				while (x < gridState.x)
				{
					std::cout << std::setw(5) << "?";
					x++;
				}
				std::cout << std::setw(5) << value;
				x++;
			}
		}
		std::cout << "\n-------------------------\n";
	}
}


void PrintStateValues(std::map<DummyState, double>& state_values)
{
	std::cout << "-------------------------\n";
	for (auto dummyValuePair : state_values)
	{
		printf("Dummy: %f\n", dummyValuePair.second);
	}
	std::cout << "\n-------------------------\n";
}
