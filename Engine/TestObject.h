#pragma once
#ifndef __TestObject_h__
#define __TestObject_h__

#include "MapObject.h"
#include "GameObjectDictionary.h"

class TestObject :
	public MapObject
{
public:
	TestObject();
	void collide();
};

REG_OBJ_TO_REF(TestObject, TestObject);

#endif