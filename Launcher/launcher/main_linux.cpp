#include <string>
#include <iostream>
#include <cassert>
#include <dlfcn.h>
#include <stdlib.h>
#include <limits.h>
#include <errno.h>
#include <unistd.h>
#define MAX_PATH PATH_MAX

typedef int (*Engine_t)(int argc, char** argv);
#define stringize(a) #a
#define engine_binary(a,b,c) a stringize(b) c 

extern "C" int LauncherMain(int argc, char** argv)
{
	const char* pBinaryName = engine_binary("bin/lib", MOD_NAME_L, ".so");
	void* engine = dlopen(pBinaryName, RTLD_NOW);
	int ret = 0;
	
	if (!engine)
	{
		printf("Failed to open %s (%s)\n", pBinaryName, dlerror());
		return -1;
	}
	Engine_t main = (Engine_t)dlsym(engine, "Engine");
	if (!main)
	{
		printf("Failed to find engine entry point (%s)\n", dlerror());
		return -1;
	}

	ret = main(argc, argv);
	dlclose(engine);
}
