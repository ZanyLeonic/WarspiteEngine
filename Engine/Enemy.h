#pragma once
#ifndef __Enemy__
#define __Enemy__

#include "WarspiteObject.h"

class CEnemy : public CWarspiteObject
{
public:
	CEnemy();

	bool OnThink();
};
#endif /* defined(__Enemy__) */