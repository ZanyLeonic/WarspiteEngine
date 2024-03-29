#pragma once
#ifndef __FONTMANAGER_H__
#define __FONTMANAGER_H__
#include <map>
#include <string>
#include <SDL_ttf.h>
#include <rapidjson/document.h>

class CFontManager
{
	static CFontManager* s_pInstance;
	CFontManager();

public:
	// more singleton stuff
	static CFontManager* Instance()
	{
		if (s_pInstance == nullptr)
		{
			s_pInstance = new CFontManager();
		}

		return s_pInstance;
	}

	enum class EFontRenderType
	{
		NONE = 0,
		SOLID = 1,
		SHADED = 2,
		BLENDED = 3,
	};

	bool LoadFont(std::string path, std::string type, int size);
	bool RemoveFont(std::string name, std::string type, int size);

	bool RenderText(std::string text, std::string fontID, std::string textureID, 
		EFontRenderType rType = EFontRenderType::BLENDED, 
		SDL_Colour tColour = { 0,0,0 }, SDL_Colour bColour = { 255,255,255 });

	bool IsLoaded(std::string name, std::string type, int size);

	std::map<std::string, TTF_Font*> GetLoadedFonts() const { return m_loadedFonts; }

private:
	bool lookForType(const rapidjson::Value& pNode, std::string type) const;
	
	std::map<std::string, TTF_Font*> m_loadedFonts;
};
#endif // #ifndef __FONTMANAGER_H__