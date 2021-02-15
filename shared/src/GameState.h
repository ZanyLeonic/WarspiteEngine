#pragma once
#ifndef __GAMESTATE_H__
#define __GAMESTATE_H__

#include <string>
#include <vector>
#include <memory>
#include "GameObject.h"
#include "EngineTypes.h"

class IGameObject;
class CVector2D;

class CGameState
{
public:
	virtual void OnThink() = 0;
	virtual void Draw() = 0;
	
	virtual bool OnPlay() = 0;
	virtual bool OnEnd() = 0;

	virtual std::string GetStateID() const = 0;

	// virtual std::vector<std::unique_ptr<IGameObject>> GetGameObjects() const = 0;
	virtual std::vector<std::string> GetLoadedTextures() const = 0;
	virtual std::vector<std::string> GetLoadedScripts() const = 0;

	virtual SCollisionData IsColliding(CVector2D v1) = 0;

	virtual bool ShouldBeTicking() const = 0;
	virtual bool ShouldBeDrawing() const = 0;

	virtual bool StateFinishedTicking() const = 0;
	virtual bool StateFinishedDrawing() const = 0;
};
#endif