#include "TestObject.h"

TestObject::TestObject()
	: SDLGameObject()
{
}

void TestObject::collide()
{
	std::cout << "Collided!\n";
}
