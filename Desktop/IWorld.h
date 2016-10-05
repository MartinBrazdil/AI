#pragma once

#include "IState.h"

#include <vector>
#include <queue>
#include <set>
#include <type_traits>
#include <memory>

template<class Perception, class State = IState>
class IWorld
{
public:
	using state_type = State;
	using perception_type = Perception;

	struct StateAndPerception { State state; Perception perception; };

	virtual ~IWorld() {};

	virtual void InitDefault() = 0;
	virtual void InitRandom() = 0;

	virtual State DefaultState() = 0;
	virtual State RandomState() = 0;

	virtual size_t Actions() = 0;
	virtual double Transition(State& state, size_t action) = 0;
			
	virtual Perception Observation(State& state) = 0;
	virtual bool Terminal(State& state) = 0;
		
	virtual void Print(State& state) = 0;

	virtual std::set<State> AllStates();
	//virtual PGreedy<Perception> ValueIteration(double gamma, double epsilon);
};

template<class Perception, class State>
std::set<State> IWorld<Perception, State>::AllStates()
{
	std::set<State> states;
	std::queue<State> state_queue;
	state_queue.push(DefaultState());
	while (!state_queue.empty())
	{
		State& state = state_queue.front();
		for (size_t a = 0; a < Actions(); a++)
		{
			State next_state = state;
			Transition(next_state, a);
			if (!states.count(next_state)
				&& !Terminal(next_state))
			{
				states.insert(next_state);
				state_queue.push(next_state);
				//Print(next_state);
			}
		}
		state_queue.pop();
	}
	return states;
}
