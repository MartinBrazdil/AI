#pragma once

#include "IWorld.h"
#include "IState.h"
#include "Tensor.h"
#include "IPerception.h"

#include <iostream>
#include <vector>
#include <array>
#include <set>
#include <chrono>
#include <random>
#include <cstdint>
#include <array>

class PaddleAndBall : public IPerception
{
public:
	enum Params { PX, BX, BY };
	std::array<size_t, 3> params;

	PaddleAndBall()
	{
		params.fill(0);
	}
				
	PaddleAndBall(size_t px, size_t bx, size_t by)
	{
		params = {px, bx, by};
	}

	bool operator<(const IPerception& rhs) const override
	{
		const PaddleAndBall* rhsPtr = dynamic_cast<const PaddleAndBall*>(&rhs);
		return params < rhsPtr->params;
	}

	bool operator==(const IPerception& rhs) const override
	{
		const PaddleAndBall* rhsPtr = dynamic_cast<const PaddleAndBall*>(&rhs);
		return params == rhsPtr->params;
	}
};

class PaddleBallAndForce : public IPerception
{
public:
	enum Params { PX, BX, BY, FX, FY };
	std::array<size_t, 5> params;

	PaddleBallAndForce()
	{
		params.fill(0);
	}

	PaddleBallAndForce(size_t px, size_t bx, size_t by, size_t fx, size_t fy)
	{
		params = {px, bx, by, fx, fy};
	}

	bool operator<(const IPerception& rhs) const override
	{
		const PaddleBallAndForce* rhsPtr = dynamic_cast<const PaddleBallAndForce*>(&rhs);
		return params < rhsPtr->params;
	}

	bool operator==(const IPerception& rhs) const override
	{
		const PaddleBallAndForce* rhsPtr = dynamic_cast<const PaddleBallAndForce*>(&rhs);
		return params == rhsPtr->params;
	}
};

class Pixels : public IPerception
{
public:
	Tensor<> pixels;

	Pixels(size_t width, size_t height, size_t depth)
		: pixels{{width, height, depth}}
	{}

	bool operator<(const IPerception& rhs) const override
	{
		const Pixels* rhsPtr = dynamic_cast<const Pixels*>(&rhs);
		return pixels.memory < rhsPtr->pixels.memory;
	}

	bool operator==(const IPerception& rhs) const override
	{
		const Pixels* rhsPtr = dynamic_cast<const Pixels*>(&rhs);
		return pixels.memory == rhsPtr->pixels.memory;
	}
};

class BreakOutState : public IState
{
public:
	// time, width, height, paddle(x), paddle width, ball(x,y), force(x,y), force range, time frequency (speed)
	int t, W, H, px, pw, bx, by, fx, fy, fr, tf;

	// terminal state flag
	bool terminal{false};

	BreakOutState() {}

	BreakOutState(int t, int W, int H, int px, int pw, int bx, int by, int fx, int fy, int fr, int tf)
		: t{t}, W{W}, H{H}, px{px}, pw{pw}, bx{bx}, by{by}, fx{fx}, fy{fy}, fr{fr}, tf{tf} {}

	~BreakOutState() override {}

	std::array<int, 6> CompArray() const
	{
		//return std::array<int, 11>{t, W, H, px, pw, bx, by, fx, fy, fr, tf};
		return std::array<int, 6>{px, bx, by, fx, fy, t};
	}

	bool operator<(const BreakOutState& rhs) const
	{
		return CompArray() < rhs.CompArray();
	}
};

template <class Perception>
class BreakOut : public IWorld<Perception, BreakOutState>
{
public:
	std::mt19937 rng{static_cast<unsigned int>(std::chrono::high_resolution_clock::now().time_since_epoch().count())};

	// paddle x samplers
	std::uniform_int_distribution<int> pxs;

	// ball x, y samplers
	std::uniform_int_distribution<int> bys;
	std::uniform_int_distribution<int> bxs;

	// force x and y samplers
	std::uniform_int_distribution<int> fxs;
	std::uniform_int_distribution<int> fys;

	// fair coin sampler
	std::bernoulli_distribution BreakOut::coin{0.5};

	// uniform real (0, 1) sampler
	std::uniform_real_distribution<double> BreakOut::zero_to_one{0, 1};

	double noise_probability;
	int max_width, max_height, max_paddle_width, max_force_range, max_time_speed;

	bool only_positive_reward, terminal_bounce;

	BreakOut(int max_width, int max_height, int max_paddle_width, int max_force_range, int max_time_speed, double noise_probability, bool only_positive_reward, bool terminal_bounce);

	void InitDefault() override;
	BreakOutState DefaultState() override;

	void InitRandom() override;
	BreakOutState RandomState() override;

	size_t Actions() override { return 3; }
	double Transition(BreakOutState& state, size_t action) override;

	Perception Observation(BreakOutState& state) override;
	bool Terminal(BreakOutState& state) override;

	void Print(BreakOutState& state) override;

	bool Validate(BreakOutState& state);
	void Move(BreakOutState& state, int& nbx, int& nby);
	void Bounce(BreakOutState& state, bool fxb = 0, bool fyb = 0);
	void Noise(BreakOutState& state);
	bool Collision(BreakOutState& state, int& nbx, int& nby, double& r);

	void Print(PaddleAndBall & perception);

	//double Step(int a);
	//double Run(int a);

	//void State(int sw, int sh);
	//std::vector<std::uint64_t> StateHash();
};
 
// setup width, height and reinitialize
template <class Perception>
BreakOut<Perception>::BreakOut(int max_width, int max_height, int max_paddle_width, int max_force_range, int max_time_speed, double noise_probability, bool only_positive_reward, bool terminal_bounce)
	: max_width{max_width}, max_height{max_height}, max_paddle_width{max_paddle_width}, max_force_range{max_force_range}, max_time_speed{max_time_speed}, noise_probability{noise_probability}, only_positive_reward{only_positive_reward}, terminal_bounce{terminal_bounce}
{
	pxs = std::uniform_int_distribution<int>(0, max_width - max_paddle_width);
	bxs = std::uniform_int_distribution<int>(0, max_width - 1);
	bys = std::uniform_int_distribution<int>(max_height - 2, max_height - 1);
	fxs = std::uniform_int_distribution<int>(-max_force_range, max_force_range);
	fys = std::uniform_int_distribution<int>(-max_force_range, max_force_range);
}

template <class Perception>
void BreakOut<Perception>::InitDefault()
{
}

template <class Perception>
BreakOutState BreakOut<Perception>::DefaultState()
{
	BreakOutState state;
	state.bx = max_width / 2;
	state.by = max_height / 2;
	state.fr = max_force_range;
	state.fx = max_force_range;
	state.fy = max_force_range;
	state.H = max_height;
	state.pw = max_paddle_width;
	state.px = (max_width - max_paddle_width) / 2;
	state.t = 0;
	state.tf = max_time_speed;
	state.W = max_width;
	return state;
}

template <class Perception>
void BreakOut<Perception>::InitRandom()
{
	//state.W = w, state.H = h, state.pw = pw, state.fr = fr, state.tf = tf;

	//pxs = std::uniform_int_distribution<int>(0, state.W - state.pw);
	//bxs = std::uniform_int_distribution<int>(0, state.W - 1);
	//bys = std::uniform_int_distribution<int>(2, state.H - 1);
	//fxs = std::uniform_int_distribution<int>(-state.fr, state.fr);
	//fys = std::uniform_int_distribution<int>(-state.fr, state.fr);

	//Reset();
}

template <class Perception>
BreakOutState BreakOut<Perception>::RandomState()
{
	BreakOutState state;

	state.t = 0;
	state.fr = max_force_range;
	state.H = max_height;
	state.pw = max_paddle_width;
	state.tf = max_time_speed;
	state.W = max_width;
				
	// random paddle position
	state.px = pxs(rng);

	// random ball position
	state.bx = bxs(rng);
	state.by = bys(rng);

	if (coin(rng)) { // select fx at boundary
		if (coin(rng)) // select fx negative
			state.fx = -state.fr;
		else // select fx positive
			state.fx = state.fr;
		// select random fy
		while (state.fy == 0)
			state.fy = fys(rng);
	}
	else { // select fy at boundary
		if (coin(rng)) // select fx negative
			state.fy = -state.fr;
		else // select fy positive
			state.fy = state.fr;
		// select random fx
		while (state.fx == 0)
			state.fx = fxs(rng);
	}

	return state;
}

template <class Perception>
double BreakOut<Perception>::Transition(BreakOutState& state, size_t action)
{
	double r = 0;

	// move paddle
	if (action == 1 && 0 <= state.px - 1)
		state.px--;
	else if (action == 2 && state.px + state.pw < state.W)
		state.px++;

	// collision and noise flags
	bool cf = false, nf = false;
	// init next move from current position
	int nbx = state.bx, nby = state.by;
	// simulate next move
	Move(state, nbx, nby);
	// resolve collision (change force)
	while (Collision(state, nbx, nby, r))
	{ // if collision was detected (force changed)
		// init next move from current position
		nbx = state.bx, nby = state.by;
		// simulate next move
		Move(state, nbx, nby);
		// set collision flag
		cf = true;
	}

	Move(state, state.bx, state.by); // apply move

	// add noise only on collision
	if (cf) Noise(state);

	state.t++;

	//if (!Validate()) // ball off screen check
	//	throw new exception("Invalid state!");

	return r;
}

template <>
PaddleAndBall BreakOut<PaddleAndBall>::Observation(BreakOutState& state)
{
	PaddleAndBall perception;
	perception.params[perception.BX] = state.bx;
	perception.params[perception.BY] = state.by;
	perception.params[perception.PX] = state.px;
	return perception;
}

template <>
PaddleBallAndForce BreakOut<PaddleBallAndForce>::Observation(BreakOutState& state)
{
	PaddleBallAndForce perception;
	perception.params[perception.BX] = state.bx;
	perception.params[perception.BY] = state.by;
	perception.params[perception.PX] = state.px;
	perception.params[perception.FX] = state.fx;
	perception.params[perception.FY] = state.fy;
	return perception;
}

template <>
Pixels BreakOut<Pixels>::Observation(BreakOutState& state)
{
	size_t sh = 1, sw = 1;
	Pixels perception(sw * state.W, sh * state.H, 1);

	for (int y = 0; y < state.H; y++)
	{
		for (size_t sy = sh * y; sy < sh * (y + 1); sy++)
		{
			for (int x = 0; x < state.W; x++)
			{
				for (size_t sx = sw * x; sx < sw * (x + 1); sx++)
				{
					// pixel (default) empty
					double pixel = 0;

					// pixel paddle
					if (state.px <= x && x < (state.px + state.pw) && y == 0)
						pixel = 1;

					// pixel ball
					if (x == state.bx && y == state.by)
						pixel = 1;

					//std::cout << c;
					*perception.pixels[{(size_t)sx, (size_t)sy}] = pixel;
				}
			}
		}
	}

	std::normal_distribution<double> normal(0, 0.001);
	for (size_t i = 0; i < perception.pixels.Volume(); i++)
		perception.pixels.memory[i] += normal(rng);
	perception.pixels.NormalizeToInterval();

	return perception;
}

template<class Perception>
bool BreakOut<Perception>::Terminal(BreakOutState& state)
{
	return state.terminal;
}

template <class Perception>
bool BreakOut<Perception>::Validate(BreakOutState& state)
{
	return !(state.bx < 0 || state.W <= state.bx ||
				state.by < 0 || state.H <= state.by);
}

template <class Perception>
void BreakOut<Perception>::Move(BreakOutState& state, int& nbx, int& nby)
{
	state.t = state.t % state.fr;

	// apply force to ball
	if (state.t < abs(state.fx) && state.fx < 0)
		nbx--;
	if (state.t < abs(state.fx) && 0 < state.fx)
		nbx++;
	if (state.t < abs(state.fy) && state.fy < 0)
		nby--;
	if (state.t < abs(state.fy) && 0 < state.fy)
		nby++;
}

template <class Perception>
void BreakOut<Perception>::Bounce(BreakOutState& state, bool fxb, bool fyb)
{
	if (fxb) { // bounce fx
		state.fx = -state.fx;
	}
	else { // bounde fy
		state.fy = -state.fy;
	}
}

template <class Perception>
void BreakOut<Perception>::Noise(BreakOutState& state)
{
	// add noise
	if (zero_to_one(rng) < noise_probability)
	{
		if (state.fx == state.fy) { // boundary corner
						// pick force noise
			if (coin(rng)) { // add fx noise
								// move away from boundary
				state.fx < 0 ? state.fx++ : state.fx--;
			}
			else { // add fy noise
					// move away from boundary
				state.fy < 0 ? state.fy++ : state.fy--;
			}
		}
		// fy at boundary, add fx noise
		else if (abs(state.fx) < abs(state.fy)) {
			// fx not on boundary, pick direction
			coin(rng) ? state.fx++ : state.fx--;
		}
		// fx at boundary, add fy noise
		else if (abs(state.fy) < abs(state.fx)) {
			// fy not on boundary, pick direction
			coin(rng) ? state.fy++ : state.fy--;
		}
	}
}

template <class Perception>
bool BreakOut<Perception>::Collision(BreakOutState& state, int& nbx, int& nby, double& r)
{
	// bounce off horizontal walls
	if (nbx < 0 || state.W <= nbx) {
		Bounce(state, true, false);
		return true;
	}
	// bounce off vertical walls
	if (nby < 0 || state.H <= nby) {
		Bounce(state, false, true);
		return true;
	}

	// ball is at y = 0
	if (nby == 0) { // either loss or paddle
		if (state.px <= nbx && nbx < state.px + state.pw) { // bounce paddle
			Bounce(state, false, true);
			if (terminal_bounce)
				state.terminal = true;
			r += 1;
			return true;
		}
		else { // ball lost, reset
			state.terminal = true;
			if (!only_positive_reward)
				r -= 1;
			return false;
		}
	}

	return false;
}

template<class Perception>
void BreakOut<Perception>::Print(BreakOutState& state)
{
	for (int y = -1; y < state.H + 1; y++)
	{
		for (int x = -1; x < state.W + 1; x++)
		{
			// print (default) empty
			char c = ' ';

			// print borders
			if (x == -1 || x == state.W)
				c = '|';
			if (y == -1 || y == state.H)
				c = '-';

			// print paddle
			if (state.px <= x && x < (state.px + state.pw) && y == 0)
				c = '=';

			// print ball
			if (x == state.bx && y == state.by)
				c = 'o';

			std::cout << c;
		}
		std::cout << std::endl;
	}
}

template<>
void BreakOut<PaddleAndBall>::Print(PaddleAndBall& perception)
{
	for (int y = -1; y < max_height + 1; y++)
	{
		for (int x = -1; x < max_width + 1; x++)
		{
			// print (default) empty
			char c = ' ';

			// print borders
			if (x == -1 || x == max_width)
				c = '|';
			if (y == -1 || y == max_height)
				c = '-';

			// print paddle
			if (perception.params[perception.PX] <= x
				&& x < (perception.params[perception.PX] + max_paddle_width)
				&& y == 0)
			{
				c = '=';
			}

			// print ball
			if (x == perception.params[perception.BX]
				&& y == perception.params[perception.BY])
			{
				c = 'o';
			}

			std::cout << c;
		}
		std::cout << std::endl;
	}
}
