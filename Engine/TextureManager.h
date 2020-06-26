#pragma once
#ifndef __TextureManager__
#define __TextureManager__

#include "Vector2D.h"
#include <SDL2\SDL.h>
#include <string>
#include <map>

class CTextureManager
{
private:
	CTextureManager() {} ;
	static CTextureManager* s_pInstance;

public:
	static CTextureManager* Instance()
	{
		if (s_pInstance == 0)
		{
			s_pInstance = new CTextureManager();
			return s_pInstance;
		}
		return s_pInstance;
	}

	bool Load(std::string fileName, std::string id, SDL_Renderer* pRenderer);

	void CreateCheckboardPattern(CVector2D size, std::string texName, SDL_Renderer* pRenderer);

	// draw
	void Draw(std::string id, int x, int y, int width, int  height, 
		SDL_Renderer* pRenderer, SDL_RendererFlip flip = SDL_FLIP_NONE);

	// drawframe
	void DrawFrame(std::string id, int x, int y, int width, int height,
		int currentRow, int currentFrame, SDL_Renderer* pRenderer, 
		SDL_RendererFlip flip = SDL_FLIP_NONE);

	void DrawTile(std::string id, int margin, int spacing, 
		int x, int y, int width, int height, int currentRow, 
		int currentFrame, SDL_Renderer* pRenderer);

	void Remove(std::string id);

	std::map<std::string, SDL_Texture*> m_textureMap;
};

#endif /* defined(__TextureManager__) */