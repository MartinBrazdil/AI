#pragma once

#include <vector>
#include <type_traits>
#include <memory>

class IState
{
public:
	virtual ~IState() {};
};

class DummyState : public IState
{
public:
	~DummyState() override {};

	char dummy;

	virtual bool operator<(const IState& s) const
	{
		return false;
	}
};
