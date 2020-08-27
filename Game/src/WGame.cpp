#include "WGame.h"
#include "IGame.h"
#include <spdlog/spdlog.h>
#include "EngineTypes.h"

CGame* CGame::s_pInstance = 0;

// Initialises the major parts of the engine
bool CGame::Init(int argc, char** argv, std::map<ESingletonIDs, void(*)>* pPtrs)
{
    // Test stuff
    m_ptrs = *pPtrs;

    IGame* tObj = (IGame*)m_ptrs[ESingletonIDs::GAME];

    // Add Game related init code here
    spdlog::info("Is the game running? {}", (tObj->IsRunning() ? "Yes" : "No"));

    return true;
}