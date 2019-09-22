#pragma once
#ifndef __Player__
#define __Player__

#include "SDLGameObject.h"
#include <string.h>

class Player : public SDLGameObject
{
public:
	Player(const ObjectParams* pParams);

	void OnThink();
};

#endif /* defined(__Player__) */