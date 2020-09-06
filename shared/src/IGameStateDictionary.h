#pragma once
#ifndef __IGAMESTATEDICT_H_
#define __IGAMESTATEDICT_H_

#include <string>
#include "EngineTypes.h"
#include "ObjectFactory.h"

class CGameStateBase;

class IGameStateDictionary
{
public:
	virtual bool RegisterType(std::string stateID, IObjectFactory<CGameStateBase>* pCreator) = 0;
	virtual CGameStateBase* Create(std::string stateID) = 0;
};
#endif