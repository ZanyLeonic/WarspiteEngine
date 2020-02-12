#pragma once
#ifndef __Enemy__
#define __Enemy__

#include "SDLGameObject.h"

class Enemy : public SDLGameObject
{
public:
	Enemy();

	bool OnThink();

};

#endif /* defined(__Enemy__) */