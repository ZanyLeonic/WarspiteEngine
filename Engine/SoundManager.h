#pragma once

#ifndef __SOUNDMANAGER_H__
#define __SOUNDMANAGER_H__

#include <string>

enum SoundType
{
	SOUND_MUSIC = 0,
	SOUND_SFX = 1
};

class SoundManager
{
public:
	static SoundManager* Instance()
	{
		if (s_pInstance == 0)
		{
			s_pInstance = new SoundManager();
		}

		return s_pInstance;
	}

	bool Load(std::string fileName, std::string id, SoundType type);

	void PlaySound(std::string id, int loop);
	void PlayMusic(std::string id, int loop);

private:

	static SoundManager* s_pInstance;

	SoundManager();
	~SoundManager();

	SoundManager(const SoundManager&);
	SoundManager& operator=(const SoundManager&);
};

#endif
