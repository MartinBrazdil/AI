#pragma once

#include <string>
using namespace std;

#include "Window.h"
#include "Tensor.h"

namespace AI
{
	class Pixels : public Window
	{
	public:
		Pixels() {};
		Pixels(Tensor<>& tensor);

		~Pixels() override {}

		void Write(Tensor<>& tensor);
	};
};
