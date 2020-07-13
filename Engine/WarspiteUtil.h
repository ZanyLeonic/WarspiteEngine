#pragma once
#ifndef __WARSPITEUTIL_H__
#define __WARSPITEUTIL_H__

#include <string>

class WarspiteUtil
{
public:
	static std::string GetFileExtenstion(std::string path);
	static std::string ReadAllText(std::string path);
};
#endif