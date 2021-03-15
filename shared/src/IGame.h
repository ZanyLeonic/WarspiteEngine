#pragma once
#ifndef __IGAME_H_
#define __IGAME_H_

#include "EngineTypes.h"
#include "CallbackHandler.h"

struct SDL_Renderer;
struct SDL_Window;

class CGameStateManager;
class CGarbageCollector;
class CFadeObject;
class CVector2D;
class IGameObject;
class CLevel;

class IGame
{
public:
    virtual bool Init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen, int argc, char** argv, GameDLL_t pGameDLL) = 0;

    virtual void Draw() = 0;
    virtual void OnThink() = 0;
    virtual void HandleEvents() = 0;
    virtual void Destroy() = 0;
    virtual void Quit() = 0;

    virtual void FadeIn() = 0;
    virtual void FadeOut() = 0;

    virtual bool IsRunning() = 0;

    virtual SDL_Renderer* const GetRenderer() = 0;
    virtual SDL_Window* const GetWindow() = 0;
    virtual CGameStateManager* const GetStateManager() = 0;
    virtual CGarbageCollector* const GetGarbageCollector() = 0;
    virtual CCallbackHandler<HGenericCallback>* const GetCallbackHandler() = 0;

    virtual CFadeObject* GetFadeObject() = 0;

    virtual CVector2D GetViewportSize() = 0;

    virtual void SetPlayer(std::shared_ptr<IGameObject> pNO) = 0;
    virtual std::shared_ptr<IGameObject> const GetPlayer() = 0;

    virtual void SetLoadedLevel(CLevel* level) = 0;
    virtual CLevel* GetLoadedLevel() = 0;

    virtual int const GetArgc() = 0;
    virtual char** const GetArgv() = 0;

    virtual IWGame* const GetGameDLLClass() = 0;

    virtual bool const StartedWithMapParam() = 0;
	virtual std::string const GetMapParamName() = 0;

    virtual bool const ShowDebugTiles() = 0;
    virtual void SetDebugTilesVisibility(bool nVal) = 0;
};
#endif