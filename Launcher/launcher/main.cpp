#include <Windows.h>
#include <stdio.h>

typedef int (*Engine_t)();

extern "C" __declspec(dllexport) int __cdecl LauncherMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd, char* basePath)
{
	char buf[4096];

	snprintf(buf, sizeof(buf), "%s\\bin\\%s.dll", basePath, MOD_NAME);

	HINSTANCE engine = LoadLibraryEx(buf, NULL, LOAD_WITH_ALTERED_SEARCH_PATH);

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

	Engine_t main = (Engine_t)GetProcAddress(engine, "Engine");
	return main();
}