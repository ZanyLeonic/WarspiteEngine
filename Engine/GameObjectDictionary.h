#pragma once
#ifndef __GameObjectDictionary_H__
#define __GameObjectDictionary_H__

#include <string>
#include <map>

class IGameObject;

// Overridable base stuff
class IObjectFactory
{
public:
	virtual IGameObject* Create() = 0;
	virtual size_t GetObjectSize() = 0;
};

// Singleton class
class CGameObjectDictionary
{
	CGameObjectDictionary() {};
	static CGameObjectDictionary* s_pInstance;

public:
	static CGameObjectDictionary* Instance()
	{
		if (s_pInstance == 0)
		{
			s_pInstance = new CGameObjectDictionary();
			return s_pInstance;
		}
		return s_pInstance;
	}

	bool RegisterType(std::string typeID, IObjectFactory* pCreator);
	IGameObject* Create(std::string typeID);
private:
	std::map<std::string, IObjectFactory*> m_creators;
};

// For real - what will create our GameObject.
// A template class to know what we are creating.
template <class T>
class CGameObjectFactory : public IObjectFactory
{
public:
	// When the class is created - take the respective map reference string
	CGameObjectFactory(std::string pMapRef)
	{
		// Register ourselves in the dictionary.
		CGameObjectDictionary::Instance()->RegisterType(pMapRef, this);
	}

	// Simply returns the type our template is.
	IGameObject* Create()
	{
		return new T;
	}

	// Returns the size of our template
	virtual size_t GetObjectSize()
	{
		return sizeof(T);
	}
};

// Macro to make registering classes easier
#define REG_OBJ_TO_REF(mapRefName, compiledClassName) \
	static CGameObjectFactory<compiledClassName> mapRefName( #mapRefName );

#endif