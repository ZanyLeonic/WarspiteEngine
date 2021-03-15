#pragma once
#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "ICamera.h"
#include <memory>
#include "Vector2D.h"

struct SCameraObject;
typedef std::shared_ptr<SCameraObject> PCameraPtr;

class CCamera : public ICamera
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
	void SetPosition(const CVector2D& position) { m_vPosition = position; }
	void SetLevelSize(const CVector2D& sz) { m_vLevelSize = sz; }
	
	CVector2D GetLevelSize() const { return m_vLevelSize; }
	CVector2D GetPosition() const { return m_vPosition; }
	CVector2D GetPositionT();
	CVector2D* GetTarget() const { return m_pTarget; }

private:
	CCamera();
	~CCamera() {};

	CVector2D* m_pTarget = 0;
	
	CVector2D m_vPosition;
	CVector2D m_vLevelSize;
	CVector2D m_vLastSafeTarget;

	PCameraPtr m_pCameraPtr;
	
	static CCamera* s_pCamera;
};
#endif
