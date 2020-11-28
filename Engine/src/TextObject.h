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

	void Load(const CObjectParams* pParams) override;
};
REG_OBJ_TO_REF(TextObject, CTextObject);
#endif // #ifndef __FONTOBJECT_H__