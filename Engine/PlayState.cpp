#include "TextureManager.h"
#include "SoundManager.h"
#include "Game.h"
#include "Camera.h"
#include "Player.h"
#include "PauseState.h"
#include "StateParser.h"
#include "LevelParser.h"
#include <iostream>

#include "PlayState.h"

#include "WarspiteUtil.h"
#include "ScriptWrappers.h"
#include "ScriptManager.h"

const std::string CPlayState::s_playID = "Game";
SStreamingAudioData testStream;
SStreamingAudioData testStream2;
SWaveFile testFile;

bool CPlayState::OnPlay()
{
	CGameStateBase::OnPlay();

	m_screenSize = CGame::Instance()->GetViewportSize();

	CStateParser sp;
	sp.ParseState("assets\\resource\\states\\PlayState.json", s_playID, &m_GameObjects, &m_TextureIDList, &m_ScriptIDList);

	CLevelParser lp;
	pLevel = lp.ParseLevel("assets\\maps\\map02.json");

	te = std::make_shared<SLevelObject>(SLevelObject(pLevel));
	std::cout << "Q: Is our levelObject valid?\nA: " << (te.get()->IsValid() ? "Yes" : "No") << std::endl;
	if (te.get()->IsValid())
		std::cout << "Q: What is the name of our level?\nA: " << te.get()->GetName() << std::endl;

	try
	{
		CScriptManager::Instance()->GetEngineModule().attr("level") = te;
	}
	catch (const py::error_already_set&)
	{
		std::cout << "AAAA" << std::endl;
		PyErr_Print();
		std::cout << "AAAA" << std::endl;
	}
	
	CInputHandler::Instance()->AddActionKeyDown(SDL_SCANCODE_ESCAPE, [this] {
			if (!dynamic_cast<CPauseState*>(CGame::Instance()->GetStateManager()->GetCurrentState()))
			{
				CGame::Instance()->GetStateManager()->PushState(new CPauseState());
			}
		});

	CInputHandler::Instance()->AddActionKeyUp(SDL_SCANCODE_ESCAPE, [this] {
			return;
		});

	std::cout << WarspiteUtil::GetFileName("assets\\maps\\map02.json") << std::endl;
	std::cout << WarspiteUtil::GetFileName("assets\\maps\\map02.json", false) << std::endl;
	
	// This callback code is disgusting - but it works so I don't care.
	testStream.PlayCallback = [this](SStreamingAudioData* as)
	{
		std::cout << "Playing: \"" << as->Filename.c_str() << "\"." << std::endl;
	};
	
	testStream.PauseCallback = [this](SStreamingAudioData* as)
	{
		std::cout << "Pause: \"" << as->Filename.c_str() << "\"." << std::endl;
	};

	testStream.StopCallback = [this](SStreamingAudioData* as)
	{
		std::cout << "Stopped: \"" << as->Filename.c_str() << "\"." << std::endl;
	};

	testStream2.PlayCallback = [this](SStreamingAudioData* as)
	{
		std::cout << "Playing: \"" << as->Filename.c_str() << "\"." << std::endl;
	};

	testStream2.PauseCallback = [this](SStreamingAudioData* as)
	{
		std::cout << "Pause: \"" << as->Filename.c_str() << "\"." << std::endl;
	};

	testStream2.StopCallback = [this](SStreamingAudioData* as)
	{
		std::cout << "Stopped: \"" << as->Filename.c_str() << "\"." << std::endl;
	};

	// Load Wave
	CInputHandler::Instance()->AddActionKeyDown(SDL_SCANCODE_0, [this] {
			CSoundManager::Instance()->Load("assets/sound/mycode.wav", testFile);
		});
	CInputHandler::Instance()->AddActionKeyUp(SDL_SCANCODE_0, [this] {
			return;
		});

	// Play Wave
	CInputHandler::Instance()->AddActionKeyDown(SDL_SCANCODE_9, [this] {
		CSoundManager::Instance()->PlaySound(&testFile);
		});
	CInputHandler::Instance()->AddActionKeyUp(SDL_SCANCODE_9, [this] {
		return;
		});

	// Load
	CInputHandler::Instance()->AddActionKeyDown(SDL_SCANCODE_1, [this] {
		CSoundManager::Instance()->CreateStreamFromFile("assets/sound/teststream.ogg", testStream);
		CSoundManager::Instance()->CreateStreamFromFile("assets/sound/teststream2.ogg", testStream2);
		});
	CInputHandler::Instance()->AddActionKeyUp(SDL_SCANCODE_1, [this] {
		return;
		});

	// Stream 1
	CInputHandler::Instance()->AddActionKeyDown(SDL_SCANCODE_2, [this] {
		CSoundManager::Instance()->PlayStream(&testStream);
		});
	CInputHandler::Instance()->AddActionKeyUp(SDL_SCANCODE_2, [this] {
		return;
		});
	
	CInputHandler::Instance()->AddActionKeyDown(SDL_SCANCODE_3, [this] {
		CSoundManager::Instance()->PauseStream(&testStream);
		});
	CInputHandler::Instance()->AddActionKeyUp(SDL_SCANCODE_3, [this] {
		return;
		});
	CInputHandler::Instance()->AddActionKeyDown(SDL_SCANCODE_6, [this] {
		CSoundManager::Instance()->StopStream(&testStream);
		});
	CInputHandler::Instance()->AddActionKeyUp(SDL_SCANCODE_6, [this] {
		return;
		});


	// Stream 2
	CInputHandler::Instance()->AddActionKeyDown(SDL_SCANCODE_4, [this] {
		CSoundManager::Instance()->PlayStream(&testStream2);
		});
	CInputHandler::Instance()->AddActionKeyUp(SDL_SCANCODE_4, [this] {
		return;
		});
	
	// Play
	CInputHandler::Instance()->AddActionKeyDown(SDL_SCANCODE_5, [this] {
		CSoundManager::Instance()->PauseStream(&testStream2);
		});
	CInputHandler::Instance()->AddActionKeyUp(SDL_SCANCODE_5, [this] {
		return;
		});
	CInputHandler::Instance()->AddActionKeyDown(SDL_SCANCODE_7, [this] {
		CSoundManager::Instance()->StopStream(&testStream2);
		});
	CInputHandler::Instance()->AddActionKeyUp(SDL_SCANCODE_7, [this] {
		return;
		});

	CInputHandler::Instance()->AddActionKeyDown(SDL_SCANCODE_8, [this] {
		CScriptManager::Instance()->RunFromRef("TestScript");
		});
	CInputHandler::Instance()->AddActionKeyUp(SDL_SCANCODE_8, [this] {
		return;
		});

	if (pLevel)
	{
		// Give the camera the Level size
		CCamera::Instance()->SetLevelSize(pLevel->m_LevelSize);

		// Execute the OnPlay method on all the GameObjects in all Object Layers
		pLevel->OnPlay();
	}
	else
	{ 
		// If a failure happens when attempting to load - this will let us know something is up.
		CTextureManager::Instance()->CreateCheckboardPattern(m_screenSize, "levelLoadFail", CGame::Instance()->GetRenderer());
		SDL_SetRenderDrawColor(CGame::Instance()->GetRenderer(), 255, 255, 255, 255);
	}

	std::cout << "Entering PlayState\n";
	
	return true;
}

void CPlayState::Draw()
{
	if (pLevel != 0)
	{
		pLevel->Draw();
	}
	else
	{
		CTextureManager::Instance()->Draw("levelLoadFail", 0, 0, (int)m_screenSize.GetX(), (int)m_screenSize.GetY(), CGame::Instance()->GetRenderer());
	}
	// gameobjects and stuff
	CGameStateBase::Draw();
}

void CPlayState::OnThink()
{
	if (pLevel != 0)
		pLevel->OnThink();

	CGameStateBase::OnThink();
}

bool CPlayState::OnEnd()
{
	std::cout << "Exiting PlayState\n";

	// Execute the OnPlay method on all the GameObjects in all Object Layers
	if (pLevel != 0)
		pLevel->Destroy();
	
	CScriptManager::Instance()->GetEngineModule().attr("level") = py::none();

	CGameStateBase::OnEnd();

	return true;
}