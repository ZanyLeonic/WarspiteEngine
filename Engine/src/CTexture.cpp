#include "CTexture.h"
#include <SDL_image.h>
#include "Game.h"
#include "TextureManager.h"
#include "Vector2D.h"

CTexture::CTexture(SDL_Texture* nTex, double angle, SDL_Point* center)
{
	m_texture = nTex;
	m_angle = angle;
	m_center = center;
	if (m_texture != nullptr)
		SDL_QueryTexture(m_texture, NULL, NULL, &m_width, &m_height);
}

CTexture::~CTexture()
{
	Free();
}

bool CTexture::Load(std::string path)
{
	Free();

	SDL_Surface* pTempSurface = IMG_Load(path.c_str());

	if (pTempSurface == 0)
	{
		return false;
	}

	SDL_Texture* pTexture =
		SDL_CreateTextureFromSurface(CBaseGame::Instance()->GetRenderer(), pTempSurface);

	SDL_FreeSurface(pTempSurface);
	SDL_QueryTexture(pTexture, NULL, NULL, &m_width, &m_height);

	// we have loaded the texture ok, add it to the map!
	if (pTexture != 0)
	{
		m_texture = pTexture;
		return true;
	}

	// if we got here, something is wrong.
	return false;
}

void CTexture::Free()
{
	if (m_texture != nullptr)
	{
		SDL_DestroyTexture(m_texture);
		m_texture = nullptr;
		m_width = 0;
		m_height = 0;
	}
}

void CTexture::SetColour(Uint8 red, Uint8 green, Uint8 blue)
{
	if (m_texture == nullptr) return;
	SDL_SetTextureColorMod(m_texture, red, green, blue);
}

void CTexture::SetBlendMode(SDL_BlendMode blending)
{
	if (m_texture == nullptr) return;
	SDL_SetTextureBlendMode(m_texture, blending);
}

void CTexture::SetAlpha(Uint8 alpha)
{
	if (m_texture == nullptr) return;
	SDL_SetTextureAlphaMod(m_texture, alpha);
}

void CTexture::Draw(CVector2D pos)
{
	if (!m_texture) return;
	CTextureManager::Instance()->Draw(this, (int)pos.GetX(), (int)pos.GetY(), CBaseGame::Instance()->GetRenderer(), m_angle);
}

void CTexture::SetAngle(double pAngle)
{
	m_angle = pAngle;
}

void CTexture::SetCenter(SDL_Point* pNewPoint)
{
	m_center = pNewPoint;
}
