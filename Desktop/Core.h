#pragma once

#include <functional>
#include <thread>
#include <vector>
#include <memory>
#include <map>
using namespace std;

#include <SDL.h>
#include <SDL_ttf.h>

#include "Time.h"
#include "Desktop.h"

namespace AI
{
	class Controller;

	class Core
	{
	public:
		using Task = function<void(Core* core)>;

		static vector<thread> threads;
		static thread::id main_thread_id;

		map<string, Controller*> controllers;

		static Time time;
		static Desktop desktop;
		static TTF_Font* consolas;

		Core(Task task);

		Core(Core const&) = delete;
		void operator=(Core const&) = delete;

		void Run();

		static bool IsMainThread();
		static size_t ThreadId();
	};
}
