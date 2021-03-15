#pragma once
#ifndef __FONTOBJECT_H__
#define __FONTOBJECT_H__

#include "MapObject.h"

class CTextObject :
	public CMapObject
{
public:
	CTextObject();
	~CTextObject();

	void Load(CObjectParams* pParams) override;
private:
	std::string m_sText;
	std::string m_sFont;
	std::string m_sVariant;

	int m_iFontSize = 32;

	std::string m_sFontString;
};
REG_OBJ_TO_REF(TextObject, CTextObject);
#endif // #ifndef __FONTOBJECT_H__