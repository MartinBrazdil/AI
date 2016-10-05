#pragma once

#include "Time.h"
#include "Window.h"
#include "Rectangle.h"

namespace AI
{
	class Observer
	{
	public:
		timepoint_t timePrev;
		time_t delay{100};

		virtual ~Observer() {};

		virtual Window& GetWindow() = 0;
		virtual void Update() {};

		Rectangle GetRect()
		{
			Window& window = GetWindow();
			return window.GetRect();
		}

		void SetRect(Rectangle& rect)
		{
			Window& window = GetWindow();
			window.SetRect(rect);
		}
	};
}
