#include "SoundManager.h"
#include <vorbis/codec.h>

SoundManager* SoundManager::s_pInstance = 0;

bool SoundManager::Load(std::string fileName, std::string id, SoundType type)
{
    return false;
}

void SoundManager::OnThink()
{
    for (int i = 0; i < streams.size(); i++)
    {
        // Get the state of the current stream
        ALint state;
        alCall(alGetSourcei, streams[i].Source, AL_SOURCE_STATE, &state);

        // Only update the streams that are playing.
        if (state == AL_PLAYING)
        {
            UpdateStream(streams[i]);
        }
    }
}

void SoundManager::Destroy()
{
    std::cout << "Destroying device handles..." << std::endl;
    // Try to destroy our device
    ALCboolean closed;
    if (!alcCall(alcCloseDevice, closed, openALDevice, openALDevice))
    {
        std::cerr << "Error while destroying device" << std::endl;
    }
    else
    {
        std::cout << "Successfully destroyed device handle" << std::endl;
    }
}

void SoundManager::PlaySound(std::string id, int loop)
{
}

void SoundManager::PlayMusic(std::string id, int loop)
{
}

void SoundManager::Test()
{
    WaveFile file;

    if (!loadWav("assets/sound/mycode.wav", &file)) { return; }

    if (file.RawData == nullptr || file.DataSize == 0)
    {
        std::cerr << "ERROR: Could not load wav" << std::endl;
        return;
    }

    std::vector<char> soundData = std::vector<char>(file.RawData, file.RawData + file.DataSize);

    ALuint buffer;
    alCall(alGenBuffers, 1, &buffer);

    // Load the wav into the buffer
    alCall(alBufferData, buffer, file.GetFormat(), soundData.data(), (ALsizei)soundData.size(), file.SampleRate);
    soundData.clear(); // erase the sound in RAM

    // Setup a source for the file to play
    ALuint source;
    alCall(alGenSources, 1, &source);
    alCall(alSourcef, source, AL_PITCH, 1.0f);
    alCall(alSourcef, source, AL_GAIN, 1.0f);
    alCall(alSource3f, source, AL_POSITION, 0.f, 0.f, 0.f);
    alCall(alSource3f, source, AL_VELOCITY, 0.f, 0.f, 0.f);
    alCall(alSourcei, source, AL_LOOPING, AL_FALSE);
    alCall(alSourcei, source, AL_BUFFER, buffer);

    // play the source
    alCall(alSourcePlay, source);

    ALint state = AL_PLAYING;

    while (state == AL_PLAYING)
    {
        alCall(alGetSourcei, source, AL_SOURCE_STATE, &state);
    }

    // then free the memory that contained the wav file.
    alCall(alDeleteSources, 1, &source);
    alCall(alDeleteBuffers, 1, &buffer);
}

SoundManager::SoundManager()
{
    std::cout << "Initialising SoundManager..." << std::endl;

    // Enumerate all our devices
    getAvailableDevices(devices, NULL);

    std::cout << "Using device \"" << devices[0].c_str() << "\"..." << std::endl;

    // Open the first device we get
    openALDevice = alcOpenDevice(devices[0].c_str());
    if (!openALDevice)
    {
        std::cerr << "Cannot open device \"" << devices[0].c_str() << "\"!" << std::endl;
    }

    if (!alcCall(alcCreateContext, openALContext, openALDevice, openALDevice, nullptr) || !openALContext)
    {
        std::cerr << "ERROR: Could not create audio context" << std::endl;
        /* probably exit program */
    }

    ALCboolean contextMadeCurrent = false;
    if (!alcCall(alcMakeContextCurrent, contextMadeCurrent, openALDevice, openALContext)
        || contextMadeCurrent != ALC_TRUE)
    {
        std::cerr << "ERROR: Could not make audio context current" << std::endl;
        /* probably exit or give up on having sound */
    }


    std::cout << "SoundManager initialised." << std::endl;
}

SoundManager::~SoundManager()
{
    // Just destroy if we are being deconstructed
    Destroy();
}

bool SoundManager::checkALErrors(const std::string& filename, const std::uint_fast32_t line)
{
    // Spew out friendly error message for the corrisponding ENUMS.
    ALenum error = alGetError();
    if (error != AL_NO_ERROR)
    {
        std::cerr << "***ERROR*** (" << filename << ": " << line << ")\n";
        switch (error)
        {
        case AL_INVALID_NAME:
            std::cerr << "AL_INVALID_NAME: a bad name (ID) was passed to an OpenAL function";
            break;
        case AL_INVALID_ENUM:
            std::cerr << "AL_INVALID_ENUM: an invalid enum value was passed to an OpenAL function";
            break;
        case AL_INVALID_VALUE:
            std::cerr << "AL_INVALID_VALUE: an invalid value was passed to an OpenAL function";
            break;
        case AL_INVALID_OPERATION:
            std::cerr << "AL_INVALID_OPERATION: the requested operation is not valid";
            break;
        case AL_OUT_OF_MEMORY:
            std::cerr << "AL_OUT_OF_MEMORY: the requested operation resulted in OpenAL running out of memory";
            break;
        default:
            std::cerr << "UNKNOWN AL ERROR: " << error;
        }
        std::cerr << std::endl;
        return false;
    }
    return true;
}

bool SoundManager::checkALCErrors(const std::string& filename, const std::uint_fast32_t line, ALCdevice* device)
{
    // Spew out friendly error message for the corrisponding ENUMS.
    ALCenum error = alcGetError(device);
    if (error != ALC_NO_ERROR)
    {
        std::cerr << "***ERROR*** (" << filename << ": " << line << ")\n";
        switch (error)
        {
        case ALC_INVALID_VALUE:
            std::cerr << "ALC_INVALID_VALUE: an invalid value was passed to an OpenAL function";
            break;
        case ALC_INVALID_DEVICE:
            std::cerr << "ALC_INVALID_DEVICE: a bad device was passed to an OpenAL function";
            break;
        case ALC_INVALID_CONTEXT:
            std::cerr << "ALC_INVALID_CONTEXT: a bad context was passed to an OpenAL function";
            break;
        case ALC_INVALID_ENUM:
            std::cerr << "ALC_INVALID_ENUM: an unknown enum value was passed to an OpenAL function";
            break;
        case ALC_OUT_OF_MEMORY:
            std::cerr << "ALC_OUT_OF_MEMORY: an unknown enum value was passed to an OpenAL function";
            break;
        default:
            std::cerr << "UNKNOWN ALC ERROR: " << error;
        }
        std::cerr << std::endl;
        return false;
    }
    return true;
}

bool SoundManager::getAvailableDevices(std::vector<std::string>& devicesVec, ALCdevice* device)
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

std::int32_t SoundManager::convertToInt(char* buffer, std::size_t len)
{
    std::int32_t a = 0;
    if (endian::native == endian::little)
        std::memcpy(&a, buffer, len);
    else
        for (std::size_t i = 0; i < len; ++i)
            reinterpret_cast<char*>(&a)[3 - i] = buffer[i];
    return a;
}

bool SoundManager::loadWavFileHeader(std::ifstream& file, std::uint8_t& channels, std::int32_t& sampleRate, std::uint8_t& bitsPerSample, ALsizei& size)
{
    char buffer[4];
    if (!file.is_open())
        return false;

    // the RIFF
    if (!file.read(buffer, 4))
    {
        std::cerr << "ERROR: could not read RIFF" << std::endl;
        return false;
    }
    if (std::strncmp(buffer, "RIFF", 4) != 0)
    {
        std::cerr << "ERROR: file is not a valid WAVE file (header doesn't begin with RIFF)" << std::endl;
        return false;
    }

    // the size of the file
    if (!file.read(buffer, 4))
    {
        std::cerr << "ERROR: could not read size of file" << std::endl;
        return false;
    }

    // the WAVE
    if (!file.read(buffer, 4))
    {
        std::cerr << "ERROR: could not read WAVE" << std::endl;
        return false;
    }
    if (std::strncmp(buffer, "WAVE", 4) != 0)
    {
        std::cerr << "ERROR: file is not a valid WAVE file (header doesn't contain WAVE)" << std::endl;
        return false;
    }

    // "fmt/0"
    if (!file.read(buffer, 4))
    {
        std::cerr << "ERROR: could not read fmt/0" << std::endl;
        return false;
    }

    // this is always 16, the size of the fmt data chunk
    if (!file.read(buffer, 4))
    {
        std::cerr << "ERROR: could not read the 16" << std::endl;
        return false;
    }

    // PCM should be 1?
    if (!file.read(buffer, 2))
    {
        std::cerr << "ERROR: could not read PCM" << std::endl;
        return false;
    }

    // the number of channels
    if (!file.read(buffer, 2))
    {
        std::cerr << "ERROR: could not read number of channels" << std::endl;
        return false;
    }
    channels = convertToInt(buffer, 2);

    // sample rate
    if (!file.read(buffer, 4))
    {
        std::cerr << "ERROR: could not read sample rate" << std::endl;
        return false;
    }
    sampleRate = convertToInt(buffer, 4);

    // (sampleRate * bitsPerSample * channels) / 8
    if (!file.read(buffer, 4))
    {
        std::cerr << "ERROR: could not read (sampleRate * bitsPerSample * channels) / 8" << std::endl;
        return false;
    }

    // ?? dafaq
    if (!file.read(buffer, 2))
    {
        std::cerr << "ERROR: could not read dafaq" << std::endl;
        return false;
    }

    // bitsPerSample
    if (!file.read(buffer, 2))
    {
        std::cerr << "ERROR: could not read bits per sample" << std::endl;
        return false;
    }
    bitsPerSample = convertToInt(buffer, 2);

    // data chunk header "data"
    if (!file.read(buffer, 4))
    {
        std::cerr << "ERROR: could not read data chunk header" << std::endl;
        return false;
    }
    if (std::strncmp(buffer, "data", 4) != 0)
    {
        std::cerr << "ERROR: file is not a valid WAVE file (doesn't have 'data' tag)" << std::endl;
        return false;
    }

    // size of data
    if (!file.read(buffer, 4))
    {
        std::cerr << "ERROR: could not read data size" << std::endl;
        return false;
    }
    size = convertToInt(buffer, 4);

    /* cannot be at the end of file */
    if (file.eof())
    {
        std::cerr << "ERROR: reached EOF on the file" << std::endl;
        return false;
    }
    if (file.fail())
    {
        std::cerr << "ERROR: fail state set on the file" << std::endl;
        return false;
    }

    return true;
}

bool SoundManager::loadWav(const std::string& filename, WaveFile* wf)
{
    std::ifstream in(filename, std::ios::binary);
    if (!in.is_open())
    {
        std::cerr << "ERROR: Could not open \"" << filename << "\"" << std::endl;
        return false;
    }
    if (!loadWavFileHeader(in, wf->Channels, wf->SampleRate, wf->BitRate, wf->DataSize))
    {
        std::cerr << "ERROR: Could not load wav header of \"" << filename << "\"" << std::endl;
        return false;
    }

    wf->RawData = new char[wf->DataSize];

    in.read(wf->RawData, wf->DataSize);

    return true;
}

std::size_t readOggCallback(void* destination, std::size_t size1, std::size_t size2, void* fileHandle)
{
    StreamingAudioData* audioData = reinterpret_cast<StreamingAudioData*>(fileHandle);

    ALsizei length = size1 * size2;

    if (audioData->SizeConsumed + length > audioData->Size)
        length = audioData->Size - audioData->SizeConsumed;

    if (!audioData->File)
    {
        std::ifstream tempStream;
        tempStream.open(audioData->Filename, std::ios::binary);

        audioData->File = &tempStream;
    }

    if (!audioData->File->is_open())
    {
        audioData->File->open(audioData->Filename, std::ios::binary);
        if (!audioData->File->is_open())
        {
            std::cerr << "ERROR: Could not re-open streaming file \"" << audioData->Filename << "\"" << std::endl;
            return 0;
        }
    }

    char* moreData = new char[length];

    audioData->File->clear();
    audioData->File->seekg(audioData->SizeConsumed);
    if (!audioData->File->read(&moreData[0], length))
    {
        if (audioData->File->eof())
        {
            audioData->File->clear(); // just clear the error, we will resolve it later
        }
        else if (audioData->File->fail())
        {
            std::cerr << "ERROR: OGG stream has fail bit set " << audioData->Filename << std::endl;
            audioData->File->clear();
            return 0;
        }
        else if (audioData->File->bad())
        {
            perror(("ERROR: OGG stream has bad bit set " + audioData->Filename).c_str());
            audioData->File->clear();
            return 0;
        }
    }
    audioData->SizeConsumed += length;

    std::memcpy(destination, &moreData[0], length);

    delete[] moreData;

    audioData->File->clear();

    return length;
}

std::int32_t seekOggCallback(void* fileHandle, ogg_int64_t to, std::int32_t type)
{
    StreamingAudioData* audioData = reinterpret_cast<StreamingAudioData*>(fileHandle);

    if (type == SEEK_CUR)
    {
        audioData->SizeConsumed += to;
    }
    else if (type == SEEK_END)
    {
        audioData->SizeConsumed = audioData->Size - to;
    }
    else if (type == SEEK_SET)
    {
        audioData->SizeConsumed = to;
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
    StreamingAudioData* audioData = reinterpret_cast<StreamingAudioData*>(fileHandle);
    return audioData->SizeConsumed;
}

bool SoundManager::CreateStreamFromFile(const std::string& filename, StreamingAudioData& audioData)
{
    audioData.Filename = filename;

    std::ifstream tempStream;

    tempStream.open(filename, std::ios::binary);

    audioData.File = &tempStream;

    if (!audioData.File->is_open())
    {
        std::cerr << "ERROR: couldn't open file" << std::endl;
        return 0;
    }

    audioData.File->seekg(0, std::ios_base::beg);
    audioData.File->ignore(std::numeric_limits<std::streamsize>::max());
    audioData.Size = audioData.File->gcount();
    audioData.File->clear();
    audioData.File->seekg(0, std::ios_base::beg);
    audioData.SizeConsumed = 0;

    ov_callbacks oggCallbacks;
    oggCallbacks.read_func = readOggCallback;
    oggCallbacks.close_func = nullptr;
    oggCallbacks.seek_func = seekOggCallback;
    oggCallbacks.tell_func = tellOggCallback;

    if (ov_open_callbacks(reinterpret_cast<void*>(&audioData), &audioData.OggVorbisFile, nullptr, -1, oggCallbacks) < 0)
    {
        std::cerr << "ERROR: Could not ov_open_callbacks" << std::endl;
        return false;
    }

    vorbis_info* vorbisInfo = ov_info(&audioData.OggVorbisFile, -1);

    audioData.Channels = vorbisInfo->channels;
    audioData.BitRate = 16;
    audioData.SampleRate = vorbisInfo->rate;
    audioData.Duration = ov_time_total(&audioData.OggVorbisFile, -1);

    alCall(alGenSources, 1, &audioData.Source);
    alCall(alSourcef, audioData.Source, AL_PITCH, 1.f);
    alCall(alSourcef, audioData.Source, AL_GAIN, 1.f);
    alCall(alSource3f, audioData.Source, AL_POSITION, 0.f, 0.f, 0.f);
    alCall(alSource3f, audioData.Source, AL_VELOCITY, 0.f, 0.f, 0.f);
    alCall(alSourcei, audioData.Source, AL_LOOPING, AL_FALSE);

    alCall(alGenBuffers, NUM_BUFFERS, &audioData.Buffers[0]);

    if (audioData.File->eof())
    {
        std::cerr << "ERROR: Already reached EOF without loading data" << std::endl;
        return false;
    }
    else if (audioData.File->fail())
    {
        std::cerr << "ERROR: Fail bit set" << std::endl;
        return false;
    }
    else if (!audioData.File)
    {
        std::cerr << "ERROR: file is false" << std::endl;
        return false;
    }

    char* data = new char[BUFFER_SIZE];

    for (std::uint8_t i = 0; i < NUM_BUFFERS; ++i)
    {
        std::int32_t dataSoFar = 0;
        while (dataSoFar < BUFFER_SIZE)
        {
            std::int32_t result = ov_read(&audioData.OggVorbisFile, &data[dataSoFar], BUFFER_SIZE - dataSoFar, 0, 2, 1, &audioData.OggCurrentSection);
            if (result == OV_HOLE)
            {
                std::cerr << "ERROR: OV_HOLE found in initial read of buffer " << i << std::endl;
                break;
            }
            else if (result == OV_EBADLINK)
            {
                std::cerr << "ERROR: OV_EBADLINK found in initial read of buffer " << i << std::endl;
                break;
            }
            else if (result == OV_EINVAL)
            {
                std::cerr << "ERROR: OV_EINVAL found in initial read of buffer " << i << std::endl;
                break;
            }
            else if (result == 0)
            {
                std::cerr << "ERROR: EOF found in initial read of buffer " << i << std::endl;
                break;
            }

            dataSoFar += result;
        }

        if (audioData.Channels == 1 && audioData.BitRate == 8)
            audioData.Format = AL_FORMAT_MONO8;
        else if (audioData.Channels == 1 && audioData.BitRate == 16)
            audioData.Format = AL_FORMAT_MONO16;
        else if (audioData.Channels == 2 && audioData.BitRate == 8)
            audioData.Format = AL_FORMAT_STEREO8;
        else if (audioData.Channels == 2 && audioData.BitRate == 16)
            audioData.Format = AL_FORMAT_STEREO16;
        else
        {
            std::cerr << "ERROR: unrecognised ogg Format: " << audioData.Channels << " Channels, " << audioData.BitRate << " bps" << std::endl;
            delete[] data;
            return false;
        }

        alCall(alBufferData, audioData.Buffers[i], audioData.Format, data, dataSoFar, audioData.SampleRate);
    }

    alCall(alSourceQueueBuffers, audioData.Source, NUM_BUFFERS, &audioData.Buffers[0]);

    delete[] data;

    return true;
}

void SoundManager::PlayStream(const StreamingAudioData& audioData)
{
    alCall(alSourceStop, audioData.Source);
    alCall(alSourcePlay, audioData.Source);

    streams.push_back(audioData);
}

void SoundManager::UpdateStream(StreamingAudioData& audioData)
{
    ALint buffersProcessed = 0;
    alCall(alGetSourcei, audioData.Source, AL_BUFFERS_PROCESSED, &buffersProcessed);
    if (buffersProcessed <= 0)
    {
        return;
    }
    while (buffersProcessed--)
    {
        ALuint buffer;
        alCall(alSourceUnqueueBuffers, audioData.Source, 1, &buffer);

        char* data = new char[BUFFER_SIZE];
        std::memset(data, 0, BUFFER_SIZE);

        ALsizei dataSizeToBuffer = 0;
        std::int32_t sizeRead = 0;

        while (sizeRead < BUFFER_SIZE)
        {
            std::int32_t result = ov_read(&audioData.OggVorbisFile, &data[sizeRead], BUFFER_SIZE - sizeRead, 0, 2, 1, &audioData.OggCurrentSection);
            if (result == OV_HOLE)
            {
                std::cerr << "ERROR: OV_HOLE found in update of buffer " << std::endl;
                break;
            }
            else if (result == OV_EBADLINK)
            {
                std::cerr << "ERROR: OV_EBADLINK found in update of buffer " << std::endl;
                break;
            }
            else if (result == OV_EINVAL)
            {
                std::cerr << "ERROR: OV_EINVAL found in update of buffer " << std::endl;
                break;
            }
            else if (result == 0)
            {
                std::int32_t seekResult = ov_raw_seek(&audioData.OggVorbisFile, 0);
                if (seekResult == OV_ENOSEEK)
                    std::cerr << "ERROR: OV_ENOSEEK found when trying to loop" << std::endl;
                else if (seekResult == OV_EINVAL)
                    std::cerr << "ERROR: OV_EINVAL found when trying to loop" << std::endl;
                else if (seekResult == OV_EREAD)
                    std::cerr << "ERROR: OV_EREAD found when trying to loop" << std::endl;
                else if (seekResult == OV_EFAULT)
                    std::cerr << "ERROR: OV_EFAULT found when trying to loop" << std::endl;
                else if (seekResult == OV_EOF)
                    std::cerr << "ERROR: OV_EOF found when trying to loop" << std::endl;
                else if (seekResult == OV_EBADLINK)
                    std::cerr << "ERROR: OV_EBADLINK found when trying to loop" << std::endl;

                if (seekResult != 0)
                {
                    std::cerr << "ERROR: Unknown error in ov_raw_seek" << std::endl;
                    return;
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

        if (dataSizeToBuffer < BUFFER_SIZE)
        {
            std::cout << "Data missing" << std::endl;
        }

        ALint state;
        alCall(alGetSourcei, audioData.Source, AL_SOURCE_STATE, &state);
        if (state != AL_PLAYING)
        {
            alCall(alSourceStop, audioData.Source);
            alCall(alSourcePlay, audioData.Source);
        }

        delete[] data;
    }
}

void SoundManager::StopStream(const StreamingAudioData& audioData)
{
    alCall(alSourceStop, audioData.Source);
    
    // Remove the audio stream from the vector if it has been stopped.
    streams.erase(std::remove(streams.begin(), streams.end(), audioData), streams.end());
}
