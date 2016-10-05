#pragma once

#include <iostream>
#include <vector>
#include <array>
#include <algorithm>
#include <random>
#include <chrono>
#include <fstream>

#include "Neurons.h"
#include "ILayer.h"

template <class Neuron>
class Perceptron : public ILayer
{
public:
	std::mt19937 rng{unsigned int(std::chrono::system_clock::now().time_since_epoch().count())};

	const size_t I, O;
	
	double alpha;
	size_t batch_size, batch_counter{0};

	Tensor<> bias{{1}};
	Tensor<> weights{{I, O}};
	Tensor<> weights_delta{{I, O}};
	Tensor<> bias_weights{{O}};
	Tensor<> bias_weights_delta{{O}};
	
	using Definition = std::tuple<size_t, size_t, double, size_t>;
	Definition definition;

	Perceptron(Definition def)
		: Perceptron{std::get<0>(def), std::get<1>(def), std::get<2>(def), std::get<3>(def)}
	{}

	Perceptron(size_t I, size_t O, double alpha, size_t batch_size)
		: I{I}, O{O}, alpha{alpha}, batch_size{batch_size}, definition{I, O, alpha, batch_size}
	{
		Reset();
	}

	~Perceptron()
	{}

	Perceptron* CloneRandomize()
	{
		return new Perceptron(definition);
	}

	Perceptron* CloneExactly()
	{
		return new Perceptron(*this);
	}

	void Reset() override
	{
		bias.memory[0] = 1.0;
		std::normal_distribution<double> normal(0, 0.01);
		double weight_bound = std::sqrt(6.0 / (I + O));
		std::uniform_real<double> weight_distribution(-weight_bound, weight_bound);
		for (size_t i = 0; i < weights.Volume(); i++)
			weights.memory[i] = normal(rng);
		double bias_weight_bound = std::sqrt(6.0 / O);
		std::uniform_real<double> bias_weight_distribution(-bias_weight_bound, bias_weight_bound);
		for (size_t i = 0; i < bias_weights.Volume(); i++)
			bias_weights.memory[i] = normal(rng);
	}

	ResponseAndGradient Feedforward(Tensor<>& input) override
	{
		Tensor<> activation = Activation(input);
		Tensor<> response = Response(activation);
		Tensor<> gradient = Gradient(response);

		return{response, gradient};
	}

	Tensor<> Backprop(Tensor<>& input, Tensor<>& gradient, Tensor<>& delta) override
	{
		WeightsDelta(input, gradient, delta);
		return InputDelta(delta);
	}

	Tensor<> Activation(Tensor<>& input) override
	{
		Tensor<> activation{{O}};
		for (size_t o = 0; o < O; o++)
		{
			Tensor<>::InnerProduct(activation[{o}],
								   weights[{0, o}],
								   input[{}],
								   input.Volume());

			Tensor<>::Broadcast(activation[{o}],
								bias_weights[{}],
								*bias[{}],
								1);
		}
		return activation;
	}

	Tensor<> Response(Tensor<>& activation) override
	{
		Tensor<> response(activation.dims);
		Neuron::Response(response, activation);
		return response;
	}

	Tensor<> Gradient(Tensor<>& response) override
	{
		Tensor<> gradient(response.dims);
		Neuron::Gradient(gradient, response);
		return gradient;
	}

	void Autoencode(Tensor<>& input)
	{
		Tensor<> response = Response(Activation(input));
		Tensor<> gradient = Gradient(response);
		auto reflection = AutoDecode(response);
		auto auto_delta = AutoError(input, reflection);
		Tensor<> delta = AutoDelta(auto_delta);
		WeightsDelta(input, gradient, delta);
		UpdateWeights();
	}

	DecodedAndBias AutoDecode(Tensor<>& output) override
	{
		Tensor<> bias({1});
		Tensor<> input({I});
		for (size_t o = 0; o < O; o++)
		{
			Tensor<>::Broadcast(input[{}],
								weights[{0, o}],
								*output[{o}],
								input.Volume());

			Tensor<>::InnerProduct(bias[{}],
								   bias_weights[{o}],
								   output[{o}],
								   1);
		}
		return{input, bias};
	}

	DecodedAndBiasError AutoError(Tensor<>& input, DecodedAndBias& decodedAndBias) override
	{
		Tensor<> bias_error({1});
		Tensor<> input_error({I});

		Tensor<>::Subtract(input_error[{}],
						   decodedAndBias.input[{}],
						   input[{}],
						   input_error.Volume());

		Tensor<>::Subtract(bias_error[{}],
						   decodedAndBias.bias[{}],
						   bias[{}],
						   bias_error.Volume());

		return{input_error, bias_error};
	}

	Tensor<> Error(Tensor<> output, Tensor<> target) override
	{
		Tensor<> error({O});
		Tensor<>::Subtract(error[{}],
						   output[{}],
						   target[{}],
						   error.Volume());
		return error;
	}

	Tensor<> AutoDelta(DecodedAndBiasError& auto_delta) override
	{
		Tensor<> delta{{O}};
		for (size_t o = 0; o < O; o++)
		{
			Tensor<>::InnerProduct(delta[{o}],
								   weights[{0, o}],
								   auto_delta.input[{}],
								   auto_delta.input.Volume());

			Tensor<>::Broadcast(delta[{o}],
								bias_weights[{}],
								*auto_delta.bias[{}],
								1);
		}

		return delta;
	}

	void WeightsDelta(Tensor<>& input, Tensor<>& gradient, Tensor<>& delta) override
	{
		Tensor<> gradient_delta({O});
		Tensor<>::Multiply(gradient_delta[{}],
						   gradient[{}],
						   delta[{}],
						   gradient_delta.Volume());

		Tensor<>::OuterProduct(weights_delta[{}],
							   gradient_delta[{}],
							   gradient_delta.Volume(),
							   input[{}],
							   input.Volume());

		Tensor<>::OuterProduct(bias_weights_delta[{}],
							   bias[{}],
							   1,
							   gradient_delta[{}],
							   gradient_delta.Volume());
	}

	void UpdateWeights() override
	{
		if (++batch_counter == batch_size)
		{
			double learning_rate = alpha /  batch_size;

			//printf("Weight norm before: %f\n", Tensor<>::L2Norm(weights[{}], weights.Volume()));

			for (size_t i = 0; i < weights.Volume(); i++)
			{
				weights.memory[i] -= learning_rate * weights_delta.memory[i];
				weights_delta.memory[i] = 0;
			}
			//Tensor<>::Normalize(weights[{}], weights.Volume());

			for (size_t o = 0; o < weights.dims[1]; o++)
				Tensor<>::NormalizeToUnitCircle(weights[{0, o}], weights.Volume(1));

			//printf("Weight norm after: %f\n", Tensor<>::L2Norm(weights[{}], weights.Volume()));

			for (size_t i = 0; i < bias_weights.Volume(); i++)
			{
				bias_weights.memory[i] -= learning_rate * bias_weights_delta.memory[i];
				bias_weights_delta.memory[i] = 0;
			}
			Tensor<>::NormalizeToUnitCircle(bias_weights[{}], bias_weights.Volume());

			batch_counter = 0;
		}
	}

	Tensor<> InputDelta(Tensor<>& delta) override
	{
		//printf("PercDelta norm output: %f\n", Tensor<>::L2Norm(delta[{}], delta.Volume()));

		Tensor<> input_delta({I});
		for (size_t i = 0; i < input_delta.Volume(); i++)
			Tensor<>::InnerProduct(input_delta[{i}],
								   weights[{i}],
								   delta[{}],
								   delta.Volume());

		//printf("PercDelta norm input: %f\n", Tensor<>::L2Norm(input_delta[{}], input_delta.Volume()));

		return input_delta;
	}

	void PrintReceptiveField(std::string path)
	{
		for (size_t o = 0; o < weights.dims[1]; o++)
		{
			std::string modified_path = path + std::string("receptive_field_") + std::to_string(o);
			weights.NormalizeToUnitCircle();
			Tensor<>::PrintToFile(modified_path, weights[{0, o}], weights.Volume(1));
		}
	}
};
