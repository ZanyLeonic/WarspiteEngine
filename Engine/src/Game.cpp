#include "Game.h"
#include "TextureManager.h"
#include "SoundManager.h"
#include "ScriptManager.h"
#include "GameStateDictionary.h"
#include "Camera.h"
#include "InputHandler.h"
#include "LevelParser.h"
#include <iostream>
#include "ScriptWrappers.h"
#include "spdlog/spdlog.h"
#include "FPSCounter.h"
#include "FadeObject.h"
#include "IGame.h"

CBaseGame* CBaseGame::s_pInstance = 0;

IGame* CBaseGame::Instance()
{
	if (s_pInstance == 0)
	{
		s_pInstance = new CBaseGame();
		return s_pInstance;
	}
	return s_pInstance;
}

// Initialises the major parts of the engine
bool CBaseGame::Init(const char* title, int xpos, int ypos, int width, int height, 
	bool fullscreen, int argc, char** argv, GameDLL_t pGameDLL)
{
	// Init our FPS stuff
	// Set all frame times to 0ms.
	memset(m_frametimes, 0, sizeof(m_frametimes));

	m_bShowDebugTiles = false;
	m_frameCount = 0;
	m_fFPS = 0;
	m_lastFrametime = SDL_GetTicks();

	m_pFPSCounter = new CFPSCounter();
	m_pCallbackHandler = new CCallbackHandler<HGenericCallback>();

	CInputHandler::Instance()->InitialiseJoysticks();

	// attempt to initalize SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{
		int flags = 0;

		if (fullscreen)
		{
			flags = SDL_WINDOW_FULLSCREEN;
		}

		spdlog::info("SDL initalisation success");
		// init the window
		m_pWindow = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
		m_viewportSize = CVector2D((float)width, (float)height);

		if (m_pWindow != 0)
		{
			spdlog::info("Window creation success");
			m_pRenderer = SDL_CreateRenderer(m_pWindow, -1, SDL_RENDERER_TARGETTEXTURE | SDL_RENDERER_ACCELERATED);

			if (m_pRenderer != 0) // render init success
			{
				spdlog::info("Renderer creation success");
				SDL_SetRenderDrawColor(m_pRenderer,
					255, 255, 255, 255);

				// Initialise the SoundManager
				CSoundManager::Instance();

				// Assign the Game attribute with a game object
				if (CScriptManager::Instance()->GetEngineModule().attr(GAMEOBJECT_NAME).is_none())
				{
					m_gamePtr = std::make_shared<SGameObject>(SGameObject(this));
					CScriptManager::Instance()->GetEngineModule().attr(GAMEOBJECT_NAME) = m_gamePtr;
				}

				m_pGameStateManager = new CGameStateManager();
				m_pGameStateManager->ModifyState(std::move(CGameStateDictionary::Instance()->Create(SID_MM)));
				
				m_pGarbageCollector = new CGarbageCollector();

				m_argc = argc;
				m_argv = argv;
			}
			else
			{
				spdlog::error("Render initalisation fail");
				return false; // renderer init fail
			}
		}
		else
		{
			spdlog::error("SDL initalisation fail");
			return false; // SDL init fail
		}

		spdlog::info("Initalisation success");
		m_bRunning = true; // everything inited successfully, start the main loop

		// Allow the GameDLL to access the pointers of our Singletons
		// It is a nasty implementation, but I am not entirely sure how to do otherwise.
		std::map<ESingletonIDs, void(*)> m_singletonPtrs
		{
			{ ESingletonIDs::GAME,				(void(*))this},
			{ ESingletonIDs::OBJDICT,			(void(*))CGameObjectDictionary::Instance()},
			{ ESingletonIDs::SCRIPTMGR,			(void(*))CScriptManager::Instance()},
			{ ESingletonIDs::SOUNDMGR,			(void(*))CSoundManager::Instance()},
			{ ESingletonIDs::STATEMGR,			(void(*))GetStateManager()},
			{ ESingletonIDs::STATEDICT,			(void(*))CGameStateDictionary::Instance()},
			{ ESingletonIDs::OBJDICT,			(void(*))CGameObjectDictionary::Instance()},
			{ ESingletonIDs::CAMERA,			(void(*))CCamera::Instance()},
			{ ESingletonIDs::TEXTUREMANAGER,    (void(*))CTextureManager::Instance()}
		};

		// Create the FadeObject and ready it
		CTextureManager::Instance()->CreateEngineTextures(CVector2D((float)width, (float)height), m_pRenderer);
		m_pFadeObject = new CFadeObject();

		SDL_SetRenderDrawColor(m_pRenderer,
			255, 255, 255, 255);

		// Run the GameDLL's init
		m_pGame = pGameDLL(m_argc, m_argv, &m_singletonPtrs);

		// Try and get the map specified in the command line
		char mapName[100];

		if (CWarspiteUtil::GetParam(argv, argc, "-map", mapName))
		{
			// If it exists, try loading the map specified.
			m_bStartedWithMapParam = true;
			m_sMapName = std::string(mapName);

			m_pGameStateManager->ModifyState(std::move(CGameStateDictionary::Instance()->Create(SID_PLAY)));

			m_bStartedWithMapParam = false;
		}

		if (m_pGame != nullptr) return true;
	}
	return false;
}

void CBaseGame::FPS_Calc()
{
	Uint32 frametimesindex;
	Uint32 getticks;
	Uint32 count;
	Uint32 i;

	// frametimesindex is the position in the array. It ranges from 0 to FRAME_VALUES.
	// This value rotates back to 0 after it hits FRAME_VALUES.
	frametimesindex = m_frameCount % FRAME_SAMPLES;

	// store the current time
	getticks = SDL_GetTicks();

	// save the frame time value
	m_frametimes[frametimesindex] = getticks - m_lastFrametime;

	// save the last frame time for the next fpsthink
	m_lastFrametime = getticks;

	// increment the frame count
	m_frameCount++;

	// Work out the current framerate

	// The code below could be moved into another function if you don't need the value every frame.

	// I've included a test to see if the whole array has been written to or not. This will stop
	// strange values on the first few (FRAME_VALUES) frames.
	if (m_frameCount < FRAME_SAMPLES) {
		count = m_frameCount;
	}
	else {
		count = FRAME_SAMPLES;
	}

	// add up all the values and divide to get the average frame time.
	m_fFPS = 0;
	for (i = 0; i < count; i++) {
		m_fFPS += m_frametimes[i];
	}

	m_fFPS /= count;

	// now to make it an actual frames per second value...
	m_fFPS = 1000.f / m_fFPS;
}

int CBaseGame::LoadGameDLL()
{
	return 0;
}

void CBaseGame::Draw()
{
	SDL_RenderClear(m_pRenderer); // clear the renderer to draw color
	
	// Call the current GameState functionality via the GameStateManager.
	m_pGameStateManager->Draw();
	if (m_pPlayer != 0) m_pPlayer->Draw();

	m_pFadeObject->Draw();

	m_pFPSCounter->Draw();

	SDL_RenderPresent(m_pRenderer); // draw to the screen
}

void CBaseGame::OnThink()
{
	// FPS counter stuff
	FPS_Calc();
	m_pFPSCounter->SetFPSValue(m_fFPS);
	m_pFadeObject->OnThink();

	// Call the current GameState functionality via the GameStateManager.
	m_pGameStateManager->OnThink();
	m_pGarbageCollector->OnThink();

	if (m_pPlayer != 0) m_pPlayer->OnThink();
	
	CCamera::Instance()->OnThink();
}

void CBaseGame::HandleEvents()
{
	CInputHandler::Instance()->OnThink();
}

void CBaseGame::Destroy()
{
	spdlog::info("Cleaning Game instance...");

	CInputHandler::Instance()->Destroy();
	CSoundManager::Instance()->Destroy();
	m_pGameStateManager->PopState();

	// Destroy the Renderer before the window
	SDL_DestroyRenderer(m_pRenderer);
	SDL_DestroyWindow(m_pWindow);

	// Clean up any left stuff before closing completely.
	m_pGarbageCollector->Destroy();

	SDL_Quit();
}

void CBaseGame::Quit()
{
	m_bRunning = false;
}

void CBaseGame::FadeIn()
{
	if (m_pFadeObject) m_pFadeObject->FadeIn();
}

void CBaseGame::FadeOut()
{
	if (m_pFadeObject) m_pFadeObject->FadeOut();
}