#pragma once
#pragma once
#ifndef __Player__
#define __Player__

#include "SDLGameObject.h"
#include "InputHandler.h"
#include <string.h>

class Player : public SDLGameObject
{
public:
	Player(const ObjectParams* pParams);

	void OnThink();

	void KeyDown();
	void KeyUp();

private:
	void HandleInput();

	bool moving = false;

	bool moveUp = false;
	bool moveRight = false;
};

#endif /* defined(__Player__) */