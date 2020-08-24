#include "WGame.h"
#include "IGame.h"
#include <spdlog/spdlog.h>

CGame* CGame::s_pInstance = 0;

// Initialises the major parts of the engine
bool CGame::Init(int argc, char** argv, IGame* pGame)
{
    // Add Game related init code here
    spdlog::info("Is the game running? {}", (pGame->IsRunning() ? "Yes" : "No"));

    return true;
}