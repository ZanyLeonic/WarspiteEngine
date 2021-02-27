#pragma once
#ifndef __CALLBACKHANDLER_H__
#define __CALLBACKHANDLER_H__

#include <map>
#include <string>
#include <functional>
#include <spdlog/spdlog.h>

// Provide a std::function to create an instance of the class
template<typename T>
class CCallbackHandler
{
public:
	CCallbackHandler()
	{
		// Supply the default callback
		m_callbacks["default"] = defaultCall;
	};

	bool AddCallback(std::string pName, T pCallback)
	{
		if (pName.empty()) return false;

		m_callbacks[pName] = pCallback;

		return true;
	};
	bool RemoveCallback(std::string pName)
	{
		if (m_callbacks.find(pName) != m_callbacks.end())
		{
			spdlog::warn("Cannot remove callback \"{}\", since it doesn't exist!", pName);
			return false;
		}

		m_callbacks.erase(pName);

		return true;
	};

	T GetCallback(std::string pName) { return m_callbacks[pName]; }

private:
	std::map<std::string, T> m_callbacks;

	static void defaultCall() {};
};

#endif