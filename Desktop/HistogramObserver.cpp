#include "HistogramObserver.h"

#include "Window.h"
#include "Histogram.h"
using namespace std;

namespace AI
{
	HistogramObserver::HistogramObserver(GetBins getter)
		: getter{getter}, histogram{100, 40}
	{
		Update();
	}

	Window& HistogramObserver::GetWindow()
	{
		return histogram.window;
	}

	void HistogramObserver::Update()
	{
		histogram.Plot(getter());
	}
}
