#pragma once
#ifndef __Player__
#define __Player__

#include "WarspiteObject.h"
#include "InputHandler.h"
#include "EngineTypes.h"

class CPlayer;

typedef std::function<void(CPlayer*)> HPlayerCallback;

class CPlayer : public CWarspiteObject
{

public:
	CPlayer();

	void OnPlay();

	void Load(CObjectParams* pParams);

	bool OnThink();
	void Draw();

	void SetNextLocation(CVector2D nextLocation, float pos, bool callCallbacks=false);
	void SetDirection(EDirection direction) { m_ePlayerDirection = direction; }

	void DisablePlayerInput(bool newState) { m_bDisablePlayerInput = newState; }
	bool IsPlayerInputDisabled() const { return m_bDisablePlayerInput; }

	void AddMovementStartCallback(std::string id, HPlayerCallback call);
	void RemoveMovementStartCallback(std::string id);

	void AddMovementEndCallback(std::string id, HPlayerCallback call);
	void RemoveMovementEndCallback(std::string id);

	bool IsMoving() const { return m_bMoving; }

private:
	void HandleInput();
	void InteractAction(SDL_Scancode key);

	void MoveForward(float axis);
	void MoveRight(float axis);

	void HandleMovement(CVector2D* pNext);
	
	bool WillCollide(CVector2D* pNext);

	void DecideFrame();

	int GetRowFromDirection();

	void callStartCallbacks();
	void callEndCallbacks();

	CVector2D m_vLastPosition;
	CVector2D m_vNextPosition;

	bool m_bMoving = false;

	bool m_bMoveUp = false;
	bool m_bMoveRight = false;

	bool m_bStepLastFrame = false;
	bool m_bDisablePlayerInput = false;

	float m_fTimeLeft = 100;
	const int m_iMoveStep = 32;

	int m_iFrameOffset = 0;

	std::map<std::string, HPlayerCallback> m_vOnMoveStart;
	std::map<std::string, HPlayerCallback> m_vOnMoveEnd;

	CVector2D m_vCamOffset;
	SCollisionData m_sLastCollision;
};

REG_OBJ_TO_REF( Player, CPlayer ) ;
#endif /* defined(__Player__) */