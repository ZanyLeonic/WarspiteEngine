#include "SoundManager.h"
#include <spdlog/spdlog.h>

// TODO: Clean up and refine this class - it is a mess.
CSoundManager* CSoundManager::s_pInstance = 0;

int UpdateStream(SStreamingAudioData& audioData)
{
	ALint st;
	alCall(alGetSourcei, audioData.Source, AL_SOURCE_STATE, &st);

	if (st != AL_PLAYING) return 0; // We might be paused or stopped.

	ALint buffersProcessed = 0;
	bool reachedEnd = false;

	// Don't stream any more data until we have finished playing the current data in the buffer
	alCall(alGetSourcei, audioData.Source, AL_BUFFERS_PROCESSED, &buffersProcessed);
	if (buffersProcessed <= 0)
	{
		return 0;
	}

	// Stream more parts of the file into the buffer if the current buffer has been played.
	while (buffersProcessed--)
	{
		ALuint buffer;
		alCall(alSourceUnqueueBuffers, audioData.Source, 1, &buffer);

		char* data = new char[BUFFER_SIZE];
		memset(data, 0, BUFFER_SIZE);

		ALsizei dataSizeToBuffer = 0;
		std::int32_t sizeRead = 0;

		while ((sizeRead < BUFFER_SIZE) && !reachedEnd)
		{
			// Continue to read the OGG file
			std::int32_t result = ov_read(&audioData.OggVorbisFile, &data[sizeRead], BUFFER_SIZE - sizeRead, 0, 2, 1,
										  reinterpret_cast<int *>(&audioData.OggCurrentSection));

			if (result == OV_HOLE)
			{
				spdlog::error("ERROR: OV_HOLE found in update of buffer ");
				break;
			}
			else if (result == OV_EBADLINK)
			{
				spdlog::error("ERROR: OV_EBADLINK found in update of buffer");
				break;
			}
			else if (result == OV_EINVAL)
			{
				spdlog::error("ERROR: OV_EINVAL found in update of buffer ");
				break;
			}
			else if (result == 0)
			{
				if (ov_time_tell(&audioData.OggVorbisFile) == ov_time_total(&audioData.OggVorbisFile, -1))
				{
					reachedEnd = true;
				}
			}
			sizeRead += result;
		}
		dataSizeToBuffer = sizeRead;

		if (dataSizeToBuffer > 0)
		{
			alCall(alBufferData, buffer, audioData.Format, data, dataSizeToBuffer, audioData.SampleRate);
			alCall(alSourceQueueBuffers, audioData.Source, 1, &buffer);
		}

		delete[] data;
	}

	// Change flag only if we have reached the end and we haven't finished yet.
	if (reachedEnd == true && !audioData.Finished)
	{
		// Play the last bit of the remaining buffer
		ALint state = AL_PLAYING;
		while (state == AL_PLAYING)
		{
			alCall(alGetSourcei, audioData.Source, AL_SOURCE_STATE, &state);
		}

		// Close the file stream + toggle flag
		audioData.File.close();
		audioData.Finished = true;
	}

	return 0;
}

int audioStreamUpdate(void* data)
{
	auto* as = reinterpret_cast<SStreamingAudioData*>(data);

	if (as->StreamCreated)
		as->StreamCreated(as);

	CSoundManager::Instance()->GetStreams().push_back(as);
	
	while (as->Finished != true)
	{
		if (as->UpdateCallback)
			as->UpdateCallback(as);

		UpdateStream(*as);
	}

	alCall(alSourceStop, as->Source);

	if (as->StopCallback)
		as->StopCallback(as);

	if (as->File.is_open())
	{
		as->File.clear();
		as->File.close();
	}
	
	return 0;
}


std::size_t readOggCallback(void* destination, std::size_t size1, std::size_t size2, void* fileHandle)
{
	SStreamingAudioData* audioData = reinterpret_cast<SStreamingAudioData*>(fileHandle);

	ALsizei length = (ALsizei)(size1 * size2);

	if (audioData->SizeConsumed + length > audioData->Size)
	{
		length = audioData->Size - audioData->SizeConsumed;
	}

	if (!audioData->File.is_open())
	{
		audioData->File.open(audioData->Filename, std::ios::binary);
		if (!audioData->File.is_open())
		{
			spdlog::error("ERROR: Could not re-open streaming file \"{}\"", audioData->Filename);
			return 0;
		}
	}

	char* moreData = new char[length];

	audioData->File.clear();
	audioData->File.seekg(audioData->SizeConsumed);
	if (!audioData->File.read(&moreData[0], length))
	{
		if (audioData->File.eof())
		{
			audioData->File.clear(); // just clear the error, we will resolve it later
		}
		else if (audioData->File.fail())
		{
			spdlog::error("ERROR: OGG stream has fail bit set \"{}\"", audioData->Filename);
			audioData->File.clear();
			return 0;
		}
		else if (audioData->File.bad())
		{
			spdlog::error("ERROR: OGG stream has bad bit set \"{}\"", audioData->Filename);
			audioData->File.clear();
			return 0;
		}
	}
	audioData->SizeConsumed += length;

	memcpy(destination, &moreData[0], length);

	delete[] moreData;

	audioData->File.clear();

	return length;
}

std::int32_t seekOggCallback(void* fileHandle, ogg_int64_t to, std::int32_t type)
{
	SStreamingAudioData* audioData = reinterpret_cast<SStreamingAudioData*>(fileHandle);

	if (type == SEEK_CUR)
	{
		audioData->SizeConsumed += (ALsizei)to;
	}
	else if (type == SEEK_END)
	{
		audioData->SizeConsumed = audioData->Size - (ALsizei)to;
	}
	else if (type == SEEK_SET)
	{
		audioData->SizeConsumed = (ALsizei)to;
	}
	else
		return -1; // what are you trying to do vorbis?

	if (audioData->SizeConsumed < 0)
	{
		audioData->SizeConsumed = 0;
		return -1;
	}
	if (audioData->SizeConsumed > audioData->Size)
	{
		audioData->SizeConsumed = audioData->Size;
		return -1;
	}

	return 0;
}

long int tellOggCallback(void* fileHandle)
{
	SStreamingAudioData* audioData = reinterpret_cast<SStreamingAudioData*>(fileHandle);
	return audioData->SizeConsumed;
}

int createStreamOnThread(void* pdata)
{
	auto audioData = reinterpret_cast<SStreamingAudioData*>(pdata);

	// Open the audio file in a filestream
	audioData->File.open(audioData->Filename, std::ios::binary);
	if (!audioData->File.is_open())
	{
		spdlog::error("ERROR: Couldn't open file \"{}\"", audioData->Filename);
		return 0;
	}

	// Seek at the beginning of the ogg file
	audioData->File.seekg(0, std::ios_base::beg);
	audioData->File.ignore(std::numeric_limits<std::streamsize>::max());
	audioData->Size = (ALsizei)audioData->File.gcount();
	audioData->File.clear();
	audioData->File.seekg(0, std::ios_base::beg);
	audioData->SizeConsumed = 0;

	// Assign the ogg callbacks for this audio thread
	ov_callbacks oggCallbacks;
	oggCallbacks.read_func = readOggCallback;
	oggCallbacks.close_func = nullptr;
	oggCallbacks.seek_func = seekOggCallback;
	oggCallbacks.tell_func = tellOggCallback;

	// Open the OGG file with vorbis
	if (ov_open_callbacks(pdata, &audioData->OggVorbisFile, nullptr, -1, oggCallbacks) < 0)
	{
		spdlog::error("ERROR: Could not call ov_open_callbacks");
		return -1;
	}

	// Populate the struct with the information of the OGG file
	vorbis_info* vorbisInfo = ov_info(&audioData->OggVorbisFile, -1);

	audioData->Channels = vorbisInfo->channels;
	audioData->BitRate = 16;
	audioData->SampleRate = vorbisInfo->rate;
	audioData->Duration = (size_t)ov_time_total(&audioData->OggVorbisFile, -1);

	// Create the OpenAL smuck
	alCall(alGenSources, 1, &audioData->Source);
	alCall(alSourcef, audioData->Source, AL_PITCH, 1.f);
	alCall(alSourcef, audioData->Source, AL_GAIN, 1.f);
	alCall(alSource3f, audioData->Source, AL_POSITION, 0.f, 0.f, 0.f);
	alCall(alSource3f, audioData->Source, AL_VELOCITY, 0.f, 0.f, 0.f);
	alCall(alSourcei, audioData->Source, AL_LOOPING, AL_FALSE);

	alCall(alGenBuffers, (ALsizei)NUM_BUFFERS, &audioData->Buffers[0]);

	if (audioData->File.eof())
	{
		spdlog::error("ERROR: Already reached EOF without loading audio data");
		return false;
	}
	else if (audioData->File.fail())
	{
		spdlog::error("ERROR: Audio Fail bit set");
		return false;
	}
	else if (!audioData->File)
	{
		spdlog::error("ERROR: Audio File stream is closed");
		return false;
	}

	// Create our buffer
	char* data = new char[BUFFER_SIZE];

	// then FILL IT!!!
	for (std::uint8_t i = 0; i < NUM_BUFFERS; ++i)
	{
		// Read until our buffers are full
		std::int32_t dataSoFar = 0;
		while (dataSoFar < BUFFER_SIZE)
		{
			// Read a part of the OGG file
			std::int32_t result = ov_read(&audioData->OggVorbisFile, &data[dataSoFar], BUFFER_SIZE - dataSoFar, 0, 2, 1,
										  reinterpret_cast<int *>(&audioData->OggCurrentSection));
			if (result == OV_HOLE)
			{
				spdlog::error("ERROR: OV_HOLE found in initial read of buffer {}", i);
				break;
			}
			else if (result == OV_EBADLINK)
			{
				spdlog::error("ERROR: OV_EBADLINK found in initial read of buffer {}", i);
				break;
			}
			else if (result == OV_EINVAL)
			{
				spdlog::error("ERROR: OV_EINVAL found in initial read of buffer {}", i);
				break;
			}
			else if (result == 0)
			{
				spdlog::error("ERROR: EOF found in initial read of buffer {}", i);
				break;
			}

			// update the amount that was read
			dataSoFar += result;
		}

		// Set the information of the OGG to the struct
		if (audioData->Channels == 1 && audioData->BitRate == 8)
			audioData->Format = AL_FORMAT_MONO8;
		else if (audioData->Channels == 1 && audioData->BitRate == 16)
			audioData->Format = AL_FORMAT_MONO16;
		else if (audioData->Channels == 2 && audioData->BitRate == 8)
			audioData->Format = AL_FORMAT_STEREO8;
		else if (audioData->Channels == 2 && audioData->BitRate == 16)
			audioData->Format = AL_FORMAT_STEREO16;
		else
		{
			spdlog::error("ERROR: Unrecognised OGG Format: {} Channels, {} bps", audioData->Channels, audioData->BitRate);
			delete[] data;
			return -1;
		}

		// actually fill the buffers
		alCall(alBufferData, audioData->Buffers[i], audioData->Format, data, dataSoFar, audioData->SampleRate);
	}

	// Queue the buffers
	alCall(alSourceQueueBuffers, audioData->Source, (ALsizei)NUM_BUFFERS, &audioData->Buffers[0]);

	delete[] data;

	// Then call the main audio loop
	return audioStreamUpdate(pdata);
}

bool checkALErrors(const std::string& filename, const std::uint_fast32_t line)
{
	// Spew out friendly error message for the corrisponding enums.
	ALenum error = alGetError();
	if (error != AL_NO_ERROR)
	{
		spdlog::error("***ERROR*** ({}:{})", filename, line);
		switch (error)
		{
		case AL_INVALID_NAME:
			spdlog::error("AL_INVALID_NAME: a bad name (ID) was passed to an OpenAL function");
			break;
		case AL_INVALID_ENUM:
			spdlog::error("AL_INVALID_ENUM: an invalid enum value was passed to an OpenAL function");
			break;
		case AL_INVALID_VALUE:
			spdlog::error("AL_INVALID_VALUE: an invalid value was passed to an OpenAL function");
			break;
		case AL_INVALID_OPERATION:
			spdlog::error("AL_INVALID_OPERATION: the requested operation is not valid");
			break;
		case AL_OUT_OF_MEMORY:
			spdlog::error("AL_OUT_OF_MEMORY: the requested operation resulted in OpenAL running out of memory");
			break;
		default:
			spdlog::error("UNKNOWN AL ERROR: {}", error);
		}
		std::cerr << std::endl;
		return false;
	}
	return true;
}

bool checkALCErrors(const std::string& filename, const std::uint_fast32_t line, ALCdevice* device)
{
	// Spew out friendly error message for the corrisponding enums.
	ALCenum error = alcGetError(device);
	if (error != ALC_NO_ERROR)
	{
		spdlog::error("***ERROR*** ({}:{})", filename, line);
		switch (error)
		{
		case ALC_INVALID_VALUE:
			spdlog::error("ALC_INVALID_VALUE: an invalid value was passed to an OpenAL function");
			break;
		case ALC_INVALID_DEVICE:
			spdlog::error("ALC_INVALID_DEVICE: a bad device was passed to an OpenAL function");
			break;
		case ALC_INVALID_CONTEXT:
			spdlog::error("ALC_INVALID_CONTEXT: a bad context was passed to an OpenAL function");
			break;
		case ALC_INVALID_ENUM:
			spdlog::error("ALC_INVALID_ENUM: an unknown enum value was passed to an OpenAL function");
			break;
		case ALC_OUT_OF_MEMORY:
			spdlog::error("ALC_OUT_OF_MEMORY: an unknown enum value was passed to an OpenAL function");
			break;
		default:
			spdlog::error("UNKNOWN ALC ERROR: {}", error);
		}
		std::cerr << std::endl;
		return false;
	}
	return true;
}

CSoundManager::CSoundManager()
{
	spdlog::info("Initialising SoundManager...");

	// Enumerate all our devices
	getAvailableDevices(devices, NULL);

	spdlog::info("Using device \"{}\"...", devices[0].c_str());

	// Open the first device we get
	// Sidenote - does OpenAL soft even allow us to choose which device to initialise?
	openALDevice = alcOpenDevice(devices[0].c_str());
	if (!openALDevice)
	{
		spdlog::error("ERROR: Cannot open device \"{}\"!", devices[0].c_str());
	}

	// Whoops - we cannot create a context.
	if (!alcCall(alcCreateContext, openALContext, openALDevice, openALDevice, nullptr) || !openALContext)
	{
		spdlog::error("ERROR: Could not create audio context");
		/* probably exit program */
	}

	// Uh oh, cannot switch audio context, no audio for the app session?
	ALCboolean contextMadeCurrent = false;
	if (!alcCall(alcMakeContextCurrent, contextMadeCurrent, openALDevice, openALContext)
		|| contextMadeCurrent != ALC_TRUE)
	{
		spdlog::error("ERROR: Could not make audio context current");
		/* probably exit or give up on having sound */
	}

	// We somehow survived! fhew.
	spdlog::info("SoundManager initialised.");
}

CSoundManager::~CSoundManager()
{
	// Just destroy if we are being deconstructed
	Destroy();
}

void CSoundManager::OnThink()
{
}

void CSoundManager::Destroy()
{
	spdlog::info("Destroying active AudioStreams...");
	for (int i=0;i < streams.size();i++)
	{
		alCall(alSourcei, streams[i]->Source, AL_BUFFER, 0);
        alCall(alDeleteSources, 1, &streams[i]->Source);
        alCall(alDeleteBuffers, NUM_BUFFERS, &streams[i]->Buffers[0]);
	}

	std::cout << "Destroying device handles..." << std::endl;
	// Try to destroy our device
	ALCboolean closed;
	if (!alcCall(alcCloseDevice, closed, openALDevice, openALDevice))
	{
		spdlog::error("ERROR: Error while destroying active audio device");
	}
	else
	{
		spdlog::info("Successfully destroyed device handle");
	}
}

bool CSoundManager::Load(const std::string& fileName, SWaveFile& file)
{
	// The input already has been loaded! No need to load again.
	if (file.RawData != nullptr && file.DataSize > 0) return true;

	if (!loadWav(fileName, &file)) { return false; }

	if (file.RawData == nullptr || file.DataSize == 0)
	{
		spdlog::error("ERROR: Could not load wav");
		return false;
	}

	file.Filename = fileName;

	std::vector<char> soundData = std::vector<char>(file.RawData, file.RawData + file.DataSize);

	alCall(alGenBuffers, 1, &file.Buffer);

	// Load the wav into the buffer
	alCall(alBufferData, file.Buffer, file.GetFormat(), soundData.data(), (ALsizei)soundData.size(), file.SampleRate);
	soundData.clear(); // erase the sound in RAM

	// Setup a source for the file to play
	alCall(alGenSources, 1, &file.Source);
	alCall(alSourcef, file.Source, AL_PITCH, 1.0f);
	alCall(alSourcef, file.Source, AL_GAIN, 1.0f);
	alCall(alSource3f, file.Source, AL_POSITION, 0.f, 0.f, 0.f);
	alCall(alSource3f, file.Source, AL_VELOCITY, 0.f, 0.f, 0.f);
	alCall(alSourcei, file.Source, AL_LOOPING, AL_FALSE);
	alCall(alSourcei, file.Source, AL_BUFFER, file.Buffer);

	return true;
}

int playWav(void* data)
{
	SWaveFile* file = reinterpret_cast<SWaveFile*>(data);

	// play the source
	alSourcePlay(file->Source);

	ALint state = AL_PLAYING;

	while (state == AL_PLAYING)
	{
		alGetSourcei(file->Source, AL_SOURCE_STATE, &state);
	}

	return 0;
}

void CSoundManager::PlaySound(SWaveFile* file)
{
	// We are dealing with uninitialised data! don't play!
	if (file->RawData == nullptr || file->DataSize == 0) return;

	SDL_Thread* pBThread = SDL_CreateThread(playWav, file->Filename.c_str(), (void*)file);
	SDL_DetachThread(pBThread);
}

void CSoundManager::StopSound(SWaveFile* file)
{
	// We are dealing with uninitialised data! don't play!
	if (file->RawData == nullptr || file->DataSize == 0) return;
	
	alCall(alSourceStop, file->Source);
}

void CSoundManager::DeleteSound(SWaveFile* file)
{
	// then free the memory that contained the wav file.
	alCall(alDeleteSources, 1, &file->Source);
	alCall(alDeleteBuffers, 1, &file->Buffer);
}

bool CSoundManager::getAvailableDevices(std::vector<std::string>& devicesVec, ALCdevice* device)
{
	const ALCchar* devices;
	if (!alcCall(alcGetString, devices, device, nullptr, ALC_DEVICE_SPECIFIER))
		return false;

	const char* ptr = devices;

	devicesVec.clear();

	do
	{
		devicesVec.push_back(std::string(ptr));
		ptr += devicesVec.back().size() + 1;
	} while (*(ptr + 1) != '\0');

	return true;
}

std::int32_t CSoundManager::convertToInt(char* buffer, std::size_t len)
{
	std::int32_t a = 0;
	if (endian::native == endian::little)
		memcpy(&a, buffer, len);
	else
		for (std::size_t i = 0; i < len; ++i)
			reinterpret_cast<char*>(&a)[3 - i] = buffer[i];
	return a;
}

bool CSoundManager::loadWavFileHeader(std::ifstream& file, std::uint8_t& channels, std::int32_t& sampleRate, std::uint8_t& bitsPerSample, ALsizei& size)
{
	// a method for casually reading wave file headers.
	char buffer[4];
	if (!file.is_open())
		return false;

	// the RIFF
	if (!file.read(buffer, 4))
	{
		spdlog::error("ERROR: could not read RIFF");
		return false;
	}
	if (strncmp(buffer, "RIFF", 4) != 0)
	{
		spdlog::error("ERROR: file is not a valid WAVE file (header doesn't begin with RIFF)");
		return false;
	}

	// the size of the file
	if (!file.read(buffer, 4))
	{
		spdlog::error("ERROR: could not read size of file");
		return false;
	}

	// the WAVE
	if (!file.read(buffer, 4))
	{
		spdlog::error("ERROR: could not read WAVE");
		return false;
	}
	if (strncmp(buffer, "WAVE", 4) != 0)
	{
		spdlog::error("ERROR: file is not a valid WAVE file (header doesn't contain WAVE)");
		return false;
	}

	// "fmt/0"
	if (!file.read(buffer, 4))
	{
		spdlog::error("ERROR: could not read fmt/0");
		return false;
	}

	// this is always 16, the size of the fmt data chunk
	if (!file.read(buffer, 4))
	{
		spdlog::error("ERROR: could not read the 16");
		return false;
	}

	// PCM should be 1?
	if (!file.read(buffer, 2))
	{
		spdlog::error("ERROR: could not read PCM");
		return false;
	}

	// the number of channels
	if (!file.read(buffer, 2))
	{
		spdlog::error("ERROR: could not read number of channels");
		return false;
	}
	channels = convertToInt(buffer, 2);

	// sample rate
	if (!file.read(buffer, 4))
	{
		spdlog::error("ERROR: could not read sample rate");
		return false;
	}
	sampleRate = convertToInt(buffer, 4);

	// (sampleRate * bitsPerSample * channels) / 8
	if (!file.read(buffer, 4))
	{
		spdlog::error("ERROR: could not read (sampleRate * bitsPerSample * channels) / 8");
		return false;
	}

	// ?? dafaq
	if (!file.read(buffer, 2))
	{
		spdlog::error("ERROR: could not read file");
		return false;
	}

	// bitsPerSample
	if (!file.read(buffer, 2))
	{
		spdlog::error("ERROR: could not read bits per sample");
		return false;
	}
	bitsPerSample = convertToInt(buffer, 2);

	// data chunk header "data"
	if (!file.read(buffer, 4))
	{
		spdlog::error("ERROR: could not read data chunk header");
		return false;
	}
	if (strncmp(buffer, "data", 4) != 0)
	{
		spdlog::error("ERROR: file is not a valid WAVE file (doesn't have 'data' tag)");
		return false;
	}

	// size of data
	if (!file.read(buffer, 4))
	{
		spdlog::error("ERROR: could not read data size");
		return false;
	}
	size = convertToInt(buffer, 4);

	/* cannot be at the end of file */
	if (file.eof())
	{
		spdlog::error("ERROR: reached EOF on the file");
		return false;
	}
	if (file.fail())
	{
		spdlog::error("ERROR: fail state set on the file");
		return false;
	}

	return true;
}

bool CSoundManager::loadWav(const std::string& filename, SWaveFile* wf)
{
	// Open the wave file...
	std::ifstream in(filename, std::ios::binary);
	if (!in.is_open())
	{
		spdlog::error("ERROR: Could not open \"{}\"", filename);
		return false;
	}
	if (!loadWavFileHeader(in, wf->Channels, wf->SampleRate, wf->BitRate, wf->DataSize))
	{
		spdlog::error("ERROR: Could not load wav header of \"{}\"", filename);
		return false;
	}

	// ... and shove the data into the structure >:D
	wf->RawData = new char[wf->DataSize];

	in.read(wf->RawData, wf->DataSize);

	// then return true.
	return true;
}

bool CSoundManager::CreateStreamFromFile(const std::string& filename, SStreamingAudioData& audioData)
{
	// Don't try to create a new stream in an already created stream
	if (audioData.File.is_open()) return false;

	// buffer
	char tName[420];

	// Create an identifier for the thread into the buffer
	audioData.Filename = filename;
	snprintf(tName, sizeof(tName), "Audio Stream: \"%s\"", filename.c_str());

	// Create, detach the thread and return true
	SDL_Thread* aT = SDL_CreateThread(createStreamOnThread, tName, (void*)&audioData);
	SDL_DetachThread(aT);

	return true;
}

// Stream playback control
void CSoundManager::PlayStream(SStreamingAudioData* audioData)
{
	if (!audioData->File.is_open()) return;
	
    // Fully stop the source than play.
    alCall(alSourceStop, audioData->Source);
    alCall(alSourcePlay, audioData->Source);

	// Only call the callback if it exists
	if (audioData->PlayCallback)
		audioData->PlayCallback(audioData);
}

void CSoundManager::PauseStream(SStreamingAudioData* audioData)
{
	ALint st;
	alCall(alGetSourcei, audioData->Source, AL_SOURCE_STATE, &st);

	if (st != AL_PLAYING) return;
	
	alCall(alSourcePause, audioData->Source);

	// Only call the callback if it exists
	if (audioData->PauseCallback)
		audioData->PauseCallback(audioData);
}

void CSoundManager::StopStream(SStreamingAudioData* audioData)
{
	ALint st;
	alCall(alGetSourcei, audioData->Source, AL_SOURCE_STATE, &st);

	if (st != AL_PLAYING) return;

	audioData->Finished = true;
	
	streams.erase(std::remove(streams.begin(),streams.end(), audioData),	streams.end());
}