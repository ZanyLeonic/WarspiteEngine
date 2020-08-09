#pragma once
#ifndef __ENGINEFILESYSTEM_H__
#define __ENGINEFILESYSTEM_H__
#include <string>
#include <rapidjson/document.h>

// For now - hard code it
#ifndef DLLBUILD
#define BASEDIR "assets"
#else
#define BASEDIR "../assets"
#endif

enum class EPathType
{
	NONE,
	TEXTURE,
	MAP,
	SCRIPT,
	TILESET,
	STATE,
	SOUND
};

class CEngineFileSystem
{
public:
	static std::string ResolvePath(std::string ePath, EPathType pathType = EPathType::NONE, std::string base = "");
	static rapidjson::Document* ReadJSON(std::string path);
};

#endif