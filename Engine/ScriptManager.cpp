#include "ScriptManager.h"
#include <iostream>
#include "ScriptWrappers.h"
#include <pybind11/embed.h>

CScriptManager* CScriptManager::s_pInstance = 0;

CScriptManager::CScriptManager()
{
	std::cout << "Initialising ScriptManager..." << std::endl;

	if (CScriptWrappers::Init_Interpreter(&main_module, &engine_module, &main_namespace))
	{
		// Show that the ScriptManager is ready
		SGameScript* test = SGameScript::source("test", "import sys\nprint(\"Using Python Runtime %s.%s.%s\" % (sys.version_info.major, sys.version_info.minor, sys.version_info.micro))\nprint(\"Script Manager is ready!\")");

		Run(test);
	}
	else
	{
		std::cerr << "Initialisation of ScriptManager failed!" << std::endl;
	}
}

void CScriptManager::Destroy()
{
	// Destroy our interpreter
	py::finalize_interpreter();
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

bool CScriptManager::Run(SGameScript* script, py::object* ns)
{
	try
	{
		// Use the namespace provided (if there is one)
		switch (script->GetScriptType())
		{
		case EGameScriptType::SCRIPT_INLINE:
			py::exec(script->GetSource().c_str(), ns != nullptr ? *ns : main_namespace);
			break;
		case EGameScriptType::SCRIPT_FILE:
			py::eval_file(script->GetFilename().c_str(), ns != nullptr ? *ns : main_namespace);
			break;
		default:
			return false; // No type defined? what?
		}
		
		return true;
	}
	catch(py::error_already_set const & e)
	{
		switch(script->GetScriptType())
		{
		case EGameScriptType::SCRIPT_INLINE:
			std::cerr << "An internal error occurred when executing an inline script named: \"" + script->GetScriptName() + "\"" << std::endl;
			break;
		case EGameScriptType::SCRIPT_FILE:
			std::cerr << "An internal error occurred when executing an external script named: \"" + script->GetScriptName() + "\"" << std::endl;
			break;
		default:
			std::cerr << "An internal error occurred when executing script named: \"" + script->GetScriptName() + "\"" << std::endl;
		}

		// Print what happened to not make debugging hell.
		std::cerr << "Error:\n " << e.what() << std::endl;
		std::cerr << "***Error End***" << std::endl;
	}
	return false;
}

bool CScriptManager::RunFromRef(std::string scriptRef, py::object* ns)
{
	return Run(loadedScripts[scriptRef], ns);
}