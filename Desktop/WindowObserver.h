#pragma once

#include "Observer.h"

#include <functional>
#include <string>

namespace AI
{
	class Window;

	using _GetWindow = std::function<Window&()>;
	class WindowObserver : public Observer
	{
	public:
		_GetWindow getter;

		WindowObserver(_GetWindow getter);
		~WindowObserver() override {};

		Window& GetWindow() override;
	};
}
