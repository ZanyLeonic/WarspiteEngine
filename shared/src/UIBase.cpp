#include "UIBase.h"

CUIBase::CUIBase() :
	CWarspiteObject()
{
	m_collidable = false;

	// Initial callbacks
	if (m_OnClick == 0) m_OnClick = std::bind(&CUIBase::onClick, this);
	if (m_OnEnter == 0) m_OnEnter = std::bind(&CUIBase::onEnter, this);
	if (m_OnLeave == 0) m_OnLeave = std::bind(&CUIBase::onLeave, this);
}

void CUIBase::Load(CObjectParams* pParams)
{
	CWarspiteObject::Load(pParams);

	m_onClickID = pParams->GetOnClickID();
	m_onEnterID = pParams->GetOnEnterID();
	m_onLeaveID = pParams->GetOnLeaveID();
}

void CUIBase::Draw()
{
	CWarspiteObject::Draw();
}

bool CUIBase::OnThink()
{
	return true;
}

bool CUIBase::onClick()
{
	// default for the callback
	return true;
}

bool CUIBase::onEnter()
{
	// default for the callback
	return true;
}

bool CUIBase::onLeave()
{
	// default for the callback
	return true;
}

void CUIBase::Destroy()
{
	CWarspiteObject::Destroy();
}

void CUIBase::OnClick(UICallback e)
{
	m_OnClick = e != 0 ? e : std::bind(&CUIBase::onClick, this);
}

void CUIBase::OnEnter(UICallback e)
{
	m_OnEnter = e != 0 ? e : std::bind(&CUIBase::onEnter, this);
}

void CUIBase::OnLeave(UICallback e)
{
	m_OnLeave = e != 0 ? e : std::bind(&CUIBase::onLeave, this);
}