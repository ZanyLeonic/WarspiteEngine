#include "AudioSource.h"
#include "EngineFileSystem.h"
#include "WarspiteUtil.h"
#include <spdlog/spdlog.h>

CAudioSource::CAudioSource()
{
	m_soundType = ESoundType::SOUND_NONE;
	m_waveFile = new SWaveFile();
	m_streamData = new SStreamingAudioData();
}

CAudioSource::~CAudioSource()
{
	Destroy();
}

void CAudioSource::Load(const CObjectParams* pParams)
{
	CWarspiteObject::Load(pParams);

	std::string ext = CWarspiteUtil::GetFileExtenstion(pParams->GetSoundPath());

	if (ext == ".wav")
	{
		m_soundType = ESoundType::SOUND_SFX;
		CSoundManager::Instance()->Load(CEngineFileSystem::ResolvePath(pParams->GetSoundPath(), 
			CEngineFileSystem::EPathType::SOUND), *m_waveFile);
	}
	else if (ext == ".ogg")
	{
		m_soundType = ESoundType::SOUND_MUSIC;
		CSoundManager::Instance()->CreateStreamFromFile(CEngineFileSystem::ResolvePath(pParams->GetSoundPath(), 
			CEngineFileSystem::EPathType::SOUND), *m_streamData);
	}
	else
	{
		spdlog::warn("Extension \"{}\" is not recongised.  AudioSource \"{}\" will be silent!", ext, pParams->GetName());
	}
}

void CAudioSource::OnPlay()
{
	Play();
}

void CAudioSource::Destroy()
{
	CWarspiteObject::Destroy();

	switch (m_soundType)
	{
	case ESoundType::SOUND_SFX:
		CSoundManager::Instance()->DeleteSound(m_waveFile);
		break;
	case ESoundType::SOUND_MUSIC:
		CSoundManager::Instance()->DeleteStream(m_streamData);
		break;
	}

	delete m_waveFile;
	delete m_streamData;
}

void CAudioSource::Play()
{
	switch (m_soundType)
	{
	case ESoundType::SOUND_SFX:
		if (m_waveFile)
		{
			CSoundManager::Instance()->PlaySound(m_waveFile);
		}
		break;
	case ESoundType::SOUND_MUSIC:
		if (m_streamData)
		{
			CSoundManager::Instance()->PlayStream(m_streamData);
		}
		break;
	}
}

void CAudioSource::Pause()
{
	switch (m_soundType)
	{
	case ESoundType::SOUND_SFX:
		spdlog::warn("[{}] Pausing is not supported for sound effects", m_objectName);
		break;
	case ESoundType::SOUND_MUSIC:
		if (m_streamData)
		{
			CSoundManager::Instance()->PauseStream(m_streamData);
		}
		break;
	}
}

void CAudioSource::Stop()
{
	switch (m_soundType)
	{
	case ESoundType::SOUND_SFX:
		if (m_waveFile)
		{
			CSoundManager::Instance()->StopSound(m_waveFile);
		}
		break;
	case ESoundType::SOUND_MUSIC:
		if (m_streamData)
		{
			CSoundManager::Instance()->StopStream(m_streamData);
		}
		break;
	}
}
