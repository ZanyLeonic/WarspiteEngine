#pragma once
#ifndef __GameObject__
#define __GameObject__
#include <SDL.h>
#include <iostream>

class GameObject
{
public:
	GameObject();

	virtual void Load(int x, int y, int width, int height, std::string textureID);
	virtual void Draw(SDL_Renderer* pRenderer);
	virtual void OnThink(float pDeltaTime);
	virtual void Destroy();

protected:
	
	std::string m_textureID;

	int m_currentFrame;
	int m_currentRow;

	int m_x;
	int m_y;

	int m_width;
	int m_height;
};

#endif /* defined(__GameObject__) */
