#include "ScriptManager.h"
#include "ScriptWrappers.h"
#include <iostream>
#include <pybind11/embed.h>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#ifdef _WIN32
#include <iostream>
#include <fstream>
#include <filesystem>
#include <Windows.h>
#define PythonPTHName PDLLNAME "._pth"
#endif

CScriptManager* CScriptManager::s_pInstance = 0;

CScriptManager::CScriptManager()
{
	spdlog::info("Initialising ScriptManager...");

	if (CScriptWrappers::Init_Engine())
	{
		try
		{
#ifdef _WIN32
			std::ofstream pathFile;
			std::filesystem::path pPath = CWarspiteUtil::GetExecutingDirectory();
			pPath.append("bin");
			pPath.append(PythonPTHName);

			spdlog::info("Executing object: {}", pPath.string());

			pathFile.open(pPath);
			pathFile << CWarspiteUtil::GetExecutingDirectory() << "platform\\Lib" << std::endl;
			pathFile << CWarspiteUtil::GetExecutingDirectory() << "platform\\DLLs" << std::endl;
			pathFile << CWarspiteUtil::GetExecutingDirectory() << "platform\\Lib\\site-packages" << std::endl;
			pathFile << CWarspiteUtil::GetExecutingDirectory() << "assets\\scripts" << std::endl;
			pathFile.close();

			Py_SetProgramName(Py_DecodeLocale(MOD_NAME, 0));
#endif
			py::initialize_interpreter();
#ifdef _WIN32
			std::filesystem::remove(pPath);
#endif
			main_module = py::module::import("__main__");
			engine_module = py::module::import("engine");
			main_namespace = main_module.attr("__dict__");

			// Show that the ScriptManager is ready
			SGameScript* test = SGameScript::source("internal_autoexec", "import sys\nprint(\"Using Python Runtime %s.%s.%s\" % (sys.version_info.major, sys.version_info.minor, sys.version_info.micro))\nprint(\"Script Manager is ready!\")\nprint(\"Prefix: %s Path: %s\" % (sys.prefix, sys.path))\n");
			Run(test);
		}
		catch (pybind11::error_already_set const& e)
		{
			spdlog::error("***AN ERROR OCCURRED DURING INITIALISATION OF PYTHON!***");
			spdlog::error(e.what());
			spdlog::error("***************************END**************************");

			spdlog::error("Initialisation of ScriptManager failed!");
			Destroy();
		}
	}
	else
	{
		spdlog::error("Initialisation of ScriptManager failed!");
		Destroy();
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
		// Grab all the stdout
		m_stdRedirect = new PyStdErrOutStreamRedirect();

		// Use the namespace provided (if there is one)
		switch (script->GetScriptType())
		{
		case EGameScriptType::SCRIPT_INLINE:
			py::exec(script->GetSource().c_str(), ns != nullptr ? *ns : main_namespace);
			spdlog::info("====== Inline script \"{}\" output:    ======", script->GetScriptName());
			printScriptOutput(m_stdRedirect->stdoutString());
			spdlog::info("====== Inline script \"{}\" output end ======", script->GetScriptName());
			break;
		case EGameScriptType::SCRIPT_FILE:
			py::eval_file(script->GetFilename().c_str(), ns != nullptr ? *ns : main_namespace);
			spdlog::info("====== External script \"{}\" output:     ======", script->GetScriptName());
			printScriptOutput(m_stdRedirect->stdoutString());
			spdlog::info("====== External script \"{}\" output end  ======", script->GetScriptName());
			break;
		default:
			return false; // No type defined? what?
		}

		delete m_stdRedirect;

		return true;
	}
	catch (py::error_already_set const& e)
	{
		switch (script->GetScriptType())
		{
		case EGameScriptType::SCRIPT_INLINE:
			spdlog::error("An internal error occurred when executing an inline script named: \"{}\"", script->GetScriptName());
			break;
		case EGameScriptType::SCRIPT_FILE:
			spdlog::error("An internal error occurred when executing an external script named: \"{}\"", script->GetScriptName());
			break;
		default:
			spdlog::error("An internal error occurred when executing script named: \"{}\"", script->GetScriptName());
			break;
		}
		// Print what happened to not make debugging hell.
		spdlog::error("Error:");
		spdlog::error(e.what());
		spdlog::error("Python stderr:");
		spdlog::error(m_stdRedirect->stderrString());
		spdlog::error("***Error End***");
	}

	delete m_stdRedirect;
	return false;
}

bool CScriptManager::RunFromRef(std::string scriptRef, py::object* ns)
{
	return Run(loadedScripts[scriptRef], ns);
}

void CScriptManager::printScriptOutput(std::string output)
{
	std::vector<std::string> lines = CWarspiteUtil::SplitString(output, '\n');

	for (size_t i = 0; i < lines.size(); i++)
	{
		spdlog::info(lines[i]);
	}
}