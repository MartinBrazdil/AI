#pragma once

#include "HyperparameterEvolution.h"
#include "EpizodicLearning.h"
#include "GreedyEvaluation.h"

#include "EGDQN.h"

#include <fstream>
#include <string>

void DQNLearningSmall()
{
	//{
	//	printf("DQN Small one layer\n");
	//	std::ofstream batch_performance_log("dqn_learning_small/one_layer_performance.log");

	//	size_t width = 8, height = 8;
	//	BreakOut<Pixels> breakout(width, height, 2, 3, 1, 0, true, true);
	//	auto default_state = breakout.DefaultState();

	//	Network network;
	//	size_t markov_order = 3, batch_size = 1;
	//	network.layers.push_back(new Perceptron<LecunTanh>(width * height * markov_order, 32, 0.01, batch_size));

	//	EGDQN agent(breakout.Actions(), std::move(network), width, height, markov_order, batch_size);

	//	double reward = 0;
	//	for (size_t i = 1; i < 5000; i++)
	//	{
	//		EpizodicLearning(breakout, agent, 100, 100, "");
	//		reward = reward + (GreedyEvaluation(breakout, agent, 100, 100, "") - reward) / i;
	//		batch_performance_log << (i - 1) << " " << reward << std::endl;
	//		printf("%f\n", reward);
	//	}
	//}

	{
		printf("DQN Small two layers\n");
		std::ofstream batch_performance_log("dqn_learning_small/two_layers_performance.log");

		size_t width = 8, height = 8;
		BreakOut<Pixels> breakout(width, height, 2, 3, 1, 0, true, true);
		auto default_state = breakout.DefaultState();

		Network network;
		size_t markov_order = 3, batch_size = 1;
		network.layers.push_back(new Perceptron<LecunTanh>(width * height * markov_order, 32, 0.005, batch_size));
		network.layers.push_back(new Perceptron<LecunTanh>(32, breakout.Actions(), 0.01, batch_size));

		EGDQN agent(breakout.Actions(), std::move(network), width, height, markov_order, batch_size);

		double reward = 0;
		for (size_t i = 1; i < 5000; i++)
		{
			EpizodicLearning(breakout, agent, 100, 100, "");
			reward = reward + (GreedyEvaluation(breakout, agent, 100, 100, "") - reward) / i;
			batch_performance_log << (i - 1) << " " << reward << std::endl;
			printf("%f\n", reward);
		}
	}

	{
		printf("DQN Small convnet\n");
		std::ofstream batch_performance_log("dqn_learning_small/conv_perc_performance.log");

		size_t width = 8, height = 8;
		BreakOut<Pixels> breakout(width, height, 2, 3, 1, 0, true, true);
		auto default_state = breakout.DefaultState();

		Network network;
		size_t markov_order = 3, batch_size = 1;
		network.layers.push_back(new Convolution<LecunTanh>(width, height, markov_order, 4, 4, 16, 0.005, batch_size));
		network.layers.push_back(new Perceptron<LecunTanh>(64, breakout.Actions(), 0.01, batch_size));

		EGDQN agent(breakout.Actions(), std::move(network), width, height, markov_order, batch_size);

		double reward = 0;
		for (size_t i = 1; i < 5000; i++)
		{
			EpizodicLearning(breakout, agent, 100, 100, "");
			reward = reward + (GreedyEvaluation(breakout, agent, 100, 100, "") - reward) / i;
			batch_performance_log << (i - 1) << " " << reward << std::endl;
			printf("%f\n", reward);
		}
	}
}


void DQNLearningBig()
{
	{
		printf("DQN Big one layer\n");
		std::ofstream batch_performance_log("dqn_learning_big/one_layer_performance.log");

		size_t width = 12, height = 12;
		BreakOut<Pixels> breakout(width, height, 2, 3, 1, 0, true, true);
		auto default_state = breakout.DefaultState();

		Network network;
		double learning_rate = 0.01;
		size_t markov_order = 3, batch_size = 1;
		network.layers.push_back(new Perceptron<LecunTanh>(width * height * markov_order, 32, 0.01, batch_size));

		EGDQN agent(breakout.Actions(), std::move(network), width, height, markov_order, batch_size);

		double reward = 0;
		for (size_t i = 1; i < 2000; i++)
		{
			EpizodicLearning(breakout, agent, 100, 100, "");
			reward = reward + (GreedyEvaluation(breakout, agent, 100, 100, "") - reward) / i;
			batch_performance_log << (i - 1) << " " << reward << std::endl;
			printf("%f\n", reward);
		}
	}

	{
		printf("DQN Big two layers\n");
		std::ofstream batch_performance_log("dqn_learning_big/two_layers_performance.log");

		size_t width = 12, height = 12;
		BreakOut<Pixels> breakout(width, height, 2, 3, 1, 0, true, true);
		auto default_state = breakout.DefaultState();

		Network network;
		size_t markov_order = 3, batch_size = 1;
		network.layers.push_back(new Perceptron<LecunTanh>(width * height * markov_order, 32, 0.005, batch_size));
		network.layers.push_back(new Perceptron<LecunTanh>(32, breakout.Actions(), 0.01, batch_size));

		EGDQN agent(breakout.Actions(), std::move(network), width, height, markov_order, batch_size);

		double reward = 0;
		for (size_t i = 1; i < 2000; i++)
		{
			EpizodicLearning(breakout, agent, 100, 100, "");
			reward = reward + (GreedyEvaluation(breakout, agent, 100, 100, "") - reward) / i;
			batch_performance_log << (i - 1) << " " << reward << std::endl;
			printf("%f\n", reward);
		}
	}

	{
		printf("DQN Big convnet\n");
		std::ofstream batch_performance_log("dqn_learning_big/conv_perc_performance.log");

		size_t width = 12, height = 12;
		BreakOut<Pixels> breakout(width, height, 2, 3, 1, 0, true, true);
		auto default_state = breakout.DefaultState();

		Network network;
		size_t markov_order = 3, batch_size = 1;
		network.layers.push_back(new Convolution<LecunTanh>(width, height, markov_order, 6, 6, 16, 0.005, batch_size));
		network.layers.push_back(new Perceptron<LecunTanh>(64, breakout.Actions(), 0.01, batch_size));

		EGDQN agent(breakout.Actions(), std::move(network), width, height, markov_order, batch_size);

		double reward = 0;
		for (size_t i = 1; i < 2000; i++)
		{
			EpizodicLearning(breakout, agent, 100, 100, "");
			reward = reward + (GreedyEvaluation(breakout, agent, 100, 100, "") - reward) / i;
			batch_performance_log << (i - 1) << " " << reward << std::endl;
			printf("%f\n", reward);
		}
	}
}


void DQNLearningHard()
{
	{
		printf("DQN Hard one layer\n");
		std::ofstream batch_performance_log("dqn_learning_hard/one_layer_performance.log");

		size_t width = 8, height = 8;
		BreakOut<Pixels> breakout(width, height, 2, 3, 1, 0, true, false);
		auto default_state = breakout.DefaultState();

		Network network;
		size_t markov_order = 3, batch_size = 1;
		network.layers.push_back(new Perceptron<LecunTanh>(width * height * markov_order, breakout.Actions(), 0.01, batch_size));

		EGDQN agent(breakout.Actions(), std::move(network), width, height, markov_order, batch_size);

		double reward = 0;
		for (size_t i = 1; i < 5000; i++)
		{
			EpizodicLearning(breakout, agent, 100, 100, "");
			reward = reward + (GreedyEvaluation(breakout, agent, 100, 100, "") - reward) / i;
			batch_performance_log << (i - 1) << " " << reward << std::endl;
			printf("%f\n", reward);
		}
	}

	{
		printf("DQN Hard two layers\n");
		std::ofstream batch_performance_log("dqn_learning_hard/two_layers_performance.log");

		size_t width = 8, height = 8;
		BreakOut<Pixels> breakout(width, height, 2, 3, 1, 0, true, false);
		auto default_state = breakout.DefaultState();

		Network network;
		size_t markov_order = 3, batch_size = 1;
		network.layers.push_back(new Perceptron<LecunTanh>(width * height * markov_order, 32, 0.005, batch_size));
		network.layers.push_back(new Perceptron<LecunTanh>(32, breakout.Actions(), 0.01, batch_size));

		EGDQN agent(breakout.Actions(), std::move(network), width, height, markov_order, batch_size);

		double reward = 0;
		for (size_t i = 1; i < 5000; i++)
		{
			EpizodicLearning(breakout, agent, 100, 100, "");
			reward = reward + (GreedyEvaluation(breakout, agent, 100, 100, "") - reward) / i;
			batch_performance_log << (i - 1) << " " << reward << std::endl;
			printf("%f\n", reward);
		}
	}

	{
		printf("DQN Hard convnet\n");
		std::ofstream batch_performance_log("dqn_learning_hard/conv_perc_performance.log");

		size_t width = 8, height = 8;
		BreakOut<Pixels> breakout(width, height, 2, 3, 1, 0, true, false);
		auto default_state = breakout.DefaultState();

		Network network;
		size_t markov_order = 3, batch_size = 1;
		network.layers.push_back(new Convolution<LecunTanh>(width, height, markov_order, 4, 4, 16, 0.005, batch_size));
		network.layers.push_back(new Perceptron<LecunTanh>(64, breakout.Actions(), 0.01, batch_size));

		EGDQN agent(breakout.Actions(), std::move(network), width, height, markov_order, batch_size);

		double reward = 0;
		for (size_t i = 1; i < 5000; i++)
		{
			EpizodicLearning(breakout, agent, 100, 100, "");
			reward = reward + (GreedyEvaluation(breakout, agent, 100, 100, "") - reward) / i;
			batch_performance_log << (i - 1) << " " << reward << std::endl;
			printf("%f\n", reward);
		}
	}
}
