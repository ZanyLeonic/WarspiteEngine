#pragma once
#pragma once
#ifndef __CWarspiteObject__
#define __CWarspiteObject__

#include "GameObject.h"
#include "GameObjectDictionary.h" // Include used for classes that derive it! Do not remove!!!
#include "ObjectParams.h"
#include "Vector2D.h"

class CWarspiteObject :
	public IGameObject
{
public:
	CWarspiteObject();

	void OnPlay() override;
	void Draw() override;
	bool OnThink() override;
	void Destroy() override;

	void Load(const CObjectParams* pParams) override;

	CVector2D GetPosition() override
	{
		return m_position;
	}

	virtual CVector2D GetVelocity()
	{
		return m_velocity;
	}

	virtual CVector2D GetAcceleration()
	{
		return m_acceleration;
	}

	virtual CVector2D GetSize()
	{
		return CVector2D(m_width, m_height);
	}

	virtual int GetCurrentAnimRow()
	{
		return m_currentRow;
	}

	virtual int GetCurrentAnimFrame()
	{
		return m_currentFrame;
	}

	virtual int GetTotalAnimFrames()
	{
		return m_numFrames;
	}

	virtual std::string GetTextureID()
	{
		return m_textureID;
	}

	virtual std::string GetMapRefID()
	{
		return "";
	}
	
protected:
	CVector2D m_position;
	CVector2D m_velocity;
	CVector2D m_acceleration;

	int m_width;
	int m_height;

	int m_currentRow;
	int m_currentFrame;
	int m_numFrames;

	std::string m_factoryID = "";
	std::string m_textureID = "";
};

#endif /* defined(__CWarspiteObject__) */
