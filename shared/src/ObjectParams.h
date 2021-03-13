#pragma once
#ifndef __ObjectParams__
#define __ObjectParams__
#include "EngineTypes.h"
#include <string>
#include <map>
#include <any>
#include <spdlog/spdlog.h>

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

	void SetProperty(std::string id, std::any data) { m_mProperties[id] = data; }

	// Getters
	float GetX() const { return m_x; }
	float GetY() const { return m_y; }

	std::string GetName() const { return m_objName; }
	std::string GetFactoryID() const { return m_factoryID; }

	int GetTileID() const { return m_gid; }
	STileset* GetTileset() const { return m_tileset; }

	template<typename T>
	T GetProperty(std::string id) {
		try
		{
			return std::any_cast<T>(m_mProperties[id]);
		}
		catch (const std::bad_any_cast& e)
		{
			spdlog::error("Error casting property \"{}\" to \"{}\"!", id, typeid(T).name());
			spdlog::error("Exception details:");
			spdlog::error(e.what());
			m_eLastError = e;
			return T();
		}
	}

	bool PropertyExists(std::string id)
	{
		// Check if a entry exists
		return m_mProperties.count(id) > 0;
	};

	std::exception GetLastError() const { return m_eLastError; }

private:
	// Metadata + Map Logic
	std::string m_objName = "";
	std::string m_factoryID = "";

	std::map <std::string, std::any> m_mProperties
	{
		{"runScript",			std::string("")},
		{"textureID",			std::string("")},
		{"textureWidth",					 32},
		{"textureHeight",					 32},
		{"numFrames",						  1},
		{"animSpeed",						  1},
		{"onClickCallback",					  0},
		{"onEnterCallback",					  0},
		{"onLeaveCallback",		              0},
		{"soundPath",			std::string("")},
		{"targetDoorID",		std::string("")},
		{"worldTexture",		std::string("")},
		{"startOverlapFunc",	std::string("")},
		{"endOverlapFunc",		std::string("")}
	};

	int m_gid = -1;
	
	float m_x = 0;
	float m_y = 0;

	STileset* m_tileset = 0;
	std::exception m_eLastError = {};
};
#endif /* defined (__ObjectParams__) */