#pragma once
#pragma once
#ifndef __CWarspiteObject__
#define __CWarspiteObject__

#include "GameObject.h"
#include "GameObjectDictionary.h"
#include "Vector2D.h"

class CWarspiteObject :
	public IGameObject
{
public:
	CWarspiteObject();

	virtual void OnPlay();
	virtual void Draw();
	virtual bool OnThink();
	virtual void Destroy();

	virtual void Load(const CObjectParams* pParams);

	virtual inline CVector2D GetPosition()
	{
		return m_position;
	};

	virtual inline CVector2D GetVelocity()
	{
		return m_velocity;
	};

	virtual inline CVector2D GetAcceleration()
	{
		return m_acceleration;
	};

protected:
	CVector2D m_position;
	CVector2D m_velocity;
	CVector2D m_acceleration;

	int m_width;
	int m_height;

	int m_currentRow;
	int m_currentFrame;
	int m_numFrames;

	std::string m_textureID = "";
};

#endif /* defined(__CWarspiteObject__) */
