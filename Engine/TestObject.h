#pragma once
#ifndef __TestObject_h__
#define __TestObject_h__

#include "MapObject.h"
#include "BaseCreator.h"
class TestObject :
	public MapObject
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

