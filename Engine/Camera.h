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

	void OnThink(Vector2D cVel);

	void SetTarget(Vector2D* target) 
	{ 
		m_pTarget = target; 
	}
	void SetPosition(const Vector2D& position) 
	{
		m_position = position;
	}

	Vector2D GetPosition() const;

private:

	Camera();
	~Camera();

	Vector2D* m_pTarget = new Vector2D();
	
	Vector2D m_position;

	static Camera* s_pCamera;
};
#endif
