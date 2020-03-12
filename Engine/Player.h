#pragma once
#pragma once
#ifndef __Player__
#define __Player__

#include "WarspiteObject.h"
#include "InputHandler.h"
#include "Vector2D.h"
#include "BaseCreator.h"
#include <string.h>

class Player : public WarspiteObject
{
public:
	Player();

	void OnPlay();

	void Load(const ObjectParams* pParams);

	bool OnThink();
	void Draw();

	void KeyDown();
	void KeyUp();

private:
	void HandleInput();

	bool IsPositionFree(Vector2D* pNext);

	std::vector<std::vector<GameObject*>*> m_objects;

	Vector2D lastPosition;
	Vector2D nextPosition;

	bool moving = false;

	bool moveUp = false;
	bool moveRight = false;

	float m_timeLeft = 100;
	const int m_moveStep = 32;
	Vector2D m_CamOffset;
};

class PlayerCreator : public BaseCreator
{
	GameObject* CreateGameObject() const
	{
		return new Player();
	}
};

#endif /* defined(__Player__) */