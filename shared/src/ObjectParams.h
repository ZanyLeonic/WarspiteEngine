#pragma once
#ifndef __ObjectParams__
#define __ObjectParams__
#include <string>
#include <map>

struct STileset;

class CObjectParams
{
public:
	// Map Logic overload
	CObjectParams(float x, float y, const char* script)
		: m_x(x), m_y(y), m_script(script) {}

	CObjectParams(float x, float y)
		: m_x(x), m_y(y) {}

	[[deprecated("Use the setters in the class instead.")]]
	CObjectParams(float x, float y, int width, int height,
		std::string textureID, int animSpeed = 1, int numFrames = 1,
		int onClick = 0, int onEnter = 0, int onLeave = 0, std::string script = "",
		std::string objN = "", std::string factID = "")
		: m_x(x), m_y(y), m_width(width), m_height(height),
		m_textureID(textureID), m_animSpeed(animSpeed), m_numFrames(numFrames),
		m_OnClick(onClick), m_OnEnter(onEnter), m_OnLeave(onLeave), m_script(script),
		m_objName(objN), m_factoryID(factID) {}

	// Setters
	void SetX(float x) { m_x = x; }
	void SetY(float y) { m_y = y; }

	void SetName(std::string n) { m_objName = n; }
	void SetFactoryID(std::string n) { m_factoryID = n; }

	void SetTileID(int gid) { m_gid = gid; }
	void SetTileset(STileset* tileset) { m_tileset = tileset; }

	void SetWidth(int w) { m_width = w; }
	void SetHeight(int h) { m_height = h; }

	void SetAnimSpeed(int s) { m_animSpeed = s; }
	void SetNumFrames(int n) { m_numFrames = n; }

	void SetOnClick(int c) { m_OnClick = c; }
	void SetOnEnter(int c) { m_OnEnter = c; }
	void SetOnLeave(int c) { m_OnLeave = c; }

	void SetSoundPath(std::string path) { m_soundPath = path; }

	void SetTextureID(std::string id) { m_textureID = id; }
	void SetScript(std::string s) { m_script = s; }

	void SetDoorTargetID(std::string s) { m_doorTargetID = s; }
	void SetDoorWorldTexture(std::string s) { m_doorWorldTexture = s; }

	void SetStartOverlap(std::string s) { m_startOverlapFunc = s; }
	void SetEndOverlap(std::string s) { m_endOverlapFunc = s; }

	// Getters
	float GetX() const { return m_x; }
	float GetY() const { return m_y; }

	std::string GetName() const { return m_objName; }
	std::string GetFactoryID() const { return m_factoryID; }

	int GetTileID() const { return m_gid; }
	STileset* GetTileset() const { return m_tileset; }
	
	int GetWidth() const { return m_width; }
	int GetHeight() const { return m_height; }

	int GetAnimSpeed() const { return m_animSpeed; }
	int GetNumberOfFrames() const { return m_numFrames; }

	int GetOnClickID() const { return m_OnClick; }
	int GetOnEnterID() const { return m_OnEnter; }
	int GetOnLeaveID() const { return m_OnLeave; }

	template <class T>
	T GetCustomProperty(std::string id) { return (T)m_mCustomProperty[id]; }

	std::string GetSoundPath() const { return m_soundPath; }

	std::string GetTextureID() const { return m_textureID; }
	std::string GetScript() const { return m_script; }

	std::string GetDoorTargetID() const { return m_doorTargetID; }
	std::string GetDoorWorldTexture() const { return m_doorWorldTexture; }

	std::string GetStartOverlap() const { return m_startOverlapFunc; }
	std::string GetEndOverlap() const { return m_endOverlapFunc; }

	// Turn this into a dictionary
private:
	// Metadata + Map Logic
	std::string m_script = "";
	std::string m_textureID = "";
	std::string m_objName = "";
	std::string m_factoryID = "";
	std::string m_soundPath = "";

	std::string m_doorTargetID = "";
	std::string m_doorWorldTexture = "";
	std::string m_startOverlapFunc = "";
	std::string m_endOverlapFunc = "";

	std::map<std::string, std::string> m_mCustomProperty;

	int m_gid = -1;
	
	float m_x = 0;
	float m_y = 0;

	int m_width = 0;
	int m_height = 0;

	int m_animSpeed = 1;
	int m_numFrames = 1;

	int m_OnClick = 0;
	int m_OnEnter = 0;
	int m_OnLeave = 0;

	STileset* m_tileset = 0;
};
#endif /* defined (__ObjectParams__) */