#include "TextureManager.h"
#include "SoundManager.h"
#include "Game.h"
#include "Camera.h"
#include "Player.h"
#include "StateParser.h"
#include "LevelParser.h"
#include <iostream>

#include "PlayState.h"

#include "WarspiteUtil.h"
#include "ScriptWrappers.h"
#include "ScriptManager.h"

SStreamingAudioData testStream;
SStreamingAudioData testStream2;
SWaveFile testFile;

bool CPlayState::OnPlay()
{
	CGameStateBase::OnPlay();

	s_UIID = SID_PLAY;
	m_screenSize = CGame::Instance()->GetViewportSize();

	CStateParser sp;
	sp.ParseState(CEngineFileSystem::ResolvePath("PlayState.json", CEngineFileSystem::EPathType::STATE).c_str(), GetStateID(), &m_GameObjects, &m_TextureIDList, &m_ScriptIDList);

	CLevelParser lp;
	pLevel = lp.ParseLevel(CEngineFileSystem::ResolvePath("map02.json", CEngineFileSystem::EPathType::MAP).c_str());

	CInputHandler::Instance()->AddActionKeyDown(SDL_SCANCODE_ESCAPE, [this] {
			if (CGame::Instance()->GetStateManager()->GetCurrentState()->GetStateID() != SID_PAUSE)
			{
				CGame::Instance()->GetStateManager()->PushState(CGameStateDictionary::Instance()->Create(SID_PAUSE));
			}
		});

	CInputHandler::Instance()->AddActionKeyUp(SDL_SCANCODE_ESCAPE, [this] {
			return;
		});

	CInputHandler::Instance()->AddActionKeyDown(SDL_SCANCODE_8, [this] {
		if (CGame::Instance()->GetStateManager()->GetCurrentState()->GetStateID() != SID_PAUSE)
		{
			CScriptManager::Instance()->Load(SGameScript::file("Script2", CEngineFileSystem::ResolvePath("test2.py", CEngineFileSystem::EPathType::SCRIPT)));
			CScriptManager::Instance()->RunFromRef("Script2");
		}
		});

	CInputHandler::Instance()->AddActionKeyUp(SDL_SCANCODE_8, [this] {
		return;
		});

	
	std::cout << CWarspiteUtil::GetFileName(CEngineFileSystem::ResolvePath("map02.json", CEngineFileSystem::EPathType::MAP)) << std::endl;
	std::cout << CWarspiteUtil::GetFileName(CEngineFileSystem::ResolvePath("map02.json", CEngineFileSystem::EPathType::MAP), false) << std::endl;
	
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
			CSoundManager::Instance()->Load(CEngineFileSystem::ResolvePath("mycode.wav", CEngineFileSystem::EPathType::SOUND), testFile);
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
		CSoundManager::Instance()->CreateStreamFromFile(CEngineFileSystem::ResolvePath("teststream.ogg", CEngineFileSystem::EPathType::SOUND), testStream);
		CSoundManager::Instance()->CreateStreamFromFile(CEngineFileSystem::ResolvePath("teststream2.ogg", CEngineFileSystem::EPathType::SOUND), testStream2);
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

	if (pLevel)
	{
		// Give the camera the Level size
		CCamera::Instance()->SetLevelSize(pLevel->m_LevelSize);

		// Assign the Level attribute to the current loaded level
		if (CScriptManager::Instance()->GetEngineModule().attr(LEVELOBJECT_NAME).is_none())
		{
			m_levelPtr = std::make_shared<SLevelObject>(SLevelObject(pLevel));
			CScriptManager::Instance()->GetEngineModule().attr(LEVELOBJECT_NAME) = m_levelPtr;
		}
		
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

	CGameStateBase::OnEnd();

	return true;
}