#pragma once
#ifndef __StateParser_H__
#define __StateParser_H__

#include <memory>
#include <string>
#include <vector>
#include <rapidjson/document.h>

class IGameObject;

class CStateParser
{
public:
	CStateParser() {}

	 static bool ParseState(const char* stateFile, std::string stateID,
		 std::vector<std::unique_ptr<IGameObject>>* pObjects, std::vector<std::string>* pTextureIDs,
		 std::vector<std::string>* pScriptRefs);

private:
	static void ParseObjects(const rapidjson::Value* pStateRoot,
		std::vector<std::unique_ptr<IGameObject>>* pObjects);
	static void ParseTextures(const rapidjson::Value* pStateRoot,
		std::vector<std::string>* pTextureIDs);
	static void ParseScripts(const rapidjson::Value* pStateRoot,
		 std::vector<std::string>* pScriptsID);
};

#endif
