#include "WarspiteUtil.h"

std::string WarspiteUtil::GetFileExtenstion(std::string path)
{
	filesys::path pObj(path);

	if (pObj.has_extension())
	{
		return pObj.extension().string();
	}
	
	return "";
}
