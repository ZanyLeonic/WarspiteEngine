#pragma once
#ifndef __StateRegisterBuffer_H__
#define __StateRegisterBuffer_H__

#include <string>
#include <map>
#include "ObjectFactory.h"

class CGameStateBase;

class CStateRegisterBuffer
{
public:
	static CStateRegisterBuffer* Instance()
	{
		if (s_pInstance == 0)
		{
			s_pInstance = new CStateRegisterBuffer();
			return s_pInstance;
		}
		return s_pInstance;
	}
private:
	CStateRegisterBuffer() {};
	~CStateRegisterBuffer() {};
	
	static CStateRegisterBuffer* s_pInstance;
public:
	// add all the objects to the Engine
    bool RegisterType(std::string stateID, IObjectFactory<CGameStateBase>* pCreator);
	bool AddAllRegistered();

private:
    std::map<std::string, IObjectFactory<CGameStateBase>*> m_creators;
};
#endif // #ifdef(__ObjectRegisterBuffer_H__)