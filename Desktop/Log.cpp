//#include "Log.h"
//
//#include <SDL_ttf.h>
//#include <iostream>
//#include <vector>
//#include <algorithm>
//#include <chrono>
//
//using namespace chrono;
//using namespace std;
//
//#include "Core.h"
//#include "Time.h"
//#include "Window.h"
//#include "Observer.h"
//#include "TextObserver.h"
//#include "PixelsObserver.h"
//#include "GraphObserver.h"
//#include "HistogramObserver.h"
//#include "WindowObserver.h"
//#include "Window.h"
//#include "Rectangle.h"
//
//namespace AI
//{
//	size_t Log::Id::counter{0};
//
//	Log::Log(string name)
//	{
//	}
//
//	Log::~Log()
//	{
//		for (auto id_and_observer : log)
//			delete id_and_observer.second;
//	}
//
//	Observer& operator[](string name)
//	{
//
//	}
//
//	void Log::Draw()
//	{
//		window.Clear();
//		int xMax = 0, ySum = 0;
//		for (auto id_and_observer : log)
//		{
//			id_and_observer.second->Update();
//
//			Text& name_window = const_cast<Text&>(id_and_observer.first.name_window);
//			Window* observer_window = id_and_observer.second->GetWindow();
//
//			Rectangle name_window_rect = name_window.GetRect();
//			Rectangle observer_window_rect = observer.GetRect();
//
//			// width and height of info line
//			int w = max(name_window_rect.w, observer_window_rect.w);
//			int h = name_window_rect.h + observer_window_rect.h;
//
//			// set name and window position
//			name_window.position[0] = 0;
//			name_window.position[1] = ySum;
//
//			observer_window->position[0] = 0;
//			observer_window->position[1] = ySum + name_window_rect.h;
//
//			// track window height and width
//			xMax = max(w, xMax);
//			ySum += h;
//
//			window.SetSize(min(xMax, 160), ySum);
//
//			SDL_BlitSurface(name_window.front_buffer, nullptr, window.front_buffer, &name_window.GetRect());
//			SDL_BlitSurface(observer_window->front_buffer, nullptr, window.front_buffer, &observer_window->GetRect());
//		}
//
//		//window->SetSize(min(xMax, 160), ySum);
//
//		//for (auto pair : log)
//		//{
//		//	pair.first->name_window.Draw();
//		//	Window* window = pair.second->GetWindow();
//		//	if (window) window->Draw();
//		//}
//
//		//window->Render();
//	}
//
//	void Log::RegisterWindow(string name, _GetWindow getter)
//	{
//		log[name] = WindowObserver(getter);
//	}
//
//	void Log::RegisterText(string name, GetText getter)
//	{
//		log[name] = TextObserver(getter);
//	}
//
//	void Log::RegisterPixels(string name, GetPixels getter)
//	{
//		log[name] = PixelsObserver(getter);
//	}
//
//	void Log::RegisterGraph(string name, GetNumber getter)
//	{
//		log[name] = GraphObserver(getter);
//	}
//
//	void Log::RegisterHistogram(string name, GetBins getter)
//	{
//		log[name] = HistogramObserver(getter);
//	}
//}
