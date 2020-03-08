#pragma once
#ifndef __TestObject_h__
#define __TestObject_h__

#include "WarspiteObject.h"
#include "BaseCreator.h"
class TestObject :
	public WarspiteObject
{
public:
	TestObject();
	void collide();

};

class TestObjectCreator : public BaseCreator
{
	GameObject* CreateGameObject() const
	{
		return new TestObject();
	}
};
#endif

