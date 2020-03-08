#pragma once
#ifndef __Enemy__
#define __Enemy__

#include "WarspiteObject.h"

class Enemy : public WarspiteObject
{
public:
	Enemy();

	bool OnThink();

};

#endif /* defined(__Enemy__) */