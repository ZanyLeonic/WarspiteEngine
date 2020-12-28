#pragma once
#ifndef __GameObjectDictionary_H__
#define __GameObjectDictionary_H__

#include "ObjectFactory.h"
#include "IGameObjectDictionary.h"
#include "GameObject.h"
#include <string>
#include <map>

// Singleton class
class CGameObjectDictionary : public IGameObjectDictionary
{
	CGameObjectDictionary() {}
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

	virtual bool RegisterType(std::string typeID, IObjectFactory<IGameObject>* pCreator);
	virtual std::unique_ptr<IGameObject> Create(std::string typeID);
private:
	std::map<std::string, IObjectFactory<IGameObject>*> m_creators;
};

#endif