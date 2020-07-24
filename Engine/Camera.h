#pragma once

#include "Vector2D.h"

#ifndef __CAMERA_H__
#define __CAMERA_H__
class CCamera
{
public:
	static CCamera* Instance()
	{
		if (s_pCamera == 0)
		{
			s_pCamera = new CCamera();
		}

		return s_pCamera;
	}

	void OnThink();

	void SetTarget(CVector2D* target) { m_pTarget = target; }

	void SetPosition(const CVector2D& position) { m_position = position; }

	void SetLevelSize(const CVector2D& sz) { m_levelSize = sz; }

	CVector2D GetLevelSize() const { return m_levelSize; }

	CVector2D GetPosition() const { return m_position; }

	CVector2D GetPositionT() const;

	CVector2D* GetTarget() const { return m_pTarget; }

private:
	CCamera();
	~CCamera();

	CVector2D* m_pTarget = 0;
	
	CVector2D m_position;
	CVector2D m_levelSize;

	static CCamera* s_pCamera;
};
#endif
