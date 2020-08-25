#include "WGame.h"

#include <iostream>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/daily_file_sink.h>

#include "GameDLLMetadata.h"

#ifdef _WIN32
extern "C" __declspec(dllexport) bool __cdecl GameDLL(int argc, char** argv, IGame* pGame)
#elif _UNIX
extern "C" bool GameDLL(int argc, char** argv, IGame* pGame)
#endif
{
	std::vector<spdlog::sink_ptr> sinks;

	// Setup logging
	try
	{
		sinks.push_back(std::make_shared<spdlog::sinks::stdout_color_sink_st>());
		sinks.push_back(std::make_shared<spdlog::sinks::daily_file_sink_st>("logs/game.log", 23, 59));

		std::shared_ptr<spdlog::logger> combined_logger
			= std::make_shared<spdlog::logger>("game", begin(sinks), end(sinks));

		spdlog::register_logger(combined_logger);
		spdlog::set_default_logger(combined_logger);

		spdlog::set_pattern("[%Y-%m-%d %H:%M:%S.%e (%z)] [Thread/%t] [%n/%^%l%$] %v ");
	}
	catch (const spdlog::spdlog_ex& ex)
	{
		std::cerr << "Log initialization failed: " << ex.what() << std::endl;
	}

	spdlog::info("Warspite Game");

#ifdef WARDEBUG
	spdlog::info("Debug build");
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

	if (CGame::Instance()->Init(argc, argv, pGame))
	{
		spdlog::info("Initalisation successful!");
		return true;
	}

	return false;
}