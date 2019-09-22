#include "GameObject.h"
#include "TextureManager.h"
#include <iostream>

GameObject::GameObject()
{
	m_x = 0;
	m_y = 0;
}

void GameObject::Load(int x, int y, int width, int height, std::string textureID)
{
	m_x = x;
	m_y = y;
	m_width = width;
	m_height = height;
	m_textureID = textureID;

	m_currentRow = 1;
	m_currentFrame = 1;
}

void GameObject::Draw(SDL_Renderer* pRenderer)
{
	TextureManager::Instance()->drawFrame(m_textureID, m_x, m_y,
		m_width, m_height, m_currentRow, m_currentFrame, pRenderer);
}

void GameObject::OnThink(float pDeltaTime)
{
	std::cout << "GameObject::OnThink()";
	m_currentFrame = int(((SDL_GetTicks() / 100) % 8));
	m_x += 1;
}

void GameObject::Destroy()
{
	std::cout << "GameObject::Destroy()";
}
