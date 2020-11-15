#include "TextureManager.h"
#include "SoundManager.h"
#include "Game.h"
#include "Camera.h"
#include "Player.h"
#include "StateParser.h"
#include "LevelParser.h"
#include <spdlog/spdlog.h>

#include "PlayState.h"

#include "WarspiteUtil.h"
#include "ScriptWrappers.h"
#include "ScriptManager.h"
#include "FontManager.h"

SStreamingAudioData testStream;
SStreamingAudioData testStream2;
SWaveFile testFile;

bool CPlayState::OnPlay()
{
	CGameStateBase::OnPlay();

	s_UIID = SID_PLAY;
	m_screenSize = CBaseGame::Instance()->GetViewportSize();

	CStateParser::ParseState(CEngineFileSystem::ResolvePath("PlayState.json", CEngineFileSystem::EPathType::STATE).c_str(), GetStateID(), &m_GameObjects, &m_TextureIDList, &m_ScriptIDList);
	pLevel = CLevelParser::ParseLevel(CEngineFileSystem::ResolvePath("map02.json", CEngineFileSystem::EPathType::MAP).c_str());

	CInputHandler::Instance()->AddActionKeyDown(SDL_SCANCODE_ESCAPE, [this] {
			if (CBaseGame::Instance()->GetStateManager()->GetCurrentState()->GetStateID() != SID_PAUSE)
			{
				CBaseGame::Instance()->GetStateManager()->PushState(CGameStateDictionary::Instance()->Create(SID_PAUSE));
			}
		});

	CInputHandler::Instance()->AddActionKeyUp(SDL_SCANCODE_ESCAPE, [this] {
			return;
		});

	CInputHandler::Instance()->AddActionKeyDown(SDL_SCANCODE_8, [this] {
		if (CBaseGame::Instance()->GetStateManager()->GetCurrentState()->GetStateID() != SID_PAUSE)
		{
			CScriptManager::Instance()->Load(SGameScript::file("Script2", CEngineFileSystem::ResolvePath("test2.py", CEngineFileSystem::EPathType::SCRIPT)));
			CScriptManager::Instance()->RunFromRef("Script2");
		}
		});

	CInputHandler::Instance()->AddActionKeyUp(SDL_SCANCODE_8, [this] {
		return;
		});
	
	// This callback code is disgusting - but it works so I don't care.
	testStream.PlayCallback = [this](SStreamingAudioData* as)
	{
		spdlog::info("Playing: \"{}\".", as->Filename.c_str());
	};
	
	testStream.PauseCallback = [this](SStreamingAudioData* as)
	{
		spdlog::info("Paused: \"{}\".", as->Filename.c_str());
	};

	testStream.StopCallback = [this](SStreamingAudioData* as)
	{
		spdlog::info("Stopped: \"{}\".", as->Filename.c_str());
	};

	testStream2.PlayCallback = [this](SStreamingAudioData* as)
	{
		spdlog::info("Playing: \"{}\".", as->Filename.c_str());
	};

	testStream2.PauseCallback = [this](SStreamingAudioData* as)
	{
		spdlog::info("Paused: \"{}\".", as->Filename.c_str());
	};

	testStream2.StopCallback = [this](SStreamingAudioData* as)
	{
		spdlog::info("Stopped: \"{}\".", as->Filename.c_str());
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
		alCall(alSourcef, testStream.Source, AL_PITCH, 1.0f);
		alCall(alSourcef, testStream.Source, AL_GAIN, 1.0f);
		alCall(alSource3f, testStream.Source, AL_POSITION, 50.f, 50.f, 0.f);
		alCall(alSource3f, testStream.Source, AL_VELOCITY, 0.f, 0.f, 0.f);
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
		alCall(alSourcef, testStream2.Source, AL_PITCH, 1.0f);
		alCall(alSourcef, testStream2.Source, AL_GAIN, 1.0f);
		alCall(alSource3f, testStream2.Source, AL_POSITION, 180.f, 180.f, 0.f);
		alCall(alSource3f, testStream2.Source, AL_VELOCITY, 0.f, 0.f, 0.f);
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
		CTextureManager::Instance()->CreateCheckboardPattern(m_screenSize, "levelLoadFail", CBaseGame::Instance()->GetRenderer());
		SDL_SetRenderDrawColor(CBaseGame::Instance()->GetRenderer(), 255, 255, 255, 255);
	}

	CInputHandler::Instance()->AddActionKeyDown(SDL_SCANCODE_8, [this] {
		spdlog::info("test axis value: {}", CInputHandler::Instance()->GetAxisValue("test"));
		});
	CInputHandler::Instance()->AddActionKeyUp(SDL_SCANCODE_8, [this] {
		return;
		});

	spdlog::info("Entering PlayState");

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
		CTextureManager::Instance()->Draw("levelLoadFail", 0, 0, (int)m_screenSize.GetX(), (int)m_screenSize.GetY(), CBaseGame::Instance()->GetRenderer());
	}
	// gameobjects and stuff
	CGameStateBase::Draw();
}

void CPlayState::OnThink()
{
	//std::cout << '\r' << "Movement Axis Values: "
	//	<< " MoveForward: " << CInputHandler::Instance()->GetAxisValue("MoveForward")
	//	<< " MoveRight:   " << CInputHandler::Instance()->GetAxisValue("MoveRight")
	//	<< std::flush << "     ";

	if (pLevel != 0)
		pLevel->OnThink();

	CGameStateBase::OnThink();
}

bool CPlayState::OnEnd()
{
	spdlog::info("Exiting PlayState");
	
	// Execute the OnPlay method on all the GameObjects in all Object Layers
	if (pLevel != 0)
		pLevel->Destroy();

	CGameStateBase::OnEnd();

	return true;
}