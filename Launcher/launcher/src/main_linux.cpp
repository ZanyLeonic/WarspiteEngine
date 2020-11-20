#include <string>
#include <iostream>
#include <cassert>
#include <dlfcn.h>
#include <stdlib.h>
#include <limits.h>
#include <errno.h>
#include <unistd.h>
#include <map>
#define MAX_PATH PATH_MAX

enum class ESingletonIDs;
class IWGame;

typedef bool (*GameDLL_t)(int argc, char** argv, std::map<ESingletonIDs, void(*)>* pPtrs, IWGame* pGame);
typedef int (*Engine_t)(int argc, char** argv, GameDLL_t pGameDLL);
#define stringize(a) #a
#define engine_binary(a,b,c) a stringize(b) c 

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

extern "C" int LauncherMain(int argc, char** argv)
{
	char* modName = MOD_NAME;
	char* gameName = GAME_NAME;

	GetGameParam(__argv, __argc, gameName);

	const char* pBinaryName = engine_binary("bin/lib", modName, ".so");
	const char* pGameBinaryN = engine_binary("assets/bin/lib", gameName, ".so");
	
	void* engine = dlopen(pBinaryName, RTLD_NOW);
	void* game = dlopen(pGameBinaryN , RTLD_NOW);
	
	int ret = 0;
	
	if (!engine)
	{
		printf("Failed to open %s (%s)\n", pBinaryName, dlerror());
		return -1;
	}
	
	if (!game)
	{
		printf("Failed to open %s (%s)\n", pGameBinaryN, dlerror());
		return -1;
	}
	
	Engine_t main = (Engine_t)dlsym(engine, "Engine");
	GameDLL_t mGame = (GameDLL_t)dlsym(game, "GameDLL");
	
	if (!main)
	{
		printf("Failed to find engine entry point (%s)\n", dlerror());
		return -1;
	}
	
	if (!mGame)
	{
		printf("Failed to find GameDLL entry point (%s)\n", dlerror());
		return -1;
	}

	ret = main(argc, argv, mGame);
	dlclose(engine);

	return ret;
}
