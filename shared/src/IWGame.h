#pragma once
#ifndef __IWGAME_H_
#define __IWGAME_H_

#include "EngineTypes.h"

struct SDL_Renderer;
struct SDL_Window;

class CGameStateManager;
class CVector2D;
class IGameObject;

class IWGame
{
public:
    virtual void Draw() = 0;
    virtual void OnThink() = 0;
    virtual void HandleEvents() = 0;
    virtual void Destroy() = 0;
    virtual void Quit() = 0;

    virtual bool IsRunning() = 0;

    virtual std::map<ESingletonIDs, void(*)> GetPointers() = 0;
};
#endif