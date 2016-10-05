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
class HessianNet
{
public:
	std::mt19937 rng{unsigned int(std::chrono::system_clock::now().time_since_epoch().count())};

	double alpha;

	Tensor<> weights, weights_gradient, gradients, gradients_delta, input_delta, output_delta, activations, responses;
	Tensor<> hessian_weights_gradient, hessian_gradients, hessian_weights_gradient_rms, hessian_gradients_delta, hessian_input_delta, hessian_output_delta, hessian_activations, hessian_responses;

	HessianNet(size_t dim0, size_t dim1, double alpha)
		: weights{{dim0, dim1}}, alpha{alpha}
	{
		auto normal = std::normal_distribution<double>(0, 0.001);
		for (size_t i = 0; i < weights.Volume(); i++)
			weights.memory[i] = normal(rng);
		weights.NormalizeToUnitCircle();
	}

	Tensor<>& Feedforward(Tensor<>& inputs)
	{
		NN::Activation(activations, weights, inputs);
		Neuron::Response(responses, activations);
		return responses;
	}

	Tensor<>& Backprop(Tensor<>& inputs, Tensor<>& target)
	{
		//for (size_t o = 0; o < weights.dims[1]; o++)
		//	printf("init 1: %f\n", Tensor<>::L2Norm(weights[{0, o}], weights.Volume(1)));

		size_t batch_size = inputs.dims.back();
		NN::Error(output_delta, responses, target);
		printf("error before: %f\n", NN::MSE(output_delta));
		Neuron::Gradient(gradients, activations);
		NN::GradientDelta(gradients_delta, gradients, output_delta);
		NN::WeightsDelta(weights_gradient, inputs, gradients_delta);
		NN::DeltaBack(input_delta, weights, output_delta);

		for (size_t i = 0; i < 10000; i++)
		{
			NN::Activation(hessian_activations, weights_gradient, inputs);
			Neuron::Response(hessian_responses, hessian_activations);
			NN::Error(hessian_output_delta, hessian_responses, output_delta);
			//printf("hessian error: %f\n", NN::MSE(hessian_output_delta));
			Neuron::Gradient(hessian_gradients, hessian_activations);
			NN::GradientDelta(hessian_gradients_delta, hessian_gradients, hessian_output_delta);
			NN::WeightsDelta(hessian_weights_gradient, inputs, hessian_gradients_delta);
			//NN::DeltaBack(hessian_input_delta, weights_gradient, output_delta);
			NN::SGDUpdate(weights_gradient, hessian_weights_gradient, alpha / batch_size);
			//NN::RMSUpdate(weights_gradient, hessian_weights_gradient, hessian_weights_gradient_rms, alpha / batch_size);
			//NN::NormalizeWeights(weights_gradient);
		}
		NN::SGDUpdate(weights, weights_gradient, 1);
		//NN::NormalizeWeights(weights);

		Feedforward(inputs);
		NN::Error(output_delta, responses, target);
		printf("error after: %f\n", NN::MSE(output_delta));

		cin.get();

		return input_delta;
	}
};
