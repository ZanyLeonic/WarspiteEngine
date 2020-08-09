#pragma once
#ifndef __SCRIPTWRAPPERS_H__
#define __SCRIPTWRAPPERS_H__

// The current APIs we are exposing
#include "Camera.h"
#include "InputHandler.h"
#include "Level.h"
#include "GameStateBase.h"
#include "GameObjectDictionary.h"
#include "GameStateDictionary.h"
#include "GameStateManager.h"
#include "TextureManager.h"
#include "WarspiteObject.h"

// To make it easier to change how these are referenced throughout the codebase
// You can reference these objects in Python by engine module followed by any of the below.
// (e.g. engine.level)
#define LEVELOBJECT_NAME	"level"
#define CAMERAOBJECT_NAME	"camera"
#define INPUTOBJECT_NAME	"inputh"
#define GAMEOBJECT_NAME		"game"

// Create a base set of methods to prevent the repeat of code
template<class T>
struct SBaseWrapper
{
	SBaseWrapper(T* pClass)
	{
		m_inst = pClass;
	}
	
	bool IsValid() const
	{
		return m_inst != nullptr;
	}
	
protected:
	T* m_inst = nullptr; // Each of these will be containing a pointer of something.
};

struct SLevelObject : SBaseWrapper<CLevel>
{
	SLevelObject(CLevel* pClass) : SBaseWrapper<CLevel>(pClass) {}
	
	std::string GetName() const
	{
		if (!IsValid()) return "";

		return m_inst->GetName();
	}

	CVector2D GetLevelSize() const
	{
		if (!IsValid()) return CVector2D(0,0);
		
		return m_inst->m_LevelSize;
	}

	template<class T>
	std::shared_ptr<T> FindGameObject(std::string id)
	{
		if (!IsValid()) return nullptr;

		std::vector<std::vector<IGameObject*>*> m_objects;

		// Go through each ObjectLayer we got earlier
		for (size_t i = 0; i < m_objects.size(); i++)
		{
			if (!m_objects[i]) continue;

			// Get an rvalue of the list of GameObject's for the iterated layer
			std::vector<IGameObject*>& ir = *m_objects[i];

			for (size_t j = 0; j < ir.size(); j++)
			{
				if (ir[j]->GetName() == id)
				{
					auto pCWO = std::unique_ptr<T>(dynamic_cast<T*>(ir[j]));

					return pCWO;
				}
			}
		}

		return nullptr;
	}
	
	std::vector<std::vector<IGameObject*>*> GetGameObjects() const
	{
		if (!IsValid()) return std::vector<std::vector<IGameObject*>*>();
		
		return m_inst->GetGameObjects();
	}
};

struct SCameraObject : SBaseWrapper<CCamera>
{
	SCameraObject(CCamera* pClass) : SBaseWrapper<CCamera>(pClass) {}

	CVector2D GetPosition() const
	{
		if (!IsValid()) return CVector2D(0, 0);
		
		return m_inst->GetPosition();
	}

	CVector2D GetOffsetedPosition() const
	{
		if (!IsValid()) return CVector2D(0, 0);
		
		return m_inst->GetPositionT();
	}

	CVector2D* GetTarget() const
	{
		if (!IsValid()) return nullptr;

		return m_inst->GetTarget();
	}

	bool SetPosition(CVector2D pPos) const
	{
		if (!IsValid()) return false;

		m_inst->SetPosition(pPos);
		return true;
	}

	bool SetTarget(CVector2D* pTar) const
	{
		if (!IsValid()) return false;

		m_inst->SetTarget(pTar);
		return true;
	}

	CVector2D GetLevelSize() const
	{
		if (!IsValid()) return CVector2D();

		return m_inst->GetLevelSize();
	}
};

struct SInputObject : SBaseWrapper<CInputHandler>
{
	SInputObject(CInputHandler* pClass) : SBaseWrapper<CInputHandler>(pClass) {}

	bool GetButtonState(int joy, int buttonNumber) const
	{
		if (!IsValid()) return false;
		return m_inst->GetButtonState(joy, buttonNumber);
	}

	bool GetMouseButtonState(int buttonNumber) const
	{
		if (!IsValid()) return false;
		return m_inst->GetMouseButtonState(buttonNumber);
	}

	bool IsKeyDown(SDL_Scancode key) const
	{
		if (!IsValid()) return false;
		return m_inst->IsKeyDown(key);
	}

	float GetXAxis(int joy, int stick) const
	{
		if (!IsValid()) return 0.f;
		return m_inst->GetXAxis(joy, stick);
	}

	float GetYAxis(int joy, int stick) const
	{
		if (!IsValid()) return 0.f;
		return m_inst->GetYAxis(joy, stick);
	}

	bool SetReleaseState(SDL_Scancode key, bool state) const
	{
		if (!IsValid()) return false;
		m_inst->SetReleaseState(key, state);
		return true;
	}

	bool AddActionKeyDown(SDL_Scancode key, const KeyCallback callBack) const
	{
		if (!IsValid()) return false;
		m_inst->AddActionKeyDown(key, callBack);
		return true;
	}

	bool AddActionKeyUp(SDL_Scancode key, const KeyCallback callBack) const
	{
		if (!IsValid()) return false;
		m_inst->AddActionKeyUp(key, callBack);
		return true;
	}
};

// This maybe moved from this codebase to the game codebase soon.
// Internal APIs in the codebase need to be updated for further work!
struct SGameObject : SBaseWrapper<CGame>
{
	SGameObject(CGame* pClass) : SBaseWrapper<CGame>(pClass) {}

	std::shared_ptr<CGameStateBase> GetCurrentState() const
	{
		if (!IsValid()) return nullptr;
		return std::shared_ptr<CGameStateBase>(m_inst->GetStateManager()->GetCurrentState());
	}

	bool ChangeState(std::string stateID)
	{
		if (m_inst->GetStateManager()->GetCurrentState()->GetStateID() != stateID)
		{
			m_inst->GetStateManager()->ModifyState(CGameStateDictionary::Instance()->Create(stateID));
		}
		return false;
	}

	template<class T>
	std::shared_ptr<T> GetPlayer() const
	{
		if (m_inst->GetPlayer() == nullptr) return nullptr;
		return dynamic_cast<T*>(m_inst->GetPlayer());
	}

	// These next one doesn't really use CGame, but it was the only place I thought it would fit
	// May split it off later.
	std::shared_ptr<CWarspiteObject> CreateObject(std::string factID, CObjectParams params) const
	{
		// Create the object and load the params provided.
		IGameObject* pObj = CGameObjectDictionary::Instance()->Create(factID);
		if (pObj == nullptr) return nullptr; // a bit redundant but it prevents a reference of a nullptr
		
		pObj->Load(&params);

		// lul will this work? nope.
		return std::shared_ptr<CWarspiteObject>(dynamic_cast<CWarspiteObject*>(pObj));
	}

	bool LoadTexture(std::string texPath, std::string texID) const
	{
		if (!IsValid()) return false;
		return CTextureManager::Instance()->Load(texPath, texID, m_inst->GetRenderer());
	}
};
#endif