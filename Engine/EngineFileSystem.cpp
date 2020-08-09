#include "EngineFileSystem.h"
#include <filesystem>

namespace fs = std::filesystem;

std::string CEngineFileSystem::ResolvePath(std::string ePath, EPathType pathType, std::string base)
{
	// Future revision - possible use search paths for multiple asset directories
	fs::path rPath = base.empty() ? BASEDIR : base;

	// Append the correct subfolder
	switch (pathType)
	{
	case EPathType::TEXTURE:
		rPath /= "textures";
		break;
	case EPathType::MAP:
		rPath /= "maps";
		break;
	case EPathType::SCRIPT:
		rPath /= "scripts";
		break;
	case EPathType::TILESET:
		rPath /= "tilesets";
		break;
	case EPathType::STATE:
		rPath /= "resource";
		rPath /= "states";
		break;
	case EPathType::SOUND:
		rPath /= "sound";
		break;
	default:
		break;
	};

	// Append the path provided
	rPath /= ePath;
	
	return rPath.string();
}

rapidjson::Document* CEngineFileSystem::ReadJSON(std::string path)
{
	return nullptr;
}