#pragma once

#include "Tensor.h"

struct Identity
{
	static double Response(double x)
	{
		return x;
	}

	static void Response(Tensor<>& responses, Tensor<>& activations)
	{
		responses.Resize(activations.dims);
		return activations.Apply<Response>(responses);
	}

	static double Gradient(double x)
	{
		return 1;
	}

	static void Gradient(Tensor<>& gradients, Tensor<>& responses)
	{
		gradients.Resize(responses.dims);
		return responses.Apply<Gradient>(gradients);
	}
};

struct Logistic
{
	static double Response(double x)
	{
		return 1.0 / (1.0 + exp(-(x / 1.0)));
	}

	static void Response(Tensor<>& responses, Tensor<>& activations)
	{
		responses.Resize(activations.dims);
		return activations.Apply<Response>(responses);
	}

	static double Gradient(double x)
	{
		return x * (1 - x);
	}

	static void Gradient(Tensor<>& gradients, Tensor<>& responses)
	{
		gradients.Resize(responses.dims);
		return responses.Apply<Gradient>(gradients);
	}
};

struct LecunTanh
{
	static double Response(double x)
	{
		return 1.7159 * tanh((2.0 * x) / 3.0);
	}

	static void Response(Tensor<>& responses, Tensor<>& activations)
	{
		responses.Resize(activations.dims);
		return activations.Apply<Response>(responses);
	}

	static double Gradient(double x)
	{
		x = (2.0 * x) / 3.0;
		x = 2 / (exp(x) + exp(-x));
		return 1.14393 * pow(x, 2);
	}

	static void Gradient(Tensor<>& gradients, Tensor<>& responses)
	{
		gradients.Resize(responses.dims);
		return responses.Apply<Gradient>(gradients);
	}
};

struct ReLU
{
	static double Response(double x)
	{
		return std::max(0.0, x);
	}

	static void Response(Tensor<>& responses, Tensor<>& activations)
	{
		responses.Resize(activations.dims);
		return activations.Apply<Response>(responses);
	}

	static double Gradient(double x)
	{
		return x > 0 ? x : 0.01 * x;
	}

	static void Gradient(Tensor<>& gradients, Tensor<>& responses)
	{
		gradients.Resize(responses.dims);
		return responses.Apply<Gradient>(gradients);
	}
};

struct Softplus
{
	static double Response(double x)
	{
		return log(1.0 + exp(x));
	}

	static void Response(Tensor<>& responses, Tensor<>& activations)
	{
		responses.Resize(activations.dims);
		return activations.Apply<Response>(responses);
	}

	static double Gradient(double x)
	{
		return 1.0 / (1.0 + exp(-(x / 1.0)));
	}

	static void Gradient(Tensor<>& gradients, Tensor<>& responses)
	{
		gradients.Resize(responses.dims);
		return responses.Apply<Gradient>(gradients);
	}
};

struct Softmax
{
	static void Response(Tensor<>& responses, Tensor<>& activations)
	{
	}

	static void Gradient(Tensor<>& gradients, Tensor<>& responses)
	{
	}
};
