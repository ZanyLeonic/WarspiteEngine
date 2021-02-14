#pragma once
#ifndef __Player__
#define __Player__

#include "WarspiteObject.h"
#include "InputHandler.h"

class CPlayer : public CWarspiteObject
{
public:
	CPlayer();

	void OnPlay();

	void Load(const CObjectParams* pParams);

	bool OnThink();
	void Draw();

	void AddOverlappedObject(IGameObject* object);
	void RemoveOverlappedObject(IGameObject* object);

	std::vector<IGameObject*> GetOverlappedObjects() { return m_currentOverlapped; }

	//void KeyDown(SDL_Scancode e);
	//void KeyUp(SDL_Scancode e);

private:
	void HandleInput();

	void MoveForward(float axis);
	void MoveRight(float axis);
	
	bool WillCollide(CVector2D* pNext);

	void DecideFrame();

	CVector2D lastPosition;
	CVector2D nextPosition;

	bool moving = false;

	bool moveUp = false;
	bool moveRight = false;

	bool m_stepLastFrame = false;

	float m_timeLeft = 100;
	const int m_moveStep = 32;

	int m_frameOffset = 0;

	CVector2D m_CamOffset;
	std::vector<IGameObject*> m_currentOverlapped;
};

REG_OBJ_TO_REF( Player, CPlayer ) ;
#endif /* defined(__Player__) */