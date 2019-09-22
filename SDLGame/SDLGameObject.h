#pragma once
#pragma once
#ifndef __SDLGameObject__
#define __SDLGameObject__
#include "GameObject.h"
class SDLGameObject :
	public GameObject
{
public:

	SDLGameObject(const ObjectParams* pParams);

	virtual void Draw();
	virtual void OnThink();
	virtual void Destroy();

protected:

	int m_x;
	int m_y;

	int m_width;
	int m_height;

	int m_currentRow;
	int m_currentFrame;

	std::string m_textureID;
};

#endif /* defined(__SDLGameObject__) */
