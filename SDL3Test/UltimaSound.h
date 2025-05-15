#pragma once

#if HAVE_SDL3_MIXER
#include <SDL3_mixer/SDL_mixer.h>
#else
struct Mix_Music;
struct Mix_Chunk;
#endif

#include <vector>
#include <string_view>

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

	std::vector<Mix_Music*> m_music;
	std::vector<Mix_Chunk*> m_sfx;

	int m_currentSong;
	int m_nextSong;
	int m_playingSong;
	int m_cachedSong;
private:
	void playMusic(int song);
};
