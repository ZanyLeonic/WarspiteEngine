#include "FontManager.h"
#include <SDL_ttf.h>
#include <spdlog/spdlog.h>
#include <rapidjson/document.h>
#include "EngineFileSystem.h"
#include "WarspiteUtil.h"
#include <filesystem>
#include <fmt/core.h>
#include "TextureManager.h"

CFontManager* CFontManager::s_pInstance = 0;

namespace fs = std::filesystem;

CFontManager::CFontManager()
{
	if(TTF_Init() == -1)
	{
		spdlog::error("Cannot init TTF!");
		spdlog::error("Error: {}", TTF_GetError());
	}
}

bool CFontManager::LoadFont(std::string path, std::string type, int size)
{
	rapidjson::Document fFile;

	if(CEngineFileSystem::ReadJSON(path, &fFile))
	{
		std::string baseName = fFile["name"].GetString();
		const rapidjson::Value& types = fFile["types"].GetArray();

		if (!lookForType(types, type)) return false;

		fs::path fontPath = fs::path(CWarspiteUtil::RemoveFileNameFromPath(path));
		std::string nFileN = fmt::format(FMT_STRING("{}-{}"), baseName, type);
		std::string nDictN = fmt::format(FMT_STRING("{}-{}-{}"), baseName, type, size);
		
		// Janky put work plz orz
		fontPath /= (nFileN + ".ttf");
		
		TTF_Font* pFont = TTF_OpenFont(fontPath.string().c_str(), size);
		if (!pFont) return false;

		m_loadedFonts[nDictN] = pFont;

		spdlog::info(R"(Successfully loaded font "{}" {} {}pt as "{}")", baseName, type, size, nDictN);
		
		return true;
	}
	return false;
}

bool CFontManager::RemoveFont(std::string name, std::string type, int size)
{
	// Look for the specified value
	std::string fName = fmt::format(FMT_STRING("{}-{}-{}"), name, type, size);
	
	auto it
		= m_loadedFonts.find(fName);

	if(it != m_loadedFonts.end())
	{
		TTF_Font* wFont = m_loadedFonts[fName];

		TTF_CloseFont(wFont);
		m_loadedFonts[fName] = nullptr;
		
		return true;
	}
	
	return false;
}

bool CFontManager::RenderText(std::string text, std::string fontID, std::string textureID)
{
	return false;
}

bool CFontManager::lookForType(const rapidjson::Value& pNode, std::string type) const
{
	for (rapidjson::SizeType i = 0; i < pNode.Size(); i++)
	{
		if (pNode[i].GetString() == type) return true;
	}
	return false;
}
