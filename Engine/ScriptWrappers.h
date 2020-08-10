#pragma once
#ifndef __SCRIPTWRAPPERS_H__
#define __SCRIPTWRAPPERS_H__

#include <string>
#include <vector>
#include <memory>
#include <SDL_scancode.h>
#include <pybind11/pybind11.h>
#include "EngineTypes.h"

// The current APIs we are exposing
class CWarspiteObject;
class CGameStateBase;
class CVector2D;
class CGame;
class CLevel;
class IGameObject;
class CObjectParams;
class CInputHandler;
class CCamera;

namespace py = pybind11;

class CScriptWrappers
{
public:
	static bool Init_Interpreter(py::module* pMM, py::module* pEM, py::object* pMN);
};

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

struct SWarObject : SBaseWrapper<CWarspiteObject>
{
	SWarObject(CWarspiteObject* pClass) : SBaseWrapper<CWarspiteObject>(pClass) {}

	CVector2D GetPosition() const;
	void SetPosition(CVector2D& nPos) const;

	CVector2D GetVelocity() const;
	void SetVelocity(CVector2D nV) const;
	void AddVelocity(CVector2D aV) const;

	CVector2D GetAcceleration() const;
	void SetAcceleration(CVector2D nV) const;
	void AddAcceleration(CVector2D aV) const;

	CVector2D GetSize() const;

	int GetCurrentAnimRow() const;
	void SetAnimRow(int nR) const;

	int GetCurrentAnimFrame() const;
	void SetAnimFrame(int nR) const;

	int GetTotalAnimFrames() const;

	const char* GetName() const;
	std::string GetTextureID() const;
	std::string GetFactoryID() const;

	bool ShouldCollide() const;
	void SetCollision(bool nC) const;

	bool operator==(const SWarObject &w) const
	{
		return m_inst == w.m_inst;
	}
};

struct SWarState : SBaseWrapper<CGameStateBase>
{
	SWarState(CGameStateBase* pClass) : SBaseWrapper<CGameStateBase>(pClass) {}

	std::vector<std::string> GetLoadedTextures() const;
	std::vector<std::string> GetLoadedScripts() const;

	bool ShouldBeTicking() const;
	bool ShouldBeDrawing() const;
};

struct SLevelObject : SBaseWrapper<CLevel>
{
	SLevelObject(CLevel* pClass) : SBaseWrapper<CLevel>(pClass) {  }

	std::string GetName() const;
	CVector2D GetLevelSize() const;
	std::unique_ptr<SWarObject> CreateObject(std::string factID, CObjectParams params) const;
	std::unique_ptr<SWarObject> FindGameObject(std::string id);
	std::vector<std::vector<IGameObject*>*> GetGameObjects() const;
};

struct SCameraObject : SBaseWrapper<CCamera>
{
	SCameraObject(CCamera* pClass) : SBaseWrapper<CCamera>(pClass) {}

	CVector2D GetPosition() const;
	CVector2D GetOffsetedPosition() const;
	CVector2D* GetTarget() const;
	
	bool SetPosition(CVector2D pPos) const;
	bool SetTarget(CVector2D* pTar) const;
	
	CVector2D GetLevelSize() const;
};

struct SInputObject : SBaseWrapper<CInputHandler>
{
	SInputObject(CInputHandler* pClass) : SBaseWrapper<CInputHandler>(pClass) {}

	bool GetButtonState(int joy, int buttonNumber) const;
	bool GetMouseButtonState(int buttonNumber) const;
	bool IsKeyDown(SDL_Scancode key) const;
	
	float GetXAxis(int joy, int stick) const;
	float GetYAxis(int joy, int stick) const;
	
	bool SetReleaseState(SDL_Scancode key, bool state) const;
	bool AddActionKeyDown(SDL_Scancode key, const KeyCallback callBack) const;
	bool AddActionKeyUp(SDL_Scancode key, const KeyCallback callBack) const;
};

struct SGameObject : SBaseWrapper<CGame>
{
	SGameObject(CGame* pClass) : SBaseWrapper<CGame>(pClass) {}

	std::unique_ptr<SWarState> GetCurrentState() const;
	bool ChangeState(std::string stateID) const;
	std::unique_ptr<SWarObject> GetPlayer() const;
	bool LoadTexture(std::string texPath, std::string texID) const;
};

// Small container for class def in pybind
struct SKeyScancodes
{};

#endif