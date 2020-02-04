#include "TestObject.h"

TestObject::TestObject(ObjectParams* pParams)
	: SDLGameObject(pParams)
{
}

void TestObject::collide()
{
	std::cout << "Collided!\n";
}
