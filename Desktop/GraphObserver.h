#pragma once

#include <functional>
using namespace std;

#include "Window.h"
#include "Graph.h"
#include "Observer.h"

namespace AI
{
	using GetNumber = function<double()>;
	class GraphObserver : public Observer
	{
	public:
		Graph graph;
		GetNumber getter;

		GraphObserver(GetNumber getter);
		~GraphObserver() override {};

		Window& GetWindow() override;
		void Update() override;
	};
}
