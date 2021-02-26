#include "CallbackHandler.h"
#include <spdlog/spdlog.h>

template<class T>
CCallbackHandler<T>::CCallbackHandler()
{
	// Supply the default callback
	m_callbacks = { "default" : defaultCall };
}

template<class T>
bool CCallbackHandler<T>::AddCallback(std::string pName, T pCallback)
{
	if (!pName || !pCallback) return false;

	m_callbacks[pName] = pCallback;

	return true;
}

template<class T>
bool CCallbackHandler<T>::RemoveCallback(std::string pName)
{
	if (m_callbacks.find(pName) != m_callbacks.end())
	{
		spdlog::warn("Cannot remove callback \"{}\", since it doesn't exist!", pName);
		return false;
	}

	m_callbacks.erase(pName);

	return true;
}