#pragma once
#ifndef __TestGObject_h__
#define __TestGObject_h__

#include "GameObject.h"
#include "IGameObjectDictionary.h"

class CTestGObject :
	public IGameObject
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