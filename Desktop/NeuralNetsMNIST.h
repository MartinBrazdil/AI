#pragma once

#include "MNIST.h"
#include "Network.h"
#include "IFEP.h"
#include "HyperparameterEvolution.h"

#include <vector>
#include <fstream>

bool CheckClass(Tensor<> output, Tensor<> target)
{
	auto max_it = std::max_element(output.memory.begin(), output.memory.end());
	std::fill(output.memory.begin(), output.memory.end(), 0);
	*max_it = 1;
	return output.memory == target.memory;
}

double MSE(Tensor<> tensor)
{
	double sum = 0;
	for (size_t i = 0; i < tensor.Volume(); i++)
		sum += pow(tensor.memory[i], 2) / tensor.Volume();
	return sum;
}

double XValidation(MNIST& mnist, Network& network, size_t factor, std::string log_file_name = "", std::ios_base::openmode mode = std::ios_base::out)
{
	std::ofstream log_file(log_file_name.c_str());
	
	double fitness = 0;

	for (size_t i = 0; i < factor; i++)
	{
		auto t0 = std::chrono::high_resolution_clock::now();

		auto test_batch = mnist.XValidationTestBatch(factor, i);

		size_t counter = 0;
		double batch_max_right = 0;
		for (double error_old = static_cast<double>(mnist.all_count), error_new = 0; ; )
		{
			double right = 0, wrong = 0;
			for (auto it = mnist.data.begin(); it != test_batch.begin; it++)
			{
				network.Learn(it->input, it->target);
			}
			for (auto it = test_batch.end; it != mnist.data.end(); it++)
			{
				network.Learn(it->input, it->target);
			}

			for (auto it = test_batch.begin; it != test_batch.end; it++)
			{
				Tensor<> output = network.Feedforward(it->input);
				CheckClass(output, it->target) ? right++ : wrong++;
			}
			log_file << counter++ << " " << right << std::endl;
			batch_max_right = std::max(batch_max_right, right);

			error_new = error_old + 0.1 * (wrong - error_old);
			printf("wrong: %f, error_old: %f, error_new: %f\n", wrong, error_old, error_new);
			if (error_old <= error_new + 0.1) break;
			error_old = error_new;
		}
		fitness += batch_max_right;
		network.Reset();

		auto t1 = std::chrono::high_resolution_clock::now();
		printf("Time: %lli\n", std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count());
	}
	log_file.close();

	return fitness;
}

template <class Neuron>
void RunOneAndTwoLayeredNets(MNIST& mnist_evol, MNIST& mnist_eval, size_t generations, size_t population, std::string name)
{
	{
		auto Fitness = [&](std::vector<double> params, size_t size, void* data)
		{
			Network network;
			network.layers.push_back(new Perceptron<Neuron>(784, 10, params[0], 1));
			return XValidation(mnist_evol, network, 2);
		};

		IFEP evolution(generations, population);
		std::string evol_log_file_name = std::string("neural_nets_mnist/one_layer_") + name + std::string("_evol.log");
		evolution.Evolve(Fitness, {{0.0001, 0.1}}, nullptr, evol_log_file_name);

		Network network;
		std::vector<double> params = evolution.population[0].x;
		network.layers.push_back(new Perceptron<Neuron>(784, 10, params[0], 1));
		std::string eval_log_file_name = std::string("neural_nets_mnist/one_layer_") + name + std::string("_eval.log");
		XValidation(mnist_eval, network, 10, eval_log_file_name);
	}

	{
		auto Fitness = [&](std::vector<double> params, size_t size, void* data)
		{
			Network network;
			network.layers.push_back(new Perceptron<Neuron>(784, 784 * params[2], params[0], 1));
			network.layers.push_back(new Perceptron<Neuron>(784 * params[2], 10, params[1], 1));
			return XValidation(mnist_evol, network, 2);
		};

		IFEP evolution(generations, population);
		std::string evol_log_file_name = std::string("neural_nets_mnist/two_layers_") + name + std::string("_evol.log");
		evolution.Evolve(Fitness, {{0.0001, 0.1}, {0.0001, 0.1}, {0.01, 0.5}}, nullptr, evol_log_file_name);

		Network network;
		std::vector<double> params = evolution.population[0].x;
		network.layers.push_back(new Perceptron<Neuron>(784, 784 * params[2], params[0], 1));
		network.layers.push_back(new Perceptron<Neuron>(784 * params[2], 10, params[1], 1));
		std::string eval_log_file_name = std::string("neural_nets_mnist/two_layers_") + name + std::string("_eval.log");
		XValidation(mnist_eval, network, 10, eval_log_file_name);
	}

	{
		auto Fitness = [&](std::vector<double> params, size_t size, void* data)
		{
			Network network;
			network.layers.push_back(new Convolution<Neuron>(28, 28, 1, 7, 7, std::floor(params[2]), params[0], 1));
			network.layers.push_back(new Perceptron<Neuron>(16 * std::floor(params[2]), 10, params[1], 1));
			return XValidation(mnist_evol, network, 2);
		};

		IFEP evolution(generations, population);
		std::string evol_log_file_name = std::string("neural_nets_mnist/conv_perc_") + name + std::string("_evol.log");
		evolution.Evolve(Fitness, {{0.0001, 0.1},{0.0001, 0.1},{4, 96}}, nullptr, evol_log_file_name);

		Network network;
		std::vector<double> params = evolution.population[0].x;
		network.layers.push_back(new Convolution<Neuron>(28, 28, 1, 7, 7, std::floor(params[2]), params[0], 1));
		network.layers.push_back(new Perceptron<Neuron>(16 * std::floor(params[2]), 10, params[1], 1));
		std::string eval_log_file_name = std::string("neural_nets_mnist/conv_perc_") + name + std::string("_eval.log");
		XValidation(mnist_eval, network, 10, eval_log_file_name);
	}
}

void NeuralNetsMNIST(std::string mnist_dir_path)
{
	size_t population = 10, generations = 5;
	MNIST mnist_evol(mnist_dir_path, 28, 28, 500, 0, true, true);
	MNIST mnist_eval(mnist_dir_path, 28, 28, 300, 0, true, true);

	RunOneAndTwoLayeredNets<Identity>(mnist_evol, mnist_eval, population, generations, "identity");
	RunOneAndTwoLayeredNets<ReLU>(mnist_evol, mnist_eval, population, generations, "relu");
	RunOneAndTwoLayeredNets<LecunTanh>(mnist_evol, mnist_eval, population, generations, "tanh");
	RunOneAndTwoLayeredNets<Logistic>(mnist_evol, mnist_eval, population, generations, "logistic");
}
