#pragma once
#ifndef __GameObjectDictionary_H__
#define __GameObjectDictionary_H__

#include <string>
#include <map>

class CGameObject;

class IObjectFactory
{
public:
	virtual CGameObject* Create() = 0;
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
	CGameObject* Create(std::string typeID);
private:
	std::map<std::string, IObjectFactory*> m_creators;

};

template <class T>
class GameObjectFactory : public IObjectFactory
{
public:
	GameObjectFactory(std::string pMapRef)
	{
		GameObjectDictionary::Instance()->RegisterType(pMapRef, this);
	}

	CGameObject* Create()
	{
		return new T;
	}

	virtual size_t GetObjectSize()
	{
		return sizeof(T);
	}
};

#define REG_OBJ_TO_REF(mapRefName, compiledClassName) \
	static GameObjectFactory<compiledClassName> mapRefName( #mapRefName );

#endif
