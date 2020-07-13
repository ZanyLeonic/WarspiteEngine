#include "ScriptManager.h"
#include <iostream>

CScriptManager* CScriptManager::s_pInstance = 0;

// Wrapper code
namespace bp = boost::python;
BOOST_PYTHON_MODULE(hello)
{
	bp::class_<Foo, foo_ptr>("Foo")
		.def("doSomething", &Foo::doSomething)
		;
};

CScriptManager::CScriptManager()
{
	std::cout << "Initialising ScriptManager..." << std::endl;

	Py_Initialize();
	try {
		PyRun_SimpleString(
			"a_foo = None\n"
			"\n"
			"def setup(a_foo_from_cxx):\n"
			"    print('setup called with', a_foo_from_cxx)\n"
			"    global a_foo\n"
			"    a_foo = a_foo_from_cxx\n"
			"\n"
			"def run():\n"
			"    a_foo.doSomething()\n"
			"\n"
			"print('main module loaded')\n"
		);

		foo_ptr a_cxx_foo = boost::make_shared<Foo>("c++");

		bp::object main = bp::object(bp::handle<>(bp::borrowed(
			PyImport_AddModule("__main__")
		)));

		// pass the reference to a_cxx_foo into python:
		bp::object setup_func = main.attr("setup");
		setup_func(a_cxx_foo);

		// now run the python 'main' function
		bp::object run_func = main.attr("run");
		run_func();
	}
	catch (bp::error_already_set) {
		PyErr_Print();
	}
	
	//
	//main_module = boost::python::import("__main__");
	//main_namespace = main_module.attr("__dict__");

	//// Show that the ScriptManager is ready
	//SGameScript *test = SGameScript::source("test", "import sys\nprint(\"Using Python Runtime %s.%s.%s\" % (sys.version_info.major, sys.version_info.minor, sys.version_info.micro))\nprint(\"Script Manager is ready!\")");
	//SGameScript* tt = SGameScript::source("test2","import emb\nprint(\"Number of arguments\", emb.numargs())");

	//Run(test);
	//Run(tt);
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