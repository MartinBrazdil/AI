#pragma once

#include "Tensor.h"

class ILayer
{
public:
	virtual ~ILayer() {}

	virtual void Reset() = 0;

	virtual ILayer* CloneRandomize() = 0;
	virtual ILayer* CloneExactly() = 0;

	struct ResponseAndGradient { Tensor<> response, gradient; };
	virtual ResponseAndGradient Feedforward(Tensor<>& input) = 0;

	virtual Tensor<> Activation(Tensor<>& input) = 0;

	virtual Tensor<> Response(Tensor<>& activation) = 0;

	virtual Tensor<> Gradient(Tensor<>& response) = 0;
	
	virtual void Autoencode(Tensor<>& input) = 0;

	struct DecodedAndBias { Tensor<> input, bias; };
	virtual DecodedAndBias AutoDecode(Tensor<>& response) = 0;

	struct DecodedAndBiasError { Tensor<> input, bias; };
	virtual DecodedAndBiasError AutoError(Tensor<>& input, DecodedAndBias& decodedAndBias) = 0;

	virtual Tensor<> Error(Tensor<> output, Tensor<> target) = 0;

	virtual Tensor<> AutoDelta(DecodedAndBiasError& auto_delta) = 0;

	virtual void WeightsDelta(Tensor<>& input, Tensor<>& gradient, Tensor<>& delta) = 0;

	virtual Tensor<> InputDelta(Tensor<>& delta) = 0;

	virtual Tensor<> Backprop(Tensor<>& input, Tensor<>& gradient, Tensor<>& delta) = 0;
	
	virtual void UpdateWeights() = 0;

	virtual void PrintReceptiveField(std::string directory) = 0;
};
