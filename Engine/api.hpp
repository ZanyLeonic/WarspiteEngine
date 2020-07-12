#pragma once
#include "Game.h"
#include <boost/python.hpp>

struct GameWrapper
{
	GameWrapper()
	{
		inst = CGame::Instance();
	}

	int test()
	{
		if (inst != nullptr)
		{
			inst->TestMethod();
			return 0;
		}
		return 1;
	}

	CGame* inst = nullptr;
};


GameWrapper& get_it()
{
	static GameWrapper just_one;
	return just_one;
}

// Wrapper code
using namespace boost::python;
BOOST_PYTHON_MODULE(game)
{
	def("get_it", get_it,
		return_value_policy<reference_existing_object>());

	class_<GameWrapper>("GameWrapper")
		.def("test", &GameWrapper::test)
		;
}