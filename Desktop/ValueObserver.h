#pragma once

#include <functional>
#include <string>
#include <vector>
using namespace std;

#include "Observer.h"
#include "Text.h"

namespace AI
{
	template <class T>
	using GetValue = function<vector<vector<T>>()>;

	template <class T>
	class ValueObserver : public Observer
	{
	public:
		T value;
		Text text_window;

		GetValue<T> getter;

		ValueObserver(GetValue<T> getter);
		~ValueObserver() override {};

		Window& GetWindow() override;
		void Update() override;
	};
}
