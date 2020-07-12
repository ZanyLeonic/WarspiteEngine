#include <windows.h>
#include <string>
#include <iostream>
#include <cassert>

typedef int (*LauncherMain_t)(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd, char* basePath);

static char* GetBaseDir(const char* pFileName)
{
	static char	basedir[MAX_PATH];
	char szBuffer[MAX_PATH];
	size_t j;
	char* pBuffer = NULL;

	strcpy_s(szBuffer, pFileName);

	// Get a pointer to the last / in the path
	pBuffer = strrchr(szBuffer, '\\');
	if (pBuffer)
	{
		*(pBuffer + 1) = '\0';
	}

	strcpy_s(basedir, szBuffer);

	j = strlen(basedir);
	if (j > 0)
	{
		if ((basedir[j - 1] == '\\') ||
			(basedir[j - 1] == '/'))
		{
			basedir[j - 1] = 0;
		}
	}

	return basedir;
}

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	char* pPath = nullptr;

	char moduleBuffer[MAX_PATH];
	char szBuf[4096];

	int sp = 0;

	char* basePath = nullptr;

	if (_dupenv_s(&pPath, 0, "PATH") == 0 && pPath != nullptr)
	{
		if (GetModuleFileName(hInstance, moduleBuffer, MAX_PATH))
		{
			basePath = GetBaseDir(moduleBuffer);

			snprintf(szBuf, sizeof(szBuf), "PATH=%s\\bin;%s", basePath, pPath);
			_putenv(szBuf);

			snprintf(szBuf, sizeof(szBuf), "%s\\bin\\launcher.dll", basePath);

			HINSTANCE launcher = LoadLibraryEx(szBuf, NULL, LOAD_WITH_ALTERED_SEARCH_PATH);

			if (!launcher)
			{
				char* pszError;
				FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&pszError, 0, NULL);

				char szBuf[1024];
				snprintf(szBuf, sizeof(szBuf), "Cannot load Launcher library:\n\n%s", pszError);
				szBuf[sizeof(szBuf) - 1] = '\0';
				MessageBox(0, szBuf, "Launcher Error", MB_ICONHAND | MB_OK);

				LocalFree(pszError);
				return 0;
			}

			LauncherMain_t main = (LauncherMain_t)GetProcAddress(launcher, "LauncherMain");
			return main(hInstance, hPrevInstance, lpCmdLine, nShowCmd, basePath);
		}
		
	}

	return -1;
}