#pragma once
#include <string>
#ifndef __ObjectParams__
#define __ObjectParams__

class ObjectParams
{
public:
	ObjectParams(int x, int y, int width, int height,
		std::string textureID) : m_x(x), m_y(y), m_width(width),
		m_height(height), m_textureID(textureID)
	{}

	int GetX() const { return m_x; }
	int GetY() const { return m_y; }

	int GetWidth() const { return m_width; }
	int GetHeight() const { return m_height; }

	std::string GetTextureID() const { return m_textureID; }

private:
	std::string m_textureID;

	int m_x;
	int m_y;

	int m_width;
	int m_height;
};

#endif /* defined (__ObjectParams__) */