#include "Button.h"
#include "InputHandler.h"

Button::Button(const ObjectParams* pParams, void (*onclick)()) :
	SDLGameObject(pParams), m_OnClick(onclick)
{
	m_currentFrame = NO_HOVER; // Frame 0
}

void Button::Draw()
{
	SDLGameObject::Draw();
}

void Button::OnThink()
{
	// Get the mouse position on the screen
	Vector2D* pMousePos = InputHandler::Instance()->
		GetMousePosition();

	// Is it within the boundaries of the button?
	if (pMousePos->GetX() < (m_position.GetX() + m_width)
		&& pMousePos->GetX() > m_position.GetX()
		&& pMousePos->GetY() < (m_position.GetY() + m_height)
		&& pMousePos->GetY() > m_position.GetY())
	{
		// Have they pressed down on the button while within the boundaries?
		if (InputHandler::Instance()->GetMouseButtonState(LEFT) && m_bReleased)
		{
			m_currentFrame = PRESSED;
			m_OnClick(); // call the callback noooow!
			m_bReleased = false;
		}
		else if (!InputHandler::Instance()->GetMouseButtonState(LEFT))
		{
			m_bReleased = true;
			m_currentFrame = HOVER;
		}
	}
	else
	{
		// reset yourself
		m_currentFrame = NO_HOVER;
	}
}

void Button::Destroy()
{
	SDLGameObject::Destroy();
}