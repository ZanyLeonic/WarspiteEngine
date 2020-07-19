#pragma once
#include <string>
#ifndef __ObjectParams__
#define __ObjectParams__

class CObjectParams
{
public:
	// Map Logic overload
	CObjectParams(float x, float y, const char* script)
		: m_x(x), m_y(y), m_script(script) {}
	
	CObjectParams(float x, float y)
		: m_x(x), m_y(y) {}

	CObjectParams(float x, float y, int width, int height,
		std::string textureID, int animSpeed = 1, int numFrames = 1,
		int onClick = 0, int onEnter = 0, int onLeave = 0, std::string script = "",
		std::string objN = "", std::string factID = "")
		: m_x(x), m_y(y), m_width(width), m_height(height),
		m_textureID(textureID), m_animSpeed(animSpeed), m_numFrames(numFrames),
		m_OnClick(onClick), m_OnEnter(onEnter), m_OnLeave(onLeave), m_script(script),
		m_objName(objN), m_factoryID(factID) {}

	// Setters
	void SetName(std::string n) { m_objName = n; }
	void SetFactoryID(std::string n) { m_factoryID = n; }
	
	void SetWidth(int w) { m_width = w; }
	void SetHeight(int h) { m_width = h; }

	void SetAnimSpeed(int s) { m_animSpeed = s; }
	void SetNumFrames(int n) { m_numFrames = n; }
	
	void SetOnClick(int c) { m_OnClick = c; }
	void SetOnEnter(int c) { m_OnEnter = c; }
	void SetOnLeave(int c) { m_OnLeave = c; }

	void SetTextureID(std::string id) { m_textureID = id; }
	void SetScript(std::string s) { m_script = s; }
	
	// Getters
	float GetX() const { return m_x; }
	float GetY() const { return m_y; }

	std::string GetName() const { return m_objName; }
	std::string GetFactoryID() const { return m_factoryID; }
	
	int GetWidth() const { return m_width; }
	int GetHeight() const { return m_height; }

	int GetAnimSpeed() const { return m_animSpeed; }
	int GetNumberOfFrames() const { return m_numFrames; }

	int GetOnClickID() const { return m_OnClick; }
	int GetOnEnterID() const { return m_OnEnter; }
	int GetOnLeaveID() const { return m_OnLeave; }

	std::string GetTextureID() const { return m_textureID; }
	std::string GetScript() const { return m_script; }

private:
	// Metadata + Map Logic
	std::string m_script = "";
	std::string m_textureID = "";
	std::string m_objName = "";
	std::string m_factoryID = "";
	
	float m_x = 0;
	float m_y = 0;

	int m_width = 0;
	int m_height = 0;

	int m_animSpeed = 1;
	int m_numFrames = 1;

	int m_OnClick = 0;
	int m_OnEnter = 0;
	int m_OnLeave = 0;
	

};

#endif /* defined (__ObjectParams__) */