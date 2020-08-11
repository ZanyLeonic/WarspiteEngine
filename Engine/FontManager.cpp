#include "FontManager.h"
#include <SDL_ttf.h>
#include <spdlog/spdlog.h>
#include <rapidjson/document.h>
#include "EngineFileSystem.h"

CFontManager* CFontManager::s_pInstance = 0;

CFontManager::CFontManager()
{
	if(TTF_Init() == -1)
	{
		spdlog::error("Cannot init TTF!");
		spdlog::error("Error: {}", SDL_GetError());
	}
}

bool CFontManager::LoadFont(std::string path, std::string type, std::string id, int size)
{
	rapidjson::Document fFile;

	if(CEngineFileSystem::ReadJSON(path, &fFile))
	{
		std::string baseName = fFile["name"].GetString();
		auto types = fFile["types"].GetArray();



		
		spdlog::info("Font \"{}\" has types:", baseName);
		for (size_t i=0; i < types.Size(); i++)
		{
			spdlog::info(types[i].GetString());
		}
	}
	return false;
}

bool CFontManager::RemoveFont(std::string id)
{
	return false;
}

bool CFontManager::RenderText(std::string text, std::string fontID, std::string textureID)
{
	return false;
}
