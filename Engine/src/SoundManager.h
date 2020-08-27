#pragma once

#ifndef __SOUNDMANAGER_H__
#define __SOUNDMANAGER_H__

#include <AL/al.h>
#include <AL/alc.h>
#include <SDL2/SDL_thread.h>
#include <vorbis/vorbisfile.h>
#include <string>
#include <vector>
#include <functional>
#include "ISoundManager.h"

#define alCall(function, ...) alCallImpl(__FILE__, __LINE__, function, __VA_ARGS__)
#define alcCall(function, device, ...) alcCallImpl(__FILE__, __LINE__, function, device, __VA_ARGS__)

// Reports errors encountered with OpenAL
bool checkALErrors(const std::string& filename, const std::uint_fast32_t line);
bool checkALCErrors(const std::string& filename, const std::uint_fast32_t line, ALCdevice* device);

// Wrapper for OpenAL (AL and ALC) calls
	// Template for methods that return a non-void type
template<typename alFunction, typename... Params>
auto alCallImpl(const char* filename,
	const std::uint_fast32_t line,
	alFunction function,
	Params... params)
	->typename std::enable_if_t<!std::is_same_v<void, decltype(function(params...))>, decltype(function(params...))>
{
	auto ret = function(std::forward<Params>(params)...);
	checkALErrors(filename, line);
	return ret;
}

// Template for methods that return a void type
template<typename alFunction, typename... Params>
auto alCallImpl(const char* filename,
	const std::uint_fast32_t line,
	alFunction function,
	Params... params)
	->typename std::enable_if_t<std::is_same_v<void, decltype(function(params...))>, bool>
{
	function(std::forward<Params>(params)...);
	return checkALErrors(filename, line);
}

template<typename alcFunction, typename... Params>
auto alcCallImpl(const char* filename,
	const std::uint_fast32_t line,
	alcFunction function,
	ALCdevice* device,
	Params... params)
	->typename std::enable_if_t<std::is_same_v<void, decltype(function(params...))>, bool>
{
	function(std::forward<Params>(params)...);
	return checkALCErrors(filename, line, device);
}

template<typename alcFunction, typename ReturnType, typename... Params>
auto alcCallImpl(const char* filename,
	const std::uint_fast32_t line,
	alcFunction function,
	ReturnType& returnValue,
	ALCdevice* device,
	Params... params)
	->typename std::enable_if_t<!std::is_same_v<void, decltype(function(params...))>, bool>
{
	returnValue = function(std::forward<Params>(params)...);
	return checkALCErrors(filename, line, device);
}

class CSoundManager : public ISoundManager
{
	// Singleton stuff
	static CSoundManager* s_pInstance;
	CSoundManager();
	~CSoundManager();

public:
	// more singleton stuff
	static CSoundManager* Instance()
	{
		if (s_pInstance == nullptr)
		{
			s_pInstance = new CSoundManager();
		}

		return s_pInstance;
	}

	virtual void OnThink();
	virtual void Destroy();

	// Wave methods
	virtual bool Load(const std::string& fileName, SWaveFile& file);

	virtual void PlaySound(SWaveFile* file);
	virtual void StopSound(SWaveFile* file);
	
	virtual void DeleteSound(SWaveFile* file);

private:
	// Utility for loading WAVE files
	static std::int32_t convertToInt(char* buffer, std::size_t len);

	bool loadWavFileHeader(std::ifstream& file,
		std::uint8_t& channels,
		std::int32_t& sampleRate,
		std::uint8_t& bitsPerSample,
		ALsizei& size);

	bool loadWav(const std::string& filename, SWaveFile* wf);

	bool getAvailableDevices(std::vector<std::string>& devicesVec, ALCdevice* device);

public:
	// Ogg Implementation
	virtual bool CreateStreamFromFile(const std::string& filename, SStreamingAudioData& audioData);
	
	virtual void PlayStream(SStreamingAudioData* audioData);
	virtual void PauseStream(SStreamingAudioData* audioData);

	virtual void StopStream(SStreamingAudioData* audioData);
	virtual void DeleteStream(SStreamingAudioData* audioData);

	bool IsInitialised() override { return m_initalised; }

private:
	// Variables
	ALCdevice* openALDevice = 0;
	ALCcontext* openALContext = 0;

	std::vector<SStreamingAudioData*> streams;

	std::vector<std::string> devices;

	bool m_initalised = false;

public:
	std::vector<SStreamingAudioData*> GetStreams() override { return streams; };
};

#endif