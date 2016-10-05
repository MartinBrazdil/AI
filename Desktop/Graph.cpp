#include "Graph.h"

#include <vector>
#include <limits>
#include <cmath>
#include <algorithm>
using namespace std;

namespace AI
{
	Graph::Graph(int w, int h, bool colored)
		: window{w, h}, colored {colored}
	{
		min = actual_min = numeric_limits<double>::max();
		max = actual_max = numeric_limits<double>::lowest();
	}

	void Graph::LogToFile()
	{
	}

	void Graph::ClearSweepLine(int x)
	{
		const int c1 = 0x4C0000;
		const char r1 = (c1 >> 16);
		const char b1 = ((c1 >> 8) & 0x00FF);
		const char g1 = (c1 & 0x0000FF);

		const int c2 = 0x4c4c00;
		const char r2 = (c2 >> 16);
		const char b2 = ((c2 >> 8) & 0x00FF);
		const char g2 = (c2 & 0x0000FF);

		const char rd12 = r2 - r1;
		const char bd12 = b2 - b1;
		const char gd12 = g2 - g1;

		const int c3 = 0x001E00;
		const char r3 = (c3 >> 16);
		const char b3 = ((c3 >> 8) & 0x00FF);
		const char g3 = (c3 & 0x0000FF);

		const char rd23 = r3 - r2;
		const char bd23 = b3 - b2;
		const char gd23 = g3 - g2;

		double yUnit = (actual_max - actual_min) / window.front_buffer->w;
		for (int _y = 0; _y < window.front_buffer->h; ++_y)
		{
			if (colored)
			{
				int color;
				double valueAtY = actual_max - _y * yUnit;
				double t = std::min(1.0, (0.1 * min) / (valueAtY - min));
				if (t < 0.5)
				{
					char r = r1 + (2 * t) * rd12;
					char g = g1 + (2 * t) * gd12;
					char b = b1 + (2 * t) * bd12;
					color = g | (b << 8) | (r << 16);
				}
				else
				{
					char r = r2 + ((t - 0.5) / 2) * rd23;
					char g = g2 + ((t - 0.5) / 2) * gd23;
					char b = b2 + ((t - 0.5) / 2) * bd23;
					color = g | (b << 8) | (r << 16);
				}
				window.SetPixel(x, _y, color);
			}
			else
			{
				window.SetPixel(x, _y, black);
			}
		}
	}

	void Graph::UpdateBounds(double value)
	{
		actual_min = std::min(value, actual_min);
		min = std::min(value, min);
		actual_max = std::max(value, actual_max);
		max = std::max(value, max);
	}

	void Graph::Renormalize(double value)
	{
		// if no change return
		if (actual_min < value && value < actual_max)
			return;

		// update actual_min, actual_max, min, max
		UpdateBounds(value);

		// redraw curve
		for (int _x = 0; _x < curve.size(); ++_x)
		{
			// delete whole sweep line
			ClearSweepLine(_x);
			// skip to avoid connection between actual and history
			if (_x == x + 1)
				continue;
			// plot renormalized curve
			Plot(_x, curve[_x]);
		}
	}

	int Graph::Normalize(double value)
	{
		// h-1 inverts y axis second term is normalization
		return window.front_buffer->h - int((double)window.front_buffer->h * (value - actual_min) / (actual_max - actual_min));
	}

	void Graph::Plot(int x, double value)
	{
		// get graph y value
		int y = Normalize(value);

		if (x == 0)
		{
			window.SetPixel(x, y, color);
			return;
		}

		// previous y coord
		int _y = Normalize(curve[x - 1]);
		// difference between previous and this
		int d = _y - y;
		for (int i = y; i != _y; i += d / abs(d))
		{
			window.SetPixel(x, i, color);
		}
		// insert last value (skipped by i != _y)
		window.SetPixel(x, _y, color);
	}

	void Graph::PlotNext(double value)
	{
		// move sweep line to next position
		x = (x + 1) % window.front_buffer->w;
		// if at begining refresh actual_min and actual_max values
		// write curve value
		if (curve.size() < window.front_buffer->w)
			curve.push_back(value);
		else curve[x] = value;
		// check if renormalization is needed
		Renormalize(value);
		// delete whole sweep line
		ClearSweepLine(x);
		// plot next value
		Plot(x, value);
	}
}
