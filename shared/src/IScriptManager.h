#pragma once
#ifndef __ISCRIPTMANAGER_H__
#define __ISCRIPTMANAGER_H__

#include "EngineTypes.h"
#include <pybind11/pybind11.h>

class PyStdErrOutStreamRedirect;
namespace py = pybind11;

enum class EGameScriptType
{
	NONE = 0,
	SCRIPT_INLINE = 1,
	SCRIPT_FILE = 2
};

struct SGameScript
{
	static SGameScript* file(std::string sName, std::string sPath)
	{
		return new SGameScript(sName, EGameScriptType::SCRIPT_FILE, sPath);
	}

	static SGameScript* source(std::string sName, std::string sSource)
	{
		return new SGameScript(sName, EGameScriptType::SCRIPT_INLINE, sSource);
	}
	
	std::string GetScriptName()  { return m_ScriptName; }
	
	std::string GetFilename() const { return m_FileName; }
	std::string GetSource() const { return m_ScriptSource; }
	
	EGameScriptType GetScriptType() const { return m_type; }

	bool operator==(const SGameScript s1) const
	{
		return (m_ScriptName == s1.m_ScriptName) && (m_FileName == s1.m_FileName)
				&& (m_ScriptSource == s1.m_ScriptSource) && (m_type == s1.m_type);
	}
	
private:
	SGameScript(std::string sName, EGameScriptType sType, std::string sData)
		: m_type(sType), m_ScriptName(sName)
	{
		if (sType == EGameScriptType::SCRIPT_FILE)
			m_FileName = sData;
		else if (sType == EGameScriptType::SCRIPT_INLINE)
			m_ScriptSource = sData;
	}
	
	EGameScriptType m_type = EGameScriptType::NONE;
	std::string m_ScriptName = "";

	std::string m_ScriptSource = "";
	std::string m_FileName = "";
};

class IScriptManager
{
public:
    virtual void Destroy() = 0;

	virtual void Load(SGameScript* script) = 0;
	virtual bool Remove(const char* scriptRef) = 0;
	virtual void RemoveAll() = 0;

	virtual bool Run(SGameScript* script, py::object* ns = nullptr) = 0;
	virtual bool RunFromRef(std::string scriptRef, py::object* ns = nullptr) = 0;

	virtual py::module GetMainModule() const = 0;
	virtual py::module GetEngineModule() const = 0;
	virtual py::object GetMainNamespace() const = 0;
};
#endif