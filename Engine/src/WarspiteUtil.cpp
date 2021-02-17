#include "WarspiteUtil.h"

#include <memory>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <vector>

#ifdef _WIN32
#include <Windows.h>
#elif _UNIX
#include <unistd.h>
#include <limits.h>
#include <sys/param.h>
#endif

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

std::shared_ptr<IGameObject> CWarspiteUtil::FindGameObject(CLevel* pLevel, std::string id)
{
	std::vector<std::vector<std::shared_ptr<IGameObject>>*> m_objects = pLevel->GetGameObjects();

	// Add the ScriptLayer
	m_objects.push_back(pLevel->GetScriptLayer()->GetGameObjects());

	// Go through each ObjectLayer we got earlier
	for (size_t i = 0; i < m_objects.size(); i++)
	{
		if (!m_objects[i]) continue;

		// Get an rvalue of the list of GameObject's for the iterated layer
		std::vector<std::shared_ptr<IGameObject>>& ir = *m_objects[i];

		for (size_t j = 0; j < ir.size(); j++)
		{
			if (ir[j]->GetName() == id) return ir[j];
		}
	}
	return nullptr;
}

bool CWarspiteUtil::GetParam(char** argv, int argc, const char* param, char* returnval)
{
	if (argc <= 1) return false;

	for (int i = 0; i < argc; i++)
	{
		if (strcmp(argv[i], param) == 0 && (i + 1) < argc)
		{
			returnval = argv[i + 1];
			return true;
		}
	}

	return false;
}

std::string CWarspiteUtil::GetExecutingDirectory()
{
	char pBuf[256];
#ifdef _WIN32
	DWORD len = sizeof(pBuf);
	GetModuleFileName(NULL, pBuf, len);
#elif _UNIX
	size_t len = sizeof(pBuf);
	int bytes = MIN(readlink("/proc/self/exe\0", pBuf, len), len - 1);
	if (bytes >= 0)
		pBuf[bytes] = '\0';
#endif
	return RemoveFileNameFromPath(std::string(pBuf));
}

float CWarspiteUtil::FLerp(float f1, float f2, float t)
{
	return (1 - t) * f1 + t * f2;
}