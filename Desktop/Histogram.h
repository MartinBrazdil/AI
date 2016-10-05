#pragma once

#include <vector>
#include <cmath>
#include <limits>
using namespace std;

#include "Window.h"

namespace AI
{
	class Histogram
	{
	public:
		Window window;
		Uint32 color{0x00FF00};
		Uint32 black{0x000000};

		Histogram(int w, int h);

		void Plot(vector<double> bins);
	};
}
