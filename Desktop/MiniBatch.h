#pragma once

#include "Tensor.h"

#include <vector>
#include <memory>

template <class T = double>
class MiniBatch
{
	Tensor<> batch{{0,0}};
	Tensor<> mean, variance;

	Tensor<> operator[](std::vector<size_t> idxs)
	{

	}

	//void Clear()
	//{
	//	batch.Resize({batch.dims[0], 0});
	//}

	void Insert(Tensor<>& input)
	{
		batch.Resize({input.Volume(), batch.dims[1] + 1});
		Tensor<>::Copy(batch[{0, batch.dims[1] - 1}], input[{}], input.Volume());
	}

	void Mean(Tensor<>& mean)
	{
		mean.Resize({batch.dims[0]});

		for (size_t i = 0; i < batch.dims[1]; i++)
			Tensor<>::Add(mean[{}], mean[{}], batch[{0, i}], mean.Volume());
	}

	void Variance(Tensor<>& variance, Tensor<>& mean)
	{
		variance.Resize(mean.dims);

		for (size_t i = 0; i < batch.dims[1]; i++)
			Tensor<>::SquaredDifference(variance[{}], batch[{0, i}], mean[{}]);
	}

	void Normalize()
	{
		Mean(mean);
		Variance(variance, mean);

		for (size_t i = 0; i < batch.dims[1]; i++)
		{
			Tensor<>::Subtract(batch[{0, i}], batch[{0, i}], mean[{}], mean.Volume());
			Tensor<>::Divide(batch[{0, i}], batch[{0, i}], variance[{}], variance.Volume());
		}
	}
};
