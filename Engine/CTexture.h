#pragma once
#ifndef __CTEXTURE_H__
#define __CTEXTURE_H__
#include <SDL.h>
#include <string>

class CVector2D;

class CTexture
{
public:
	CTexture(SDL_Texture* nTex, double angle = 0.f, SDL_Point* center = nullptr);

	~CTexture();

	bool Load(std::string path);
	void Free();

	//Set color modulation
	void SetColor(Uint8 red, Uint8 green, Uint8 blue);

	//Set blending
	void SetBlendMode(SDL_BlendMode blending);

	//Set alpha modulation
	void SetAlpha(Uint8 alpha);

	void Draw(CVector2D pos);

	SDL_Texture* GetTexture() const { return m_texture; }

	//Gets image dimensions
	int GetWidth() const { return m_width; }
	int GetHeight() const { return m_height; }

	double GetAngle() const { return m_angle; }
	SDL_Point* GetCenter() const { return m_center; }
private:
	SDL_Texture* m_texture;
	SDL_Point* m_center;

	int m_width = 0;
	int m_height = 0;

	double m_angle = 0.f;
};
#endif // #ifndef __CTEXTURE_H__