#pragma once
#ifndef __SCRIPTWRAPPERS_H__
#define __SCRIPTWRAPPERS_H__

#include <string>
#include <vector>
#include <memory>
#include <pybind11/pybind11.h>
#include "EngineTypes.h"
#include <SDL_pixels.h>
#include "FontManager.h"

// The current APIs we are exposing
class CWarspiteObject;
class CGameStateBase;
class CVector2D;
class CBaseGame;
class CLevel;
class IGameObject;
class CObjectParams;
class CInputHandler;
class CTextureManager;
class CCamera;
class CTexture;

namespace py = pybind11;

class CScriptWrappers
{
public:
	static bool Init_Engine();
};
#ifdef _WIN32
class PyStdErrOutStreamRedirect
#elif _UNIX
class __attribute__ ((visibility("hidden"))) PyStdErrOutStreamRedirect
#endif
{
	py::object _stdout;
	py::object _stderr;
	py::object _stdout_buffer;
	py::object _stderr_buffer;

public:
	PyStdErrOutStreamRedirect() {
		auto sysm = py::module::import("sys");
		_stdout = sysm.attr("stdout");
		_stderr = sysm.attr("stderr");
		auto stringio = py::module::import("io").attr("StringIO");
		_stdout_buffer = stringio();  // Other filelike object can be used here as well, such as objects created by pybind11
		_stderr_buffer = stringio();
		sysm.attr("stdout") = _stdout_buffer;
		sysm.attr("stderr") = _stderr_buffer;
	}
	std::string stdoutString() {
		_stdout_buffer.attr("seek")(0);
		return py::str(_stdout_buffer.attr("read")());
	}
	std::string stderrString() {
		_stderr_buffer.attr("seek")(0);
		return py::str(_stderr_buffer.attr("read")());
	}
	~PyStdErrOutStreamRedirect() {
		auto sysm = py::module::import("sys");
		sysm.attr("stdout") = _stdout;
		sysm.attr("stderr") = _stderr;
	}
};

// To make it easier to change how these are referenced throughout the codebase
// You can reference these objects in Python by engine module followed by any of the below.
// (e.g. engine.level)
#define LEVELOBJECT_NAME	"level"
#define CAMERAOBJECT_NAME	"camera"
#define INPUTOBJECT_NAME	"inputh"
#define GAMEOBJECT_NAME		"game"
#define FONTOBJECT_NAME		"font"

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

	bool ShouldOverlap() const;
	void SetOverlap(bool nO) const;

	bool operator==(const SWarObject &w) const
	{
		return m_inst == w.m_inst;
	}
};

struct SWarTexture : SBaseWrapper<CTexture>
{
	SWarTexture(CTexture* pClass) : SBaseWrapper<CTexture>(pClass) {}

	bool Load(std::string path);
	void Free();

	//Set color modulation
	void SetColour(Uint8 red, Uint8 green, Uint8 blue);

	//Set blending
	void SetBlendMode(SDL_BlendMode blending);

	//Set alpha modulation
	void SetAlpha(Uint8 alpha);

	void Draw(CVector2D pos);

	//Gets image dimensions
	int GetWidth() const;
	int GetHeight() const;

	double GetAngle() const;
	SDL_Point* GetCenter() const;

	void SetAngle(double pAngle);
	void SetCenter(SDL_Point* pNewPoint);
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
	bool IsKeyDown(int key) const;
	
	float GetXAxis(int joy, int stick) const;
	float GetYAxis(int joy, int stick) const;
	
	bool SetReleaseState(int key, bool state) const;
	bool AddActionKeyDown(int key, const KeyCallback callBack) const;
	bool AddActionKeyUp(int key, const KeyCallback callBack) const;
};

struct SGameObject : SBaseWrapper<CBaseGame>
{
	SGameObject(CBaseGame* pClass) : SBaseWrapper<CBaseGame>(pClass) {}

	std::unique_ptr<SWarState> GetCurrentState() const;
	bool ChangeState(std::string stateID) const;
	std::unique_ptr<SWarObject> GetPlayer() const;
	bool LoadTexture(std::string texPath, std::string texID) const;
};

struct SFontObject : SBaseWrapper<CFontManager>
{
	SFontObject(CFontManager* pClass) : SBaseWrapper<CFontManager>(pClass) {}
	
	bool LoadFont(std::string path, std::string type, int size);
	bool RemoveFont(std::string name, std::string type, int size);

	bool RenderText(std::string text, std::string fontID, std::string textureID,
		CFontManager::EFontRenderType rType = (CFontManager::EFontRenderType)3,
		SDL_Colour tColour = { 0,0,0 }, SDL_Colour bColour = { 255,255,255 });

	bool IsLoaded(std::string name, std::string type, int size);
};
#endif