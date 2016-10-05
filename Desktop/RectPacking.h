#pragma once

#include <type_traits>
#include <vector>
#include <array>
#include <SDL.h>
using namespace std;

#include "Random.h"
#include "Rectangle.h"

namespace AI
{
	template <class T>
	class RectPacking
	{
	public:
		RectPacking() = delete;

		static void Center(vector<T*>& objects, array<double, 2> middle);

		static Rectangle Envelope(Rectangle& o1, Rectangle& o2);
		static Rectangle Envelope(vector<T>& objects);
		static Rectangle Envelope(vector<T*>& objects);

		static Rectangle PackRandom(vector<T>& objects, int padding);
		static Rectangle PackRandom(vector<T*>& objects, int padding);
	};

	template <class T>
	void RectPacking<T>::Center(vector<T*>& objects, array<double, 2> middle)
	{
		Rectangle envelope = Envelope(objects);
		int shift_x = middle[0] - envelope.w / 2 - envelope.x;
		int shift_y = middle[1] - envelope.h / 2 - envelope.y;
		for (auto& object : objects)
		{
			Rectangle r = object->GetRect();
			r.x += shift_x;
			r.y += shift_y;
			object->SetRect(r);
		}
	}

	template <class T>
	Rectangle RectPacking<T>::Envelope(Rectangle& r1, Rectangle& r2)
	{
		return{min(r1.x, r2.x), min(r1.y, r2.y), max(r1.x + r1.w, r2.x + r2.w), max(r1.y + r1.h, r2.y + r2.h)};
	}

	template <class T>
	Rectangle RectPacking<T>::Envelope(vector<T>& objects)
	{
		vector<T*> pointers{objects.size()};
		for (size_t i = 0; i < objects.size(); i++)
			pointers[i] = &objects[i];
		return Envelope(pointers);
	}

	template <class T>
	Rectangle RectPacking<T>::Envelope(vector<T*>& objects)
	{
		int x_min = numeric_limits<int>::max();
		int x_max = numeric_limits<int>::lowest();
		int y_min = numeric_limits<int>::max();
		int y_max = numeric_limits<int>::lowest();

		// compute x_min, y_max, x_max, y_max defining the envelope
		for (auto& object : objects)
		{
			Rectangle r = object->GetRect();
			x_min = min(r.x, x_min);
			x_max = max(r.x + r.w, x_max);
			y_min = min(r.y, y_min);
			y_max = max(r.y + r.h, y_max);
		}
		return{x_min, y_max, (x_max - x_min), (y_max - y_max)};
	}

	template <class T>
	Rectangle RectPacking<T>::PackRandom(vector<T>& objects, int padding)
	{
		vector<T*> pointers{objects.size()};
		for (size_t i = 0; i < objects.size(); i++)
			pointers[i] = &objects[i];
		return PackRandom(pointers, padding);
	}

	template <class T>
	Rectangle RectPacking<T>::PackRandom(vector<T*>& objects, int padding)
	{
		// summ all objects widths (width_sum) and heights (height_sum)
		int width_sum = 0, height_sum = 0;
		for (auto& object : objects)
		{
			Rectangle& r = object->GetRect();
			width_sum += r.w;
			height_sum += r.h;
		}

		Rectangle envelope{0, 0, 0, 0};
		// sufficient envelope size, big enough for all by proof
		Rectangle max_envelope{0, 0, width_sum, height_sum};
		// create samplers of points from max_envelope
		uniform_int<int> x_sampler{max_envelope.x, max_envelope.x + max_envelope.w};
		uniform_int<int> y_sampler{max_envelope.y, max_envelope.y + max_envelope.h};

		// iteratively order objects
		for (size_t i = 0; i < objects.size(); i++)
		{
			// current envelope of all ordered objects so far
			Rectangle current_envelope = envelope;
			// 10 random positions are tried for each object
			for (size_t trial = 0; trial < 1000; trial++)
			{
			try_again:
				// sample candidate position for object[i]
				Rectangle candidate = objects[i]->GetRect();
				candidate.x = x_sampler(rng);
				candidate.y = y_sampler(rng);

				// check collisions with already ordered objects
				for (size_t j = 0; j < i; j++)
					if (candidate.Collision(objects[j]->GetRect(), padding))
						goto try_again; // if collision, sample another candidate

				// initialize with first valid candidate
				if (trial == 0)
				{
					envelope = Envelope(current_envelope, candidate);
					objects[i]->SetRect(candidate);
				}
				else
				{
					Rectangle candidate_envelope = Envelope(current_envelope, candidate);
					// select candidate if candidate envelope has smallest volume so far
					if (candidate_envelope.Volume() < envelope.Volume())
					{
						envelope = candidate_envelope;
						objects[i]->SetRect(candidate);
					}
				}
			}
		}
		return envelope;
	}
}
