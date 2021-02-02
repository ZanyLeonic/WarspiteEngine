#include "TestGObject.h"
#include "WGame.h"
#include <iostream>

CTestGObject::CTestGObject()
{
}

void CTestGObject::Draw()
{}

void CTestGObject::DoDraw(bool newVis)
{
	bDrawObject = newVis;
}