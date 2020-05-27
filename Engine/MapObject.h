#pragma once
#ifndef __MAPOBJECT_H__
#define __MAPOBJECT_H__

#include "WarspiteObject.h"
class MapObject :
	public WarspiteObject
{
public:
	MapObject();
	virtual void Draw();
};

#endif // __MAPOBJECT_H__