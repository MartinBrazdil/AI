#pragma once

#include "ValueIteration.h"

template <class World>
void WorldValueIteration(World& _world, double gamma, double epsilon, size_t limit, std::string name)
{
	std::cout << std::string("\n=== ") + name + std::string(" State Values ===\n") << std::endl;
	ValueIteration(breakout, gamma, epsilon, limit, "world_state_values/bopbf_ff.log");
}

void WorldsStateValues()
{
	size_t width = 8, height = 8, paddle_width = 3, force_range = 3, speed = 1;
	double gamma = 0.9, epsilon = 0.0001, limit = 50;

	{
		printf("\n=== GridWorld<GridCoords> State Values ===\n");

		GridWorld<GridCoords> gridworld(5, 5);
		gridworld.InitDefault();

		ValueIteration(gridworld, gamma, epsilon, limit, "world_state_values/gwcoords.log");
	}

	{
		printf("\n=== BreakOut<PaddleBallAndForce> State Values ===\n");

		BreakOut<PaddleBallAndForce> breakout(width, height, paddle_width, force_range, speed, 0, false, false);
		breakout.InitDefault();

		printf("Positive/Negative reward, do not terminate on bounce\n");
		ValueIteration(breakout, gamma, epsilon, limit, "world_state_values/bopbf_ff.log");

		printf("Positive/Negative reward, terminate on bounce\n");
		breakout.only_positive_reward = false;
		breakout.terminal_bounce = true;
		ValueIteration(breakout, gamma, epsilon, limit, "world_state_values/bopbf_ft.log");

		printf("Positive reward, do not terminate on bounce\n");
		breakout.only_positive_reward = true;
		breakout.terminal_bounce = false;
		ValueIteration(breakout, gamma, epsilon, limit, "world_state_values/bopbf_tf.log");

		printf("Positive reward, terminate on bounce\n");
		breakout.only_positive_reward = true;
		breakout.terminal_bounce = true;
		ValueIteration(breakout, gamma, epsilon, limit, "world_state_values/bopbf_tt.log");
	}

	{
		printf("\n=== BreakOut<PaddleAndBall> State Values ===\n");

		BreakOut<PaddleAndBall> breakout(width, height, paddle_width, force_range, speed, 0, false, false);
		breakout.InitDefault();

		printf("Positive/Negative reward, do not terminate on bounce\n");
		ValueIteration(breakout, gamma, epsilon, limit, "world_state_values/bopb_ff.log");

		printf("Positive/Negative reward, terminate on bounce\n");
		breakout.only_positive_reward = false;
		breakout.terminal_bounce = true;
		ValueIteration(breakout, gamma, epsilon, limit, "world_state_values/bopb_ft.log");

		printf("Positive reward, do not terminate on bounce\n");
		breakout.only_positive_reward = true;
		breakout.terminal_bounce = false;
		ValueIteration(breakout, gamma, epsilon, limit, "world_state_values/bopb_tf.log");

		printf("Positive reward, terminate on bounce\n");
		breakout.only_positive_reward = true;
		breakout.terminal_bounce = true;
		ValueIteration(breakout, gamma, epsilon, limit, "world_state_values/bopb_tt.log");
	}

	{
		printf("\n=== BreakOut<Pixels> State Values ===\n");

		BreakOut<Pixels> breakout(width, height, paddle_width, force_range, speed, 0, false, false);
		breakout.InitDefault();

		printf("Positive/Negative reward, do not terminate on bounce\n");
		ValueIteration(breakout, gamma, epsilon, limit, "world_state_values/bopixels_ff.log");

		printf("Positive/Negative reward, terminate on bounce\n");
		breakout.only_positive_reward = false;
		breakout.terminal_bounce = true;
		ValueIteration(breakout, gamma, epsilon, limit, "world_state_values/bopixels_ft.log");

		printf("Positive reward, do not terminate on bounce\n");
		breakout.only_positive_reward = true;
		breakout.terminal_bounce = false;
		ValueIteration(breakout, gamma, epsilon, limit, "world_state_values/bopixels_tf.log");

		printf("Positive reward, terminate on bounce\n");
		breakout.only_positive_reward = true;
		breakout.terminal_bounce = true;
		ValueIteration(breakout, gamma, epsilon, limit, "world_state_values/bopixels_tt.log");
	}

	{
		printf("\n=== Noisy BreakOut<PaddleBallAndForce> State Values ===\n");

		BreakOut<PaddleBallAndForce> breakout(width, height, paddle_width, force_range, speed, 0.2, false, false);
		breakout.InitDefault();

		printf("Positive/Negative reward, do not terminate on bounce\n");
		ValueIteration(breakout, gamma, epsilon, limit, "world_state_values/noisy_bopbf_ff.log");

		printf("Positive/Negative reward, terminate on bounce\n");
		breakout.only_positive_reward = false;
		breakout.terminal_bounce = true;
		ValueIteration(breakout, gamma, epsilon, limit, "world_state_values/noisy_bopbf_ft.log");

		printf("Positive reward, do not terminate on bounce\n");
		breakout.only_positive_reward = true;
		breakout.terminal_bounce = false;
		ValueIteration(breakout, gamma, epsilon, limit, "world_state_values/noisy_bopbf_tf.log");

		printf("Positive reward, terminate on bounce\n");
		breakout.only_positive_reward = true;
		breakout.terminal_bounce = true;
		ValueIteration(breakout, gamma, epsilon, limit, "world_state_values/noisy_bopbf_tt.log");
	}

	{
		printf("\n=== Noisy BreakOut<PaddleAndBall> State Values ===\n");

		BreakOut<PaddleAndBall> breakout(width, height, paddle_width, force_range, speed, 0.2, false, false);
		breakout.InitDefault();

		printf("Positive/Negative reward, do not terminate on bounce\n");
		ValueIteration(breakout, gamma, epsilon, limit, "world_state_values/noisy_bopb_ff.log");

		printf("Positive/Negative reward, terminate on bounce\n");
		breakout.only_positive_reward = false;
		breakout.terminal_bounce = true;
		ValueIteration(breakout, gamma, epsilon, limit, "world_state_values/noisy_bopb_ft.log");

		printf("Positive reward, do not terminate on bounce\n");
		breakout.only_positive_reward = true;
		breakout.terminal_bounce = false;
		ValueIteration(breakout, gamma, epsilon, limit, "world_state_values/noisy_bopb_tf.log");

		printf("Positive reward, terminate on bounce\n");
		breakout.only_positive_reward = true;
		breakout.terminal_bounce = true;
		ValueIteration(breakout, gamma, epsilon, limit, "world_state_values/noisy_bopb_tt.log");
	}

	{
		printf("\n=== Noisy BreakOut<Pixels> State Values ===\n");

		BreakOut<Pixels> breakout(width, height, paddle_width, force_range, speed, 0.2, false, false);
		breakout.InitDefault();

		printf("Positive/Negative reward, do not terminate on bounce\n");
		ValueIteration(breakout, gamma, epsilon, limit, "world_state_values/noisy_bopixels_ff.log");

		printf("Positive/Negative reward, terminate on bounce\n");
		breakout.only_positive_reward = false;
		breakout.terminal_bounce = true;
		ValueIteration(breakout, gamma, epsilon, limit, "world_state_values/noisy_bopixels_ft.log");

		printf("Positive reward, do not terminate on bounce\n");
		breakout.only_positive_reward = true;
		breakout.terminal_bounce = false;
		ValueIteration(breakout, gamma, epsilon, limit, "world_state_values/noisy_bopixels_tf.log");

		printf("Positive reward, terminate on bounce\n");
		breakout.only_positive_reward = true;
		breakout.terminal_bounce = true;
		ValueIteration(breakout, gamma, epsilon, limit, "world_state_values/noisy_bopixels_tt.log");
	}
}
