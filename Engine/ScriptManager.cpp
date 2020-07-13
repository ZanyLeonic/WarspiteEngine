#include "ScriptManager.h"
#include <iostream>
#include <pybind11/embed.h>
#include "WarspiteUtil.h"

CScriptManager* CScriptManager::s_pInstance = 0;

// Wrapper code
// Define Python module "bar" and Python class "bar.Foo" wrapping the C++ class
PYBIND11_MODULE(game, m) {
	py::class_<GameWrapper, WrapPtr>(m, "GameWrapper")
		.def("test", &GameWrapper::test);
}

CScriptManager::CScriptManager()
{
	std::cout << "Initialising ScriptManager..." << std::endl;

	try {
		// Create a C++ instance of Foo
		WrapPtr foo = std::make_shared<GameWrapper>();

		// Initialize Python interpreter and import bar module
		PyImport_AppendInittab("game", PyInit_game);
		Py_Initialize();
		//PyRun_SimpleString("import game");
		
		// Make C++ instance accessible in Python as a variable named "foo"
		main_module = py::module::import("__main__");
		// main_module.attr("inst") = foo;
		main_namespace = main_module.attr("__dict__");
	}
	catch(pybind11::error_already_set const&)
	{
		PyErr_Print();
	}

	// Run some Python code using foo
	// Show that the ScriptManager is ready
	SGameScript* test = SGameScript::source("test", "import sys\nprint(\"Using Python Runtime %s.%s.%s\" % (sys.version_info.major, sys.version_info.minor, sys.version_info.micro))\nprint(\"Script Manager is ready!\")");
	SGameScript* tt = SGameScript::source("test2", "from game import *\ninst = CGame.instance()\nprint(dir(inst))");

	Run(test);
	Run(tt);
}


void CScriptManager::Load(SGameScript* script)
{
	loadedScripts[script->GetScriptName()] = script;
}

bool CScriptManager::Remove(const char* scriptRef)
{
	auto search = loadedScripts.find(scriptRef);
	if (search != loadedScripts.end())
	{
		loadedScripts.erase(search);
		return true;
	}
	return false;
}

void CScriptManager::RemoveAll()
{
	loadedScripts.clear();
}

bool CScriptManager::Run(SGameScript* script)
{
	try
	{
		// Use the namespace provided (if there is one)
		switch (script->GetScriptType())
		{
		case EGameScriptType::SCRIPT_INLINE:
			PyRun_SimpleString(script->GetSource().c_str());
			break;
		case EGameScriptType::SCRIPT_FILE:
			PyRun_SimpleString(WarspiteUtil::ReadAllText(script->GetFilename()).c_str());
			break;
		default:
			return false; // No type defined? what?
		}
		
		return true;
	}
	catch(pybind11::error_already_set const &)
	{
		PyErr_Print(); // print all errors for now
	}
	return false;
}

bool CScriptManager::RunFromRef(std::string scriptRef)
{
	return Run(loadedScripts[scriptRef]);
}