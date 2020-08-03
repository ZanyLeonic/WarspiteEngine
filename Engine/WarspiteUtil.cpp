#include "WarspiteUtil.h"
#include <fstream>
#include <sstream>

std::string CWarspiteUtil::GetFileExtenstion(std::string path)
{
	std::size_t pos = path.rfind('.');

	if (pos != std::string::npos)
	{
		return path.substr(pos);
	}
	
	return "";
}

std::string CWarspiteUtil::GetFileName(std::string path, bool ext, char sep)
{
#ifdef _WIN32
	std::string pathDis = "\\";
#else
	std::string pathDis = "/";
#endif

	if (sep != 'n') pathDis = sep;
	
	auto pos = path.rfind(pathDis);

	if (pos == std::string::npos) return "";
	
	std::string fN = path.substr(pos+1);
	
	if (!ext)
	{
		auto fa = fN.rfind('.');
		
		fN.erase(fa, (fN.length() - fa));
	}
	
	return fN;
}

std::string CWarspiteUtil::ReadAllText(std::string path)
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
			fStr += "\n";
		}
	}

	return fStr;
}