#pragma once
#ifndef __GameObject__
#define __GameObject__
#include "ObjectParams.h"
#include "Vector2D.h"

class CGameObject
{
public:
	virtual void OnPlay() = 0;
	virtual void Draw() = 0;
	virtual bool OnThink() = 0;
	virtual void Destroy() = 0;

	virtual void Load(const ObjectParams* pParams) = 0;

	virtual Vector2D GetPosition() = 0;
	
protected:
	CGameObject() {} 
	virtual ~CGameObject() {}
};
#endif /* defined(__GameObject__) */