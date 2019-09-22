#pragma once
#ifndef __Player__
#define __Player__

#include "GameObject.h"
#include <string.h>

class Player : public GameObject
{
public:
	Player();

	void OnThink(float pDeltaTime);
};

#endif /* defined(__Player__) */