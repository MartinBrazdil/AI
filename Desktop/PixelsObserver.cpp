#include "PixelsObserver.h"

namespace AI
{
	PixelsObserver::PixelsObserver(GetPixels getter)
		: getter{getter}
	{
		Update();
	}

	Window& PixelsObserver::GetWindow()
	{
		return pixels;
	}

	void PixelsObserver::Update()
	{
		pixels.Write(getter());
	}
}
