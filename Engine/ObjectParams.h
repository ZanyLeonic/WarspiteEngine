#pragma once
#include <string>
#ifndef __ObjectParams__
#define __ObjectParams__

class ObjectParams
{
public:
	ObjectParams(float x, float y, int width, int height,
		std::string textureID) : m_x(x), m_y(y), m_width(width),
		m_height(height), m_textureID(textureID)
	{}

	float GetX() const { return m_x; }
	float GetY() const { return m_y; }

	int GetWidth() const { return m_width; }
	int GetHeight() const { return m_height; }

	std::string GetTextureID() const { return m_textureID; }

private:
	std::string m_textureID;

	float m_x;
	float m_y;

	int m_width;
	int m_height;
};

#endif /* defined (__ObjectParams__) */