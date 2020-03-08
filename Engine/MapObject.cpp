#include "Camera.h"
#include "MapObject.h"
#include "TextureManager.h"
#include "Game.h"

MapObject::MapObject()
	: WarspiteObject()
{

}

void MapObject::Draw()
{
	Vector2D cPos = Camera::Instance()->GetPosition();
	if (m_velocity.GetX() > 0)
	{
		TextureManager::Instance()->DrawFrame(m_textureID, (int)m_position.GetX(),
			(int)m_position.GetY(), m_width, m_height, cPos.GetX(), cPos.GetY(),
			m_currentRow, m_currentFrame, Game::Instance()->GetRenderer(),
			SDL_FLIP_HORIZONTAL);
	}
	else
	{
		TextureManager::Instance()->DrawFrame(m_textureID, (int)m_position.GetX(),
			(int)m_position.GetY(), m_width, m_height, cPos.GetX(), cPos.GetY(),
			m_currentRow, m_currentFrame, Game::Instance()->GetRenderer());
	}
}
