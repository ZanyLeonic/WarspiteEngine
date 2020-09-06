#ifdef _WIN32
#include <windows.h>
#include <string>
#include <iostream>
#include <cassert>
#elif _UNIX
#include <string>
#include <cstring>
#include <iostream>
#include <cassert>
#include <dlfcn.h>
#include <stdlib.h>
#include <limits.h>
#include <errno.h>
#include <unistd.h>
#define MAX_PATH PATH_MAX
#endif

#ifdef _WIN32
typedef int (*LauncherMain_t)(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd);
#elif _UNIX
typedef int (*LauncherMain_t)(int argc, char** argv);
#endif

static char* GetBaseDir(const char* pFileName)
{
	static char	basedir[MAX_PATH];
	char szBuffer[MAX_PATH];
	size_t j;
	char* pBuffer = NULL;

	strcpy(szBuffer, pFileName);

	// Get a pointer to the last / in the path
	pBuffer = strrchr(szBuffer, '\\');
	if (pBuffer)
	{
		*(pBuffer + 1) = '\0';
	}

	strcpy(basedir, szBuffer);

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

#ifdef _WIN32
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
#ifdef USE_CONSOLE
	// Create a console and output all stdout and sterr to it.
	AllocConsole();

	// Redirect the std stuff
	freopen("CONIN$", "r", stdin);
	freopen("CONOUT$", "w", stdout);
	freopen("CONOUT$", "w", stderr);
#endif
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

			snprintf(szBuf, sizeof(szBuf), "PATH=%s\\bin;%s\\assets\\bin;%s", basePath, basePath, pPath);
			_putenv(szBuf);
			snprintf(szBuf, sizeof(szBuf), "launcher.dll");

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

			// Load our distributed DLLs from bin instead of the Windows Folder >:(
			snprintf(szBuf, sizeof(szBuf), "%s\\bin", basePath);
			SetDllDirectoryA(szBuf); // might be unsafe but ehhhhhhhhhhhhhhh

			LauncherMain_t main = (LauncherMain_t)GetProcAddress(launcher, "LauncherMain");
			return main(hInstance, hPrevInstance, lpCmdLine, nShowCmd);
		}
	}
	return -1;
}
#elif _UNIX
#define stringize(a) #a
#define engine_binary(a,b,c) a stringize(b) c 
int main(int argc, char** argv)
{
	// Must add 'bin' to the path....
	char* pPath = getenv("LD_LIBRARY_PATH");
	char szBuffer[4096];
	char cwd[MAX_PATH];
	if (!getcwd(cwd, sizeof(cwd)))
	{
		printf("getcwd failed (%s)", strerror(errno));
	}

	snprintf(szBuffer, sizeof(szBuffer) - 1, "LD_LIBRARY_PATH=%s/bin:%s", cwd, pPath);
	int ret = putenv(szBuffer);
	if (ret)
	{
		printf("%s\n", strerror(errno));
	}

	const char* pBinaryName = "bin/liblauncher.so";
	void* launcher = dlopen(pBinaryName, RTLD_NOW);
	if (!launcher)
	{
		printf("Failed to open %s (%s)\n", pBinaryName, dlerror());
		return -1;
	}
	LauncherMain_t main = (LauncherMain_t)dlsym(launcher, "LauncherMain");
	if (!main)
	{
		printf("Failed to find launcher entry point (%s)\n", dlerror());
		return -1;
	}

	ret = main(argc, argv);
	dlclose(launcher);
}
#endif