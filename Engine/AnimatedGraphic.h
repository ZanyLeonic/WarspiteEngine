#pragma once
#ifndef __AnimatedGraphic_H__
#define __AnimatedGraphic_H__

#include "WarspiteObject.h"

class CAnimatedGraphic : public CWarspiteObject
{
public:
	CAnimatedGraphic() {};

	virtual void Load(const CObjectParams* pParams);
	virtual bool OnThink();

protected:
	int m_animSpeed = 1;
};

#endif