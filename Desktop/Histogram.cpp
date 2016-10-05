#include "Histogram.h"

#include "Core.h"
#include "Rectangle.h"

namespace AI
{
	Histogram::Histogram(int w, int h)
		: window{w, h}
	{
	}

	void Histogram::Plot(vector<double> bins)
	{
		window.Clear();

		int bin_width = window.front_buffer->w / bins.size();

		double min = numeric_limits<double>::max();
		double max = numeric_limits<double>::lowest();

		for (int i = 0; i < bins.size(); ++i)
		{
			double y = bins[i];
			if (y < min) min = y;
			if (y > max) max = y;
		}

		SDL_Rect* bars = new SDL_Rect[bins.size()];
		for (int i = 0; i < bins.size(); ++i)
		{
			int bin_height = int((double)window.front_buffer->h * (bins[i] - min) / (max - min));
			bars[i] = {i * bin_width, window.front_buffer->h - bin_height, bin_width, bin_height};
		}
		SDL_FillRects(window.front_buffer, bars, bins.size(), color);
		delete[] bars;
	}
}
