#include "WGame.h"

#ifdef _WIN32
extern "C" __declspec(dllexport) bool __cdecl GameDLL(int argc, char** argv)
#elif _UNIX
extern "C" bool GameDLL(int argc, char** argv)
#endif
{
	if (CGame::Instance()->Init(argc, argv))
	{

	}

	return 0;
}