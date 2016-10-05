#include "Text.h"

#include "Core.h"

namespace AI
{
	//const SDL_Color Text::white = SDL_MapRGB(window.front_buffer->format, 255, 255, 255);
	//const SDL_Color Text::black = SDL_MapRGB(window.front_buffer->format, 0, 0, 0);

	const SDL_Color Text::white{255, 255, 255};
	const SDL_Color Text::black{0, 0, 0};

	Text::Text(string text, SDL_Color color)
		: Window(), text{text}
	{
		Write(text, color);
	}

	bool Text::operator<(const Text& rhs)
	{
		return text < rhs.text;
	}

	bool Text::operator<(const string& rhs)
	{
		return text < rhs;
	}

	void Text::Write(string text, SDL_Color color)
	{
		this->text = text;

		if (!text.empty())
		{
			if (front_buffer != nullptr)
			{
				SDL_FreeSurface(front_buffer);
				front_buffer = nullptr;
			}
			
			SDL_Surface* temporary = TTF_RenderText_Shaded(AI::Core::consolas, text.c_str(), color, black);
			//SDL_Surface* temporary = TTF_RenderText_Solid(AI::Core::consolas, text.c_str(), color);
			front_buffer = SDL_ConvertSurface(temporary, SDL_GetWindowSurface(Core::desktop.window)->format, NULL);
			SDL_FreeSurface(temporary);
		}
	}
}
