#pragma once
#ifndef __IGAMESTATEDICT_H_
#define __IGAMESTATEDICT_H_

#include <string>
#include <memory>
#include "EngineTypes.h"
#include "ObjectFactory.h"

#ifdef _GAME_
#include "WGame.h"
#endif

class CGameStateBase;

class IGameStateDictionary
{
public:
	virtual bool RegisterType(std::string stateID, IObjectFactory<CGameStateBase>* pCreator) = 0;
	virtual std::unique_ptr<CGameStateBase> Create(std::string stateID) = 0;
};

#endif