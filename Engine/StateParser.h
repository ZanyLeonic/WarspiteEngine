#pragma once
#ifndef __StateParser_H__
#define __StateParser_H__

#include <string>
#include <vector>
#include <rapidjson/document.h>

class IGameObject;

class CStateParser
{
public:
	CStateParser() {}

	 bool ParseState(const char* stateFile, std::string stateID,
		std::vector<IGameObject*>* pObjects, std::vector<std::string>* pTextureIDs);

private:
	 void ParseObjects(const rapidjson::Value* pStateRoot,
		std::vector<IGameObject*>* pObjects);
	 void ParseTextures(const rapidjson::Value* pStateRoot,
		std::vector<std::string>* pTextureIDs);
};

#endif
