#pragma once
#ifndef __Game_H__
#define __Game_H__

class IGame;

class CGame
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
	bool Init(int argc, char** argv, IGame* pGame);
};

#endif