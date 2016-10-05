#include "WindowObserver.h"

#include "Window.h"

using namespace std;

namespace AI
{
	WindowObserver::WindowObserver(_GetWindow getter)
		: getter{getter}
	{
	}

	Window& WindowObserver::GetWindow()
	{
		return getter();
	}
}