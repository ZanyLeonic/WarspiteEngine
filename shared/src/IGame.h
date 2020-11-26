#pragma once
#ifndef __IGAME_H_
#define __IGAME_H_

#include "EngineTypes.h"

struct SDL_Renderer;
struct SDL_Window;

class CGameStateManager;
class CVector2D;
class IGameObject;

class IGame
{
public:
    virtual bool Init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen, int argc, char** argv, GameDLL_t pGameDLL) = 0;

    virtual void Draw() = 0;
    virtual void OnThink() = 0;
    virtual void HandleEvents() = 0;
    virtual void Destroy() = 0;
    virtual void Quit() = 0;

    virtual bool IsRunning() = 0;

    virtual SDL_Renderer* const GetRenderer() = 0;
    virtual SDL_Window* const GetWindow() = 0;
    virtual CGameStateManager* const GetStateManager() = 0;

    virtual CVector2D const GetViewportSize() = 0;

    virtual void SetPlayer(IGameObject* pNO) = 0;
    virtual IGameObject* const GetPlayer() = 0;

    virtual int const GetArgc() = 0;
    virtual char** const GetArgv() = 0;

    virtual IWGame* const GetGameDLLClass() = 0;

    virtual bool const StartedWithMapParam() = 0;
	virtual std::string const GetMapParamName() = 0;
};
#endif