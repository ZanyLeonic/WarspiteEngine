#pragma once
#ifndef __Enemy__
#define __Enemy__

#include "GameObject.h"

class Enemy : public GameObject
{
public:
	Enemy();

	void OnThink(float pDeltaTime);

};

#endif /* defined(__Enemy__) */