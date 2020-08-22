#include <Windows.h>
#include <stdio.h>

typedef bool (*GameDLL_t)(int argc, char** argv);
typedef int (*Engine_t)(int argc, char** argv, GameDLL_t pGameDLL);

extern "C" __declspec(dllexport) int __cdecl LauncherMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd, char* basePath)
{
	char buf[4096];
	char buf2[4096];

	snprintf(buf, sizeof(buf), "%s\\bin\\%s.dll", basePath, MOD_NAME);
	snprintf(buf2, sizeof(buf2), "%s\\assets\\bin\\%s.dll", basePath, GAME_NAME);

	HINSTANCE engine = LoadLibraryEx(buf, NULL, LOAD_WITH_ALTERED_SEARCH_PATH);
	HINSTANCE game = LoadLibraryEx(buf2, NULL, LOAD_WITH_ALTERED_SEARCH_PATH);

	if (!engine)
	{
		char* pszError;
		FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&pszError, 0, NULL);

		char szBuf[1024];
		snprintf(szBuf, sizeof(szBuf), "Cannot load Engine library:\n\n%s", pszError);
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
		snprintf(szBuf, sizeof(szBuf), "Cannot load GameDLL library:\n\n%s", pszError);
		szBuf[sizeof(szBuf) - 1] = '\0';
		MessageBox(0, szBuf, "Launcher Error", MB_ICONHAND | MB_OK);

		LocalFree(pszError);
		return 0;
	}

	Engine_t main = (Engine_t)GetProcAddress(engine, "Engine");
	GameDLL_t mainGame = (GameDLL_t)GetProcAddress(game, "GameDLL");

	return main(__argc, __argv, mainGame);
}
