#include "GameObject.h"
#include "TextureManager.h"
#include <iostream>

BaseObject::BaseObject()
{
	m_x = 0;
	m_y = 0;
}

void BaseObject::Load(int x, int y, int width, int height, std::string textureID)
{
	m_x = x;
	m_y = y;
	m_width = width;
	m_height = height;
	m_textureID = textureID;

	m_currentRow = 1;
	m_currentFrame = 1;
}

void BaseObject::Draw(SDL_Renderer* pRenderer)
{
	TextureManager::Instance()->drawFrame(m_textureID, m_x, m_y,
		m_width, m_height, m_currentRow, m_currentFrame, pRenderer);
}

void BaseObject::OnThink(float pDeltaTime)
{
	std::cout << "BaseObject::OnThink()";
	m_currentFrame = int(((SDL_GetTicks() / 100) % 8));
	m_x += 1;
}

void BaseObject::Destroy()
{
	std::cout << "BaseObject::Destroy()";
}
