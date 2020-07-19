#include "LogicAuto.h"
#include "ScriptManager.h"

CLogicAuto::CLogicAuto()
{}

void CLogicAuto::OnPlay()
{
	CScriptManager::Instance()->RunFromRef(m_scriptName);
}

void CLogicAuto::Load(const CObjectParams* pParams)
{
	m_position = CVector2D(pParams->GetX(), pParams->GetY());
	m_scriptName = pParams->GetScript();
}
