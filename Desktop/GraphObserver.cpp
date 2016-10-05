#include "GraphObserver.h"

namespace AI
{
	GraphObserver::GraphObserver(GetNumber getter)
		: getter{getter}, graph{100, 40, false}
	{
		Update();
	}

	Window& GraphObserver::GetWindow()
	{
		return graph.window;
	}

	void GraphObserver::Update()
	{
		double value = getter();

		if (!isnan(value))
			graph.PlotNext(value);
	}
}
