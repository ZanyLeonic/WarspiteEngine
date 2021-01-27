#include "WGame.h"
#include "IGame.h"
#include <spdlog/spdlog.h>
#include "EngineTypes.h"
#include "ObjectRegisterBuffer.h"
#include "StateRegisterBuffer.h"
#include "Vector2D.h"

CGame* CGame::s_pInstance = 0;

// Initialises the major parts of the engine
bool CGame::Init(int argc, char** argv, std::map<ESingletonIDs, void(*)>* pPtrs)
{
    // Test stuff
    m_ptrs = *pPtrs;

    // Register the GameObjects
    spdlog::info("Registering objects and states in GameDLL to the Engine...");
    CObjectRegisterBuffer::Instance()->AddAllRegistered();
    CStateRegisterBuffer::Instance()->AddAllRegistered();
    spdlog::info("Finished registering!");

    tObj = (IGame*)m_ptrs[ESingletonIDs::GAME];

    spdlog::info("**************************");
    spdlog::info("You are currently running");
    spdlog::info("DLL 2!");
    spdlog::info("Modularity for the WIN!!!");
    spdlog::info("**************************");
    // Add Game related init code here
    spdlog::info("Is the game running? {}", (tObj->IsRunning() ? "Yes" : "No"));

    return true;
}

void CGame::Draw()
{
    tObj->Draw();
}

void CGame::OnThink()
{
    tObj->OnThink();
}

void CGame::HandleEvents()
{
    tObj->HandleEvents();
}

void CGame::Destroy()
{
    tObj->Destroy();
}

void CGame::Quit()
{
    tObj->Quit();
}

bool CGame::IsRunning()
{
    if (!tObj) return false;
    return tObj->IsRunning();
}