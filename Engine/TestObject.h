#pragma once
#ifndef __TestObject_h__
#define __TestObject_h__

#include "MapObject.h"

class CTestObject :
	public CMapObject
{
public:
	CTestObject();
	void collide();
};

REG_OBJ_TO_REF(TestObject, CTestObject);

#endif