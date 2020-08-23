#include "WGame.h"
#include "Game.h"
#include <iostream>

CGame* CGame::s_pInstance = 0;

// Initialises the major parts of the engine
bool CGame::Init(int argc, char** argv)
{
    std::cout << "Is the game running?" << (CBaseGame::Instance()->IsRunning() ? "Yes" : "No") << std::endl;
    return true;
}