#include "UltimaSound.h"
#include <string>
#include <filesystem>

U3Audio::U3Audio()
{
	m_music.resize(10);
}

U3Audio::~U3Audio()
{
	for (auto& curMusic : m_music)
	{
		if (curMusic)
		{
			Mix_FreeMusic(curMusic);
		}
	}
	m_music.clear();
}

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

	for (size_t index = 0; index < m_music.size(); ++index)
	{
		std::filesystem::path currentPath = std::filesystem::current_path();
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
	return true;
}

void U3Audio::playMusic([[maybe_unused]] int song)
{
#ifdef NDEBUG
	if (song >= 0 && song < m_music.size())
	{
		if (Mix_PausedMusic())
		{
			Mix_ResumeMusic();
		}
		else
		{
			Mix_PlayMusic(m_music[song], 1);
		}
	}
#endif
}

void U3Audio::stopMusic()
{
#ifdef NDEBUG
	Mix_HaltMusic();
#endif
}

void U3Audio::pauseMusic()
{
#ifdef NDEBUG
	Mix_PauseMusic();
#endif
}