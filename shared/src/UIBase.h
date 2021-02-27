#pragma once
#ifndef __UIBASE_H__
#define __UIBASE_H__

#include <functional>
#include "WarspiteObject.h"

class CUIBase : public CWarspiteObject
{
protected:
	typedef std::function<bool()> UICallback;
public:
	CUIBase();

	virtual void Load(CObjectParams* pParams);

	virtual void Draw();
	virtual bool OnThink();
	virtual void Destroy();

	virtual void OnClick(UICallback e);
	virtual void OnEnter(UICallback e);
	virtual void OnLeave(UICallback e);

	int GetOnClickID() const { return m_onClickID; }
	int GetOnEnterID() const { return m_onEnterID; }
	int GetOnLeaveID() const { return m_onLeaveID; }

protected:
	// Holds the callback to trigger on events
	UICallback m_OnClick = 0;
	UICallback m_OnEnter = 0;
	UICallback m_OnLeave = 0;

	// Initial values for callbacks
	virtual bool onClick();
	virtual bool onEnter();
	virtual bool onLeave();

private:
	// Whether the button has been released or not.
	int m_onClickID = 0;
	int m_onEnterID = 0;
	int m_onLeaveID = 0;
};
#endif