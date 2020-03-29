#include "TextureManager.h"
#include <SDL_image.h>

TextureManager* TextureManager::s_pInstance = 0;

bool TextureManager::Load(std::string fileName, std::string id, 
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

void TextureManager::Draw(std::string id, int x, int y, 
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

void TextureManager::DrawFrame(std::string id, int x, int y, 
	int width, int height, int camX, int camY, int currentRow, int currentFrame, 
	SDL_Renderer* pRenderer, SDL_RendererFlip flip)
{
	if (id == "") return;

	SDL_Rect srcRect;
	SDL_Rect destRect;

	srcRect.x = width * currentFrame;
	srcRect.y = height * (currentRow - 1);
	srcRect.w = destRect.w = width;
	srcRect.h = destRect.h = height;
	destRect.x = x - camX;
	destRect.y = y - camY;

	SDL_RenderCopyEx(pRenderer, m_textureMap[id], 
		&srcRect, &destRect, 0, 0, flip);
}

void TextureManager::DrawTile(std::string id, int margin, int spacing, 
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

void TextureManager::Remove(std::string id)
{
	SDL_DestroyTexture(m_textureMap[id]);
	m_textureMap.erase(id);
}