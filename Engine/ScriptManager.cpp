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
	Run("import sys\nprint(\"Using Python Runtime %s.%s.%s\" % (sys.version_info.major, sys.version_info.minor, sys.version_info.micro))\n");
	Run("print(\"ScriptManager ready!\")");
}

bool CScriptManager::Run(const char* script, boost::python::object* ns)
{
	try
	{
		// Use the namespace provided (if there is one)
		exec(script, ns != nullptr ? *ns : main_namespace);
		return true;
	}
	catch(boost::python::error_already_set const &)
	{
		PyErr_Print();
	}
	return false;
}

bool CScriptManager::RunScript(const char* filename, boost::python::object* ns)
{
	try
	{
		// Use the namespace provided (if there is one)
		exec_file(filename, ns != nullptr ? *ns : main_namespace);
		return true;
	}
	catch (boost::python::error_already_set const&)
	{
		PyErr_Print();
	}
	return false;
}
