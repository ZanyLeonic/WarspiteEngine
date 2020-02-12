#pragma once
#ifndef __BUTTON_H__
#define __BUTTON_H__

#include <functional>
#include "SDLGameObject.h"

typedef std::function<bool()> ButtonCallback;

class Button : public SDLGameObject
{
public:

	Button(ButtonCallback onClick=0, ButtonCallback onEnter=0, ButtonCallback onLeave=0);

	virtual void Load(const ObjectParams* pParams);

	virtual void Draw();
	virtual bool OnThink();
	virtual void Destroy();

	virtual inline void OnClick(ButtonCallback e)
	{
		m_OnClick = e;
	};
	virtual inline void OnEnter(ButtonCallback e)
	{
		m_OnEnter = e;
	};
	virtual inline void OnLeave(ButtonCallback e)
	{
		m_OnLeave = e;
	};

private:
	// Holds the callback to trigger on events
	ButtonCallback m_OnClick;
	ButtonCallback m_OnEnter;
	ButtonCallback m_OnLeave;

	enum button_state
	{
		NO_HOVER = 0,
		HOVER = 1,
		PRESSED = 2
	};

protected:
	// Initial values for callbacks
	virtual bool onClick();
	virtual bool onEnter();
	virtual bool onLeave();

private:
	// Whether the button has been released or not.
	bool m_bReleased = true;
};

#endif