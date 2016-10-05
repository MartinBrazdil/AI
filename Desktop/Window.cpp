#include "Window.h"

#include <SDL.h>
#include <SDL_ttf.h>
#include <cstdlib>
#include <string>
#include <chrono>
using namespace std;
using namespace chrono;

#include "Core.h"
#include "SDLHelpers.h"
#include "CheckPixelFormat.h"

namespace AI
{
	Window::Window(int w, int h)
	{
		SDL_PixelFormat* f = SDL_GetWindowSurface(Core::desktop.window)->format;
		front_buffer = SDL_CreateRGBSurface(NULL, w, h, f->BitsPerPixel, f->Rmask, f->Gmask, f->Bmask, f->Amask);
	}

	Window::~Window()
	{
		if (front_buffer != nullptr)
		{
			SDL_FreeSurface(front_buffer);
			front_buffer = nullptr;
		}
	}

	Rectangle Window::GetRect()
	{
		return{position[0], position[1], front_buffer->w, front_buffer->h};
	}

	void Window::SetRect(Rectangle& rect)
	{
		position = {rect.x, rect.y};
		SetSize(rect.w, rect.h);
	}

	void Window::SetSize(int width, int height)
	{
		if (front_buffer->w != width || front_buffer->h != height)
		{
			SDL_FreeSurface(front_buffer);
			SDL_PixelFormat* f = SDL_GetWindowSurface(Core::desktop.window)->format;
			front_buffer = SDL_CreateRGBSurface(NULL, width, height, f->BitsPerPixel, f->Rmask, f->Gmask, f->Bmask, f->Amask);
		}
	}

	Uint32 Window::GetPixel(int x, int y)
	{
		Uint32* pixels = (Uint32*)front_buffer->pixels;
		return pixels[(y * front_buffer->w) + x];
	}

	void Window::SetPixel(int x, int y, Uint32 pixel)
	{
		Uint32* pixels = (Uint32*)front_buffer->pixels;
		pixels[(y * front_buffer->w) + x] = pixel;
	}

	void Window::Clear()
	{
		SDL_FillRect(front_buffer, NULL, 0x000000);
	}
}
