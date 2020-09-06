#pragma once
#ifndef __LOGICAUTO_H__
#define __LOGICAUTO_H__

#include "MapObject.h"

class CLogicAuto : public CMapObject
{
public:
	CLogicAuto();

	// Right now - it does not need to be drawn.
	// Maybe for a debug mode it might be drawn?
	void Draw() override {}
	void OnPlay() override;
	void Load(const CObjectParams* pParams) override;
	
private:
	std::string m_scriptName = "";
};

REG_OBJ_TO_REF(LogicAuto, CLogicAuto)

#endif