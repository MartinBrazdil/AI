#include "Rectangle.h"

namespace AI
{
	bool Rectangle::Collision(Rectangle& rhs, int padding)
	{
		if (x + w < rhs.x - padding) return false;
		else if (rhs.x + rhs.w < x - padding) return false;
		else if (y + h < rhs.y - padding) return false;
		else if (rhs.y + rhs.h < y - padding) return false;
		return true;
	}

	double Rectangle::Overlap(Rectangle& rhs)
	{
		int x1 = max(x, rhs.x);
		int y1 = max(y, rhs.y);
		int x2 = min(x + w, rhs.x + rhs.w);
		int y2 = min(y + h, rhs.y + rhs.h);
		return (x2 - x1) * (y2 - y1);
	}
}