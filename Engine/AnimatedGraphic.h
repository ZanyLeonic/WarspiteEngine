#pragma once
#ifndef __AnimatedGraphic_H__
#define __AnimatedGraphic_H__

#include "WarspiteObject.h"

class AnimatedGraphic : public WarspiteObject
{
public:
	AnimatedGraphic() {};

	virtual void Load(const ObjectParams* pParams);
	virtual bool OnThink();

protected:
	int m_animSpeed = 1;
};

#endif