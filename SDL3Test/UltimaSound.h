#pragma once

#include <SDL3_mixer/SDL_mixer.h>
#include <vector>
#include <string_view>

class U3Audio
{
public:
	explicit U3Audio();
	~U3Audio();

	bool initMusic();
	void playMusic(int song);
	void playSfx(int sfx);
	void stopMusic();
	void pauseMusic();
	void stopSfx();

	std::vector<Mix_Music*> m_music;
	std::vector<Mix_Chunk*> m_sfx;
private:
	
};
