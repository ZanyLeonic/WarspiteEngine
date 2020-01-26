#pragma once
#pragma once
#ifndef __SDLGameObject__
#define __SDLGameObject__

#include "GameObject.h"
#include "Vector2D.h"

class SDLGameObject :
	public GameObject
{
public:

	SDLGameObject(const ObjectParams* pParams);

	virtual void Draw();
	virtual bool OnThink();
	virtual void Destroy();

protected:

	Vector2D m_position;
	Vector2D m_velocity;
	Vector2D m_acceleration;

	int m_width;
	int m_height;

	int m_currentRow;
	int m_currentFrame;

	std::string m_textureID;
};

#endif /* defined(__SDLGameObject__) */
