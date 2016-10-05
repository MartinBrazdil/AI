#pragma once

#include "IAgent.h"
#include "Network.h"

#include "BreakOut.h"

#include <array>

class EGDQN : public IAgent<Pixels>
{
public:
	double gamma = 0.9;
	std::array<double, 2> gamma_bounds{{0, 1}};

	double epsilon = 0.9;
	std::array<double, 2> epsilon_bounds{{0, 1}};

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

	size_t input_width, input_height, markov_order, replay_batch_size;
	Tensor<> input_stack{{input_width, input_height, markov_order}};

	Network network;

	struct Replay { Tensor<> input_stack; Tensor<> target; };
	std::vector<Replay> replay_buffer;

	EGDQN(size_t actions, std::tuple<Network&, size_t, size_t, size_t, size_t> params)
		: EGDQN{actions, std::get<0>(params).CloneRandomize(), std::get<1>(params), std::get<2>(params), std::get<3>(params), std::get<4>(params)}
	{}

	EGDQN(size_t actions, Network& network, size_t input_width, size_t input_height, size_t markov_order, size_t replay_batch_size)
		: IAgent{actions}, network{network.CloneRandomize()}, input_width{input_width}, input_height{input_height}, markov_order{markov_order}, replay_batch_size{replay_batch_size}
	{
		for (size_t i = 0; i < markov_order - 1; i++)
			experiences.push_back({Pixels(input_width, input_height, markov_order), 0, 0});
	}

	void ExperiencesInit()
	{
		experiences.clear();
		for (size_t i = 0; i < markov_order - 1; i++)
			experiences.push_back({Pixels(input_width, input_height, markov_order), 0, 0});
	}

	void StackInit()
	{
		Tensor<> input_stack{{input_width, input_height, markov_order}};
	}

	void StackInput(Pixels& perception)
	{
		// shift sequence (list would be much more efficient than vector in tensor)
		for (size_t i = 0; i < markov_order - 1; i++)
			Tensor<>::Copy(input_stack[{0, 0, i}],
						   input_stack[{0, 0, i + 1}],
						   input_stack.Volume(2));

		// copy perception pixels to its place
		Tensor<>::Copy(input_stack[{0, 0, markov_order - 1}],
					   perception.pixels[{}],
					   input_stack.Volume(2));
	}

	Tensor<> last_action_values;
	std::vector<double>& ActionValues(Pixels& perception) override
	{
		StackInput(perception);
		last_action_values = network.Feedforward(input_stack);
		return last_action_values.memory;
	}

	std::vector<double> Policy(Pixels& perception) override
	{
		StackInput(perception);

		std::vector<double> action_values = network.Feedforward(input_stack).memory;
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

	double N = 1;
	double error = 0;
	void LearnEpisode() override
	{
		StackInit();
		if (experiences.back().reward == 0)
		{
			ExperiencesInit();
			return;
		}

		double reward_return = 0;
		for (size_t e = experiences.size() - 1; markov_order <= e; e--)
		{
			for (size_t i = 0; i < markov_order; i++)
				Tensor<>::Copy(input_stack[{0, 0, (markov_order - 1) - i}],
							   experiences[e - i].perception.pixels[{}],
							   input_stack.Volume(2));

			size_t action = experiences[e].action;
			double reward = experiences[e].reward;

			Tensor<> action_values = network.Feedforward(input_stack);
			double& value = action_values.memory[action];

			double action_value = reward + gamma * reward_return;
			reward_return = *std::max_element(action_values.memory.begin(), action_values.memory.end());
			action_values.memory[action] = action_value;

			replay_buffer.push_back({input_stack, action_values});
		}

		if (replay_buffer.size() > 100)
		{
			auto mse = [](Tensor<> tensor) {
				double sum = 0;
				for (size_t i = 0; i < tensor.Volume(); i++)
					sum += pow(tensor.memory[i], 2) / tensor.Volume();
				return sum;
			};

			double batch_error = 0;
			for (size_t i = 0; i < 32; i++)
			{
				for (size_t r = 0; r < replay_batch_size; r++)
				{
					std::uniform_int<size_t> replay_buffer_sampler(0, replay_buffer.size() - 1);
					Replay& chosen_replay = replay_buffer[replay_buffer_sampler(rng)];
					Tensor<> delta = network.Learn(chosen_replay.input_stack, chosen_replay.target);
					batch_error += mse(delta);
				}
			}
			error = error + (batch_error - error) / N++;
			//printf("Batch error: %f\n", error);
		}

		if (replay_buffer.size() > 100000)
		{
			std::random_shuffle(replay_buffer.begin(), replay_buffer.end());
			replay_buffer.resize(10000);
		}

		ExperiencesInit();
		StackInit();
	}
};
