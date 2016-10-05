#pragma once

#include <string>
using namespace std;

#include "Window.h"

namespace AI
{
	class Text : public Window
	{
	public:
		static const SDL_Color white;
		static const SDL_Color black;

		string text;

		Text() : Text("") {}
		Text(string text, SDL_Color color = white);

		~Text() override {};

		bool operator<(const Text& rhs);
		bool operator<(const string& rhs);

		void Write(string text, SDL_Color color = white);
	};
};
