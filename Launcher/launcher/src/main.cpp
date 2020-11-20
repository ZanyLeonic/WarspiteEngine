#include <Windows.h>
#include <stdio.h>
#include <string.h>
#include <map>

enum class ESingletonIDs;
class IWGame;

typedef IWGame* (*GameDLL_t)(int argc, char** argv, std::map<ESingletonIDs, void(*)>* pPtrs);
typedef int (*Engine_t)(int argc, char** argv, GameDLL_t pGameDLL);

bool GetGameParam(char** argv, int argc, char*& gameName)
{
	if (argc <= 1) return false;

	for (int i = 0; i < argc; i++)
	{
		if (strcmp(argv[i], "-dll") == 0 && (i + 1) < argc)
		{
			gameName = argv[i + 1];
			return true;
		}
	}

	return false;
}

extern "C" __declspec(dllexport) int __cdecl LauncherMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	char buf[4096];
	char buf2[4096];

	char* modName = MOD_NAME;
	char* gameName = GAME_NAME;

	GetGameParam(__argv, __argc, gameName);

	snprintf(buf, sizeof(buf), "%s.dll", modName);
	snprintf(buf2, sizeof(buf2), "%s.dll", gameName);

	HINSTANCE engine = LoadLibraryEx(buf, NULL, LOAD_WITH_ALTERED_SEARCH_PATH);
	HINSTANCE game = LoadLibraryEx(buf2, NULL, LOAD_WITH_ALTERED_SEARCH_PATH);

	if (!engine)
	{
		char* pszError;
		FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&pszError, 0, NULL);

		char szBuf[1024];
		snprintf(szBuf, sizeof(szBuf), "Cannot load Engine library:\n\n%s\nAttempted to load: \"%s.dll\"", pszError, modName);
		szBuf[sizeof(szBuf) - 1] = '\0';
		MessageBox(0, szBuf, "Launcher Error", MB_ICONHAND | MB_OK);

		LocalFree(pszError);
		return 0;
	}

	if (!game)
	{
		char* pszError;
		FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&pszError, 0, NULL);

		char szBuf[1024];
		snprintf(szBuf, sizeof(szBuf), "Cannot load GameDLL library:\n\n%s\nAttempted to load: \"%s.dll\"", pszError, gameName);
		szBuf[sizeof(szBuf) - 1] = '\0';
		MessageBox(0, szBuf, "Launcher Error", MB_ICONHAND | MB_OK);

		LocalFree(pszError);
		return 0;
	}

	Engine_t main = (Engine_t)GetProcAddress(engine, "Engine");
	GameDLL_t mainGame = (GameDLL_t)GetProcAddress(game, "GameDLL");

	return main(__argc, __argv, mainGame);
}