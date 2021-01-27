#pragma once
#ifndef __ObjectRegisterBuffer_H__
#define __ObjectRegisterBuffer_H__

#include <string>
#include <map>
#include "ObjectFactory.h"

class IGameObject;

class CObjectRegisterBuffer
{
public:
	static CObjectRegisterBuffer* Instance()
	{
		if (s_pInstance == 0)
		{
			s_pInstance = new CObjectRegisterBuffer();
			return s_pInstance;
		}
		return s_pInstance;
	}
private:
	CObjectRegisterBuffer() {};
	~CObjectRegisterBuffer() {};
	
	static CObjectRegisterBuffer* s_pInstance;
public:
	// add all the objects to the Engine
    bool RegisterType(std::string typeID, IObjectFactory<IGameObject>* pCreator);
	bool AddAllRegistered();

private:
    std::map<std::string, IObjectFactory<IGameObject>*> m_creators;
};
#endif // #ifdef(__ObjectRegisterBuffer_H__)