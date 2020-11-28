#pragma once
#ifndef __IGAMEOBJDICT_H_
#define __IGAMEOBJDICT_H_

#include <string>
#include "EngineTypes.h"
#include "ObjectFactory.h"

class IGameObject;

class IGameObjectDictionary
{
public:
	virtual bool RegisterType(std::string typeID, IObjectFactory<IGameObject>* pCreator) = 0;
	virtual std::unique_ptr<IGameObject> Create(std::string typeID) = 0;
};
#endif