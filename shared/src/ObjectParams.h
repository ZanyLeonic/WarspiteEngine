#pragma once
#ifndef __ObjectParams__
#define __ObjectParams__
#include "EngineTypes.h"
#include <string>
#include <map>
#include <any>

struct STileset;

class CObjectParams
{
public:
	// Map Logic overload
	CObjectParams(float x, float y)
		: m_x(x), m_y(y) {}

	// Setters
	void SetX(float x) { m_x = x; }
	void SetY(float y) { m_y = y; }

	void SetName(std::string n) { m_objName = n; }
	void SetFactoryID(std::string n) { m_factoryID = n; }

	void SetTileID(int gid) { m_gid = gid; }
	void SetTileset(STileset* tileset) { m_tileset = tileset; }

	void SetProperty(MapProperties id, std::any data) { m_mProperties[id] = data; }

	// Getters
	float GetX() const { return m_x; }
	float GetY() const { return m_y; }

	std::string GetName() const { return m_objName; }
	std::string GetFactoryID() const { return m_factoryID; }

	int GetTileID() const { return m_gid; }
	STileset* GetTileset() const { return m_tileset; }

	template<typename T>
	T GetProperty(MapProperties id) {
		try
		{
			return std::any_cast<T>(m_mProperties[id]);
		}
		catch (const std::bad_any_cast& e)
		{
			m_eLastError = e;
			return T();
		}
	}

	std::exception GetLastError() const { return m_eLastError; }

private:
	// Metadata + Map Logic
	std::string m_objName = "";
	std::string m_factoryID = "";

	std::map<MapProperties, std::any> m_mProperties
	{
		{MapProperties::PROP_SCRIPT,		   ""},
		{MapProperties::PROP_TEXTUREID,		   ""},
		{MapProperties::PROP_TEXWIDTH,		   32},
		{MapProperties::PROP_TEXHEIGHT,		   32},
		{MapProperties::PROP_NUMFRAMES,		    1},
		{MapProperties::PROP_ANIMSPEED,		    1},
		{MapProperties::PROP_ONCLICKCALL,      ""},
		{MapProperties::PROP_ONENTERCALL,      ""},
		{MapProperties::PROP_ONLEAVECALL,      ""},
		{MapProperties::PROP_SOUNDPATH,		   ""},
		{MapProperties::PROP_DOORTARGET,	   ""},
		{MapProperties::PROP_DOORWORLDTEXTURE, ""},
		{MapProperties::PROP_STARTOVERLAP,     ""},
		{MapProperties::PROP_ENDOVERLAP,	   ""}
	};

	int m_gid = -1;
	
	float m_x = 0;
	float m_y = 0;

	STileset* m_tileset = 0;
	std::exception m_eLastError = {};
};
#endif /* defined (__ObjectParams__) */