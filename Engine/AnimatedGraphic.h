#pragma once
#ifndef __AnimatedGraphic_H__
#define __AnimatedGraphic_H__

#include "SDLGameObject.h"

class AnimatedGraphic : public SDLGameObject
{
public:
	AnimatedGraphic() {};

	virtual void Load(const ObjectParams* pParams);
	virtual bool OnThink();

protected:
	int m_animSpeed = 1;
};

#endif