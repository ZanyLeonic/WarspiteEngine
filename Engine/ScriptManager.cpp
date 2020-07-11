#include "ScriptManager.h"
#include <iostream>

CScriptManager* CScriptManager::s_pInstance = 0;

CScriptManager::CScriptManager()
{
	std::cout << "Initialising ScriptManager..." << std::endl;

	Py_Initialize();

	main_module = boost::python::import("__main__");
	main_namespace = main_module.attr("__dict__");

	// Show that the ScriptManager is ready
	SGameScript *test = SGameScript::source("test", "import sys\nprint(\"Using Python Runtime %s.%s.%s\" % (sys.version_info.major, sys.version_info.minor, sys.version_info.micro))\nprint(\"Script Manager is ready!\")");
	Run(test);
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

bool CScriptManager::Run(SGameScript* script, boost::python::object* ns)
{
	try
	{
		// Use the namespace provided (if there is one)
		switch (script->GetScriptType())
		{
		case EGameScriptType::SCRIPT_INLINE:
			exec(script->GetSource().c_str(), main_namespace);
			break;
		case EGameScriptType::SCRIPT_FILE:
			exec_file(script->GetFilename().c_str(), main_namespace);
			break;
		default:
			return false; // No type defined? what?
		}
		
		return true;
	}
	catch(boost::python::error_already_set const &)
	{
		PyErr_Print();
	}
	return false;
}

bool CScriptManager::RunFromRef(std::string scriptRef, boost::python::object* ns)
{
	return Run(loadedScripts[scriptRef], ns);
}