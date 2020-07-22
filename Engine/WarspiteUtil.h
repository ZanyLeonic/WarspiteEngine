#pragma once
#ifndef __WARSPITEUTIL_H__
#define __WARSPITEUTIL_H__

#include <string>

class WarspiteUtil
{
public:
	static std::string GetFileExtenstion(std::string path);
	static std::string GetFileName(std::string path, bool ext = true, char sep = 'n');
	static std::string ReadAllText(std::string path);
};
#endif