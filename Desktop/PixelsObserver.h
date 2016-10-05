#pragma once

#include <functional>
#include <string>
#include <vector>
using namespace std;

#include "Observer.h"
#include "Tensor.h"
#include "Pixels.h"
#include "Window.h"
#include "Rectangle.h"

namespace AI
{
	using GetPixels = function<Tensor<>&()>;
	class PixelsObserver : public Observer
	{
	public:
		Pixels pixels;
		GetPixels getter;

		PixelsObserver(GetPixels getter);
		~PixelsObserver() override {}

		Window& GetWindow() override;

		Rectangle GetRect() { return GetWindow().GetRect(); }
		void SetRect(Rectangle& rect) { GetWindow().SetRect(rect); };

		void Update() override;
	};
}
