#pragma once
#include <string>
#ifndef __ObjectParams__
#define __ObjectParams__



class ObjectParams
{
public:
	ObjectParams(float x, float y, int width, int height,
		std::string textureID, int animSpeed = 1, int numFrames = 1,
		int onClick = 0, int onEnter = 0, int onLeave = 0)
		: m_x(x), m_y(y), m_width(width), m_height(height),
		m_textureID(textureID), m_animSpeed(animSpeed), m_numFrames(numFrames),
		m_OnClick(onClick), m_OnEnter(onEnter), m_OnLeave(onLeave)
	{}

	float GetX() const { return m_x; }
	float GetY() const { return m_y; }

	int GetWidth() const { return m_width; }
	int GetHeight() const { return m_height; }

	int GetAnimSpeed() const { return m_animSpeed; }
	int GetNumberOfFrames() const { return m_numFrames; }

	int GetOnClickID() const { return m_OnClick; }
	int GetOnEnterID() const { return m_OnEnter; }
	int GetOnLeaveID() const { return m_OnLeave; }

	std::string GetTextureID() const { return m_textureID; }

private:
	std::string m_textureID;

	float m_x;
	float m_y;

	int m_width;
	int m_height;

	int m_animSpeed;
	int m_numFrames;

	int m_OnClick;
	int m_OnEnter;
	int m_OnLeave;
};

#endif /* defined (__ObjectParams__) */