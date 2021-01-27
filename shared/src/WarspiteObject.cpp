#ifdef _ENGINE_
#include "TextureManager.h"
#include "Game.h"
#elif _GAME_
#include "ITextureManager.h"
#include "WGame.h"
#include "IGame.h"
#endif

#include "WarspiteObject.h"

CWarspiteObject::CWarspiteObject() 
	: IGameObject(), m_height(0), m_width(0), 
	m_numFrames(1), m_currentFrame(0), m_currentRow(0)
{
#ifdef _GAME_
	pTex = (ITextureManager*)CGame::Instance()->GetPointers()[ESingletonIDs::TEXTUREMANAGER];
	pGame = (IGame*)CGame::Instance()->GetPointers()[ESingletonIDs::GAME];
#endif
}

void CWarspiteObject::Load(const CObjectParams* pParams)
{
	m_objectName = pParams->GetName();
	m_factoryID = pParams->GetFactoryID();
	
	m_position = CVector2D(pParams->GetX(), pParams->GetY());
	m_velocity = CVector2D(0, 0);
	m_acceleration = CVector2D(0, 0);

	m_width = pParams->GetWidth();
	m_height = pParams->GetHeight();
	m_textureID = pParams->GetTextureID();

	m_currentRow = 1;
	m_currentFrame = 0;
	m_numFrames = pParams->GetNumberOfFrames();
}

void CWarspiteObject::OnPlay()
{
}

void CWarspiteObject::Draw()
{
#ifdef _ENGINE_
	if (m_velocity.GetX() > 0)
	{
		CTextureManager::Instance()->DrawFrame(m_textureID, (int)m_position.GetX(),
			(int)m_position.GetY(), m_width, m_height, 
			m_currentRow, m_currentFrame, CBaseGame::Instance()->GetRenderer(), 
			SDL_FLIP_HORIZONTAL);
	}
	else
	{
		CTextureManager::Instance()->DrawFrame(m_textureID, (int)m_position.GetX(),
			(int)m_position.GetY(), m_width, m_height,
			m_currentRow, m_currentFrame, CBaseGame::Instance()->GetRenderer());
	}
#elif _GAME_
	if (m_velocity.GetX() > 0)
	{
		pTex->DrawFrame(m_textureID, (int)m_position.GetX(),
			(int)m_position.GetY(), m_width, m_height,
			m_currentRow, m_currentFrame, pGame->GetRenderer(),
			0.0, nullptr, EWarRendererFlip::FLIP_HORIZONTAL);
	}
	else
	{
		pTex->DrawFrame(m_textureID, (int)m_position.GetX(),
			(int)m_position.GetY(), m_width, m_height,
			m_currentRow, m_currentFrame, pGame->GetRenderer());
	}
#endif
}

bool CWarspiteObject::OnThink()
{
	m_velocity += m_acceleration;  
	m_position += m_velocity;

	return true;
}

void CWarspiteObject::Destroy()
{
}