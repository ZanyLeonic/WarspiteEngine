#pragma once
#ifndef __SCRIPTMANAGER_H__
#define __SCRIPTMANAGER_H__

#include <iostream>
#include <map>
#include <memory>
#include "Level.h"
#include "Game.h"
#include "IScriptManager.h"
#include <spdlog/spdlog.h>

#ifdef _WIN32
class CScriptManager : public IScriptManager
#elif _UNIX
class __attribute__ ((visibility("hidden"))) CScriptManager : public IScriptManager
#endif
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

	virtual void Destroy();

	virtual void Load(SGameScript* script);
	virtual bool Remove(const char* scriptRef);
	virtual void RemoveAll();

	virtual bool Run(SGameScript* script, py::object* ns = nullptr);
	virtual bool RunFromRef(std::string scriptRef, py::object* ns = nullptr);

	virtual py::module GetMainModule() const { return main_module; }
	virtual py::module GetEngineModule() const { return engine_module; }
	virtual py::object GetMainNamespace() const { return main_namespace; }
	
private:
	py::module main_module;
	py::module engine_module;
	py::object main_namespace;

	void printScriptOutput(std::string output);
	
	std::shared_ptr<spdlog::logger> m_scriptLogger;

	std::map<std::string, SGameScript*> loadedScripts;

	PyStdErrOutStreamRedirect* m_stdRedirect;
};
#endif // ifndef __SCRIPTMANAGER_H__
