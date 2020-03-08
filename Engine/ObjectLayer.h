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
	virtual void OnThink();
	virtual void Draw();

	std::vector<GameObject*>* GetGameObjects()
	{
		return &m_GameObjects;
	}
private:
	std::vector<GameObject*> m_GameObjects;
};

#endif // ifndef __OBJECTLAYER_H__