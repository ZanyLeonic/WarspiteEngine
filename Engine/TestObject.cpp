#include "TestObject.h"

TestObject::TestObject()
	: MapObject()
{
}

void TestObject::collide()
{
	std::cout << "Collided!\n";
}