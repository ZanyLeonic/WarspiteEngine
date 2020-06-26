#pragma once
#ifndef __GameObjectDictionary_H__
#define __GameObjectDictionary_H__

#include <string>
#include <map>

class IGameObject;

class IObjectFactory
{
public:
	virtual IGameObject* Create() = 0;
	virtual size_t GetObjectSize() = 0;
};

class CGameObjectDictionary
{
private:
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

template <class T>
class CGameObjectFactory : public IObjectFactory
{
public:
	CGameObjectFactory(std::string pMapRef)
	{
		CGameObjectDictionary::Instance()->RegisterType(pMapRef, this);
	}

	IGameObject* Create()
	{
		return new T;
	}

	virtual size_t GetObjectSize()
	{
		return sizeof(T);
	}
};

#define REG_OBJ_TO_REF(mapRefName, compiledClassName) \
	static CGameObjectFactory<compiledClassName> mapRefName( #mapRefName );

#endif
