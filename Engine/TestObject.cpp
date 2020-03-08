#include "TestObject.h"

TestObject::TestObject()
	: WarspiteObject()
{
}

void TestObject::collide()
{
	std::cout << "Collided!\n";
}
