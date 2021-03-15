#include "FontManager.h"
#include <SDL_ttf.h>
#include <spdlog/spdlog.h>
#include <rapidjson/document.h>
#include "EngineFileSystem.h"
#include "WarspiteUtil.h"
#include <filesystem>
#include <fmt/core.h>
#include "TextureManager.h"
#include "Game.h"

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

bool CFontManager::LoadFont(std::string path, std::string type, int size, bool extProvided)
{
	if (!extProvided) path += ".json";

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

	if(IsLoaded(name, type, size))
	{
		TTF_Font* wFont = m_loadedFonts[fName];

		TTF_CloseFont(wFont);
		m_loadedFonts[fName] = nullptr;
		
		return true;
	}
	
	return false;
}

bool CFontManager::RenderText(std::string text, std::string fontID, std::string textureID, 
	EFontRenderType rType, SDL_Colour tColour, SDL_Colour bColour, Uint32 width)
{
	SDL_Surface* pSurface = nullptr;

	if (!m_loadedFonts[fontID])
	{
		spdlog::error("Cannot find type \"{}\"!", fontID);
		return false;
	}

	switch (rType)
	{
	case EFontRenderType::SOLID:
		pSurface = TTF_RenderText_Solid(m_loadedFonts[fontID], text.c_str(), tColour);
		break;
	case EFontRenderType::BLENDED:
		pSurface = TTF_RenderText_Blended(m_loadedFonts[fontID], text.c_str(), tColour);
		break;
	case EFontRenderType::BLENDED_WRAPPED:
		pSurface = TTF_RenderText_Blended_Wrapped(m_loadedFonts[fontID], text.c_str(), tColour, width);
		break;
	case EFontRenderType::SHADED:
		pSurface = TTF_RenderText_Shaded(m_loadedFonts[fontID], text.c_str(), tColour, bColour);
		break;
	default:
		spdlog::warn("Unsupported or undefined type of text rendering!");
		break;
	}

	if (!pSurface)
	{
		spdlog::error("Failed to render text with fontID \"{}\" with text \"{}\".", fontID, text);
		spdlog::error("SDL2_ttf error:");
		spdlog::error(TTF_GetError());

		return false;
	}

	SDL_Texture* pTexture =
		SDL_CreateTextureFromSurface(CBaseGame::Instance()->GetRenderer(), pSurface);

	// Free up memory, why didn't I add this earlier aaaaaaaa
	SDL_FreeSurface(pSurface);

	CTextureManager::Instance()->Add(new CTexture(pTexture), textureID);

	return true;
}

bool CFontManager::IsLoaded(std::string name, std::string type, int size)
{
	// Look for the specified value
	std::string fName = fmt::format(FMT_STRING("{}-{}-{}"), name, type, size);

	auto it
		= m_loadedFonts.find(fName);

	if (it != m_loadedFonts.end())
	{
		return m_loadedFonts[fName] != nullptr;
	}
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
