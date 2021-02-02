#include "TestState.h"
#include <spdlog/spdlog.h>

bool CTestState::OnPlay()
{
    CGameStateBase::OnPlay();

    s_UIID = SID_TEST;
    spdlog::info("Entering TestState");

    return true;
}

bool CTestState::OnEnd()
{
    CGameStateBase::OnEnd();
    spdlog::info("Leaving TestState");

    return true;
}

void CTestState::Draw()
{}

void CTestState::OnThink()
{}