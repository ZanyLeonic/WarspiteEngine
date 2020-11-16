#include "EngineFileSystem.h"
#include <filesystem>
#include <iostream>
#include <rapidjson/filereadstream.h>
#include <spdlog/spdlog.h>

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
	case EPathType::FONTS:
		rPath /= "resource";
		rPath /= "fonts";
		break;
	default:
		break;
	};

	// Append the path provided
	rPath /= ePath;
	
	return rPath.string();
}

bool CEngineFileSystem::ReadJSON(std::string path, rapidjson::Document* inDoc)
{
	FILE* fp = fopen(path.c_str(), "rb");

	if (fp != NULL)
	{
		char readBuffer[4096];
		rapidjson::FileReadStream is(fp, readBuffer, sizeof(readBuffer));

		inDoc->ParseStream(is);

		// Have we parsed the JSON correctly?
		if (inDoc->HasParseError())
		{
			spdlog::error("An error has occurred when loading \"{}\"", path);
			spdlog::error("Error code from RapidJSON: {}", inDoc->GetParseError());
			return false;
		}
		fclose(fp);
		return true;
	}
		
	return false;
}