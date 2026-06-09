#pragma once

#if HAVE_SDL3_MIXER
#include <SDL3_mixer/SDL_mixer.h>
#elif HAVE_OPEN_AL
#include <ogg/ogg.h>
#include <vorbis/codec.h>
#include <vorbis/vorbisfile.h>
#ifdef __APPLE__
#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#else
#include <AL/al.h>
#include <AL/alc.h>
#endif
#else
#endif


#include <vector>
#include <string_view>

#if HAVE_OPEN_AL
struct MusicData
{
	MusicData() :
		m_source(0),
		m_buffer(0)
	{

	}
	ALuint m_source;
	ALuint m_buffer;
};
#endif

class U3Audio
{
public:
	explicit U3Audio();
	~U3Audio();

	bool initMusic();
	
	void playSfx(int sfx);
	void stopMusic();
	void pauseMusic();
	void stopSfx();
	void musicUpdate();
	void playNextSong();
	void setVolumeSfx(int volume);
	void setVolumeMusic(int volume);
	void setCurrentSong(int song);
	void setNextSong(int song);
	void setCachedSong(int song);
	int getCurrentSong() const { return m_currentSong; }
	int getNextSong() const { return m_nextSong; }
	int getCachedSong() const { return m_cachedSong; }
	void changeMusic();
	void loadMusic();
	void checkIsPlaying();

#if HAVE_SDL3_MIXER
	MIX_Mixer* m_mixer;
	std::vector<MIX_Audio*> m_music;
	std::vector<MIX_Audio*> m_sfx;
	MIX_Track* m_mus_track;
	MIX_Track* m_sfx_track;
#elif HAVE_OPEN_AL
	
	bool read_wav_file(std::string fname, size_t index);
	bool read_ogg_file(std::string fname, size_t index);

	std::vector<MusicData> m_music;
	std::vector<MusicData> m_sfx;
#endif

	std::vector<std::string> m_themes;
	int m_currentTheme;
	bool m_changeMusic;

private:
	void initThemes();
	void playMusic(int song);

	int m_currentSong;
	int m_nextSong;
	int m_playingSong;
	int m_cachedSong;
	bool m_hasAudio;

#if HAVE_OPEN_AL
	ALCdevice* m_device;
	ALCcontext* m_context;

#endif
};
