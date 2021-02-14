#pragma once
#ifndef __LOGICAUTO_H__
#define __LOGICAUTO_H__

#include "TileObject.h"

class CLogicAuto : public CTileObject
{
public:
	CLogicAuto();

	void OnPlay() override;
	void Load(const CObjectParams* pParams) override;
	
private:
	std::string m_scriptName = "";
};

REG_OBJ_TO_REF(LogicAuto, CLogicAuto)
#endif