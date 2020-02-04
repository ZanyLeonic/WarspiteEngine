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

	bool OnThink();

	void KeyDown();
	void KeyUp();

private:
	void HandleInput();

	bool IsPositionFree(Vector2D* pNext);

	bool moving = false;

	bool moveUp = false;
	bool moveRight = false;

	int timeLeft = 0;
	const int moveStep = 20;
};

#endif /* defined(__Player__) */