#pragma once

#include <vector>
#include <limits>
using namespace std;

#include "Window.h"

namespace AI
{
	class Graph
	{
	public:
		Window window;
		Uint32 color{0x00FF00};
		Uint32 black{0x000000};

		int x{-1};
		double min, max;
		double actual_min, actual_max;
		bool colored;
		vector<double> curve;
		fstream* logFile;

		Graph(int w, int h, bool colored = false);

		void LogToFile();

		void UpdateBounds(double value);
		void ClearSweepLine(int x);
		void Renormalize(double value);
		int Normalize(double value);
		void Plot(int x, double value);
		void PlotNext(double value);
	};
}
