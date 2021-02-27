#include "LogicAuto.h"
#include "ScriptManager.h"

CLogicAuto::CLogicAuto()
{
	m_collidable = false;
}

void CLogicAuto::OnPlay()
{
	CTileObject::OnPlay();

	if (m_scriptName.empty())
		return;
	
	CScriptManager::Instance()->RunFromRef(m_scriptName);
}

void CLogicAuto::Load(CObjectParams* pParams)
{
	CTileObject::Load(pParams);

	m_position = CVector2D(pParams->GetX(), pParams->GetY());
	m_scriptName = pParams->GetProperty<std::string>("runScript");
}
