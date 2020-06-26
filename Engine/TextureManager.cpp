#include "TextureManager.h"
#include <SDL2/SDL_image.h>

CTextureManager* CTextureManager::s_pInstance = 0;

bool CTextureManager::Load(std::string fileName, std::string id, 
	SDL_Renderer* pRenderer)
{
	SDL_Surface* pTempSurface = IMG_Load(fileName.c_str());

	if (pTempSurface == 0)
	{
		return false;
	}

	SDL_Texture* pTexture =
		SDL_CreateTextureFromSurface(pRenderer, pTempSurface);

	SDL_FreeSurface(pTempSurface);

	// we have loaded the texture ok, add it to the map!
	if (pTexture != 0)
	{
		m_textureMap[id] = pTexture;
		return true;
	}

	// if we got here, something is wrong.
	return false;
}

void CTextureManager::CreateCheckboardPattern(CVector2D size, std::string texName, SDL_Renderer* pRenderer)
{
	const int cSize = 32;
	
	const SDL_Colour c1 = { 0xFF, 0x00, 0xFF };
	const SDL_Colour c2 = { 0x00, 0x00, 0x00 };

	int tRows = int(size.GetX() / 32);
	int tCols = int(size.GetY() / 32);

	SDL_Texture* pTexture = SDL_CreateTexture(pRenderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, size.GetX(), size.GetY());
	SDL_SetTextureBlendMode(pTexture, SDL_BLENDMODE_NONE);

	SDL_SetRenderTarget(pRenderer, pTexture);
	
	SDL_SetRenderDrawColor(pRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(pRenderer);

	SDL_Colour d1;
	SDL_Colour d2;

	for (int i = 0; i < tRows; i++)
	{
		if (int(i % 2) == 0)
		{
			d1 = c1;
			d2 = c2;
		}
		else
		{
			d1 = c2;
			d2 = c1;
		}

		for (int j = 0; j < tCols; j++)
		{
			if (int(j % 2) == 0)
			{
				SDL_SetRenderDrawColor(pRenderer, d1.r, d1.g, d1.b, 0xFF);
			}
			else
			{
				SDL_SetRenderDrawColor(pRenderer, d2.r, d2.g, d2.b, 0xFF);
			}

			SDL_Rect r = {i * cSize, j * cSize, cSize, cSize};

			SDL_RenderFillRect(pRenderer, &r);
		}
	}

	m_textureMap[texName] = pTexture;

	SDL_SetRenderTarget(pRenderer, NULL);
	SDL_RenderPresent(pRenderer);
}

void CTextureManager::Draw(std::string id, int x, int y, 
	int width, int height, SDL_Renderer* pRenderer, 
	SDL_RendererFlip flip)
{
	// Don't try to draw if there is no id specified.
	if (id == "") return;

	SDL_Rect srcRect;
	SDL_Rect destRect;

	srcRect.x = 0;
	srcRect.y = 0;
	srcRect.w = destRect.w = width;
	srcRect.h = destRect.h = height;
	destRect.x = x;
	destRect.y = y;

	SDL_RenderCopyEx(pRenderer, m_textureMap[id], &srcRect,
		&destRect, 0, 0, flip);
}

void CTextureManager::DrawFrame(std::string id, int x, int y, 
	int width, int height, int currentRow, int currentFrame, 
	SDL_Renderer* pRenderer, SDL_RendererFlip flip)
{
	if (id == "") return;

	SDL_Rect srcRect;
	SDL_Rect destRect;

	srcRect.x = width * currentFrame;
	srcRect.y = height * (currentRow - 1);
	srcRect.w = destRect.w = width;
	srcRect.h = destRect.h = height;
	destRect.x = x;
	destRect.y = y;

	SDL_RenderCopyEx(pRenderer, m_textureMap[id], 
		&srcRect, &destRect, 0, 0, flip);
}

void CTextureManager::DrawTile(std::string id, int margin, int spacing, 
	int x, int y, int width, int height, int currentRow, 
	int currentFrame, SDL_Renderer* pRenderer)
{
	SDL_Rect srcRect;
	SDL_Rect destRect;

	// Get the tile from the tileset we want...
	srcRect.x = margin + (spacing + width) * currentFrame;
	srcRect.y = margin + (spacing + height) * currentRow;
	srcRect.w = destRect.w = width;
	srcRect.h = destRect.h = height;
	
	destRect.x = x;
	destRect.y = y;

	// render it on the renderer
	SDL_RenderCopyEx(pRenderer, m_textureMap[id], 
		&srcRect, &destRect, 0, 0, SDL_FLIP_NONE);
}

void CTextureManager::Remove(std::string id)
{
	SDL_DestroyTexture(m_textureMap[id]);
	m_textureMap.erase(id);
}