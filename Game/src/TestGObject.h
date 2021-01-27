#pragma once
#ifndef __TestGObject_h__
#define __TestGObject_h__

#include "WarspiteObject.h"
#include "IGameObjectDictionary.h"

class CTestGObject :
	public CWarspiteObject
{
public:
	CTestGObject();
	virtual void Draw();
	virtual void DoDraw(bool newVis);
private:
	bool bDrawObject = true;
};

REG_OBJ_TO_REF(TestGObject, CTestGObject);

#endif