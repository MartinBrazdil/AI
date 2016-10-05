#pragma once

#include <iostream>
#include <vector>
#include <array>
#include <algorithm>
#include <random>
#include <chrono>
#include <fstream>
#include <cmath>

#include "Neurons.h"

template <class Neuron>
class UnbiasedPerceptronRMS
{
public:
	std::mt19937 rng{unsigned int(std::chrono::system_clock::now().time_since_epoch().count())};

	double alpha;

	Tensor<> weights, weights_gradient, weights_gradient_rms, gradients, gradients_delta, input_delta, output_delta, activations, responses;
	
	UnbiasedPerceptronRMS(size_t dim0, size_t dim1, double alpha)
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

	Tensor<>& Backprop(Tensor<>& inputs, Tensor<>& output_delta)
	{
		size_t batch_size = inputs.dims.back();
		Neuron::Gradient(gradients, responses);
		NN::GradientDelta(gradients_delta, gradients, output_delta);
		NN::WeightsDelta(weights_gradient, inputs, gradients_delta);
		NN::DeltaBack(input_delta, weights, output_delta);
		NN::RMSUpdate(weights, weights_gradient, weights_gradient_rms, alpha / batch_size);
		NN::NormalizeWeights(weights);
		return input_delta;
	}
};

template <class Neuron>
class UnbiasedPerceptronSGD
{
public:
	std::mt19937 rng{unsigned int(std::chrono::system_clock::now().time_since_epoch().count())};

	double alpha;

	Tensor<> weights, weights_gradient, weights_gradient_rms, gradients, gradients_delta, input_delta, output_delta, activations, responses;

	UnbiasedPerceptronSGD(size_t dim0, size_t dim1, double alpha)
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

	Tensor<>& Backprop(Tensor<>& inputs, Tensor<>& output_delta)
	{
		size_t batch_size = inputs.dims.back();
		Neuron::Gradient(gradients, responses);
		NN::GradientDelta(gradients_delta, gradients, output_delta);
		NN::WeightsDelta(weights_gradient, inputs, gradients_delta);
		NN::DeltaBack(input_delta, weights, output_delta);
		NN::SGDUpdate(weights, weights_gradient, alpha / batch_size);
		NN::NormalizeWeights(weights);
		return input_delta;
	}
};

template <class Neuron>
class UnbiasedPerceptronAD
{
public:
	std::mt19937 rng{unsigned int(std::chrono::system_clock::now().time_since_epoch().count())};

	Tensor<> weights, weights_gradient, weights_delta_rms, weights_gradient_rms, gradients, gradients_delta, input_delta, output_delta, activations, responses;

	UnbiasedPerceptronAD(size_t dim0, size_t dim1)
		: weights{{dim0, dim1}}
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

	Tensor<>& Backprop(Tensor<>& inputs, Tensor<>& output_delta)
	{
		size_t batch_size = inputs.dims.back();
		Neuron::Gradient(gradients, responses);
		NN::GradientDelta(gradients_delta, gradients, output_delta);
		NN::WeightsDelta(weights_gradient, inputs, gradients_delta);
		NN::DeltaBack(input_delta, weights, output_delta);
		NN::ADUpdate(weights, weights_gradient, weights_gradient_rms, weights_delta_rms);
		NN::NormalizeWeights(weights);
		return input_delta;
	}
};

template <class Neuron>
class BiasedPerceptron
{
public:
	std::mt19937 rng{unsigned int(std::chrono::system_clock::now().time_since_epoch().count())};

	double alpha;

	Tensor<> weights, weights_gradient, weights_gradient_rms, bias, bias_weights, bias_weights_delta, bias_weights_gradient_rms,
		activations, bias_activations, responses, gradients, gradients_delta, input_delta, output_delta;

	BiasedPerceptron(size_t dim0, size_t dim1, double alpha)
		: weights{{dim0, dim1}}, bias_weights{{1, dim1}}, alpha{alpha}
	{
		auto normal = std::normal_distribution<double>(0, 0.001);
		for (size_t i = 0; i < bias_weights.Volume(); i++)
			bias_weights.memory[i] = normal(rng);
		bias_weights.NormalizeToUnitCircle();
		for (size_t i = 0; i < weights.Volume(); i++)
			weights.memory[i] = normal(rng);
		weights.NormalizeToUnitCircle();
	}

	Tensor<>& Feedforward(Tensor<>& inputs)
	{
		size_t batch_size = inputs.dims.back();
		if (bias.Resize({1, batch_size}))
			bias.Fill(1);
		NN::Activation(activations, weights, inputs);
		NN::Activation(bias_activations, bias_weights, bias);
		activations += bias_activations;
		Neuron::Response(responses, activations);
		return responses;
	}

	Tensor<>& Backprop(Tensor<>& inputs, Tensor<>& output_delta)
	{
		size_t batch_size = inputs.dims.back();
		Neuron::Gradient(gradients, responses);
		NN::GradientDelta(gradients_delta, gradients, output_delta);
		NN::WeightsDelta(bias_weights_delta, bias, gradients_delta);
		NN::WeightsDelta(weights_gradient, inputs, gradients_delta);
		NN::DeltaBack(input_delta, weights, output_delta);
		NN::SGDUpdate(weights, weights_gradient, alpha / batch_size);
		//NN::RMSUpdate(weights, weights_gradient, weights_gradient_rms, alpha / batch_size);
		NN::SGDUpdate(bias_weights, bias_weights_delta, alpha / batch_size);
		//NN::RMSUpdate(bias_weights, bias_weights_delta, bias_weights_gradient_rms, alpha / batch_size);
		NN::NormalizeWeights(weights);
		//NN::NormalizeWeights(bias_weights);
		return input_delta;
	}
};

class NN
{
public:
	static void Activation(Tensor<>& activations, Tensor<>& weights, Tensor<>& inputs)
	{
		size_t batch_size = inputs.dims.back();
		activations.Resize({weights.dims[1], batch_size});
		for (size_t b = 0; b < batch_size; b++)
			for (size_t o = 0; o < weights.dims[1]; o++)
				Tensor<>::InnerProduct(activations[{o, b}],
									   weights[{0, o}],
									   inputs[{0, b}],
									   weights.dims[0]);
	}

	static void Error(Tensor<>& errors, Tensor<>& output, Tensor<>& targets)
	{
		errors.Resize(targets.dims);
		Tensor<>::Subtract(errors[{}],
						   output[{}],
						   targets[{}],
						   errors.Volume());
	}

	static void GradientDelta(Tensor<>& gradients_delta, Tensor<>& gradients, Tensor<>& output_delta)
	{
		gradients_delta.Resize(gradients.dims);
		Tensor<>::Multiply(gradients_delta[{}],
						   gradients[{}],
						   output_delta[{}],
						   gradients_delta.Volume());
	}

	static void WeightsDelta(Tensor<>& weights_gradient, Tensor<>& inputs, Tensor<>& gradients_delta)
	{
		size_t batch_size = inputs.dims.back();
		weights_gradient.Resize({inputs.Volume(-1), gradients_delta.Volume(-1)});
		for (size_t b = 0; b < batch_size; b++)
			Tensor<>::OuterProductAdditive(weights_gradient[{}],
										   gradients_delta[{0, b}],
										   weights_gradient.dims[1],
										   inputs[{0, b}],
										   weights_gradient.dims[0]);
	}

	static void SGDUpdate(Tensor<>& weights, Tensor<>& weights_gradient, double alpha)
	{
		for (size_t i = 0; i < weights.Volume(); i++)
		{
			weights.memory[i] -= alpha * weights_gradient.memory[i];
			weights_gradient.memory[i] = 0;
		}
	}

	static void RMSUpdate(Tensor<>& weights, Tensor<>& weights_gradient, Tensor<>& weights_gradient_rms, double alpha)
	{
		const double gamma = 0.9;
		weights_gradient_rms.Resize(weights_gradient.dims, 0.00001);

		for (size_t i = 0; i < weights.Volume(); i++)
		{
			weights_gradient_rms.memory[i] = gamma * weights_gradient_rms.memory[i] + (1 - gamma) * std::pow(weights_gradient.memory[i], 2);
			double learning_rate = alpha / std::sqrt(weights_gradient_rms.memory[i]);
			weights.memory[i] -= learning_rate * weights_gradient.memory[i];
			weights_gradient.memory[i] = 0;
		}
	}

	static void ADUpdate(Tensor<>& weights, Tensor<>& weights_gradient, Tensor<>& weights_gradient_rms, Tensor<>& weights_delta_rms)
	{
		const double gamma = 0.9;
		weights_gradient_rms.Resize(weights_gradient.dims, 0.00001);
		weights_delta_rms.Resize(weights_gradient.dims, 0.00001);

		for (size_t i = 0; i < weights.Volume(); i++)
		{
			weights_gradient_rms.memory[i] = gamma * weights_gradient_rms.memory[i] + (1 - gamma) * std::pow(weights_gradient.memory[i], 2);
			double learning_rate = std::sqrt(weights_delta_rms.memory[i]) / std::sqrt(weights_gradient_rms.memory[i]);
			double weight_delta = -learning_rate * weights_gradient.memory[i];
			//printf("%f + %f\n", gamma * weights_delta_rms.memory[i], (1 - gamma) *  std::pow(weight_delta, 2));
			weights_delta_rms.memory[i] = gamma * weights_delta_rms.memory[i] + (1 - gamma) *  std::pow(weight_delta, 2);
			weights.memory[i] += weight_delta;
			weights_gradient.memory[i] = 0;
		}
	}

	static void NormalizeWeights(Tensor<>& weights)
	{
		for (size_t o = 0; o < weights.dims[1]; o++)
			Tensor<>::NormalizeToUnitCircle(weights[{0, o}], weights.Volume(1));
	}

	static void DeltaBack(Tensor<>& input_delta, Tensor<>& weights, Tensor<>& output_delta)
	{
		size_t batch_size = output_delta.dims.back();
		input_delta.Resize({weights.dims[0], batch_size});
		for (size_t b = 0; b < batch_size; b++)
			for (size_t i = 0; i < weights.dims[0]; i++)
				Tensor<>::InnerProduct(input_delta[{i, b}],
									   weights[{i}],
									   output_delta[{0, b}],
									   weights.dims[1]);
	}

	static void Mean(Tensor<>& mean, Tensor<>& batch)
	{
		mean.Resize({batch.Volume(-1)});
		for (size_t i = 0; i < batch.dims.back(); i++)
			Tensor<>::Add(mean[{}], mean[{}], batch[{0, i}], mean.Volume());
		mean /= batch.dims.back();
	}

	static void Variance(Tensor<>& variance, Tensor<>& mean, Tensor<>& batch)
	{
		variance.Resize({batch.Volume(-1)});
		for (size_t i = 0; i < batch.dims.back(); i++)
			Tensor<>::Deviation(variance[{}], batch[{0, i}], mean[{}], variance.Volume());
		variance /= (batch.dims.back() - 1);
	}

	static void Standardize(Tensor<>& variance, Tensor<>& mean, Tensor<>& batch)
	{
		Tensor<>::Sqrt(variance[{}], variance[{}], variance.Volume());
		for (size_t i = 0; i < batch.dims.back(); i++)
		{
			Tensor<>::Subtract(batch[{0, i}], batch[{0, i}], mean[{}], mean.Volume());
			Tensor<>::Divide(batch[{0, i}], batch[{0, i}], variance[{}], variance.Volume());
		}
	}
};
