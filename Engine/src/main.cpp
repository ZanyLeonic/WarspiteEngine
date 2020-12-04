#include "Game.h"
#include "EngineMetadata.h"
#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include "EngineFileSystem.h"
#include "IWGame.h"

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/daily_file_sink.h>

// our Game object
CBaseGame* g_game = 0;

typedef IWGame* (*GameDLL_t)(int argc, char** argv, std::map<ESingletonIDs, void(*)>* pPtrs);

const int FPS = 62;
const int DELAY_TIME = 1000 / FPS;

const char* WIDTH_PARAM = "-w";
const char* HEIGHT_PARAM = "-h";
const char* FULLSCREEN_PARAM = "-f";

bool FlagExists(char** argv, int argc, const char* flag)
{
	if (argc <= 1) return false;

	for (int i = 0; i < argc; i++)
	{
		if (strcmp(argv[i], flag) == 0) return true;
	}

	return false;
}

bool GetParam(char** argv, int argc, const char* param, char*& returnval)
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

#ifdef _WIN32
extern "C" __declspec(dllexport) int __cdecl Engine(int argc, char** argv, GameDLL_t pGameDLL)
#elif _UNIX
extern "C" int Engine(int argc, char** argv, GameDLL_t pGameDLL)
#endif
{
	Uint32 frameStart, frameTime;
	char title[420];
	
	std::vector<spdlog::sink_ptr> sinks;

	// Setup logging
	try
	{
		sinks.push_back(std::make_shared<spdlog::sinks::stdout_color_sink_st>());
		sinks.push_back(std::make_shared<spdlog::sinks::daily_file_sink_st>("logs/engine.log", 23, 59));

		std::shared_ptr<spdlog::logger> combined_logger
			= std::make_shared<spdlog::logger>("engine", begin(sinks), end(sinks));
		
		spdlog::register_logger(combined_logger);
		spdlog::set_default_logger(combined_logger);

		spdlog::set_pattern("[%Y-%m-%d %H:%M:%S.%e (%z)] [Thread/%t] [%n/%^%l%$] %v ");
#ifdef WARDEBUG
		spdlog::set_level(spdlog::level::debug);
#else
		if (FlagExists(argv, argc, "-dev"))
		{
			spdlog::set_level(spdlog::level::debug);
		}
#endif
	}
	catch (const spdlog::spdlog_ex& ex)
	{
		std::cerr << "Log initialization failed: " << ex.what() << std::endl;
	}

	spdlog::info("Warspite Engine");
	
#ifndef WARDEBUG
	if (FlagExists(argv, argc, "-dev"))
	{
		snprintf(title, sizeof(title), "Engine (DEV) (%d/%s)", GAME_BUILD_NUMBER, GAME_GIT_DESC);
		spdlog::debug("Running in developer mode.");
	}
	else
	{
		snprintf(title, sizeof(title), "Engine (%d/%s)", GAME_BUILD_NUMBER, GAME_GIT_DESC);
	}
#else
	snprintf(title, sizeof(title), "Engine (DEBUG) (%d/%s)", GAME_BUILD_NUMBER, GAME_GIT_DESC);
	spdlog::debug("This is a debug build.");
#endif

	auto start = std::chrono::system_clock::now();
	std::chrono::seconds dur((long)GAME_BUILD_TIME);
	std::chrono::time_point<std::chrono::system_clock> dt(dur);
	
	auto ptr = std::chrono::system_clock::to_time_t(dt);
	auto stptr = std::chrono::system_clock::to_time_t(start);
	
	std::string st = std::string(ctime(&ptr));
	std::string rt = std::string(ctime(&stptr));

	st.erase(std::remove(st.begin(), st.end(), '\n'), st.end());
	rt.erase(std::remove(rt.begin(), rt.end(), '\n'), rt.end());
	
	spdlog::info("Build: {}", GAME_BUILD_NUMBER);
	spdlog::info("Using source: {}", GAME_GIT_HASH);
	spdlog::info("Extra Git info: {}", GAME_GIT_DESC);
	spdlog::info("Build Date: {}", st);
	spdlog::info("Running at: {}", rt);
	
	spdlog::info("Attempting Game initialization...");
	spdlog::info("Target FPS is {} FPS", FPS);

	spdlog::debug("**** PARAMETER LISTING BEGIN ****");
	for (int i = 0; i < argc; i++)
	{
		spdlog::debug("[{}]: \"{}\"", i, argv[i]);
	}
	spdlog::debug("**** PARAMETER LISTING END ****");

	int desiredWidth = DEF_WIDTH;
	int desiredHeight = DEF_HEIGHT;
	bool desiredFullscreen = FlagExists(argv, argc, FULLSCREEN_PARAM);

	char* widthParam = "";
	char* heightParam = "";

	if (GetParam(argv, argc, WIDTH_PARAM, widthParam))
	{
		desiredWidth = atoi(widthParam);

		if ((desiredWidth == 0)
			|| desiredWidth < DEF_WIDTH)
		{
			spdlog::warn("Specified width parameter is too low or invalid.");
			spdlog::warn("Got \"{}\", Lowest supported value \"{}\"", desiredWidth, DEF_WIDTH);
			desiredWidth = DEF_WIDTH;
		}
	}

	if (GetParam(argv, argc, HEIGHT_PARAM, widthParam))
	{
		desiredHeight = atoi(widthParam);

		if (desiredHeight == 0
			|| desiredHeight < DEF_HEIGHT)
		{
			spdlog::warn("Specified height parameter is too low or invalid.");
			spdlog::warn("Got \"{}\", Lowest supported value \"{}\"", desiredHeight, DEF_HEIGHT);
			desiredHeight = DEF_HEIGHT;
		}
	}

	if (CBaseGame::Instance()->Init(title, 100, 100, desiredWidth, desiredHeight, desiredFullscreen, argc, argv, pGameDLL))
	{
		IWGame* pGD = CBaseGame::Instance()->GetGameDLLClass();

		while (CBaseGame::Instance()->IsRunning())
		{
			frameStart = SDL_GetTicks();

			pGD->HandleEvents();
			pGD->OnThink();
			pGD->Draw();

			frameTime = SDL_GetTicks() - frameStart;

			if (frameTime < DELAY_TIME)
			{
				SDL_Delay((int)(DELAY_TIME - frameTime));
			}
		}
	}
	else
	{
		spdlog::error("Engine Error (Game Init failed!) - {}", SDL_GetError());
		return -1;
	}
	spdlog::info("Cleaning up...");
	CBaseGame::Instance()->Destroy();

	return 0;
}