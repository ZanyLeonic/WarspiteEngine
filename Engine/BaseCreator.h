#pragma once
#ifndef __CreatorBase_H__
#define __CreatorBase_H__

#include <string>
#include <map>
#include "GameObject.h"

class BaseCreator
{
public:
	virtual GameObject* CreateGameObject() const = 0;
	virtual ~BaseCreator() {}
};
#endif
