#include "WGame.h"
#include "Game.h"
#include "GameStateDictionary.h"
#include "GameState.h"

CGame* CGame::s_pInstance = 0;

// Initialises the major parts of the engine
bool CGame::Init(int argc, char** argv)
{
    GetGame()->GetStateManager()->ModifyState(GetStateDict()->Create(SID_MM));
    return true;
}