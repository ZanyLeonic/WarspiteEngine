#pragma once
#ifndef __CALLBACKHANDLER_H__
#define __CALLBACKHANDLER_H__

#include <map>
#include <string>
#include <functional>

// Provide a std::function to create an instance of the class
template<class T>
class CCallbackHandler
{
public:
	CCallbackHandler();

	bool AddCallback(std::string pName, T pCallback);
	bool RemoveCallback(std::string pName);

	T GetCallback(std::string pName) { return m_callbacks[pName]; }

private:
	std::map<std::string, T> m_callbacks;

	static void defaultCall() {};
};

#endif