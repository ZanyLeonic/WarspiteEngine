#include "TextureManager.h"
#include "SoundManager.h"
#include "Game.h"
#include "Camera.h"
#include "Player.h"
#include "StateParser.h"
#include "LevelParser.h"
#include "ObjectLayer.h"
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

	if (CBaseGame::Instance()->StartedWithMapParam())
	{
		CBaseGame::Instance()->SetLoadedLevel(CLevelParser::ParseLevel(CEngineFileSystem::ResolvePath(fmt::format(FMT_STRING("{}.json"), CBaseGame::Instance()->GetMapParamName()), CEngineFileSystem::EPathType::MAP).c_str()));
	}
	else
	{
		CBaseGame::Instance()->SetLoadedLevel(CLevelParser::ParseLevel(CEngineFileSystem::ResolvePath("map01.json", CEngineFileSystem::EPathType::MAP).c_str()));
	}

	CInputHandler::Instance()->AddActionKeyDown(SDL_SCANCODE_ESCAPE, [this](SDL_Scancode e) {
		if (CBaseGame::Instance()->GetStateManager()->GetCurrentStateID() != SID_PAUSE)
		{
			CBaseGame::Instance()->GetStateManager()->PushState(CGameStateDictionary::Instance()->Create(SID_PAUSE));
		}
		});

	CInputHandler::Instance()->AddActionKeyUp(SDL_SCANCODE_ESCAPE, [this](SDL_Scancode e) {
		return;
		});

	// This callback code is disgusting - but it works so I don't care.
	testStream.PlayCallback = s_PlayCallback;
	testStream.PauseCallback = s_PauseCallback;
	testStream.StopCallback = s_StopCallback;

	testStream2.PlayCallback = s_PlayCallback;
	testStream2.PauseCallback = s_PauseCallback;
	testStream2.StopCallback = s_StopCallback;

	// Load Wave
	CInputHandler::Instance()->AddActionKeyDown(SDL_SCANCODE_0, [this](SDL_Scancode e) {
		CSoundManager::Instance()->Load(CEngineFileSystem::ResolvePath("mycode.wav", CEngineFileSystem::EPathType::SOUND), testFile);
	});
	CInputHandler::Instance()->AddActionKeyUp(SDL_SCANCODE_0, [this](SDL_Scancode e) {
		return;
	});

	// Load
	CInputHandler::Instance()->AddActionKeyDown(SDL_SCANCODE_1, [this](SDL_Scancode e) {
		CSoundManager::Instance()->CreateStreamFromFile(CEngineFileSystem::ResolvePath("teststream.ogg", CEngineFileSystem::EPathType::SOUND), testStream);
		CSoundManager::Instance()->CreateStreamFromFile(CEngineFileSystem::ResolvePath("teststream2.ogg", CEngineFileSystem::EPathType::SOUND), testStream2);
	});
	CInputHandler::Instance()->AddActionKeyUp(SDL_SCANCODE_1, [this](SDL_Scancode e) {
		return;
	});

	// Stream 1
	CInputHandler::Instance()->AddActionKeyDown(SDL_SCANCODE_2, [this](SDL_Scancode e) {
		alCall(alSourcef, testStream.Source, AL_PITCH, 1.0f);
		alCall(alSourcef, testStream.Source, AL_GAIN, 1.0f);
		alCall(alSource3f, testStream.Source, AL_POSITION, 50.f, 50.f, 0.f);
		alCall(alSource3f, testStream.Source, AL_VELOCITY, 0.f, 0.f, 0.f);
		CSoundManager::Instance()->PlayStream(&testStream);
	});
	CInputHandler::Instance()->AddActionKeyUp(SDL_SCANCODE_2, [this](SDL_Scancode e) {
		return;
	});

	CInputHandler::Instance()->AddActionKeyDown(SDL_SCANCODE_3, [this](SDL_Scancode e) {
		CSoundManager::Instance()->PauseStream(&testStream);
	});
	CInputHandler::Instance()->AddActionKeyUp(SDL_SCANCODE_3, [this](SDL_Scancode e) {
		return;
	});

	// Stream 2
	CInputHandler::Instance()->AddActionKeyDown(SDL_SCANCODE_4, [this](SDL_Scancode e) {
		alCall(alSourcef, testStream2.Source, AL_PITCH, 1.0f);
		alCall(alSourcef, testStream2.Source, AL_GAIN, 1.0f);
		alCall(alSource3f, testStream2.Source, AL_POSITION, 180.f, 180.f, 0.f);
		alCall(alSource3f, testStream2.Source, AL_VELOCITY, 0.f, 0.f, 0.f);
		CSoundManager::Instance()->PlayStream(&testStream2);
	});
	CInputHandler::Instance()->AddActionKeyUp(SDL_SCANCODE_4, [this](SDL_Scancode e) {
		return;
	});

	// Play
	CInputHandler::Instance()->AddActionKeyDown(SDL_SCANCODE_5, [this](SDL_Scancode e) {
		CSoundManager::Instance()->PauseStream(&testStream2);
	});
	CInputHandler::Instance()->AddActionKeyUp(SDL_SCANCODE_5, [this](SDL_Scancode e) {
		return;
	});

	CInputHandler::Instance()->AddActionKeyDown(SDL_SCANCODE_6, [this](SDL_Scancode e) {
		CSoundManager::Instance()->StopStream(&testStream);
	});
	CInputHandler::Instance()->AddActionKeyUp(SDL_SCANCODE_6, [this](SDL_Scancode e) {
		return;
	});

	CInputHandler::Instance()->AddActionKeyDown(SDL_SCANCODE_7, [this](SDL_Scancode e) {
		CSoundManager::Instance()->StopStream(&testStream2);
	});
	CInputHandler::Instance()->AddActionKeyUp(SDL_SCANCODE_7, [this](SDL_Scancode e) {
		return;
	});

	CInputHandler::Instance()->AddActionKeyDown(SDL_SCANCODE_8, [this](SDL_Scancode e) {
		if (CBaseGame::Instance()->GetStateManager()->GetCurrentStateID() != SID_PAUSE)
		{
			CScriptManager::Instance()->Load(SGameScript::file("Script2", CEngineFileSystem::ResolvePath("test2.py", CEngineFileSystem::EPathType::SCRIPT)));
			CScriptManager::Instance()->RunFromRef("Script2");
		}
	});

	CInputHandler::Instance()->AddActionKeyUp(SDL_SCANCODE_8, [this](SDL_Scancode e) {
		return;
	});

	// Play Wave
	CInputHandler::Instance()->AddActionKeyDown(SDL_SCANCODE_9, [this](SDL_Scancode e) {
		CSoundManager::Instance()->PlaySound(&testFile);
	});
	CInputHandler::Instance()->AddActionKeyUp(SDL_SCANCODE_9, [this](SDL_Scancode e) {
		return;
	});

	// Try test state
	CInputHandler::Instance()->AddActionKeyDown(SDL_SCANCODE_B, [this](SDL_Scancode e) {
		if (CBaseGame::Instance()->GetStateManager()->GetCurrentStateID() != SID_TEST)
		{
			CBaseGame::Instance()->GetStateManager()->PushState(CGameStateDictionary::Instance()->Create(SID_TEST));
		}
		else if (CBaseGame::Instance()->GetStateManager()->GetCurrentStateID() == SID_TEST
			&& CBaseGame::Instance()->GetStateManager()->GetCurrentStateID() != SID_PLAY)
		{
			CBaseGame::Instance()->GetStateManager()->PopState();
		}
		});
	CInputHandler::Instance()->AddActionKeyUp(SDL_SCANCODE_B, [this](SDL_Scancode e) {
		return;
		});

	if (CBaseGame::Instance()->GetLoadedLevel())
	{
		// Give the camera the Level size
		CCamera::Instance()->SetLevelSize(CBaseGame::Instance()->GetLoadedLevel()->m_LevelSize);
		// CBaseGame::Instance()->SetLoadedLevel(pLevel);

		// Assign the Level attribute to the current loaded level
		if (CScriptManager::Instance()->GetEngineModule().attr(LEVELOBJECT_NAME).is_none())
		{
			m_levelPtr = std::make_shared<SLevelObject>(SLevelObject(CBaseGame::Instance()->GetLoadedLevel()));
			CScriptManager::Instance()->GetEngineModule().attr(LEVELOBJECT_NAME) = m_levelPtr;
		}

		// Execute the OnPlay method on all the GameObjects in all Object Layers
		CBaseGame::Instance()->GetLoadedLevel()->OnPlay();
	}
	else
	{
		// If a failure happens when attempting to load - this will let us know something is up.
		CTextureManager::Instance()->CreateCheckboardPattern(m_screenSize, "levelLoadFail", CBaseGame::Instance()->GetRenderer());
		SDL_SetRenderDrawColor(CBaseGame::Instance()->GetRenderer(), 255, 255, 255, 255);
	}

	spdlog::info("Entering PlayState");

	return true;
}

void CPlayState::Draw()
{
	if (CBaseGame::Instance()->GetLoadedLevel() != 0)
	{
		CBaseGame::Instance()->GetLoadedLevel()->Draw();
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
	if (CBaseGame::Instance()->GetLoadedLevel() != 0)
		CBaseGame::Instance()->GetLoadedLevel()->OnThink();

	CGameStateBase::OnThink();
}

bool CPlayState::OnEnd()
{
	spdlog::info("Exiting PlayState");

	CInputHandler::Instance()->RemoveActionKeyDown(SDL_SCANCODE_ESCAPE);
	CInputHandler::Instance()->RemoveActionKeyDown(SDL_SCANCODE_0);
	CInputHandler::Instance()->RemoveActionKeyDown(SDL_SCANCODE_1);
	CInputHandler::Instance()->RemoveActionKeyDown(SDL_SCANCODE_2);
	CInputHandler::Instance()->RemoveActionKeyDown(SDL_SCANCODE_3);
	CInputHandler::Instance()->RemoveActionKeyDown(SDL_SCANCODE_4);
	CInputHandler::Instance()->RemoveActionKeyDown(SDL_SCANCODE_5);
	CInputHandler::Instance()->RemoveActionKeyDown(SDL_SCANCODE_6);
	CInputHandler::Instance()->RemoveActionKeyDown(SDL_SCANCODE_7);
	CInputHandler::Instance()->RemoveActionKeyDown(SDL_SCANCODE_8);
	CInputHandler::Instance()->RemoveActionKeyDown(SDL_SCANCODE_9);
	CInputHandler::Instance()->RemoveActionKeyDown(SDL_SCANCODE_B);

	CInputHandler::Instance()->RemoveActionKeyUp(SDL_SCANCODE_ESCAPE);
	CInputHandler::Instance()->RemoveActionKeyUp(SDL_SCANCODE_0);
	CInputHandler::Instance()->RemoveActionKeyUp(SDL_SCANCODE_1);
	CInputHandler::Instance()->RemoveActionKeyUp(SDL_SCANCODE_2);
	CInputHandler::Instance()->RemoveActionKeyUp(SDL_SCANCODE_3);
	CInputHandler::Instance()->RemoveActionKeyUp(SDL_SCANCODE_4);
	CInputHandler::Instance()->RemoveActionKeyUp(SDL_SCANCODE_5);
	CInputHandler::Instance()->RemoveActionKeyUp(SDL_SCANCODE_6);
	CInputHandler::Instance()->RemoveActionKeyUp(SDL_SCANCODE_7);
	CInputHandler::Instance()->RemoveActionKeyUp(SDL_SCANCODE_8);
	CInputHandler::Instance()->RemoveActionKeyUp(SDL_SCANCODE_9);
	CInputHandler::Instance()->RemoveActionKeyUp(SDL_SCANCODE_B);

	// Execute the OnPlay method on all the GameObjects in all Object Layers
	if (CBaseGame::Instance()->GetLoadedLevel() != 0)
		CBaseGame::Instance()->GetLoadedLevel()->Destroy();

	CGameStateBase::OnEnd();

	return true;
}

SCollisionData CPlayState::IsColliding(CVector2D v1)
{
	SCollisionData r;
	bool sGameCollide = false;

	if (CBaseGame::Instance()->GetLoadedLevel())
	{
		// Script layer has priority
		std::vector<std::shared_ptr<IGameObject>>  sObjs = *CBaseGame::Instance()->GetLoadedLevel()->GetScriptLayer()->GetGameObjects();

		for (size_t k = 0; k < sObjs.size(); k++)
		{
			if (sObjs[k].get() != CBaseGame::Instance()->GetPlayer().get() && sObjs[k]->GetPosition() == v1)
			{
				r.m_otherObject = sObjs[k].get();
				r.m_location = sObjs[k]->GetPosition();

				// Trigger overlap statements
				if (sObjs[k]->ShouldOverlap() && !sObjs[k]->IsOverlapping())
				{
					r.m_result = ECollisionResult::OVERLAP;
				}

				// Check if the GameObject is in the way and isn't us + collision flag
				if (sObjs[k]->ShouldCollide())
				{
					r.m_result = ECollisionResult::COLLIDED;
				}

				return r;
			}
			else if (sObjs[k]->ShouldOverlap() && sObjs[k]->IsOverlapping())
			{
				sObjs[k]->OnOverlapEnd();
			}
		}

		// If so - we can grab the current level and its Layers
		std::vector<ILayer*> objs = *CBaseGame::Instance()->GetLoadedLevel()->GetLayers();

		for (size_t i = 0; i < objs.size(); i++)
		{
			// Only care about the ObjectLayers - since that's what we are going to collide with.
			CObjectLayer* obl = dynamic_cast<CObjectLayer*>(objs[i]);
			if (!obl) continue;
			
			std::vector<std::shared_ptr<IGameObject>> cObjs = *obl->GetGameObjects();

			for (size_t j = 0; j < cObjs.size(); j++)
			{
				// Check if the GameObject is in the way and isn't us + collision flag
				if (cObjs[j].get() != CBaseGame::Instance()->GetPlayer().get() && cObjs[j]->GetPosition() == v1)
				{
					r.m_otherObject = cObjs[j].get();
					r.m_location = cObjs[j]->GetPosition();

					// Trigger overlap statements
					if (cObjs[j]->ShouldOverlap() && !cObjs[j]->IsOverlapping())
					{
						r.m_result = ECollisionResult::OVERLAP;
					}

					if (cObjs[j]->ShouldCollide())
					{
						r.m_result = ECollisionResult::COLLIDED;
					}

					return r;
				}
				//else if (cObjs[j]->ShouldOverlap() && cObjs[j]->IsOverlapping())
				//{
				//	cObjs[j]->OnOverlapEnd();
				//}
			}
		}
	}

	return r;
}

void CPlayState::s_PlayCallback(SStreamingAudioData* as)
{
	spdlog::info("Playing: \"{}\".", as->Filename.c_str());
}

void CPlayState::s_PauseCallback(SStreamingAudioData* as)
{
	spdlog::info("Paused: \"{}\".", as->Filename.c_str());
}

void CPlayState::s_StopCallback(SStreamingAudioData* as)
{
	spdlog::info("Stopped: \"{}\".", as->Filename.c_str());
}