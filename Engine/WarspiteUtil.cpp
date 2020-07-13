#include "WarspiteUtil.h"
#include <fstream>
#include <sstream>
#include <cerrno>

std::string WarspiteUtil::GetFileExtenstion(std::string path)
{
	std::size_t pos = path.rfind('.');

	if (pos != std::string::npos)
	{
		return path.substr(pos);
	}
	
	return "";
}

std::string WarspiteUtil::ReadAllText(std::string path)
{
	std::fstream nFile;
	std::string fStr;

	nFile.open(path, std::ios::in);
	if(nFile.is_open())
	{
		std::string tl;
		while(std::getline(nFile, tl))
		{
			fStr += tl;
		}
	}

	return fStr;
}