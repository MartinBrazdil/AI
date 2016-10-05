#pragma once

#include <functional>
#include <string>

using namespace std;

#include "Observer.h"
#include "Text.h"

namespace AI
{
	using GetText = std::function<std::string()>;
	class TextObserver : public Observer
	{
	public:
		Text text_window;
		string text;

		GetText getter;

		TextObserver(GetText getter);
		~TextObserver() override {};

		Window& GetWindow() override;
		void Update() override;
	};
}
