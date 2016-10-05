#include "ValueObserver.h"

#include "Window.h"

namespace AI
{
	template <class T>
	ValueObserver<T>::ValueObserver(GetValue<T> getter)
		: Observer{owner}, getter{getter}
	{
		Update();
	}

	template <class T>
	Window& ValueObserver<T>::GetWindow()
	{
		return text_window;
	}

	template <class T>
	void ValueObserver<T>::Update()
	{
		if (value != getter())
			text_window.Write(to_string(getter()));
	}
}