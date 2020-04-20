#include "SoundManager.h"
#include <iostream>
#include <fstream>

SoundManager* SoundManager::s_pInstance = 0;

bool SoundManager::Load(std::string fileName, std::string id, SoundType type)
{
    return false;
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

    if (!loadWav("assets/sound/test.wav", &file)) { return; }

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
