#define _CRT_SECURE_NO_WARNINGS
#include "Game.h"
#include "EngineMetadata.h"
#include <iostream>
#include <string>
#include <chrono>

#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/daily_file_sink.h"

// our Game object
CGame* g_game = 0;

const int FPS = 62;
const int DELAY_TIME = 1000 / FPS;

int main(int argc, char* argv[])
{
	Uint32 frameStart, frameTime;
	char title[420];
	
	std::vector<spdlog::sink_ptr> sinks;

	// Setup logging
	try
	{
#ifdef WARDEBUG
		sinks.push_back(std::make_shared<spdlog::sinks::stdout_color_sink_st>());
#endif
		sinks.push_back(std::make_shared<spdlog::sinks::daily_file_sink_st>("logs/engine.log", 23, 59));

		std::shared_ptr<spdlog::logger> combined_logger
			= std::make_shared<spdlog::logger>("engine", begin(sinks), end(sinks));
		
		spdlog::register_logger(combined_logger);
		spdlog::set_default_logger(combined_logger);

		spdlog::set_pattern("[%Y-%m-%d %H:%M:%S.%e (%z)] [Thread/%t] [%n/%^%l%$] %v ");
	}
	catch (const spdlog::spdlog_ex& ex)
	{
		std::cerr << "Log initialization failed: " << ex.what() << std::endl;
	}

	spdlog::info("Warspite Engine");
	
#ifndef WARDEBUG
	snprintf(title, sizeof(title), "Engine (%d/%s)", GAME_BUILD_NUMBER, GAME_GIT_DESC);
#else
	snprintf(title, sizeof(title), "Engine (DEBUG) (%d/%s)", GAME_BUILD_NUMBER, GAME_GIT_DESC);
	spdlog::debug("This is a debug build.\n");
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

	if (CGame::Instance()->Init(title, 100, 100, 640, 480, false))
	{
		while (CGame::Instance()->IsRunning())
		{
			frameStart = SDL_GetTicks();

			CGame::Instance()->HandleEvents();
			CGame::Instance()->OnThink();
			CGame::Instance()->Draw();

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
	CGame::Instance()->Destroy();

	return 0;
}