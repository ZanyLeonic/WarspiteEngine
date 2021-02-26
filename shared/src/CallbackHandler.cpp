#include "CallbackHandler.h"
#include <spdlog/spdlog.h>

CCallbackHandler::CCallbackHandler()
{
	// Supply the default callback
	m_callbacks = {
		{ "default", defaultCall }
	};
}

bool CCallbackHandler::AddCallback(std::string pName, T pCallback)
{
	if (!pName || !pCallback) return false;

	m_callbacks[pName] = pCallback;

	return true;
}

bool CCallbackHandler::RemoveCallback(std::string pName)
{
	if (m_callbacks.find(pName) != m_callbacks.end())
	{
		spdlog::warn("Cannot remove callback \"{}\", since it doesn't exist!", pName);
		return false;
	}

	m_callbacks.erase(pName);

	return true;
}