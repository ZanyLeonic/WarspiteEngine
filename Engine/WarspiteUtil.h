#pragma once
#ifndef __WARSPITEUTIL_H__
#define __WARSPITEUTIL_H__

#include <string>
#include <boost/filesystem.hpp>

namespace filesys = boost::filesystem;

class WarspiteUtil
{
public:
	static std::string GetFileExtenstion(std::string path);
};
#endif