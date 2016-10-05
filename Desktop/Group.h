#pragma once

#include <list>
#include <memory>
#include <vector>
#include <filesystem>
#include <type_traits>
#include <SDL.h>
using namespace std;
using namespace tr2::sys;

#include "Text.h"
#include "Observer.h"
#include "RectPacking.h"

namespace AI
{
	class Group
	{
	public:
		const string name;
		list<Group> subgroups;
		vector<Observer*> observers;

		Group(string name) : name{name} {}
		
		~Group()
		{
			for (Observer* observer : observers)
				delete observer;
		}

		bool operator<(const Group& rhs) const { return name < rhs.name; }

		Group& operator[](string path_str) { return (*this)[path(path_str).begin()]; }
		Group& operator[](path::iterator path_it)
		{
			if (path_it->empty())
				return *this;
			auto group_it = lower_bound(subgroups.begin(), subgroups.end(), Group{path_it->string()});
			if (group_it == subgroups.end() || (path_it->string() < group_it->name))
				group_it = subgroups.emplace(group_it, path_it->string());
			return (*group_it)[++path_it];
		}

		void Remove(string path_str) { return this->Remove(path(path_str).begin()); }
		void Remove(path::iterator path_it)
		{
			auto group_it = lower_bound(subgroups.begin(), subgroups.end(), Group{path_it->string()});
			if (!next(path_it)->empty())
				return group_it->Remove(++path_it);
			if (group_it != subgroups.end())
				subgroups.erase(group_it);
		}
	};
}
