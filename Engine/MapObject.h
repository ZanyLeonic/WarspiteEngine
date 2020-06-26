#pragma once
#ifndef __MAPOBJECT_H__
#define __MAPOBJECT_H__

#include "WarspiteObject.h"
class CMapObject :
	public CWarspiteObject
{
public:
	CMapObject();
	virtual void Draw();
};

#endif // __MAPOBJECT_H__