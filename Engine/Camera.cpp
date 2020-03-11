#include "Game.h"
#include "Camera.h"

Camera* Camera::s_pCamera = 0;

Camera::Camera()
{}

void Camera::OnThink()
{

}

Vector2D Camera::GetPosition() const
{
	if (m_pTarget != 0)
	{
		Vector2D pos(((m_pTarget->GetX()) - (Game::Instance()->GetViewportSize().GetX() / 2)),
			((m_pTarget->GetY()) - (Game::Instance()->GetViewportSize().GetY() / 2))) ;

		if (pos.GetX() < 0)
		{
			pos.SetX(0);
		}

		if (pos.GetY() < 0)
		{
			pos.SetY(0);
		}

		if (pos.GetX() > Game::Instance()->GetViewportSize().GetX())
		{
			pos.SetX(Game::Instance()->GetViewportSize().GetX());
		}

		if (pos.GetY() > Game::Instance()->GetViewportSize().GetY())
		{
			pos.SetY(Game::Instance()->GetViewportSize().GetY());
		}

		return pos;
	}
	return m_position;
}
