#pragma once
#ifndef __WARSPITEUTIL_H__
#define __WARSPITEUTIL_H__

#include <string>
#include <stdexcept>

class WarspiteUtil
{
public:
	static std::string GetFileExtenstion(std::string path);
	static std::string GetFileName(std::string path, bool ext = true, char sep = 'n');
	static std::string ReadAllText(std::string path);
};

class NotImplemented : public std::logic_error
{
public:
	NotImplemented() : std::logic_error("Method not yet implemented!") {}
};

#endif