#pragma once
#pragma once
#ifndef __CWarspiteObject__
#define __CWarspiteObject__

#include "EngineTypes.h"
#include "GameObject.h"
#include "GameObjectDictionary.h"
#include "ITextureManager.h"
#include "IGameObjectFactory.h"
#include "ObjectParams.h"
#include "Vector2D.h"
#include <memory>

class CWarspiteObject :
	public IGameObject, std::enable_shared_from_this<CWarspiteObject>
{
public:
	CWarspiteObject();

	void OnPlay() override;
	void Draw() override;
	bool OnThink() override;
	void Destroy() override;

	void Load(CObjectParams* pParams) override;

	CVector2D GetPosition() override { return m_vPosition; }
	void SetPosition(CVector2D& nPos) override { m_vPosition = nPos; }

	virtual CVector2D GetVelocity() { return m_vVelocity; }
	virtual void SetVelocity(CVector2D nV) { m_vVelocity = nV; }
	virtual void AddVelocity(CVector2D aV) { m_vVelocity += aV; }

	virtual CVector2D GetAcceleration() { return m_vAcceleration; }
	virtual void SetAcceleration(CVector2D nA) { m_vAcceleration = nA; }
	virtual void AddAcceleration(CVector2D nA) { m_vAcceleration += nA; }

	virtual CVector2D GetSize() { return CVector2D((float)m_iWidth, (float)m_iHeight); }

	virtual int GetCurrentAnimRow() { return m_iCurrentRow; }
	virtual void SetAnimRow(int nR) { m_iCurrentRow = nR; }
	
	virtual int GetCurrentAnimFrame() { return m_iCurrentFrame; }
	virtual void SetAnimFrame(int aF) { m_iCurrentFrame = aF; }
	
	virtual int GetTotalAnimFrames() { return m_iNumFrames; }

	const char* GetName() override { return m_sObjectName.c_str(); }
	virtual std::string GetTextureID() { return m_sTextureID; }
	virtual std::string GetFactoryID() { return m_sFactoryID; }

	bool ShouldCollide() override { return m_bCollides; }
	void SetCollision(bool nC) { m_bCollides = nC; }

	bool ShouldOverlap() override { return m_bOverlap; }
	void SetOverlap(bool nO) { m_bOverlap = nO; }

	bool IsOverlapping() override { return m_bIsOverlapping; }
	bool CancelMovementOnOverlap() override { return m_bCancelMovementOnOverlap; }

	void OnOverlapStart();
	void OnOverlapEnd();

	void SetObjectDirection(EDirection pDir) { m_eObjectDirection = pDir; }
	EDirection GetObjectDirection() { return m_eObjectDirection; };

	void SetCollisionOnChannel(ECollisionChannel pChannel, bool pState);
	bool CollidesOnChannel(ECollisionChannel pChannel);

	bool InteractAction(IGameObject* pOther);
	
protected:
	CVector2D m_vPosition;
	CVector2D m_vVelocity;
	CVector2D m_vAcceleration;
	
	int m_iWidth;
	int m_iHeight;

	int m_iCurrentRow;
	int m_iCurrentFrame;
	int m_iNumFrames;

	std::string m_sObjectName = "";
	std::string m_sFactoryID = "";
	std::string m_sTextureID = "";

	bool m_bCollides = false;
	bool m_bOverlap = false;

	bool m_bIsOverlapping = false;
	bool m_bCancelMovementOnOverlap = false;

	EDirection m_eObjectDirection = EDirection::NONE;

	std::vector<bool> m_aCollisionChannels;

#ifdef _GAME_
	IGame* pGame = nullptr;
	ITextureManager* pTex = nullptr;
#endif
};
#endif /* defined(__CWarspiteObject__) */