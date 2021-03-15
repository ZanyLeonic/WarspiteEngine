#include "Game.h"
#include "Camera.h"
#include "ScriptManager.h"
#include "ScriptWrappers.h"

CCamera* CCamera::s_pCamera = 0;

CCamera::CCamera()
{
	// Expose the current camera to the Python API
	if(CScriptManager::Instance()->GetEngineModule().attr(CAMERAOBJECT_NAME).is_none())
	{
		m_pCameraPtr = std::make_shared<SCameraObject>(SCameraObject(this));
		CScriptManager::Instance()->GetEngineModule().attr(CAMERAOBJECT_NAME) = m_pCameraPtr;
	}
}

void CCamera::OnThink()
{
}

CVector2D CCamera::GetPositionT() 
{
	if (m_pTarget != 0)
	{
		CVector2D vsz = CBaseGame::Instance()->GetViewportSize();

		// Offset the target by half of the viewport
		CVector2D pos(((m_pTarget->GetX()) - (vsz.GetX() / 2)),
			((m_pTarget->GetY()) - (vsz.GetY() / 2))) ;

		// if the X and Y are less than 0 � that means the camera is not past half of
		// the screen � set to 0. 
		if (pos.GetX() < 0)
		{
			pos.SetX(0);
		}

		if (pos.GetY() < 0)
		{
			pos.SetY(0);
		}

		float fScaleF = vsz.GetX() / m_vLevelSize.GetX();

		if (pos.GetX() * 2 >= m_vLevelSize.GetX())
		{
			pos.SetX(fScaleF * m_vLevelSize.GetX());
		}

		if (pos.GetY() * 2 > m_vLevelSize.GetY())
		{
			pos.SetY(fScaleF * m_vLevelSize.GetY());
		}

		// Return the new offsetted position
		return pos;
	}

	// No target? return the last stored position.
	return m_vPosition;
}