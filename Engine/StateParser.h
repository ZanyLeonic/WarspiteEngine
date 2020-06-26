#pragma once
#ifndef __StateParser_H__
#define __StateParser_H__

#include <iostream>
#include <vector>
#include <rapidjson/document.h>

class CGameObject;

class StateParser
{
public:
	StateParser() {}

	 bool ParseState(const char* stateFile, std::string stateID,
		std::vector<CGameObject*>* pObjects, std::vector<std::string>* pTextureIDs);

private:
	 void ParseObjects(const rapidjson::Value* pStateRoot,
		std::vector<CGameObject*>* pObjects);
	 void ParseTextures(const rapidjson::Value* pStateRoot,
		std::vector<std::string>* pTextureIDs);
};

#endif
