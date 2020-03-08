#include "Button.h"
#include "InputHandler.h"
#include "Game.h"

Button::Button() :
	WarspiteObject()
{
	m_currentFrame = NO_HOVER; // Frame 0

	// Initial callbacks
	if (m_OnClick == 0) m_OnClick = std::bind(&Button::onClick, this);
	if (m_OnEnter == 0) m_OnEnter = std::bind(&Button::onEnter, this);
	if (m_OnLeave == 0) m_OnLeave = std::bind(&Button::onLeave, this);
}

void Button::Load(const ObjectParams* pParams)
{
	WarspiteObject::Load(pParams);
	
	m_onClickID = pParams->GetOnClickID();
	m_onEnterID = pParams->GetOnEnterID();
	m_onLeaveID = pParams->GetOnLeaveID();
}

void Button::Draw()
{
	WarspiteObject::Draw();
}

bool Button::OnThink()
{
	// Get the mouse Position on the screen
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
			if (!m_OnClick()) // call the callback noooow!
			{
				return false;
			}
			m_bReleased = false;
		}
		else if (!InputHandler::Instance()->GetMouseButtonState(LEFT))
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

bool Button::onClick()
{
	// default for the callback
	return true;
}

bool Button::onEnter()
{
	// default for the callback
	return true;
}

bool Button::onLeave()
{
	// default for the callback
	return true;
}

void Button::Destroy()
{
	WarspiteObject::Destroy();
}

void Button::OnClick(ButtonCallback e)
{
	m_OnClick = e != 0 ? e : std::bind(&Button::onClick, this);
}

void Button::OnEnter(ButtonCallback e)
{
	m_OnEnter = e != 0 ? e : std::bind(&Button::onEnter, this);
}

void Button::OnLeave(ButtonCallback e)
{
	m_OnLeave = e != 0 ? e : std::bind(&Button::onLeave, this);
}
