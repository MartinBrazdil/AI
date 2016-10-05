#pragma once

#include "Time.h"

#include <SDL.h>
#include <SDL_ttf.h>
#include <string>
#include <array>
#include <map>
using namespace std;

#include "Rectangle.h"

namespace AI
{
	class Window
	{
	public:
		using Size = array<int, 2>;
		using Position = array<int, 2>;
		Position position{0, 0};

		SDL_Surface* front_buffer{nullptr};

		Window() : Window(0, 0) {};
		Window(int w, int h);

		virtual ~Window();

		void SetSize(int width, int height);

		Rectangle GetRect();
		void SetRect(Rectangle& rect);

		Uint32 GetPixel(int x, int y);
		void SetPixel(int x, int y, Uint32 pixel);

		void Clear();
	};
}
