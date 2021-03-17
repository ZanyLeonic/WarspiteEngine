#include "Button.h"
#include "InputHandler.h"
#include "Game.h"

CButton::CButton() :
	CUIBase()
{
	m_iCurrentFrame = NO_HOVER; // Frame 0
	m_initalPos = *CInputHandler::Instance()->GetMousePosition();
}

bool CButton::OnThink()
{
	// Get the mouse Position on the screen
	CVector2D* pMousePos = CInputHandler::Instance()->
		GetMousePosition();

	// Stop clicking a button while changing states
	if (m_initalPos == *pMousePos) return false;

	// Is it within the boundaries of the CButton?
	if (pMousePos->GetX() < (m_vPosition.GetX() + m_iWidth)
		&& pMousePos->GetX() > m_vPosition.GetX()
		&& pMousePos->GetY() < (m_vPosition.GetY() + m_iHeight)
		&& pMousePos->GetY() > m_vPosition.GetY())
	{
		// Have they pressed down on the CButton while within the boundaries?
		if (CInputHandler::Instance()->GetMouseButtonState(LEFT) && m_bReleased)
		{
			m_iCurrentFrame = PRESSED;
			if (!m_OnClick()) // call the callback noooow!
			{
				return false;
			}
			m_bReleased = false;
		}
		else if (!CInputHandler::Instance()->GetMouseButtonState(LEFT))
		{
			m_bReleased = true;
			m_iCurrentFrame = HOVER;
			m_OnEnter();
		}
	}
	else if (m_iCurrentFrame != NO_HOVER)
	{
		// reset yourself
		m_iCurrentFrame = NO_HOVER;
		m_OnLeave();
	}

	SDL_Delay(5);

	return true;
}