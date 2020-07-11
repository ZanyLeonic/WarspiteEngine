#pragma once
#ifndef __SCRIPTMANAGER_H__
#define __SCRIPTMANAGER_H__

#include <boost/python.hpp>
#include <map>

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

	void Load(SGameScript* script);
	bool Remove(const char* scriptRef);
	void RemoveAll();
	
	bool Run(SGameScript* script, boost::python::object* ns = 0);
	bool RunFromRef(std::string scriptRef, boost::python::object* ns = 0);
private:
	boost::python::object main_module;
	boost::python::object main_namespace;

	std::map<std::string, SGameScript*> loadedScripts;
};
#endif // ifndef __SCRIPTMANAGER_H__