#pragma once
#ifndef __SCRIPTMANAGER_H__
#define __SCRIPTMANAGER_H__

#include <boost/python.hpp>

class CScriptManager
{
	static CScriptManager* s_pInstance;
	CScriptManager();
public:
	// more singleton stuff
	static CScriptManager* Instance()
	{
		if (s_pInstance == nullptr)
		{
			s_pInstance = new CScriptManager();
		}

		return s_pInstance;
	}

	bool Run(const char* script, boost::python::object* ns = 0);
	bool RunScript(const char * filename, boost::python::object* ns = 0);
private:
	boost::python::object main_module;
	boost::python::object main_namespace;
};
#endif // ifndef __SCRIPTMANAGER_H__