#pragma once
#ifndef __BUTTON_H__
#define __BUTTON_H__

#include <functional>
#include "SDLGameObject.h"
#include "BaseCreator.h"

class Button : public SDLGameObject
{
protected:
	typedef std::function<bool()> ButtonCallback;
public:

	Button();

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

	int GetOnClickID() const { return m_onClickID; }
	int GetOnEnterID() const { return m_onEnterID; }
	int GetOnLeaveID() const { return m_onLeaveID; }

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

	int m_onClickID = 0;
	int m_onEnterID = 0;
	int m_onLeaveID = 0;
};

class ButtonCreator : public BaseCreator
{
	GameObject* CreateGameObject() const
	{
		return new Button();
	}
};

#endif