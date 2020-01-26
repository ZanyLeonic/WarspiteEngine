#pragma once
#ifndef __GameObject__
#define __GameObject__
#include <SDL.h>
#include <iostream>
#include "ObjectParams.h"

class GameObject
{
public:

	virtual void Draw() = 0;
	virtual bool OnThink() = 0;
	virtual void Destroy() = 0;
	
protected:
	GameObject(const ObjectParams* pParams) {} 
	virtual ~GameObject() {}

};

#endif /* defined(__GameObject__) */
