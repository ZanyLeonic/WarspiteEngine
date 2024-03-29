#pragma once
#ifndef __ENGINEFILESYSTEM_H__
#define __ENGINEFILESYSTEM_H__

#include <string>
#include <rapidjson/document.h>

// For now - hard code it
#ifndef DLLBUILD
#define BASEDIR "./assets"
#else
#define BASEDIR "../assets"
#endif

class CEngineFileSystem
{
public:
	enum class EPathType
	{
		NONE,
		TEXTURE,
		MAP,
		SCRIPT,
		TILESET,
		STATE,
		SOUND,
		FONTS
	};
	
	static std::string ResolvePath(std::string ePath, EPathType pathType = EPathType::NONE, std::string base = "");
	static bool ReadJSON(std::string path, rapidjson::Document* inDoc);
};

#endif