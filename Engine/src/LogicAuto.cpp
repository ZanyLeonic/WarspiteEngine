#include "LogicAuto.h"
#include "ScriptManager.h"

CLogicAuto::CLogicAuto()
{
	m_collidable = false;
}

void CLogicAuto::OnPlay()
{
	if (m_scriptName.empty())
		return;
	
	CScriptManager::Instance()->RunFromRef(m_scriptName);
}

void CLogicAuto::Load(const CObjectParams* pParams)
{
	m_position = CVector2D(pParams->GetX(), pParams->GetY());
	m_scriptName = pParams->GetScript();
}
