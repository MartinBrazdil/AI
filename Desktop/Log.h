//#pragma once
//
//#include <SDL.h>
//#include <SDL_ttf.h>
//#include <map>
//#include <chrono>
//#include <functional>
//#include <vector>
//using namespace std;
//
//#include "Time.h"
//#include "PixelsObserver.h"
//#include "Window.h"
//#include "Text.h"
//#include "Tensor.h"
//
//namespace AI
//{
//	class Observer;
//
//	class Log
//	{
//	public:
//		struct Id
//		{
//			static size_t counter;
//			size_t id; Text name;
//			Id(string name) : id{counter++}, name { name } {}
//			bool operator<(const Id& rhs) { return id < rhs.id; }
//		};
//
//		Window window;
//		void Draw();
//
//	public:
//		map<Text, Observer> log;
//
//		Log(string name);
//		~Log();
//
//		using _GetWindow = std::function<Window&()>;
//		void Log::RegisterWindow(string name, _GetWindow getter);
//
//		using GetText = std::function<std::string()>;
//		void RegisterText(string name, GetText getter);
//
//		using GetPixels = std::function<Tensor<>&()>;
//		void RegisterPixels(string name, GetPixels getter);
//
//		using GetNumber = std::function<double()>;
//		void RegisterGraph(string name, bool colored, GetNumber getter);
//
//		using GetBins = std::function<std::vector<double>()>;
//		void RegisterHistogram(string name, GetBins getter);
//	};
//}
