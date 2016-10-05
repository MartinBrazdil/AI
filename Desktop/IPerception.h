#pragma once

#include <vector>
#include <type_traits>
#include <memory>

class IPerception
{
public:
	virtual ~IPerception() {};

	virtual bool operator<(const IPerception& rhs) const = 0;
	virtual bool operator==(const IPerception& rhs) const = 0;
		
	virtual bool operator!=(const IPerception& rhs) const
	{
		return !(*this == rhs);
	}
};
