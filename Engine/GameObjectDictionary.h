#pragma once
#ifndef __GameObjectDictionary_H__
#define __GameObjectDictionary_H__

#include <string>
#include <map>

class GameObject;
class BaseCreator;

class IObjectFactory
{
public:
	virtual GameObject* Create(std::string pMapRef) = 0;
	virtual size_t GetObjectSize() = 0;
};

class GameObjectDictionary
{
private:
	GameObjectDictionary() {};
	static GameObjectDictionary* s_pInstance;

public:
	static GameObjectDictionary* Instance()
	{
		if (s_pInstance == 0)
		{
			s_pInstance = new GameObjectDictionary();
			return s_pInstance;
		}
		return s_pInstance;
	}

	bool RegisterType(std::string typeID, IObjectFactory* pCreator);
	GameObject* Create(std::string typeID);

	template <class T>
	T* _CreateObject(T* newObj, std::string className);
private:
	std::map<std::string, IObjectFactory*> m_creators;

};

template <class T>
class GameObjectFactory : public IObjectFactory
{
	GameObjectFactory(std::string pMapRef)
	{
		GameObjectDictionary::Instance()->RegisterType(pMapRef, this);
	}

	GameObject* Create(std::string pMapRef)
	{
		T* pObj = GameObjectDictionary::Instance()->Create(pMapRef);
		return pObj;
	}

	virtual size_t GetObjectSize()
	{
		return sizeof(T);
	}
};

#define REG_OBJ_TO_REF(mapRefName, CompiledClassName) \
	static GameObjectFactory<CompiledClassName> mapRefName( #CompiledClassName );

#endif
