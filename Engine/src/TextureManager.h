#pragma once
#ifndef __TextureManager__
#define __TextureManager__

#include "ITextureManager.h"
#include <SDL.h>
#include "CTexture.h"

class CTextureManager :
	public ITextureManager
{
	static CTextureManager* s_pInstance;
	CTextureManager() {};

public:
	static CTextureManager* Instance()
	{
		if (s_pInstance == nullptr)
		{
			s_pInstance = new CTextureManager();
			return s_pInstance;
		}
		return s_pInstance;
	}

	bool Load(std::string fileName, std::string id, SDL_Renderer* pRenderer, int opacity=255);
	void Add(CTexture* nText, std::string id);

	void CreateCheckboardPattern(CVector2D size, std::string texName, SDL_Renderer* pRenderer);
	void CreateEngineTextures(CVector2D resolution, SDL_Renderer* pRenderer);

	void SaveTextureToDisk(std::string fileName, CTexture* texture, SDL_Renderer* pRenderer);

	// draw
	void Draw(std::string id, int x, int y, int width, int height,
		SDL_Renderer* pRenderer, double angle = 0.0f, 
		CVector2D* center = nullptr, EWarRendererFlip flip = EWarRendererFlip::FLIP_NONE);

	// draw
	void Draw(CTexture* texture, int x, int y, SDL_Renderer* pRenderer, 
		double angle = 0.0f, CVector2D* center = nullptr,
		EWarRendererFlip flip = EWarRendererFlip::FLIP_NONE);

	// drawframe
	void DrawFrame(std::string id, int x, int y, int width, int height,
		int currentRow, int currentFrame, SDL_Renderer* pRenderer, 
		double angle = 0.0f, CVector2D* center = nullptr,
		EWarRendererFlip flip = EWarRendererFlip::FLIP_NONE);

	void DrawFrame(CTexture* texture, int x, int y,
		int currentRow, int currentFrame, SDL_Renderer* pRenderer,
		double angle = 0.0f, CVector2D* center = nullptr,
		EWarRendererFlip flip = EWarRendererFlip::FLIP_NONE);

	void DrawTile(std::string id, int margin, int spacing, 
		int x, int y, int width, int height, int currentRow, 
		int currentFrame, SDL_Renderer* pRenderer);

	void Remove(std::string id);

	std::map<std::string, CTexture*> m_textureMap;
};

#endif /* defined(__TextureManager__) */