#pragma once
#ifndef __ITextureManager__
#define __ITextureManager__

#include <string>
#include <map>
#include "EngineTypes.h"

class CTexture;
class CVector2D;
struct SDL_Renderer;

class ITextureManager
{
public:

	virtual bool Load(std::string fileName, std::string id, SDL_Renderer* pRenderer) = 0;
	virtual void Add(CTexture* nText, std::string id) = 0;

	virtual void CreateCheckboardPattern(CVector2D size, std::string texName, SDL_Renderer* pRenderer) = 0;

	// draw
	virtual void Draw(std::string id, int x, int y, int width, int height,
		SDL_Renderer* pRenderer, double angle = 0.0f, 
		CVector2D* center = nullptr, EWarRendererFlip flip = EWarRendererFlip::FLIP_NONE) = 0;

	// draw
	virtual void Draw(CTexture* texture, int x, int y, SDL_Renderer* pRenderer, 
		double angle = 0.0f, CVector2D* center = nullptr,
		EWarRendererFlip flip = EWarRendererFlip::FLIP_NONE) = 0;

	// drawframe
	virtual void DrawFrame(std::string id, int x, int y, int width, int height,
		int currentRow, int currentFrame, SDL_Renderer* pRenderer, 
		double angle = 0.0f, CVector2D* center = nullptr,
		EWarRendererFlip flip = EWarRendererFlip::FLIP_NONE) = 0;

	virtual void DrawFrame(CTexture* texture, int x, int y,
		int currentRow, int currentFrame, SDL_Renderer* pRenderer,
		double angle = 0.0f, CVector2D* center = nullptr,
		EWarRendererFlip flip = EWarRendererFlip::FLIP_NONE) = 0;

	virtual void DrawTile(std::string id, int margin, int spacing, 
		int x, int y, int width, int height, int currentRow, 
		int currentFrame, SDL_Renderer* pRenderer) = 0;

	virtual void Remove(std::string id) = 0;
};

#endif /* defined(__TextureManager__) */