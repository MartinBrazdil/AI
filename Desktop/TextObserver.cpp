#include "TextObserver.h"

#include "Window.h"

using namespace std;

namespace AI
{
	TextObserver::TextObserver(GetText getter)
		: getter{getter}
	{
		Update();
	}

	Window& TextObserver::GetWindow()
	{
		return text_window;
	}

	void TextObserver::Update()
	{
		if (text != getter())
		{
			text = getter();
			text_window.Write(text);
		}
	}
}
