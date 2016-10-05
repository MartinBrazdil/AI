#pragma once

#include <iostream>
#include <iomanip>
#include <fstream>
#include <numeric>
#include <vector>
#include <algorithm>
#include <functional>
#include <random>
#include <chrono>
#include <fstream>
#include <cmath>

template <class T = double>
class Tensor
{
public:
	using Iterator = typename std::vector<T>::iterator;

	std::vector<size_t> dims;
	std::vector<T> memory;

	Tensor()
		:dims{1}, memory(1)
	{}

	Tensor(std::vector<size_t> dims)
		: dims{dims}, memory(Volume())
	{}

	static void PrintToConsole(Iterator v_it, size_t size)
	{
		for (size_t i = 0; i < size; i++)
		{
			if (i % 12 == 0)
				printf("\n");
			printf("%.02f, ", *v_it++);
		}
		printf("\n");
	}

	void PrintToConsole()
	{
		for (size_t i = 0; i < Volume(); i++)
		{
			if (i % 12 == 0)
				printf("\n");
			printf("%.02f, ", memory[i]);
		}
		printf("\n");
	}

	static void PrintToFile(std::string path, Iterator v_it, size_t size)
	{
		std::ofstream file(path);
		for (size_t i = 0; i < size; i++)
			file << (*v_it++) << " ";
		file << std::endl;
		file.close();
	}

	void PrintToFile(std::string path)
	{
		std::ofstream file(path);
		for (size_t i = 0; i < Volume(); i++)
			file << memory[i] << " ";
		file << std::endl;
		file.close();
	}

	void Fill(T value)
	{
		for (size_t i = 0; i < memory.size(); i++)
			memory[i] = value;
	}

	size_t Volume(int N) const
	{
		N = N % dims.size();
		N = 0 < N ? N : N + dims.size();
		size_t volume = 1;
		for (size_t i = 0; i < N; i++)
			volume *= dims[i];
		return volume;
	}

	size_t Volume() const
	{
		return Volume(dims.size());
	}

	Iterator operator[](std::vector<size_t> idxs)
	{
		if (idxs.empty())
			return memory.begin();

		size_t offset = idxs[0];
		size_t dim_size = dims[0];
		for (size_t i = 1; i < idxs.size(); i++)
		{
			offset += idxs[i] * dim_size;
			dim_size *= dims[i];
		}
		return memory.begin() + offset;
	}

	bool Resize(std::vector<size_t> dims, T value = 0)
	{
		if (this->dims != dims)
		{
			this->dims = dims;
			memory.resize(Volume(), value);
			return true;
		}
		return false;
	}

	void Reshape(std::vector<size_t> dims)
	{
		size_t volume = Volume();
		this->dims = dims;
		if (volume != Volume())
			throw "Cannot reshape to different volume";
	}

	void Transpose(Tensor<> out, size_t n)
	{
		std::vector<size_t> out_dims(dims.size());
		std::copy(dims.begin(), dims.begin() + n, out_dims.begin());
		std::copy(dims.begin() + n, dims.end(), out_dims.begin() + n);


	}

	static void Add(Iterator out_it, Iterator v1_it, Iterator v2_it, size_t size)
	{
		for (size_t i = 0; i < size; i++)
		{
			T& v1 = (*v1_it++);
			T& v2 = (*v2_it++);
			T& out = (*out_it++);
			out = v1 + v2;
		}
	};

	static void Add(Iterator out_it, Iterator v1_it, T value, size_t size)
	{
		for (size_t i = 0; i < size; i++)
		{
			T& v1 = (*v1_it++);
			T& out = (*out_it++);
			out = v1 + value;
		}
	};

	Tensor<T> operator+(const Tensor<>& rhs)
	{
		Tensor<> out(dims);
		for (size_t i = 0; i < Volume(); i++)
			out.memory[i] = memory[i] + rhs.memory[i];
		return out;
	}

	Tensor<T>& operator+=(const Tensor<>& rhs)
	{
		for (size_t i = 0; i < Volume(); i++)
			memory[i] += rhs.memory[i];
		return *this;
	}

	Tensor<T> operator+(const T& rhs)
	{
		Tensor<> out(dims);
		for (size_t i = 0; i < Volume(); i++)
			out.memory[i] = memory[i] + rhs;
		return out;
	}

	Tensor<T>& operator+=(const T& rhs)
	{
		for (size_t i = 0; i < Volume(); i++)
			memory[i] += rhs;
		return *this;
	}

	static void Subtract(Iterator out_it, Iterator v1_it, Iterator v2_it, size_t size)
	{
		for (size_t i = 0; i < size; i++)
		{
			T& v1 = (*v1_it++);
			T& v2 = (*v2_it++);
			T& out = (*out_it++);
			out = v1 - v2;
		}
	};

	static void Subtract(Iterator out_it, Iterator v_it, T value, size_t size)
	{
		for (size_t i = 0; i < size; i++)
		{
			T& v = (*v_it++);
			out = v - value;
		}
	};

	Tensor<T> operator-(const Tensor<>& rhs)
	{
		Tensor<> out(dims);
		for (size_t i = 0; i < Volume(); i++)
			out.memory[i] = memory[i] - rhs.memory[i];
		return out;
	}

	Tensor<T>& operator-=(const Tensor<>& rhs)
	{
		for (size_t i = 0; i < Volume(); i++)
			memory[i] -= rhs.memory[i];
		return *this;
	}

	Tensor<T> operator-(const T& rhs)
	{
		Tensor<> out(dims);
		for (size_t i = 0; i < Volume(); i++)
			out.memory[i] = memory[i] - rhs;
		return out;
	}

	Tensor<T>& operator-=(const T& rhs)
	{
		for (size_t i = 0; i < Volume(); i++)
			memory[i] -= rhs;
		return *this;
	}

	static void Multiply(Iterator out_it, Iterator v1_it, Iterator v2_it, size_t size)
	{
		for (size_t i = 0; i < size; i++)
		{
			T& v1 = (*v1_it++);
			T& v2 = (*v2_it++);
			T& out = (*out_it++);
			out = v1 * v2;
		}
	};

	static void Multiply(Iterator out_it, Iterator v_it, T value, size_t size)
	{
		for (size_t i = 0; i < size; i++)
		{
			T& v = (*v_it++);
			out = v * value;
		}
	};

	Tensor<T> operator*(const Tensor<>& rhs)
	{
		Tensor<> out(dims);
		for (size_t i = 0; i < Volume(); i++)
			out.memory[i] = memory[i] * rhs.memory[i];
		return out;
	}

	Tensor<T>& operator*=(const Tensor<>& rhs)
	{
		for (size_t i = 0; i < Volume(); i++)
			memory[i] *= rhs.memory[i];
		return *this;
	}

	Tensor<T> operator*(const T& rhs)
	{
		Tensor<> out(dims);
		for (size_t i = 0; i < Volume(); i++)
			out.memory[i] = memory[i] * rhs;
		return out;
	}

	Tensor<T>& operator*=(const T& rhs)
	{
		for (size_t i = 0; i < Volume(); i++)
			memory[i] *= rhs;
		return *this;
	}

	static void Divide(Iterator out_it, Iterator v1_it, Iterator v2_it, size_t size)
	{
		for (size_t i = 0; i < size; i++)
		{
			T& v1 = (*v1_it++);
			T& v2 = (*v2_it++);
			T& out = (*out_it++);
			out = v1 / v2;
		}
	};

	static void Divide(Iterator out_it, Iterator v_it, T value, size_t size)
	{
		for (size_t i = 0; i < size; i++)
		{
			T& v = (*v_it++);
			out = v / value;
		}
	};

	Tensor<T> operator/(const Tensor<>& rhs)
	{
		Tensor<> out(dims);
		for (size_t i = 0; i < Volume(); i++)
			out.memory[i] = memory[i] / rhs.memory[i];
		return out;
	}

	Tensor<T>& operator/=(const Tensor<>& rhs)
	{
		for (size_t i = 0; i < Volume(); i++)
			memory[i] /= rhs.memory[i];
		return *this;
	}

	Tensor<T> operator/(const T& rhs)
	{
		Tensor<> out(dims);
		for (size_t i = 0; i < Volume(); i++)
			out.memory[i] = memory[i] / rhs;
		return out;
	}

	Tensor<T>& operator/=(const T& rhs)
	{
		for (size_t i = 0; i < Volume(); i++)
			memory[i] /= rhs;
		return *this;
	}

	static bool CheckClass(Iterator output, Iterator binary, size_t size)
	{
		size_t max_idx = 0;
		T max = std::numeric_limits<T>::min();

		for (size_t i = 0; i < size; i++)
		{
			T& out = (*output++);

			if (max < out)
			{
				max = out;
				max_idx = i;
			}
		}
		return *(binary + max_idx) == 1.0;
	}

	void NormalizeToInterval(T min = 0, T max = 1)
	{
		T min_val = std::numeric_limits<T>::max();
		T max_val = std::numeric_limits<T>::min();

		for (size_t i = 0; i < Volume(); i++)
		{
			min_val = std::min(min_val, memory[i]);
			max_val = std::max(max_val, memory[i]);
		}

		for (size_t i = 0; i < Volume(); i++)
			memory[i] = (max - min) * ((memory[i] - min_val) / (max_val - min_val)) + min;
	}

	static void Deviation(Iterator out_it, Iterator v1_it, Iterator v2_it, size_t size)
	{
		for (size_t i = 0; i < size; i++)
		{
			T& out = (*out_it++);
			T& v1 = (*v1_it++);
			T& v2 = (*v2_it++);

			out += std::pow(v1 - v2, 2);
		}
	}

	static void Sqrt(Iterator out_it, Iterator v_it, size_t size)
	{
		for (size_t i = 0; i < size; i++)
		{
			T& out = (*out_it++);
			T& v = (*v_it++);

			out = std::sqrt(v);
		}
	}

	T L2Norm()
	{
		T norm = 0;
		for (size_t i = 0; i < memory.size(); i++)
			norm += std::pow(memory[i], 2);
		return std::sqrt(norm);
	}

	static T L2Norm(Iterator v_it, size_t size)
	{
		T norm = 0;
		for (size_t i = 0; i < size; i++)
		{
			T& v = (*v_it++);
			norm += std::pow(v, 2);
		}
		return std::sqrt(norm);
	}

	void NormalizeToUnitCircle()
	{
		T norm = L2Norm();
		for (size_t i = 0; i < memory.size(); i++)
			memory[i] /= norm;
	}

	static void NormalizeToUnitCircle(Iterator v_it, size_t size)
	{
		T norm = L2Norm(v_it, size);
		for (size_t i = 0; i < size; i++)
		{
			T& v = (*v_it++);
			v /= norm;
		}
	}

	using Function = T(*)(T);
	template <Function function>
	void Apply(Tensor<>& output)
	{
		for (size_t i = 0; i < memory.size(); i++)
			output.memory[i] = function(memory[i]);
	}

	static void Copy(Iterator v_out, Iterator v_it, size_t size)
	{
		std::copy(v_it, v_it + size, v_out);
	}

	static void InnerProduct(Iterator out_it, Iterator v1_it, Iterator v2_it, size_t size)
	{
		T& out = (*out_it);
		//out = 0;
		for (size_t i = 0; i < size; i++)
		{
			T& v1 = (*v1_it++);
			T& v2 = (*v2_it++);

			out += v1 * v2;
		}
	}

	static void InnerProductAdditive(Iterator out_it, Iterator v1_it, Iterator v2_it, size_t size)
	{
		T& out = (*out_it);
		for (size_t i = 0; i < size; i++)
		{
			T& v1 = (*v1_it++);
			T& v2 = (*v2_it++);

			out += v1 * v2;
		}
	}

	static void OuterProduct(Iterator out_it, Iterator v1_it, size_t v1_size, Iterator v2_it, size_t v2_size)
	{
		auto v2_it_begin = v2_it;
		for (size_t i = 0; i < v1_size; i++)
		{
			T& v1 = (*v1_it++);
			v2_it = v2_it_begin;
			for (size_t j = 0; j < v2_size; j++)
			{
				T& v2 = (*v2_it++);
				T& out = (*out_it++);

				out = v1 * v2;
			}
		}
	}

	static void OuterProductAdditive(Iterator out_it, Iterator v1_it, size_t v1_size, Iterator v2_it, size_t v2_size)
	{
		auto v2_it_begin = v2_it;
		for (size_t i = 0; i < v1_size; i++)
		{
			T& v1 = (*v1_it++);
			v2_it = v2_it_begin;
			for (size_t j = 0; j < v2_size; j++)
			{
				T& v2 = (*v2_it++);
				T& out = (*out_it++);

				out += v1 * v2;
			}
		}
	}

	static void Broadcast(Iterator out_it, Iterator v_it, T value, size_t size)
	{
		for (size_t i = 0; i < size; i++)
		{
			T& v = (*v_it++);
			T& out = (*out_it++);

			out += v * value;
		}
	}
};


//void PrintImage(Tensor<> image)
//{
//	for (size_t i = 0; i < image.dims[0]; i++)
//	{
//		for (size_t j = 0; j < image.dims[1]; j++)
//			printf("%.2f, ", image.memory[i * image.dims[0] + j]);
//		printf("\n");
//	}
//	printf("\n");
//}
