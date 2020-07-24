#include "Game.h"
#include "Camera.h"

CCamera* CCamera::s_pCamera = 0;

CCamera::CCamera()
{}

void CCamera::OnThink()
{
}

CVector2D CCamera::GetPositionT() const
{
	if (m_pTarget != 0)
	{
		CVector2D vsz = CGame::Instance()->GetViewportSize();

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

		// Return the new offsetted position
		return pos;
	}

	// No target? return the last stored position.
	return m_position;
}