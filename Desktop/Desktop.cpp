#include "Desktop.h"

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
//#include <SDL2_gfxPrimitives.h>
#include <cstdlib>
#include <string>
#include <chrono>
using namespace std;
using namespace chrono;

#include "Core.h"
#include "Window.h"
#include "RectPacking.h"
#include "SDLHelpers.h"
#include "Rectangle.h"
#include "SDLHelpers.h"

namespace AI
{
	TTF_Font* Desktop::consolas{nullptr};

	Desktop::Desktop(int display_idx)
	{
		//if (!SDL_WasInit(SDL_INIT_EVERYTHING))
		//	SDL_Init(SDL_INIT_EVERYTHING);

		//if (!TTF_WasInit())
		//	TTF_Init();
		//
		//printf("1\n");

		//int display_count = SDL_GetNumVideoDisplays();
		//if (display_count > 0)
		//{
		//	if (display_idx > display_count)
		//		display_idx = 0;
		//	SDL_GetCurrentDisplayMode(display_idx, &display_mode);
		//}

		//printf("2\n");

		//window = SDL_CreateWindow("Desktop", 0, 0, display_mode.w, display_mode.h, SDL_WINDOW_BORDERLESS | SDL_WINDOW_RESIZABLE);
		//if (window == NULL)printf("Could not create window: %s\n", SDL_GetError());
		//renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
		//if (renderer == NULL) printf("Could not create renderer: %s\n", SDL_GetError());
		//if (window && renderer) printf("Desktop was created!\n");
		//
		//printf("3\n");

		//if (!consolas)
		//	consolas = TTF_OpenFont("Consolas.ttf", 12);

		//printf("4\n");

		//SDL_Surface* wallpaper_raw = SDL_LoadBMP("w6.bmp");
		//SDL_Surface* wallpaper = SDL_ConvertSurface(wallpaper_raw, SDL_GetWindowSurface(window)->format, NULL);
		//background_texture = SDL_CreateTextureFromSurface(renderer, wallpaper);
		//SDL_RenderCopy(renderer, background_texture, NULL, NULL);
		//desktop_texture = SDL_CreateTexture(renderer, SDL_GetWindowPixelFormat(window), SDL_TEXTUREACCESS_STREAMING, display_mode.w, display_mode.h);
		//SDL_FreeSurface(wallpaper_raw);
		//SDL_FreeSurface(wallpaper);
		//printf("5\n");
	}

	Desktop::~Desktop()
	{
		if (window)
			SDL_DestroyWindow(window);
		if (renderer)
			SDL_DestroyRenderer(renderer);
		if (desktop_texture)
			SDL_DestroyTexture(desktop_texture);
	}

	Group& Desktop::operator[](string path_str)
	{
		return desktop[path_str];
	}

	void Desktop::Run()
	{
		HandleInput();
		Render();
	}

	void Desktop::Order()
	{
		Group& group = desktop[active_path];

		// order all subgroup observers and collect their envelopes
		vector<Rectangle> all_envelopes;
		all_envelopes.reserve(group.subgroups.size() + 1);
		for (auto& subgroup : group.subgroups)
			all_envelopes.push_back(RectPacking<Observer>::PackRandom(subgroup.observers, 5));
		// order observers and add observers envelope to sugbroup observers envelopes
		all_envelopes.push_back(RectPacking<Observer>::PackRandom(group.observers, 5));

		// calculate viewport offset which will be added to each observer
		Point resolution = {display_mode.w, display_mode.h};
		Rectangle viewport = RectPacking<Rectangle>::PackRandom(all_envelopes, 50);
		Point offset = (resolution - viewport.GetWH()) / 2;

		// offset (order) all according to ordered envelopes and viewport offset
		int subgroup_idx = 0;
		for (auto& subgroup : group.subgroups)
		{
			for (Observer* observer : subgroup.observers)
				observer->SetRect(observer->GetRect() + all_envelopes[subgroup_idx].GetXY() + offset);
			subgroup_idx++;
		}
		for (Observer* observer : group.observers)
			observer->SetRect(observer->GetRect() + all_envelopes.back().GetXY() + offset);
	}

	bool Desktop::UpdateAndRender(Group& group)
	{
		bool has_rendered = false;
		for (Observer* observer : group.observers)
			if (Time::AlarmClock(observer->timePrev, observer->delay))
			{
				observer->Update();
				RenderWindow(observer->GetWindow());
				has_rendered = true;
			}
		return has_rendered;
	}

	void Desktop::RenderWindow(Window& window)
	{
		Rectangle window_rect = window.GetRect();
		BlitSurface(window.front_buffer, nullptr, desktop_texture, window_rect);
		SDL_RenderCopy(renderer, desktop_texture, window_rect, window_rect);
	}

	void Desktop::RenderEnvelope(Window& window)
	{
		SDL_Point points[4];
		SDL_SetRenderDrawColor(renderer, 0x00, 0xFF, 0x00, 0xFF);
		Rectangle observers_envelope = RectPacking<Observer>::Envelope(desktop[active_path].observers);
		points[0] = {observers_envelope.x, observers_envelope.y};
		points[1] = {observers_envelope.x, observers_envelope.y + observers_envelope.h};
		points[2] = {observers_envelope.x + observers_envelope.w, observers_envelope.y + observers_envelope.h};
		points[3] = {observers_envelope.x + observers_envelope.w, observers_envelope.y};
		SDL_RenderDrawLines(renderer, points, 4);
	}

	void Desktop::Render(string path_str)
	{
		SDL_SetRenderTarget(renderer, desktop_texture);

		bool has_rendered = false;
		Group& group = desktop[path_str];
		for (auto& subgroup : group.subgroups)
			has_rendered |= UpdateAndRender(subgroup);
		has_rendered |= UpdateAndRender(group);

		if (has_rendered)
			SDL_RenderPresent(renderer);
	}

	void PrintWindowEvent(const SDL_Event * event)
	{
		if (event->type == SDL_WINDOWEVENT) {
			switch (event->window.event) {
				case SDL_WINDOWEVENT_SHOWN:
					SDL_Log("Window %d shown", event->window.windowID);
					break;
				case SDL_WINDOWEVENT_HIDDEN:
					SDL_Log("Window %d hidden", event->window.windowID);
					break;
				case SDL_WINDOWEVENT_EXPOSED:
					SDL_Log("Window %d exposed", event->window.windowID);
					break;
				case SDL_WINDOWEVENT_MOVED:
					SDL_Log("Window %d moved to %d,%d",
							event->window.windowID, event->window.data1,
							event->window.data2);
					break;
				case SDL_WINDOWEVENT_RESIZED:
					SDL_Log("Window %d resized to %dx%d",
							event->window.windowID, event->window.data1,
							event->window.data2);
					break;
				case SDL_WINDOWEVENT_SIZE_CHANGED:
					SDL_Log("Window %d size changed to %dx%d",
							event->window.windowID, event->window.data1,
							event->window.data2);
					break;
				case SDL_WINDOWEVENT_MINIMIZED:
					SDL_Log("Window %d minimized", event->window.windowID);
					break;
				case SDL_WINDOWEVENT_MAXIMIZED:
					SDL_Log("Window %d maximized", event->window.windowID);
					break;
				case SDL_WINDOWEVENT_RESTORED:
					SDL_Log("Window %d restored", event->window.windowID);
					break;
				case SDL_WINDOWEVENT_ENTER:
					SDL_Log("Mouse entered window %d",
							event->window.windowID);
					break;
				case SDL_WINDOWEVENT_LEAVE:
					SDL_Log("Mouse left window %d", event->window.windowID);
					break;
				case SDL_WINDOWEVENT_FOCUS_GAINED:
					SDL_Log("Window %d gained keyboard focus",
							event->window.windowID);
					break;
				case SDL_WINDOWEVENT_FOCUS_LOST:
					SDL_Log("Window %d lost keyboard focus",
							event->window.windowID);
					break;
				case SDL_WINDOWEVENT_CLOSE:
					SDL_Log("Window %d closed", event->window.windowID);
					break;
				default:
					SDL_Log("Window %d got unknown event %d",
							event->window.windowID, event->window.event);
					break;
			}
		}
	}

	void Desktop::HandleInput()
	{
		const int BUFSIZE = 32;
		SDL_Event events[BUFSIZE];

		// update event queue
		SDL_PumpEvents();

		// check last mouse button event
		int event_count = SDL_PeepEvents(events, BUFSIZE, SDL_GETEVENT, SDL_FIRSTEVENT, SDL_LASTEVENT);

		//printf("Poll: %i\n", SDL_PollEvent(nullptr));

		for (size_t i = 0; i < event_count; i++)
		{
			SDL_Event& event = events[i];

			PrintWindowEvent(&event);

			switch (event.type)
			{
				case SDL_MOUSEBUTTONDOWN:
				{
					SDL_MouseButtonEvent& buttonEvent = event.button;
					if (buttonEvent.button == SDL_BUTTON_RIGHT)
					{
						//printf("RightClick\n");
						int mouseX, mouseY;
						SDL_GetMouseState(&mouseX, &mouseY);
						
						//filledCircleColor(renderer, mouseX, mouseY, 200, 0xFF0000FF);
					}
					break;
				}
				case SDL_MOUSEMOTION:
				{
					SDL_MouseMotionEvent& motionEvent = event.motion;
					printf("Mouse X: %i, Y: %i\n", motionEvent.x, motionEvent.y);

					//SDL_Point points[4];
					//SDL_SetRenderDrawColor(renderer, 0x00, 0xFF, 0x00, 0xFF);
					//Rectangle observers_envelope = RectPacking<Observer>::Envelope(desktop[active_path].observers);
					//points[0] = {observers_envelope.x, observers_envelope.y};
					//points[1] = {observers_envelope.x, observers_envelope.y + observers_envelope.h};
					//points[2] = {observers_envelope.x + observers_envelope.w, observers_envelope.y + observers_envelope.h};
					//points[3] = {observers_envelope.x + observers_envelope.w, observers_envelope.y};
					//SDL_RenderDrawLines(renderer, points, 4);

					break;
				}
				case SDL_MOUSEWHEEL:
				{
					//SDL_MouseWheelEvent& wheelEvent = event.wheel;
					//int delta = 50 * wheelEvent.y;
					//viewport.x += delta;
					//viewport.y += delta;
					//viewport.w += delta;
					//viewport.h += delta;
					////ReRender();
					break;
				}
				case SDL_KEYDOWN:
				{
					switch (event.key.keysym.sym)
					{
						case SDLK_TAB:
						{
							//Toggle();
							break;
						}
						case SDLK_F5:
						{
							SDL_RenderCopy(renderer, background_texture, NULL, NULL);
							Order();
							break;
						}
						case SDLK_LEFT:
						{
							//viewport.x -= 100;
							//ReRender();
							break;
						}
						case SDLK_RIGHT:
						{
							//viewport.x += 100;
							//ReRender();
							break;
						}
						case SDLK_UP:
						{
							//viewport.y -= 100;
							//ReRender();
							break;
						}
						case SDLK_DOWN:
						{
							//viewport.y += 100;
							//ReRender();
							break;
						}
					}
					break;
				}
			}
		}
	}
}