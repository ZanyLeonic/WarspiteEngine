#pragma once
#ifndef __AUDIOSOURCE_H__
#define __AUDIOSOURCE_H__

#include "MapObject.h"
#include "SoundManager.h"

class CAudioSource :
	public CMapObject
{
public:
	CAudioSource();
	~CAudioSource();

	void Load(const CObjectParams* pParams) override;
	void OnPlay() override;
	void Destroy() override;



	void Play();
	void Pause();
	void Stop();

private:
	ESoundType m_soundType = ESoundType::SOUND_NONE;
	SWaveFile* m_waveFile = nullptr;
	SStreamingAudioData* m_streamData = nullptr;
};
REG_OBJ_TO_REF(AudioSource, CAudioSource);
#endif // __AUDIOSOURCE_H__