#include "WGame.h"
#include "IGame.h"
#include <spdlog/spdlog.h>
#include "EngineTypes.h"
#include "Vector2D.h"
#include "discord-game-sdk/discord.h"

CGame* CGame::s_pInstance = 0;
discord::Core* core{};

// Initialises the major parts of the engine
bool CGame::Init(int argc, char** argv, std::map<ESingletonIDs, void(*)>* pPtrs)
{
    // Test stuff
    m_ptrs = *pPtrs;

    tObj = (IGame*)m_ptrs[ESingletonIDs::GAME];

    // Add Game related init code here
    spdlog::info("Is the game running? {}", (tObj->IsRunning() ? "Yes" : "No"));

    auto result = discord::Core::Create(400721399846797313, DiscordCreateFlags_Default, &core);
    discord::Activity activity{
 
    };
    activity.SetState("In-Game");
    activity.SetDetails("Testing");
    activity.GetAssets().SetLargeImage("game_icon");
    activity.GetAssets().SetLargeText("In-Game");
    core->ActivityManager().UpdateActivity(activity, [](discord::Result result) {
        spdlog::info("Discord result returned: {}", result);
    });
    return true;
}

void CGame::Draw()
{
    tObj->Draw();
}

void CGame::OnThink()
{
    tObj->OnThink();
    ::core->RunCallbacks();
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