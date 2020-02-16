#pragma once
#ifndef __TestObject_h__
#define __TestObject_h__

#include "SDLGameObject.h"
#include "BaseCreator.h"
class TestObject :
	public SDLGameObject
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

