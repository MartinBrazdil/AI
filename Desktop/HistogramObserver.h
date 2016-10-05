#pragma once

#include <functional>
#include <vector>

#include "Histogram.h"
#include "Observer.h"

namespace AI
{
	using GetBins = std::function<std::vector<double>()>;
	class HistogramObserver : public Observer
	{
	public:
		Histogram histogram;
		GetBins getter;

		HistogramObserver(GetBins getter);
		~HistogramObserver() override {};

		Window& GetWindow() override;
		void Update() override;
	};
}
