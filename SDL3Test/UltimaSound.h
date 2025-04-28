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
	void stopMusic();
	void pauseMusic();

	std::vector<Mix_Music*> m_music;
	std::vector<Mix_Chunk*> m_sfx;
private:
	static constexpr std::string_view ResourceLoc = "Resources";
	static constexpr std::string_view MusicLoc = "Music";
	static constexpr std::string_view SfxLoc = "Sounds";
	static constexpr std::string_view Standard = "Standard";
};
