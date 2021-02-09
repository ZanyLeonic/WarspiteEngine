#pragma once
#ifndef __BUTTON_H__
#define __BUTTON_H__

#include "UIBase.h"

class CButton : public CUIBase
{
public:
	CButton();

	virtual bool OnThink();

	int GetOnClickID() const { return m_onClickID; }
	int GetOnEnterID() const { return m_onEnterID; }
	int GetOnLeaveID() const { return m_onLeaveID; }

private:
	enum button_state
	{
		NO_HOVER = 0,
		HOVER = 1,
		PRESSED = 2
	};

private:
	// Whether the button has been released or not.
	bool m_bReleased = true;

	int m_onClickID = 0;
	int m_onEnterID = 0;
	int m_onLeaveID = 0;	

	CVector2D m_initalPos;
};
REG_OBJ_TO_REF(Button, CButton);
#endif