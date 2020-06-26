#pragma once
#ifndef __OBJECTLAYER_H__
#define __OBJECTLAYER_H__
#include <vector>

#include "Layer.h"
#include "GameObject.h"
class ObjectLayer :
	public Layer
{
public:
	virtual void OnPlay();
	virtual void Destroy();

	virtual void OnThink();
	virtual void Draw();

	std::vector<CGameObject*>* GetGameObjects()
	{
		return &m_GameObjects;
	}
private:
	std::vector<CGameObject*> m_GameObjects;
};

#endif // ifndef __OBJECTLAYER_H__