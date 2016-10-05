#pragma once

#include <SDL.h>
#include <array>
#include <algorithm>
using namespace std;

namespace AI
{
	class Point
	{
	public:
		int x, y;

		Point operator+(Point& rhs) { return{x + rhs.x, y + rhs.y}; }
		void operator+=(Point& rhs) { x += rhs.x, y += rhs.y; }
		Point operator-(Point& rhs) { return{x - rhs.x, y - rhs.y}; }
		void operator-=(Point& rhs) { x -= rhs.x, y -= rhs.y; }
		Point operator/(int d) { return{x / d, y / d}; }
		void operator/=(int d) { x /= d, y /= d; }
		Point operator*(int d) { return{x * d, y * d}; }
		void operator*=(int d) { x *= d, y *= d; }
	};

	class Rectangle
	{
	public:
		int x, y, w, h;

		inline int Volume() { return w * h; }
		inline Point GetXY() { return{x, y}; }
		inline Point GetWH() { return{w, h}; }

		bool Collision(Rectangle& rhs, int padding = 0);
		double Overlap(Rectangle& rhs);

		operator SDL_Rect() { return SDL_Rect{x, y, w, w}; }
		operator SDL_Rect*() { return reinterpret_cast<SDL_Rect*>(this); }

		Rectangle operator+(Point& rhs) { return{x + rhs.x, y + rhs.y, w, h}; }
		void operator+=(Point& rhs) { x += rhs.x, y += rhs.y; }

		Rectangle operator+(Rectangle& rhs) { return{x + rhs.x, y + rhs.y, w + rhs.w, h + rhs.h}; }
		void operator+=(Rectangle& rhs) { x += rhs.x, y += rhs.y, w += rhs.w, h += rhs.h; }
		Rectangle operator-(Rectangle& rhs) { return{x - rhs.x, y - rhs.y, w - rhs.w, h - rhs.h}; }
		void operator-=(Rectangle& rhs) { x -= rhs.x, y -= rhs.y, w -= rhs.w, h -= rhs.h; }
		Rectangle operator/(int d) { return{x / d, y / d, w / d, h / d}; }
		void operator/=(int d) { x /= d, y /= d, w /= d, h /= d; }

		Rectangle GetRect() { return *this; }
		void SetRect(Rectangle& rect) { *this = rect; }
	};
}
