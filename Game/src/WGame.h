#pragma once
#ifndef __Game_H__
#define __Game_H__

#include <map>
#include "EngineTypes.h"
#include "IWGame.h"

class IGame;

class CGame : public IWGame
{
public:
	static CGame* Instance()
	{
		if (s_pInstance == 0)
		{
			s_pInstance = new CGame();
			return s_pInstance;
		}
		return s_pInstance;
	}
private:
	CGame() {};
	~CGame() {};
	
	static CGame* s_pInstance;
public:
	// simply set the running variable to true
	bool Init(int argc, char** argv, std::map<ESingletonIDs, void(*)>* pPtrs);

	virtual void Draw();
	virtual void OnThink();
	virtual void HandleEvents();
	virtual void Destroy();
	virtual void Quit();

	virtual bool IsRunning();

	virtual std::map<ESingletonIDs, void(*)> GetPointers() { return m_ptrs; }

private:
	std::map<ESingletonIDs, void(*)> m_ptrs;
	
	IGame* tObj = nullptr;
};

#endif