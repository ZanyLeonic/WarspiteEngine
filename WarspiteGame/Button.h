#pragma once
#ifndef __BUTTON_H__
#define __BUTTON_H__

#include <functional>
#include "SDLGameObject.h"

typedef std::function<void()> ButtonCallback;

class Button : public SDLGameObject
{
public:

	Button(const ObjectParams* pParams, 
		ButtonCallback onClick=nullptr, ButtonCallback onEnter=nullptr, ButtonCallback onLeave=nullptr);

	virtual void Draw();
	virtual void OnThink();
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
	virtual void onClick();
	virtual void onEnter();
	virtual void onLeave();

private:
	// Whether the button has been released or not.
	bool m_bReleased = true;
};

#endif