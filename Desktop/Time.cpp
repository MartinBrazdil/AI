#include "Time.h"

namespace AI
{
	bool Time::AlarmClock(timepoint_t& t, time_t ms)
	{
		timepoint_t now = chrono::high_resolution_clock::now();
		if (duration_cast<milliseconds>(now - t) > milliseconds(ms))
		{
			t = now;
			return true;
		}
		return false;
	}
}
