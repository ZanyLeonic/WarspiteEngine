#pragma once
#ifndef __Player__
#define __Player__

#include "GameObject.h"
#include <string.h>

class Player : public GameObject
{
public:
	Player();

	void Load(int x, int y, int width, int height, std::string textureID);
	void Draw(SDL_Renderer* pRenderer);
	void OnThink(float pDeltaTime);
	void Destroy();
};

#endif /* defined(__Player__) */