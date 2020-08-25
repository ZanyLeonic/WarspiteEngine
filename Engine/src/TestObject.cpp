#include "TestObject.h"
#include <iostream>

CTestObject::CTestObject()
	: CMapObject()
{
}

void CTestObject::collide()
{
	std::cout << "Collided!\n";
}