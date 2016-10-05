#pragma once

#include <SDL.h>
#include <SDL_ttf.h>
#include <string>
#include <vector>
#include <map>
using namespace std;

#include "Window.h"
#include "Group.h"
#include "Observer.h"
#include "Rectangle.h"

namespace AI
{
	class Desktop
	{
	public:
		static TTF_Font* consolas;

		string active_path{""};
		Group desktop{""};

		SDL_Window* window;
		SDL_Renderer* renderer;
		SDL_DisplayMode display_mode;

		SDL_Texture* background_texture;
		SDL_Texture* desktop_texture;

		Desktop(int display_idx = 0);
		~Desktop();

		Group& operator[](string path_str);

		void HandleInput();
		void Run();

		void Order();

		bool UpdateAndRender(Group& group);

		void RenderWindow(Window& window);
		void RenderEnvelope(Window& window);

		void Render(string path_str="");
	};
}
