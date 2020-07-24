#pragma once
#ifndef __SCRIPTWRAPPERS_H__
#define __SCRIPTWRAPPERS_H__

#include "Camera.h"
#include "InputHandler.h"
#include "Level.h"

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
	T* m_inst = nullptr;
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

	IGameObject* FindGameObject(std::string id)
	{
		if (!IsValid()) return nullptr;
		
		std::vector<std::vector<IGameObject*>*> objs = m_inst->GetGameObjects();
		
		for (size_t i = 0; i < objs.size(); i++)
		{
			for (size_t j = 0; j < objs[i]->size(); j++)
			{
				for (size_t k = 0; k < objs[i][j].size(); k++)
				{
					if (objs[i][j][k]->GetName() == id) return objs[i][j][k];
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

typedef std::shared_ptr<SLevelObject>  PLevelPtr;
typedef std::shared_ptr<SCameraObject> PCameraPtr;
typedef std::shared_ptr<SInputObject>  PInputPtr;

#endif