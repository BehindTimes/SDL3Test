#include <filesystem>
#include <iostream>
#include <string>
#if HAVE_OPEN_AL
#include <fstream>
#include <sndfile.h>
#endif
#include "UltimaIncludes.h"
#include "UltimaSound.h"
#include "U3Misc.h"
#include "U3Resources.h"

extern std::unique_ptr<U3Misc> m_misc;
extern std::unique_ptr<U3Resources> m_resources;
extern std::unique_ptr<U3Audio> m_audio;

#if HAVE_SDL3_MIXER
static void musicFinished()
{
	m_audio->playNextSong();
}
#endif

#if HAVE_OPEN_AL
void U3Audio::checkIsPlaying()
{
    ALint state;
    if(m_playingSong >= 0)
    {
        if(m_playingSong > 0 && m_playingSong - 1 < m_music.size() && m_music[m_playingSong - 1].m_source > 0)
        {
            alGetSourcei(m_music[m_playingSong - 1].m_source, AL_SOURCE_STATE, &state);
            if(state == AL_STOPPED)
            {
                m_audio->playNextSong();
            }
        }
    }
}
#endif

void U3Audio::playNextSong()
{
	m_currentSong = m_nextSong;
	playMusic(m_currentSong);
}

U3Audio::U3Audio() :
	m_currentSong(0),
	m_nextSong(0),
	m_playingSong(0),
	m_cachedSong(0)
{
#if HAVE_SDL3_MIXER
	Mix_HookMusicFinished(musicFinished);
#elif HAVE_OPEN_AL
    m_device = nullptr;
    m_context = nullptr;
#endif
}

U3Audio::~U3Audio()
{
#if HAVE_SDL3_MIXER
	for (auto& curMusic : m_music)
	{
		if (curMusic)
		{
			Mix_FreeMusic(curMusic);
		}
	}
	m_music.clear();
	for (auto& curSfx : m_sfx)
	{
		if (curSfx)
		{
			Mix_FreeChunk(curSfx);
		}
	}
#elif HAVE_OPEN_AL
    for(auto& cur_music : m_music)
    {
        if(cur_music.m_source)
        {
            alDeleteSources(1, &cur_music.m_source);
            cur_music.m_source = 0;
        }
        if(cur_music.m_buffer)
        {
            alDeleteBuffers(1, &cur_music.m_buffer);
            cur_music.m_buffer = 0;
        }
    }
    m_music.clear();
    
    for(auto& cur_sfx : m_sfx)
    {
        if(cur_sfx.m_source)
        {
            alDeleteSources(1, &cur_sfx.m_source);
            cur_sfx.m_source = 0;
        }
        if(cur_sfx.m_buffer)
        {
            alDeleteBuffers(1, &cur_sfx.m_buffer);
            cur_sfx.m_buffer = 0;
        }
    }
    m_sfx.clear();
    
    if(m_context)
    {
        alcDestroyContext(m_context);
    }
    if(m_device)
    {
        alcCloseDevice(m_device);
    }
#endif
}

#if HAVE_OPEN_AL
bool U3Audio::read_wav_file(std::string fname, size_t index)
{
    ALenum error = 0;
    SNDFILE* inFile;
    SF_INFO inFileInfo;
    inFile = sf_open(fname.c_str(), SFM_READ, &inFileInfo);
    if (!inFile) {
        fprintf(stderr, "Error opening sound file: %s\n", sf_strerror(NULL));
        return false;
    }
    
    // make a buffer
    alGenBuffers(1, &m_sfx[index].m_buffer);
    
    // check for errors
    if((error = alGetError()) != AL_NO_ERROR)
    {
        fprintf(stderr, "Failed to generate sound buffer %d\n", error);
        sf_close(inFile);
        return false;
    }
    // Calculate buffer size based on frames, channels, and sample format
    size_t buffer_size = inFileInfo.frames * inFileInfo.channels * sizeof(short); // Assuming 16-bit PCM
    short *audio_data = (short *)malloc(buffer_size);
    if (!audio_data) {
        fprintf(stderr, "Error allocating memory.\n");
        sf_close(inFile);
        return false;
    }
    
    sf_readf_short(inFile, audio_data, inFileInfo.frames); // Read all frames
    
    ALenum format;
    if (inFileInfo.channels == 1) {
        format = AL_FORMAT_MONO16;
    }
    else if (inFileInfo.channels == 2) {
        format = AL_FORMAT_STEREO16;
    }
    else
    {
        fprintf(stderr, "Unsupported number of channels.\n");
        // Handle other formats or error
        free(audio_data);
        sf_close(inFile);
        return false;
    }
    
    // send data to openal, sfinfo.rate is your freq in Hz, dont assume 44100
    alBufferData(m_sfx[index].m_buffer, format, audio_data, (ALsizei)buffer_size, inFileInfo.samplerate);
    if ((error = alGetError()) != AL_NO_ERROR) {
        printf("Failed to send audio information buffer to OpenAL! 0x%06x\n", error);
        free(audio_data);
        sf_close(inFile);
        return false;
    }
    
    alGenSources(1, &m_sfx[index].m_source);
    
    // check for errors
    if((error = alGetError()) != AL_NO_ERROR) {
        fprintf(stderr, "Failed to generate sound buffer %d\n", error);
        free(audio_data);
        sf_close(inFile);
        return false;
    }
    
    alSourcei(m_sfx[index].m_source, AL_BUFFER, m_sfx[index].m_buffer);
    if((error = alGetError()) != AL_NO_ERROR) {
        fprintf(stderr, "Failed to generate sound source %d\n", error);
        free(audio_data);
        sf_close(inFile);
        return false;
    }
    
    free(audio_data);
    sf_close(inFile);
    
    
    return true;
}

bool U3Audio::read_ogg_file(std::string fname, size_t index)
{
    OggVorbis_File vf;
    ALenum error = 0;
    FILE* fp = 0;
    vorbis_info* vi = 0;
    ALenum format = 0;
    short* pcmout = 0;
    
    // open the file in read binary mode
#if defined(_MSC_VER)
    auto err = fopen_s(&fp, fname.c_str(), "rb");
    if(err != 0)
    {
        fprintf(stderr, "Could not open file `%s`\n", fname.c_str());
        return false;
    }
#else
    fp = fopen(fname.c_str(), "rb");
    if (fp == 0)
    {
        fprintf(stderr, "Could not open file `%s`\n", fname.c_str());
        return false;
    }
#endif
    
    // make a buffer
    alGenBuffers(1, &m_music[index].m_buffer);
    
    // check for errors
    if((error = alGetError()) != AL_NO_ERROR)
    {
        fprintf(stderr, "Failed to generate sound buffer %d\n", error);
        fclose(fp);
        return false;
    }
    
    // open the ogg vorbis file. This is a must on windows, do not use ov_open.
    // set OV_CALLBACKS_NOCLOSE else it will close your fp when ov_close() is reached, which is fine.
    if(ov_open_callbacks(fp, &vf, NULL, 0, OV_CALLBACKS_NOCLOSE) < 0) {
        fprintf(stderr, "Stream is not a valid OggVorbis stream!\n");
        fclose(fp);
        ov_clear(&vf);
        return false;
    }
    
    // fill vi with a new ogg vorbis info struct, determine audio format
    // audio format will always been a length of 16bits, vi->channels determines mono or stereo
    vi = ov_info(&vf, -1);
    format = vi->channels == 1 ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16;
    
    // data_len is the amount of data to read, allocate said data space
    // this is calculated by (samples * channels * 2 (aka 16bits))
    size_t data_len = ov_pcm_total(&vf, -1) * vi->channels * 2;
    pcmout = (short*)malloc(data_len);
    if(pcmout == 0) {
        fprintf(stderr, "Out of memory.\n");
        free(pcmout);
        fclose(fp);
        ov_clear(&vf);
        return false;
    }
    
    // fill pcmout buffer with ov_read data samples
    // you can't just slap data_len in place of 4096, it doesn't work that way
    // 0 is endianess, 0 for little, 1 for big
    // 2 is the data type short's size, mine is 2
    // 1 is the signedness you want, I want short not unsigned short (for openal) so 1
    for (size_t size = 0, offset = 0, sel = 0;
         (size = ov_read(&vf, (char*) pcmout + offset, 4096, 0, 2, 1, (int*) &sel)) != 0;
         offset += size) {
        if(size < 0)
        {
            puts("Faulty ogg file :o"); // use https://xiph.org/vorbis/doc/vorbisfile/ov_read.html for handling enums
        }
    }
    
    // send data to openal, vi->rate is your freq in Hz, dont assume 44100
    alBufferData(m_music[index].m_buffer, AL_FORMAT_MONO16, pcmout, (ALsizei)data_len, vi->rate);
    if ((error = alGetError()) != AL_NO_ERROR) {
        printf("Failed to send audio information buffer to OpenAL! 0x%06x\n", error);
        free(pcmout);
        fclose(fp);
        ov_clear(&vf);
        return false;
    }
    
    alGenSources(1, &m_music[index].m_source);
    
    // check for errors
    if((error = alGetError()) != AL_NO_ERROR) {
        fprintf(stderr, "Failed to generate sound buffer %d\n", error);
        free(pcmout);
        fclose(fp);
        ov_clear(&vf);
        return false;
    }
    
    alSourcei(m_music[index].m_source, AL_BUFFER, m_music[index].m_buffer);
    
    // free your resources
    free(pcmout);
    fclose(fp);
    ov_clear(&vf);
    return true;
}
#endif

bool U3Audio::initMusic()
{
    std::vector<std::string> musicList = {
        { "Song_1.ogg" },
        { "Song_2.ogg" },
        { "Song_3.ogg" },
        { "Song_4.ogg" },
        { "Song_5.ogg" },
        { "Song_6.ogg" },
        { "Song_7.ogg" },
        { "Song_8.ogg" },
        { "Song_A.ogg" },
        { "Song_B.ogg" }
    };
    
    std::vector<std::string> sfxList = {
        { "Alarm.wav" },
        { "Attack.wav" },
        { "BigDeath.wav" },
        { "Bump.wav" },
        { "CombatStart.wav" },
        { "CombatVictory.wav" },
        { "Creak.wav" },
        { "DeathFemale.wav" },
        { "DeathMale.wav" },
        { "Downwards.wav" },
        { "Error1.wav" },
        { "Error2.wav" },
        { "ExpLevelUp.wav" },
        { "FailedSpell.wav" },
        { "ForceField.wav" },
        { "Heal.wav" },
        { "Hit.wav" },
        { "Horsewalk.wav" },
        { "Immolate.wav" },
        { "Invocation.wav" },
        { "LBLevelRise.wav" },
        { "MiscSpell.wav" },
        { "MonsterSpell.wav" },
        { "Moongate.wav" },
        { "MountHorse.wav" },
        { "Ouch.wav" },
        { "Shoot.wav" },
        { "Shrine.wav" },
        { "Sink.wav" },
        { "Step.wav" },
        { "Swish1.wav" },
        { "Swish2.wav" },
        { "Swish3.wav" },
        { "Swish4.wav" },
        { "TorchIgnite.wav" },
        { "Upwards.wav" },
        { "Fade1.wav" },
        { "Fade2.wav" },
    };
    
#if HAVE_SDL3_MIXER
	
	m_music.resize(musicList.size());

	for (size_t index = 0; index < m_music.size(); ++index)
	{
		std::filesystem::path currentPath = m_resources->m_exePath;
		currentPath /= ResourceLoc;
		currentPath /= MusicLoc;
		currentPath /= Standard;
		currentPath /= musicList[index];

		m_music[index] = Mix_LoadMUS(currentPath.string().c_str());
		if (m_music[index] == nullptr)
		{
			return false;
		}
	}

	m_sfx.resize(sfxList.size());

	for (size_t index = 0; index < sfxList.size(); ++index)
	{
		std::filesystem::path currentPath = m_resources->m_exePath;
		currentPath /= ResourceLoc;
		currentPath /= SfxLoc;
		currentPath /= sfxList[index];

		m_sfx[index] = Mix_LoadWAV(currentPath.string().c_str());
		if (m_sfx[index] == nullptr)
		{
			return false;
		}
	}
#elif HAVE_OPEN_AL
    
    m_music.resize(musicList.size());
    m_sfx.resize(sfxList.size());
    
    
    m_device = alcOpenDevice(NULL);
    if (!m_device)
    {
        return false;
    }
    
    m_context = alcCreateContext(m_device, NULL);
    alcMakeContextCurrent(m_context);
    
    for(size_t index = 0; index < sfxList.size(); ++index)
    {
        std::filesystem::path currentPath = m_resources->m_exePath;
        currentPath /= ResourceLoc;
        currentPath /= SfxLoc;
        currentPath /= sfxList[index];
        
        read_wav_file(currentPath.string(), index);
    }
    
    for(size_t index = 0; index < musicList.size(); ++index)
    {
        std::filesystem::path currentPath = m_resources->m_exePath;
        currentPath /= ResourceLoc;
        currentPath /= MusicLoc;
        currentPath /= Standard;
        currentPath /= musicList[index];
        
        read_ogg_file(currentPath.string(), index);
    }
#endif
    
	return true;
}

void U3Audio::playMusic([[maybe_unused]] int song)
{
#if HAVE_SDL3_MIXER
	m_playingSong = song;
	song--;
	if (!m_resources->m_preferences.play_music)
	{
		return;
	}
	if (song >= 0 && song < m_music.size())
	{
		if (Mix_PausedMusic())
		{
			Mix_ResumeMusic();
		}
		else
		{
			Mix_PlayMusic(m_music[song], 0);
		}
	}
#elif HAVE_OPEN_AL
    m_playingSong = song;
    song--;
    if (!m_resources->m_preferences.play_music)
    {
        return;
    }
    if (song >= 0 && song < m_music.size())
    {
        
        /*if (Mix_PausedMusic())
        {
            Mix_ResumeMusic();
        }
        else
        {
            Mix_PlayMusic(m_music[song], 0);
        }*/
        if(m_music[song].m_source > 0)
        {
            alSourcePlay(m_music[song].m_source);
        }
    }
#endif
}

void U3Audio::stopSfx()
{
#if HAVE_SDL3_MIXER
	Mix_HaltChannel(-1);
#ifdef NDEBUG
	Mix_HaltMusic();
#endif
#elif HAVE_OPEN_AL
    for(auto& curSfx : m_sfx)
    {
        if(curSfx.m_source > 0)
        {
            alSourceStop(curSfx.m_source);
        }
    }
#endif
}


void U3Audio::stopMusic()
{
#if HAVE_SDL3_MIXER
	if (Mix_PausedMusic())
	{
		Mix_ResumeMusic();
	}
	Mix_HaltMusic();
#elif HAVE_OPEN_AL
    for(auto& curMusic : m_music)
    {
        if(curMusic.m_source > 0)
        {
            alSourceStop(curMusic.m_source);
        }
    }
#endif
}

void U3Audio::pauseMusic()
{
#if HAVE_SDL3_MIXER
	Mix_PauseMusic();
#endif
}

void U3Audio::playSfx([[maybe_unused]] int sfx)
{
#if HAVE_SDL3_MIXER
	if (!m_resources->m_preferences.play_sfx)
	{
		return;
	}
	if (m_sfx.size() <= sfx)
	{
		return;
	}
	Mix_PlayChannel(-1, m_sfx[sfx], 0);
#elif HAVE_OPEN_AL
    if (!m_resources->m_preferences.play_sfx)
    {
        return;
    }
    if (m_sfx.size() <= sfx)
    {
        return;
    }
    alSourcePlay(m_sfx[sfx].m_source);
#endif
}

void U3Audio::musicUpdate()
{
#if HAVE_SDL3_MIXER
	if (!m_resources->m_preferences.play_music)
	{
		stopMusic();
		return;
	}

	if (m_currentSong == 0 && m_nextSong == 0)
	{
		stopMusic();
		return;
	}
	if (m_playingSong != m_currentSong)
	{
		int tempsong = m_currentSong;
		stopMusic();
		m_currentSong = tempsong;
		playMusic(m_currentSong);
	}
	if (m_currentSong == 0)
	{
		m_currentSong = m_nextSong;
		playMusic(m_currentSong);
	}
	else if (Mix_PlayingMusic() == 0)
	{
		playMusic(m_currentSong);
	}
#elif HAVE_OPEN_AL
    if (!m_resources->m_preferences.play_music)
    {
        stopMusic();
        return;
    }

    if (m_currentSong == 0 && m_nextSong == 0)
    {
        stopMusic();
        return;
    }
    if (m_playingSong != m_currentSong)
    {
        int tempsong = m_currentSong;
        stopMusic();
        m_currentSong = tempsong;
        playMusic(m_currentSong);
    }
    if (m_currentSong == 0)
    {
        stopMusic();
        m_currentSong = m_nextSong;
        playMusic(m_currentSong);
    }
    else
    {
    }
    /*else if (Mix_PlayingMusic() == 0)
    {
        playMusic(m_currentSong);
    }*/
#endif
}

void U3Audio::setVolumeSfx(int volume)
{
#if HAVE_SDL3_MIXER
    if (volume >= 0 && volume <= 100)
    {
        float volume_percent = float(volume) / MIX_MAX_VOLUME;
        int new_volume = (int)(volume_percent * MIX_MAX_VOLUME);
        Mix_Volume(new_volume, -1);
    }
#elif HAVE_OPEN_AL
    if (volume >= 0 && volume <= 100)
    {
        float volume_percent = float(volume) / 100.0f;
        for (auto& curSfx : m_sfx)
        {
            if (curSfx.m_source >= 0)
            {
                alSourcef(curSfx.m_source, AL_GAIN, volume_percent);
            }
        }
    }
#endif
}

void U3Audio::setVolumeMusic(int volume)
{
#if HAVE_SDL3_MIXER
    if (volume >= 0 && volume <= 100)
    {
        float volume_percent = float(volume) / MIX_MAX_VOLUME;
        int new_volume = (int)(volume_percent * MIX_MAX_VOLUME);
        Mix_VolumeMusic(new_volume);
    }
#elif HAVE_OPEN_AL
    if (volume >= 0 && volume <= 100)
    {
        float volume_percent = float(volume) / 100.0f;
        for (auto& curMusic : m_music)
        {
            if (curMusic.m_source >= 0)
            {
                alSourcef(curMusic.m_source, AL_GAIN, volume_percent);
            }
        }
    }
#endif
}
