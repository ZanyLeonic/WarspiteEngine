#pragma once
#ifndef __OBJECTLAYER_H__
#define __OBJECTLAYER_H__
#include <vector>
#include <memory>

#include "Layer.h"
#include "GameObject.h"
class CObjectLayer :
	public ILayer
{
public:
	virtual void OnPlay();
	virtual void Destroy();

	virtual void OnThink();
	virtual void Draw();

	std::vector<std::shared_ptr<IGameObject>>* GetGameObjects()
	{
		return &m_GameObjects;
	}
private:
	std::vector<std::shared_ptr<IGameObject>> m_GameObjects;
};

#endif // ifndef __OBJECTLAYER_H__