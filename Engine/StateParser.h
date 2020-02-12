#pragma once
#ifndef __StateParser_H__
#define __StateParser_H__

#include <iostream>
#include <vector>
#include <rapidjson/document.h>

class GameObject;

class StateParser
{
private:
	StateParser() {};
	static StateParser* s_pInstance;

public:
	static StateParser* Instance()
	{
		if (s_pInstance == 0)
		{
			s_pInstance = new StateParser();
			return s_pInstance;
		}
		return s_pInstance;
	}

	bool ParseState(const char* stateFile, std::string stateID,
		std::vector<GameObject*>* pObjects, std::vector<std::string>* pTextureIDs);

private:
	void ParseObjects(const rapidjson::Value* pStateRoot,
		std::vector<GameObject*>* pObjects);
	void ParseTextures(const rapidjson::Value* pStateRoot,
		std::vector<std::string>* pTextureIDs);
};

#endif
