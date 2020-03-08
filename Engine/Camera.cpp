#include "Game.h"
#include "Camera.h"

Camera* Camera::s_pCamera = 0;

Camera::Camera()
{}

void Camera::OnThink(Vector2D cVel)
{
}

Vector2D Camera::GetPosition() const
{
	if (m_pTarget != 0)
	{
		Vector2D pos(m_pTarget->GetX() - (Game::Instance()->GetViewportSize().GetX() / 2), 
			m_pTarget->GetY() - (Game::Instance()->GetViewportSize().GetY() / 2));

		if (pos.GetX() < 0)
		{
			pos.SetX(0);
		}

		if (pos.GetY() < 0)
		{
			pos.SetY(0);
		}

		return pos;
	}
	return m_position;
}
