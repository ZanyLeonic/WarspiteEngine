#pragma once
#include "SDLGameObject.h"
class TestObject :
	public SDLGameObject
{
public:
	TestObject(ObjectParams* pParams);
	void collide();

};

