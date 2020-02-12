#pragma once
#ifndef __GameObject__
#define __GameObject__
#include <SDL.h>
#include <iostream>
#include "ObjectParams.h"
#include "Vector2D.h"

class GameObject
{
public:

	virtual void Draw() = 0;
	virtual bool OnThink() = 0;
	virtual void Destroy() = 0;

	virtual void Load(const ObjectParams* pParams) = 0;

	virtual Vector2D GetPosition() = 0;
	
protected:
	GameObject() {} 
	virtual ~GameObject() {}

};

#endif /* defined(__GameObject__) */
