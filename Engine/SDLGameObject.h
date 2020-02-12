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

	SDLGameObject();

	virtual void Draw();
	virtual bool OnThink();
	virtual void Destroy();

	virtual void Load(const ObjectParams* pParams);

	virtual inline Vector2D GetPosition()
	{
		return m_position;
	};

	virtual inline Vector2D GetVelocity()
	{
		return m_velocity;
	};

	virtual inline Vector2D GetAcceleration()
	{
		return m_acceleration;
	};

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
