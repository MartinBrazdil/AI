#pragma once

#include <iostream>
#include <vector>
#include <array>
#include <algorithm>
#include <random>
#include <chrono>
#include <fstream>
#include <string>

#include "Neurons.h"
#include "ILayer.h"

template <class Neuron>
class Convolution : public ILayer
{
public:
	std::mt19937 rng{unsigned int(std::chrono::system_clock::now().time_since_epoch().count())};

	const size_t IX, IY, Z, Wx, Wy, F;

	const size_t WX = IX / Wx;
	const size_t WY = IY / Wy;
	
	double alpha;
	size_t batch_size, batch_counter{0};

	Tensor<> bias{{1}};
	Tensor<> bias_weights{{F}};
	Tensor<> bias_weights_delta{{F}};
	Tensor<> weights{{Wx, Wy, Z, F}};
	Tensor<> weights_delta{{Wx * Wy * Z, F}};

	using Definition = std::tuple<size_t, size_t, size_t, size_t, size_t, size_t, double, size_t>;
	Definition definition;

	Convolution(Definition def)
		: Convolution{std::get<0>(def), std::get<1>(def), std::get<2>(def), std::get<3>(def), std::get<4>(def), std::get<5>(def), std::get<6>(def), std::get<7>(def)}
	{}

	Convolution(size_t IX, size_t IY, size_t Z, size_t Wx, size_t Wy, size_t F, double alpha, size_t batch_size)
		: IX{IX}, IY{IY}, Z{Z}, Wx{Wx}, Wy{Wy}, F{F}, alpha{alpha}, batch_size{batch_size}, definition{IX, IY, Z, Wx, Wy, F, alpha, batch_size}
	{
		Reset();
	}

	~Convolution()
	{}

	Convolution* CloneRandomize()
	{
		return new Convolution(definition);
	}

	Convolution* CloneExactly()
	{
		return new Convolution(*this);
	}

	void Reset() override
	{
		bias.memory[0] = 1.0;
		std::normal_distribution<double> normal(0, 0.01);
		double weight_bound = std::sqrt(6.0 / (Wx * Wy * Z + F));
		std::uniform_real<double> weight_distribution(-weight_bound, weight_bound);
		for (size_t i = 0; i < weights.Volume(); i++)
			weights.memory[i] = normal(rng);
		double bias_weight_bound = std::sqrt(6.0 / F);
		std::uniform_real<double> bias_weight_distribution(-bias_weight_bound, bias_weight_bound);
		for (size_t i = 0; i < bias_weights.Volume(); i++)
			bias_weights.memory[i] = normal(rng);
	}

	Tensor<> Convolve(Tensor<>& input)
	{
		Tensor<> windows({Wx, Wy, Z, WX, WY});
		for (size_t z = 0; z < Z; z++)
			for (size_t y = 0; y < IY; y++)
				for (size_t x = 0; x < IX; x += Wx)
				{
					auto image_it = input[{x, y, z}];
					auto window_it = windows[{x % Wx, y % Wy, z, x / Wx, y / Wy}];

					std::copy(image_it, image_it + Wx, window_it);
				}

		return windows;
	}

	Tensor<> Deconvolve(Tensor<>& windows)
	{
		Tensor<> input({IX, IY, Z});
		for (size_t z = 0; z < Z; z++)
			for (size_t wY = 0; wY < WY; wY++)
				for (size_t wy = 0; wy < Wy; wy++)
					for (size_t wX = 0; wX < WX; wX++)
					{
						auto input_it = input[{wX * Wx, wY * Wy + wy, z}];
						auto windows_it = windows[{0, wy, z, wX, wY}];

						std::copy(windows_it, windows_it + Wx, input_it);
					}

		return input;
	}

	ResponseAndGradient Feedforward(Tensor<>& input) override
	{
		Tensor<> response = Response(Activation(Convolve(input)));
		Tensor<> gradient = Gradient(response);

		return{response, gradient};
	}

	Tensor<> Backprop(Tensor<>& input, Tensor<>& gradient, Tensor<>& delta) override
	{
		WeightsDelta(Convolve(input), gradient, delta);
		return Deconvolve(InputDelta(delta));
	}

	Tensor<> Activation(Tensor<>& windows) override
	{
		Tensor<> activation{{WX, WY, F}};
		for (size_t f = 0; f < F; f++)
			for (size_t wY = 0; wY < WY; wY++)
				for (size_t wX = 0; wX < WX; wX++)
				{
					Tensor<>::InnerProduct(activation[{wX, wY, f}],
										   weights[{0, 0, 0, f}],
										   windows[{0, 0, 0, wX, wY}],
										   windows.Volume(3));

					Tensor<>::Broadcast(activation[{wX, wY, f}],
										bias_weights[{f}],
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
		Tensor<> windows = Convolve(input);
		Tensor<> response = Response(Activation(windows));
		Tensor<> gradient = Gradient(response);
		auto reflection = AutoDecode(response);
		auto auto_delta = AutoError(input, reflection);
		Tensor<> delta = AutoDelta(auto_delta);
		WeightsDelta(windows, gradient, delta);
		UpdateWeights();
	}

	DecodedAndBias AutoDecode(Tensor<>& response) override
	{
		Tensor<> bias({1});
		Tensor<> windows({Wx, Wy, Z, WX, WY});
		for (size_t f = 0; f < F; f++)
			for (size_t wY = 0; wY < WY; wY++)
				for (size_t wX = 0; wX < WX; wX++)
				{
					Tensor<>::Broadcast(windows[{0, 0, 0, wX, wY}],
										weights[{0, 0, 0, f}],
										*response[{wX, wY, f}],
										windows.Volume(3));

					Tensor<>::InnerProduct(bias[{}],
										   bias_weights[{f}],
										   response[{wX, wY, f}],
										   1);
				}
		return{windows, bias};
	}

	DecodedAndBiasError AutoError(Tensor<>& input, DecodedAndBias& decodedAndBias) override
	{
		Tensor<> windows = Convolve(input);

		Tensor<> bias_error({1});
		Tensor<> windows_error({Wx, Wy, Z, WX, WY});

		Tensor<>::Subtract(windows_error[{}],
						   decodedAndBias.input[{}],
						   windows[{}],
						   windows_error.Volume());

		Tensor<>::Subtract(bias_error[{}],
						   decodedAndBias.bias[{}],
						   bias[{}],
						   bias_error.Volume());

		return{windows_error, bias_error};
	}

	Tensor<> Error(Tensor<> output, Tensor<> target) override
	{
		Tensor<> error({Wx, Wy, Z});
		Tensor<>::Subtract(error[{}],
						   output[{}],
						   target[{}],
						   error.Volume());
		return error;
	}

	Tensor<> AutoDelta(DecodedAndBiasError& auto_delta) override
	{
		Tensor<> delta{{WX, WY, F}};
		for (size_t f = 0; f < F; f++)
			for (size_t wY = 0; wY < WY; wY++)
				for (size_t wX = 0; wX < WX; wX++)
				{
					Tensor<>::InnerProduct(delta[{wX, wY, f}],
										   weights[{0, 0, 0, f}],
										   auto_delta.input[{0, 0, 0, wX, wY}],
										   auto_delta.input.Volume(3));

					Tensor<>::Broadcast(delta[{wX, wY, f}],
										bias_weights[{f}],
										*auto_delta.bias[{}],
										1);
				}

		return delta;
	}

	void WeightsDelta(Tensor<>& windows, Tensor<>& gradient, Tensor<>& delta) override
	{
		Tensor<> gradient_delta({WX, WY, F});
		Tensor<>::Multiply(gradient_delta[{}],
						   gradient[{}],
						   delta[{}],
						   gradient_delta.Volume());

		for (size_t f = 0; f < F; f++)
			for (size_t wY = 0; wY < WY; wY++)
				for (size_t wX = 0; wX < WX; wX++)
					Tensor<>::Broadcast(weights_delta[{0, f}],
										windows[{0, 0, 0, wX, wY}],
										*gradient_delta[{wX, wY, f}],
										windows.Volume(3));

		for (size_t f = 0; f < F; f++)
			for (size_t wY = 0; wY < WY; wY++)
				for (size_t wX = 0; wX < WX; wX++)
					Tensor<>::Broadcast(bias_weights_delta[{f}],
										bias[{}],
										*gradient_delta[{wX, wY, f}],
										1);
	}

	void UpdateWeights() override
	{
		if (++batch_counter == batch_size)
		{
			double learning_rate = alpha / (WX * WY * Z * batch_size);

			//printf("Weight norm before: %f\n", Tensor<>::L2Norm(weights[{}], weights.Volume()));

			for (size_t i = 0; i < weights.Volume(); i++)
			{
				weights.memory[i] -= learning_rate * weights_delta.memory[i];
				weights_delta.memory[i] = 0;
			}
			//Tensor<>::Normalize(weights[{}], weights.Volume());

			for (size_t f = 0; f < weights.dims[3]; f++)
				weights.NormalizeToUnitCircle();
		
			//printf("Weight norm after: %f\n", Tensor<>::L2Norm(weights[{}], weights.Volume()));

			for (size_t i = 0; i < bias_weights.Volume(); i++)
			{
				bias_weights.memory[i] -= learning_rate * bias_weights_delta.memory[i];
				bias_weights_delta.memory[i] = 0;
			}
			bias_weights.NormalizeToUnitCircle();

			batch_counter = 0;
		}
	}

	Tensor<> InputDelta(Tensor<>& delta) override
	{
		delta.Reshape({WX, WY, F});

		//printf("ConvDelta norm output: %f\n", Tensor<>::L2Norm(delta[{}], delta.Volume()));

		Tensor<> windows_delta({Wx, Wy, Z, WX, WY});
		for (size_t f = 0; f < F; f++)
			for (size_t wY = 0; wY < WY; wY++)
				for (size_t wX = 0; wX < WX; wX++)
					Tensor<>::Broadcast(windows_delta[{0, 0, 0, wX, wY}],
										weights[{0, 0, 0, f}],
										*delta[{wX, wY, f}],
										weights.Volume(3));

		//printf("ConvDelta norm input: %f\n", Tensor<>::L2Norm(windows_delta[{}], windows_delta.Volume()));

		for (size_t f = 0; f < windows_delta.Volume(); f++)
			windows_delta.memory[f] /= F;

		return windows_delta;
	}

	void PrintReceptiveField(std::string path)
	{
		for (size_t f = 0; f < weights.dims[3]; f++)
		{
			for (size_t z = 0; z < weights.dims[2]; z++)
			{
				std::string modified_path = path + std::string("receptive_field_f") + std::to_string(f) + std::string("_z") + std::to_string(z);
				//Tensor<>::Normalize(weights[{0, 0, 0, f}], weights.Volume(3));
				Tensor<>::PrintToFile(modified_path, weights[{0, 0, z, f}], weights.Volume(2));
			}
		}
	}
};
