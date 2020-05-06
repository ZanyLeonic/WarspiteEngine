#pragma once

#ifndef __SOUNDMANAGER_H__
#define __SOUNDMANAGER_H__

#include <AL/al.h>
#include <AL/alc.h>
#include <vorbis/vorbisfile.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>

#define alCall(function, ...) alCallImpl(__FILE__, __LINE__, function, __VA_ARGS__)
#define alcCall(function, device, ...) alcCallImpl(__FILE__, __LINE__, function, device, __VA_ARGS__)

enum class SoundType
{
	SOUND_MUSIC = 0,
	SOUND_SFX = 1
};

// A work around for compilers using an older C++ specification
enum class endian
{
#ifdef _WIN32
	little = 0,
	big = 1,
	native = little
#else
	little = __ORDER_LITTLE_ENDIAN__,
	big = __ORDER_BIG_ENDIAN__,
	native = __BYTE_ORDER__
#endif
};

struct WaveFile
{
	std::uint8_t	Channels   = 0;
	ALsizei			SampleRate = 0;
	std::uint8_t	BitRate	= 0;
	ALsizei			DataSize	= 0;
	char*			RawData	= 0;

	// Determines and returns the correct format depending on our data
	ALenum			GetFormat()
	{
		if (Channels == 1 && BitRate == 8)
			return AL_FORMAT_MONO8;
		else if (Channels == 1 && BitRate == 16)
			return AL_FORMAT_MONO16;
		else if (Channels == 2 && BitRate == 8)
			return AL_FORMAT_STEREO8;
		else if (Channels == 2 && BitRate == 16)
			return AL_FORMAT_STEREO16;
		else
		{
			// We have no idea what this is.
			std::cerr
				<< "ERROR: unrecognised wave format: "
				<< Channels << " channels, "
				<< BitRate << " bps" << std::endl;
			return NULL;
		}
	}
};

// Streaming audio
const std::size_t NUM_BUFFERS = 4;
const ALsizei BUFFER_SIZE = 65536;

struct StreamingAudioData
{
	ALuint Buffers[NUM_BUFFERS];
	std::string Filename;
	std::ifstream File;
	std::uint8_t Channels;
	std::int32_t SampleRate;
	std::uint8_t BitRate;
	bool Finished;
	ALsizei Size;
	ALuint Source;
	ALsizei SizeConsumed = 0;
	ALenum Format;
	OggVorbis_File OggVorbisFile;
	std::int_fast32_t OggCurrentSection = 0;
	std::size_t Duration;

	bool operator==(const StreamingAudioData s1)
	{
		// Compare some of the fields - may need to change to include more in the future.
		// TODO
		return
			Buffers == s1.Buffers &&
			Filename == s1.Filename &&
			Format == s1.Format &&
			Duration == s1.Duration;
	}
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
	void OnThink();
	void Destroy();

	void PlaySound(std::string id, int loop);
	void PlayMusic(std::string id, int loop);

	void Test();

private:

	static SoundManager* s_pInstance;

	SoundManager();
	~SoundManager();

	SoundManager(const SoundManager&);

	// Reports errors encountered with OpenAL
	bool checkALErrors(const std::string& filename, const std::uint_fast32_t line);
	bool checkALCErrors(const std::string& filename, const std::uint_fast32_t line, ALCdevice* device);

	bool getAvailableDevices(std::vector<std::string>& devicesVec, ALCdevice* device);
	
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

	// Utility for loading WAVE files
	std::int32_t convertToInt(char* buffer, std::size_t len);
	
	bool loadWavFileHeader(std::ifstream& file,
		std::uint8_t& channels,
		std::int32_t& sampleRate,
		std::uint8_t& bitsPerSample,
		ALsizei& size);

	bool loadWav(const std::string& filename, WaveFile* wf);

public:
	// Ogg Implementation
	bool CreateStreamFromFile(const std::string& filename, StreamingAudioData& audioData);

	void PlayStream(StreamingAudioData* audioData);
	void StopStream(StreamingAudioData* audioData);

private:
	void UpdateStream(StreamingAudioData& audioData);

	// std::vector<StreamingAudioData> streams;

	// Variables
	ALCdevice* openALDevice = 0;
	ALCcontext* openALContext = 0;

	std::vector<StreamingAudioData*> streams;

	std::vector<std::string> devices;

	// testing!
	int posi = 0;
};

#endif
