#pragma once

#include "Vector2D.h"

#ifndef __CAMERA_H__
#define __CAMERA_H__
class Camera
{
public:
	static Camera* Instance()
	{
		if (s_pCamera == 0)
		{
			s_pCamera = new Camera();
		}

		return s_pCamera;
	}

	void OnThink();

	void SetTarget(Vector2D* target) 
	{ 
		m_pTarget = target; 
	}

	void SetPosition(const Vector2D& position) 
	{
		m_position = position;
	}

	void SetLevelSize(const Vector2D& sz)
	{
		m_levelSize = sz;
	}

	Vector2D GetLevelSize() const
	{
		return m_levelSize;
	}

	Vector2D GetPosition() const
	{
		return m_position;
	}

	Vector2D GetPositionT() const;

private:
	Camera();
	~Camera();

	Vector2D* m_pTarget = 0;
	
	Vector2D m_position;
	Vector2D m_levelSize;

	static Camera* s_pCamera;
};
#endif
