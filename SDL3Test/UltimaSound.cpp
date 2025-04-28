#include "UltimaSound.h"
#include <string>
#include <filesystem>

U3Audio::U3Audio()
{
	m_music.resize(10);
	m_sfx.resize(36);
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
	for (auto& curSfx : m_sfx)
	{
		if (curSfx)
		{
			Mix_FreeChunk(curSfx);
		}
	}
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
	};

	for (size_t index = 0; index < sfxList.size(); ++index)
	{
		std::filesystem::path currentPath = std::filesystem::current_path();
		currentPath /= ResourceLoc;
		currentPath /= SfxLoc;
		currentPath /= sfxList[index];

		m_sfx[index] = Mix_LoadWAV(currentPath.string().c_str());
		if (m_sfx[index] == nullptr)
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