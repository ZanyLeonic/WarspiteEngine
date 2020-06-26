#pragma once
#ifndef __GameObject__
#define __GameObject__
#include "ObjectParams.h"
#include "Vector2D.h"

class IGameObject
{
public:
	virtual void OnPlay() = 0;
	virtual void Draw() = 0;
	virtual bool OnThink() = 0;
	virtual void Destroy() = 0;

	virtual void Load(const CObjectParams* pParams) = 0;

	virtual CVector2D GetPosition() = 0;
	
protected:
	IGameObject() {} 
	virtual ~IGameObject() {}
};
#endif /* defined(__GameObject__) */