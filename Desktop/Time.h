#pragma once

#include <chrono>
using namespace std;
using namespace chrono;

namespace AI
{
	using timepoint_t = std::chrono::high_resolution_clock::time_point;

	class Time
	{
	public:
		static bool AlarmClock(timepoint_t& t, time_t ms);
	};
}
