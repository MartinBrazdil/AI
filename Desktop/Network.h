#pragma once

#include <vector>
#include <stack>

#include "ILayer.h"
#include "Convolution.h"
#include "Perceptron.h"

class Network
{
public:
	std::vector<ILayer*> layers;

	Network() {}

	Network(Network&& rhs)
	{
		for (ILayer* layer : rhs.layers)
			layers.push_back(layer);
		rhs.layers.clear();
	}

	~Network()
	{
		for (ILayer* layer : layers)
			delete layer;
	}

	Network CloneRandomize()
	{
		Network network;
		for (ILayer* layer : layers)
			network.layers.push_back(layer->CloneRandomize());
		return network;
	}

	Network CloneExactly()
	{
		Network network;
		for (ILayer* layer : layers)
			network.layers.push_back(layer->CloneExactly());
		return network;
	}

	void Reset()
	{
		for (ILayer* layer : layers)
			layer->Reset();
	}

	Tensor<> Feedforward(Tensor<> input)
	{
		for (ILayer* layer : layers)
			input = layer->Feedforward(input).response;

		return input;
	}

	Tensor<> Learn(Tensor<> input, Tensor<> target)
	{
		std::stack<Tensor<>> inputs, gradients;
		inputs.push({input});

		for (ILayer* layer : layers)
		{
			auto output = layer->Feedforward(inputs.top());
			inputs.push(output.response);
			gradients.push(output.gradient);
		}

		Tensor<> delta = layers.back()->Error(inputs.top(), target);
		inputs.pop();

		for (auto layer_it = layers.rbegin(); layer_it != layers.rend(); layer_it++)
		{
			ILayer* layer = (*layer_it);
			delta = layer->Backprop(inputs.top(), gradients.top(), delta);
			layer->UpdateWeights();
			inputs.pop(), gradients.pop();
		}

		return delta;
	}

	//Tensor<> Autoencode(Tensor<> input, Tensor<> target)
	//{
	//	std::stack<Tensor<>> inputs, gradients;
	//	inputs.push({input});

	//	for (ILayer* layer : layers)
	//	{
	//		auto output = layer->Feedforward(inputs.top());
	//		auto decoded = layer->AutoDecode(output.response);
	//		auto error = layer->AutoError(inputs.top(), decoded);
	//		auto delta = layer->AutoDelta(error);
	//		layer->WeightsDelta(inputs.top(), output.gradient, delta);
	//		layer->UpdateWeights();
	//		inputs.push(output.response);
	//		gradients.push(output.gradient);
	//	}
	//}

	//Tensor<> LearnWithAutoencoders(Tensor<> input, Tensor<> target)
	//{
	//	std::stack<Tensor<>> inputs, gradients;
	//	inputs.push({input});

	//	for (ILayer* layer : layers)
	//	{
	//		auto output = layer->Feedforward(inputs.top());
	//		auto decoded = layer->AutoDecode(output.response);
	//		auto error = layer->AutoError(inputs.top(), decoded);
	//		auto delta = layer->AutoDelta(error);
	//		layer->WeightsDelta(inputs.top(), output.gradient, delta);
	//		inputs.push(output.response);
	//		gradients.push(output.gradient);
	//	}

	//	Tensor<> delta = layers.back()->Error(inputs.top(), target);
	//	inputs.pop();

	//	for (auto layer_it = layers.rbegin(); layer_it != layers.rend(); layer_it++)
	//	{
	//		ILayer* layer = (*layer_it);
	//		delta = layer->Backprop(inputs.top(), gradients.top(), delta);
	//		layer->UpdateWeights();
	//		inputs.pop(), gradients.pop();
	//	}

	//	return delta;
	//}
};
