#include "WarspiteUtil.h"
#include <fstream>
#include <sstream>
#include <vector>

#include "Level.h"
#include "ObjectLayer.h"

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

std::string CWarspiteUtil::RemoveFileNameFromPath(std::string path)
{
#ifdef _WIN32
	std::string pathDis = "\\";
#else
	std::string pathDis = "/";
#endif
	auto pos = path.rfind(pathDis);
	if (pos == std::string::npos) return path; // no file name

	std::string fN = path.substr(pos + 1);

	auto dif = path.length() - pos;
	auto nLen = path.length() - dif;

	return path.substr(0, nLen + 1);
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

std::vector<std::string> CWarspiteUtil::SplitString(const std::string& inStr, const char& inD)
{
	std::stringstream ss(inStr);
	std::vector<std::string> out;
	
	std::string curLine;
	while (std::getline(ss, curLine, inD))
	{
		out.push_back(curLine);
	}

	return out;
}

IGameObject* CWarspiteUtil::FindGameObject(CLevel* pLevel, std::string id)
{
	std::vector<std::vector<IGameObject*>*> m_objects = pLevel->GetGameObjects();

	// Add the ScriptLayer
	m_objects.push_back(pLevel->GetScriptLayer()->GetGameObjects());

	// Go through each ObjectLayer we got earlier
	for (size_t i = 0; i < m_objects.size(); i++)
	{
		if (!m_objects[i]) continue;

		// Get an rvalue of the list of GameObject's for the iterated layer
		std::vector<IGameObject*>& ir = *m_objects[i];

		for (size_t j = 0; j < ir.size(); j++)
		{
			if (ir[j]->GetName() == id) return ir[j];
		}
	}
	return nullptr;
}
