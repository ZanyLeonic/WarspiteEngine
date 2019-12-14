#pragma once
#ifndef __BUTTON_H__
#define __BUTTON_H__

#include "SDLGameObject.h"

class Button : public SDLGameObject
{
public:

	Button(const ObjectParams* pParams, void (*clickCallback)());

	virtual void Draw();
	virtual void OnThink();
	virtual void Destroy();

protected:
	enum button_state
	{
		NO_HOVER = 0,
		HOVER = 1,
		PRESSED = 2
	};

	// Holds the callback to trigger on events
	void (*m_OnClick)();

	bool m_bReleased;
};

#endif