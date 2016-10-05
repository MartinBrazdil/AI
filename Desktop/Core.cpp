#include "Core.h"

#include <chrono>
#include <mutex>

namespace AI
{
	Time Core::time;
	Desktop Core::desktop;
	TTF_Font* Core::consolas = TTF_OpenFont("Consolas.ttf", 12);

	vector<thread> Core::threads;
	thread::id Core::main_thread_id;

	Core::Core(Task task)
	{
		//bool set = false;
		//mutex syncStartMutex;
		
		//desktop = new Desktop();
		//consolas = TTF_OpenFont("Consolas.ttf", 12);

		task(this);

		//threads.push_back(thread([&]()
		//{
		//	syncStartMutex.lock();
		//	if (!set)
		//	{
		//		main_thread_id = this_thread::get_id();
		//		set = true;
		//	}
		//	syncStartMutex.unlock();
		//	task(*this);
		//}));
		//for (auto& thread : threads)
		//	thread.join();
	}

	void Core::Run()
	{
		desktop.Run();
		SDL_FlushEvents(SDL_FIRSTEVENT, SDL_LASTEVENT);
	}

	bool Core::IsMainThread()
	{
		return this_thread::get_id() == main_thread_id;
	}

	size_t Core::ThreadId()
	{
		auto thread_it = find_if(threads.begin(), threads.end(), [](auto& thread)
		{
			return this_thread::get_id() == thread.get_id();
		});
		return distance(threads.begin(), thread_it);
	}
}
