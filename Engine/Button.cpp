#include "Button.h"
#include "InputHandler.h"
#include "Game.h"

CButton::CButton() :
	CWarspiteObject()
{
	m_currentFrame = NO_HOVER; // Frame 0

	// Initial callbacks
	if (m_OnClick == 0) m_OnClick = std::bind(&CButton::onClick, this);
	if (m_OnEnter == 0) m_OnEnter = std::bind(&CButton::onEnter, this);
	if (m_OnLeave == 0) m_OnLeave = std::bind(&CButton::onLeave, this);
}

void CButton::Load(const CObjectParams* pParams)
{
	CWarspiteObject::Load(pParams);
	
	m_onClickID = pParams->GetOnClickID();
	m_onEnterID = pParams->GetOnEnterID();
	m_onLeaveID = pParams->GetOnLeaveID();
}

void CButton::Draw()
{
	CWarspiteObject::Draw();
}

bool CButton::OnThink()
{
	// Get the mouse Position on the screen
	CVector2D* pMousePos = CInputHandler::Instance()->
		GetMousePosition();

	// Is it within the boundaries of the CButton?
	if (pMousePos->GetX() < (m_position.GetX() + m_width)
		&& pMousePos->GetX() > m_position.GetX()
		&& pMousePos->GetY() < (m_position.GetY() + m_height)
		&& pMousePos->GetY() > m_position.GetY())
	{
		// Have they pressed down on the CButton while within the boundaries?
		if (CInputHandler::Instance()->GetMouseButtonState(LEFT) && m_bReleased)
		{
			m_currentFrame = PRESSED;
			if (!m_OnClick()) // call the callback noooow!
			{
				return false;
			}
			m_bReleased = false;
		}
		else if (!CInputHandler::Instance()->GetMouseButtonState(LEFT))
		{
			m_bReleased = true;
			m_currentFrame = HOVER;
			m_OnEnter();
		}
	}
	else if (m_currentFrame != NO_HOVER)
	{
		// reset yourself
		m_currentFrame = NO_HOVER;
		m_OnLeave();
	}

	return true;
}

bool CButton::onClick()
{
	// default for the callback
	return true;
}

bool CButton::onEnter()
{
	// default for the callback
	return true;
}

bool CButton::onLeave()
{
	// default for the callback
	return true;
}

void CButton::Destroy()
{
	CWarspiteObject::Destroy();
}

void CButton::OnClick(ButtonCallback e)
{
	m_OnClick = e != 0 ? e : std::bind(&CButton::onClick, this);
}

void CButton::OnEnter(ButtonCallback e)
{
	m_OnEnter = e != 0 ? e : std::bind(&CButton::onEnter, this);
}

void CButton::OnLeave(ButtonCallback e)
{
	m_OnLeave = e != 0 ? e : std::bind(&CButton::onLeave, this);
}
