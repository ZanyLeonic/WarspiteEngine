#pragma once
#ifndef __CWarspiteUtil_H__
#define __CWarspiteUtil_H__

#include <memory>
#include <string>
#include <stdexcept>
#include <vector>

class CLevel;
class IGameObject;

class CWarspiteUtil
{
public:
	static std::string GetFileExtenstion(std::string path);
	static std::string GetFileName(std::string path, bool ext = true, char sep = 'n');
	static std::string RemoveFileNameFromPath(std::string path);
	
	static std::string ReadAllText(std::string path);

	static std::vector<std::string> SplitString(const std::string& inStr, const char& inD);

	static std::shared_ptr<IGameObject> FindGameObject(CLevel* pLevel, std::string id);
	static bool GetParam(char** argv, int argc, const char* param, char* returnval);

	static std::string GetExecutingDirectory();
};

class NotImplemented : public std::logic_error
{
public:
	NotImplemented() : std::logic_error("Method not yet implemented!") {}
};

#endif