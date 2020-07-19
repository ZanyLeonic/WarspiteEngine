#include "LogicAuto.h"
#include "ScriptManager.h"

CLogicAuto::CLogicAuto()
{}

void CLogicAuto::OnPlay()
{
	if (m_scriptName == "")
		return;
	
	CScriptManager::Instance()->RunFromRef(m_scriptName);
}

void CLogicAuto::Load(const CObjectParams* pParams)
{
	m_position = CVector2D(pParams->GetX(), pParams->GetY());
	m_scriptName = pParams->GetScript();
}
