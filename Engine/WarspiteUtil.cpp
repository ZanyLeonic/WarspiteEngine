#include "WarspiteUtil.h"

std::string WarspiteUtil::GetFileExtenstion(std::string path)
{
	std::size_t pos = path.rfind('.');

	if (pos != std::string::npos)
	{
		return path.substr(pos);
	}
	
	return "";
}
