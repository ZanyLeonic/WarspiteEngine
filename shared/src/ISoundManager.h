#pragma once
#ifndef __ISOUNDMGR_H_
#define __ISOUNDMGR_H_

#include <AL/al.h>
#include <vorbis/vorbisfile.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <spdlog/spdlog.h>

enum class ESoundType
{
	SOUND_NONE = -1,
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

struct SWaveFile
{
	ALuint			Buffer;
	std::uint8_t	BitRate = 0;
	std::uint8_t	Channels = 0;
	ALsizei			DataSize = 0;
	std::string		Filename;
	ALsizei			SampleRate = 0;
	ALuint			Source;
	char* RawData = 0;

	// Determines and returns the correct format depending on our data
	ALenum	GetFormat()
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
			spdlog::error("ERROR: unrecognised wave format: {} channels, {} bps", Channels, BitRate);
			return 0x0;
		}
	}
};

// Streaming audio
const std::size_t NUM_BUFFERS = 4;
const ALsizei BUFFER_SIZE = 65536;

struct SStreamingAudioData
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

	// Callbacks
	std::function<void(SStreamingAudioData*)> StreamCreated;
	std::function<void(SStreamingAudioData*)> UpdateCallback;

	std::function<void(SStreamingAudioData*)> PlayCallback;
	std::function<void(SStreamingAudioData*)> PauseCallback;
	std::function<void(SStreamingAudioData*)> StopCallback;

public:
	bool operator==(const SStreamingAudioData s1)
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

class ISoundManager
{
public:
	virtual void OnThink() = 0;
	virtual void Destroy() = 0;

	// Wave methods
	virtual bool Load(const std::string& fileName, SWaveFile& file) = 0;

	virtual void PlaySound(SWaveFile* file) = 0;
	virtual void StopSound(SWaveFile* file) = 0;

	virtual void DeleteSound(SWaveFile* file) = 0;

	// Ogg Implementation
	virtual bool CreateStreamFromFile(const std::string& filename, SStreamingAudioData& audioData) = 0;

	virtual void PlayStream(SStreamingAudioData* audioData) = 0;
	virtual void PauseStream(SStreamingAudioData* audioData) = 0;

	virtual void StopStream(SStreamingAudioData* audioData) = 0;
	virtual void DeleteStream(SStreamingAudioData* audioData) = 0;

	virtual bool IsInitialised() = 0;

	virtual std::vector<SStreamingAudioData*> GetStreams() = 0;
};
#endif