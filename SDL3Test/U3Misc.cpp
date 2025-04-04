#include "U3Misc.h"
#include "UltimaGraphics.h"
#include "U3ScrollArea.h"
#include "U3Resources.h"
#include "UltimaIncludes.h"
#include <SDL3/SDL.h>

extern U3Resources m_resources;
extern U3Graphics m_graphics;
extern U3ScrollArea m_scrollArea;

U3Misc::U3Misc() :
	m_WhirlX(0),
	m_WhirlY(0),
	m_WhirlDX(0),
	m_WhirlDY(0),
	m_xpos(0),
	m_ypos(0),
	m_mapSize(0),
	m_mapOffset(0),
	m_stx(0),
	m_sty(0),
	m_gBallTileBackground(0),
	m_gHorseFacingEast(false),
	m_gMonVarType(0),
	m_gMonType(0),
	m_cHide(0),
	m_gResurrect(false),
	m_WindDir(0),
	m_heading(0)
{
	memset(m_gShapeSwapped, 0, sizeof(bool) * 256);
	memset(m_Player, NULL, sizeof(char) * (21 * 65));
	m_saved_map.reset();
}

U3Misc::~U3Misc()
{
	m_saved_map.reset();
}

void U3Misc::OpenRstr()
{
	std::filesystem::path currentPath = std::filesystem::current_path();
	currentPath /= ResourceLoc;
	currentPath /= SaveLoc;

	//std::filesystem::path partyPath = currentPath / "PARTY.ULT";
	std::filesystem::path rosterPath = currentPath / "Roster.ult";
	//std::filesystem::path sosariaPath = currentPath / "SOSARIA.ULT";

	std::filesystem::path resourcePath = std::filesystem::current_path();
	resourcePath /= ResourceLoc;
	resourcePath /= BinLoc;

	std::vector<std::string> orig_files = {
		"Sosaria Monsters.ult",
		"Preferences.ult",
		"MainResources.rsrc_MAPS_420_Sosaria.bin",
		"Sosaria Current.ult",
		"MainResources.rsrc_PRTY_500_Party.bin",
		"Party.ult",
		"MainResources.rsrc_MISC_400_Moongate Locations.bin",
		"Moongate Locations.ult",
		"MainResources.rsrc_MISC_401_Type Initial Table.bin",
		"Type Initial Table.ult",
		"MainResources.rsrc_MISC_402_Weapon Use By Class.bin",
		"Weapon Use By class.ult",
		"MainResources.rsrc_MISC_403_Armour Use By Class.bin",
		"Armour Use By class.ult",
		"MainResources.rsrc_MISC_404_Location Table.bin",
		"Location Table.ult",
		"MainResources.rsrc_MISC_405_Experience Table.bin",
		"Experience Table.ult",
		"MainResources.rsrc_ROST_500_Roster.bin",
		"Roster.ult"
	};

	try
	{
		if (!std::filesystem::is_directory(currentPath))
		{

			bool valid = std::filesystem::create_directory(currentPath);
			if (!valid)
			{
				return;
			}

		}
		/*if (std::filesystem::exists(rosterPath))
		{
			loadRoster(rosterPath);
		}*/
		// New game, so copy stuff
		std::vector<char> dummy;
		if (!std::filesystem::exists(rosterPath))
		{
			std::filesystem::path monsterFile = currentPath / orig_files[0];
			dummy.resize(256);
			SDL_IOStream* file = SDL_IOFromFile(monsterFile.string().c_str(), "wb");
			SDL_WriteIO(file, dummy.data(), dummy.size());
			SDL_CloseIO(file);
			dummy.clear();
			dummy.resize(32);
			memset(dummy.data(), 1, sizeof(char) * 8);
			std::filesystem::path preferencesFile = currentPath / orig_files[1];
			file = SDL_IOFromFile(preferencesFile.string().c_str(), "wb");
			SDL_WriteIO(file, dummy.data(), dummy.size());
			SDL_CloseIO(file);
			for (int index = 2; index < orig_files.size(); index += 2)
			{
				std::filesystem::path sosariaFileOrig = resourcePath / orig_files[index];
				std::filesystem::path sosariaFileCurrent = currentPath / orig_files[index + 1];
				std::filesystem::copy(sosariaFileOrig, sosariaFileCurrent, std::filesystem::copy_options::overwrite_existing);
			}
		}
	}
	catch (std::exception)
	{
		return;
	}
	return;
}

void U3Misc::GetMiscStuff(bool defaultData)
{
	std::vector<std::string> orig_files = {
		"Moongate Locations.ult",
		"Type Initial Table.ult",
		"Weapon Use By class.ult",
		"Armour Use By class.ult",
		"Location Table.ult",
		"Experience Table.ult",
		"MainResources.rsrc_MISC_400_Moongate Locations.bin",
		"MainResources.rsrc_MISC_401_Type Initial Table.bin",
		"MainResources.rsrc_MISC_402_Weapon Use By Class.bin",
		"MainResources.rsrc_MISC_403_Armour Use By Class.bin",
		"MainResources.rsrc_MISC_404_Location Table.bin",
		"MainResources.rsrc_MISC_405_Experience Table.bin"
	};
	unsigned short byte;
	std::filesystem::path moongateFile;
	std::filesystem::path typeFile;
	std::filesystem::path weaponFile;
	std::filesystem::path armourFile;
	std::filesystem::path locationFile;
	std::filesystem::path experienceFile;

	std::filesystem::path currentPath = std::filesystem::current_path();
	currentPath /= ResourceLoc;
	if (defaultData)
	{
		currentPath /= BinLoc;
		moongateFile = currentPath / orig_files[6];
		typeFile = currentPath / orig_files[7];
		weaponFile = currentPath / orig_files[8];
		armourFile = currentPath / orig_files[9];
		locationFile = currentPath / orig_files[10];
		experienceFile = currentPath / orig_files[11];
	}
	else
	{
		currentPath /= SaveLoc;
		moongateFile = currentPath / orig_files[0];
		typeFile = currentPath / orig_files[1];
		weaponFile = currentPath / orig_files[2];
		armourFile = currentPath / orig_files[3];
		locationFile = currentPath / orig_files[4];
		experienceFile = currentPath / orig_files[5];
	}

	std::uintmax_t file_size = std::filesystem::file_size(moongateFile);
	std::vector<unsigned char> dummy;

	memset(m_careerTable, 0, sizeof(char) * 12);
	memset(m_wpnUseTable, 0, sizeof(char) * 12);
	memset(m_armUseTable, 0, sizeof(char) * 12);
	memset(m_Experience, 0, sizeof(char) * 17);

	dummy.resize(file_size);
	SDL_IOStream* file = SDL_IOFromFile(moongateFile.string().c_str(), "rb");
	SDL_ReadIO(file, dummy.data(), dummy.size());
	SDL_CloseIO(file);

	for (byte = 0; byte < 8; byte++)
	{
		m_MoonXTable[byte] = dummy[byte];
		m_MoonYTable[byte] = dummy[byte + 8];
	}

	file_size = std::filesystem::file_size(typeFile);
	dummy.resize(file_size);
	file = SDL_IOFromFile(typeFile.string().c_str(), "rb");
	SDL_ReadIO(file, dummy.data(), dummy.size());
	SDL_CloseIO(file);
	if (dummy.size() < 12)
	{
		memcpy(m_careerTable, dummy.data(), 12);
	}

	file_size = std::filesystem::file_size(weaponFile);
	dummy.resize(file_size);
	file = SDL_IOFromFile(weaponFile.string().c_str(), "rb");
	SDL_ReadIO(file, dummy.data(), dummy.size());
	SDL_CloseIO(file);
	if (dummy.size() < 12)
	{
		memcpy(m_wpnUseTable, dummy.data(), 12);
	}

	file_size = std::filesystem::file_size(armourFile);
	dummy.resize(file_size);
	file = SDL_IOFromFile(armourFile.string().c_str(), "rb");
	SDL_ReadIO(file, dummy.data(), dummy.size());
	SDL_CloseIO(file);
	if (dummy.size() < 12)
	{
		memcpy(m_armUseTable, dummy.data(), 12);
	}

	file_size = std::filesystem::file_size(locationFile);
	dummy.resize(file_size);
	file = SDL_IOFromFile(locationFile.string().c_str(), "rb");
	SDL_ReadIO(file, dummy.data(), dummy.size());
	SDL_CloseIO(file);

	for (byte = 0; byte < 20; byte++)
	{
		m_LocationX[byte] = dummy[byte];
		m_LocationY[byte] = dummy[byte + 32];
	}

	file_size = std::filesystem::file_size(experienceFile);
	dummy.resize(file_size);
	file = SDL_IOFromFile(experienceFile.string().c_str(), "rb");
	SDL_ReadIO(file, dummy.data(), dummy.size());
	SDL_CloseIO(file);
	if (dummy.size() < 17)
	{
		memcpy(m_Experience, dummy.data(), 12);
	}
}

bool U3Misc::GetRoster()
{
	std::filesystem::path rosterPath = std::filesystem::current_path();
	rosterPath /= ResourceLoc;
	rosterPath /= SaveLoc;
	rosterPath /= std::string("Roster.ult");

	short player, byte;
	const size_t roster_size = 1280;
	unsigned char roster_data[roster_size];
	std::uintmax_t file_size = std::filesystem::file_size(rosterPath);

	if (file_size != roster_size)
	{
		return false;
	}
	SDL_IOStream* file = SDL_IOFromFile(rosterPath.string().c_str(), "rb");
	if (!file)
	{
		return false;
	}
	SDL_ReadIO(file, roster_data, roster_size);
	SDL_CloseIO(file);

	for (player = 0; player < 20; player++)
	{
		for (byte = 0; byte < 64; byte++)
		{
			m_Player[player + 1][byte] = *(roster_data + ((player) * 64) + byte);
		}
	}

	return true;
}

bool U3Misc::GetParty()
{
	std::filesystem::path partyPath = std::filesystem::current_path();
	partyPath /= ResourceLoc;
	partyPath /= SaveLoc;
	partyPath /= std::string("Party.ult");

	const size_t party_size = 64;
	std::uintmax_t file_size = std::filesystem::file_size(partyPath);

	if (file_size != party_size)
	{
		return false;
	}
	SDL_IOStream* file = SDL_IOFromFile(partyPath.string().c_str(), "rb");
	if (!file)
	{
		return false;
	}
	SDL_ReadIO(file, m_Party, party_size);
	SDL_CloseIO(file);

	m_xpos = m_Party[3];
	m_ypos = m_Party[4];

	return true;
}

void U3Misc::GetSosaria()
{
	LoadUltimaMap(19);
	BlockExodus();
}

void U3Misc::BlockExodus()
{
	if (m_Party[2] != 0)
	{
		return;
	}
	// TO DO
}

void U3Misc::LoadUltimaMap(int map)
{
	std::map<int, std::string> mapList = {
		{ 0, "MainResources.rsrc_MAPS_400_Lord British Castle" },
		{ 1, "MainResources.rsrc_MAPS_401_Exodus Castle" },
		{ 2, "MainResources.rsrc_MAPS_402_LCB Towne" },
		{ 3, "MainResources.rsrc_MAPS_403_Moon" },
		{ 4, "MainResources.rsrc_MAPS_404_Yew" },
		{ 5, "MainResources.rsrc_MAPS_405_East Montor" },
		{ 6, "MainResources.rsrc_MAPS_406_West Montor" },
		{ 7, "MainResources.rsrc_MAPS_407_Grey" },
		{ 8, "MainResources.rsrc_MAPS_408_Dawn" },
		{ 9, "MainResources.rsrc_MAPS_409_Devil Guard" },
		{ 10, "MainResources.rsrc_MAPS_410_Fawn" },
		{ 11, "MainResources.rsrc_MAPS_411_Death Gulch" },
		{ 12, "MainResources.rsrc_MAPS_412_Dungeon Doom" },
		{ 13, "MainResources.rsrc_MAPS_413_Dungeon Fires of Hell" },
		{ 14, "MainResources.rsrc_MAPS_414_Dungeon Time Awaits" },
		{ 15, "MainResources.rsrc_MAPS_415_Dungeon Clues" },
		{ 16, "MainResources.rsrc_MAPS_416_Dungeon Perinian Depths" },
		{ 17, "MainResources.rsrc_MAPS_417_Dungeon Mines of Morinia" },
		{ 18, "MainResources.rsrc_MAPS_418_Dungeon Dardin’s Pit" },
		{ 19, "Sosaria Current" },
		{ 20, "MainResources.rsrc_MAPS_420_Sosaria" },
		{ 21, "MainResources.rsrc_MAPS_421_Ambrosia" },
	};

	std::map<int, std::string> monsterList = {
		{ 0, "MainResources.rsrc_MONS_400" },
		{ 1, "MainResources.rsrc_MONS_401" },
		{ 2, "MainResources.rsrc_MONS_402" },
		{ 3, "MainResources.rsrc_MONS_403" },
		{ 4, "MainResources.rsrc_MONS_404" },
		{ 5, "MainResources.rsrc_MONS_405" },
		{ 6, "MainResources.rsrc_MONS_406" },
		{ 7, "MainResources.rsrc_MONS_407" },
		{ 8, "MainResources.rsrc_MONS_408" },
		{ 9, "MainResources.rsrc_MONS_409" },
		{ 10, "MainResources.rsrc_MONS_410" },
		{ 11, "MainResources.rsrc_MONS_411" },
		{ 19, "Sosaria Monsters" },
		{ 20, "MainResources.rsrc_MONS_420" },
		{ 21, "MainResources.rsrc_MONS_421" },
	};

	std::map<int, std::string> talkList = {
		{ 0, "MainResources.rsrc_TLKS_400_Lord British Castle" },
		{ 1, "MainResources.rsrc_TLKS_401_Exodus Castle" },
		{ 2, "MainResources.rsrc_TLKS_402_Lord British Towne" },
		{ 3, "MainResources.rsrc_TLKS_403_Moon" },
		{ 4, "MainResources.rsrc_TLKS_404_Yew" },
		{ 5, "MainResources.rsrc_TLKS_405_East Montor" },
		{ 6, "MainResources.rsrc_TLKS_406_West Montor" },
		{ 7, "MainResources.rsrc_TLKS_407_Grey" },
		{ 8, "MainResources.rsrc_TLKS_408_Dawn" },
		{ 9, "MainResources.rsrc_TLKS_409_Devil Guard" },
		{ 10, "MainResources.rsrc_TLKS_410_Fawn" },
		{ 11, "MainResources.rsrc_TLKS_411_Death Gulch" },
		{ 12, "MainResources.rsrc_TLKS_412_Dungeon Doom" },
		{ 13, "MainResources.rsrc_TLKS_413_Dungeon Fires of Hell" },
		{ 14, "MainResources.rsrc_TLKS_414_Dungeon Time Awaits" },
		{ 15, "MainResources.rsrc_TLKS_415_Dungeon Clues" },
		{ 16, "MainResources.rsrc_TLKS_416_Dungeon Perinian Depths" },
		{ 17, "MainResources.rsrc_TLKS_417_Dungeon Mines of Morinia" },
		{ 18, "MainResources.rsrc_TLKS_418_Dungeon Dardin's Pit" },
		{ 21, "MainResources.rsrc_TLKS_421_Ambrosia" },
	};

	if (mapList.find(map) == mapList.end())
	{
		return;
	}
	std::filesystem::path currentPath = std::filesystem::current_path();
	currentPath /= ResourceLoc;
	std::filesystem::path monsterPath = std::filesystem::current_path();
	monsterPath /= ResourceLoc;
	std::filesystem::path talkPath = std::filesystem::current_path();
	talkPath /= ResourceLoc;
	if (map == 19)
	{
		currentPath /= SaveLoc;
		currentPath /= mapList[map];
		currentPath += std::string(".ult");

		monsterPath /= SaveLoc;
		monsterPath /= monsterList[map];
		monsterPath += std::string(".ult");
	}
	else
	{
		currentPath /= BinLoc;
		currentPath /= mapList[map];
		currentPath += std::string(".bin");

		monsterPath /= BinLoc;
		monsterPath /= monsterList[map];
		monsterPath += std::string(".bin");
	}

	std::vector<unsigned char> map_data;
	std::vector<unsigned char> monster_data;

	try
	{
		std::uintmax_t file_size = std::filesystem::file_size(currentPath);
		map_data.resize(file_size);

		SDL_IOStream* file = SDL_IOFromFile(currentPath.string().c_str(), "rb");
		if (!file)
		{
			return;
		}
		SDL_ReadIO(file, map_data.data(), file_size);
		SDL_CloseIO(file);

		if (map > 11 && map < 19)
		{
			if (file_size != 2048)
			{
				return;
			}
			memcpy(m_Dungeon, map_data.data(), sizeof(unsigned char) * file_size);
		}
		else
		{
			if (map == 19 && file_size == 4100)
			{
				map_data.insert(map_data.begin(), 64);
				SDL_IOStream* file = SDL_IOFromFile(currentPath.string().c_str(), "wb");
				SDL_WriteIO(file, map_data.data(), map_data.size());
				SDL_CloseIO(file);
			}

			m_mapSize = map_data[0];
			if (m_mapSize == 0)
			{
				m_mapSize = 256;
			}
			int mapLength = m_mapSize * m_mapSize;
			m_map.resize(mapLength);
			if (map_data.size() < mapLength)
			{
				return;
			}
			std::copy(map_data.begin() + 1, map_data.begin() + 1 + mapLength, m_map.begin());

			file_size = std::filesystem::file_size(monsterPath);
			if (file_size == 256)
			{
				monster_data.resize(file_size);
				SDL_IOStream* file = SDL_IOFromFile(monsterPath.string().c_str(), "rb");
				SDL_ReadIO(file, monster_data.data(), file_size);
				SDL_CloseIO(file);

				memcpy(m_Monsters, monster_data.data(), sizeof(unsigned char) * file_size);
			}
			else
			{
				return;
			}
			if (map == 19)
			{
				m_WhirlX = map_data[mapLength + 1];
				m_WhirlY = map_data[mapLength + 2];
				m_WhirlDX = map_data[mapLength + 3];
				m_WhirlDY = map_data[mapLength + 4];
			}
		}
		if (map < 19 || map == 21) // "<419" *was* "<420", big mistake.
		{
			talkPath /= BinLoc;
			talkPath /= talkList[map];
			talkPath += std::string(".bin");
			std::vector<unsigned char> talk_data;

			std::uintmax_t file_size = std::filesystem::file_size(talkPath);
			if (file_size != 256)
			{
				return;
			}
			talk_data.resize(file_size);

			SDL_IOStream* file = SDL_IOFromFile(talkPath.string().c_str(), "rb");
			if (!file)
			{
				return;
			}
			SDL_ReadIO(file, talk_data.data(), file_size);
			SDL_CloseIO(file);

			memcpy(m_Talk, talk_data.data(), sizeof(unsigned char) * file_size);
		}

	}
	catch ([[maybe_unused]] const std::exception& e)
	{
		return;
	}
}

short U3Misc::MonsterHere(short x, short y)
{
	for (int index = 32; index >= 0; --index)
	{
		if (m_Monsters[index] == 0)
		{
			continue;
		}
		if (m_Monsters[index + XMON] == x && m_Monsters[index + YMON] == y)
		{
			return index;
		}
	}
	return 255;
}

unsigned char U3Misc::GetXYTile(short x, short y)
{
	if (x < 0 || x > 10 || y < 0 || y > 10)
	{
		return 0;
	}
	return m_resources.m_TileArray[y * 11 + x];
}

unsigned char U3Misc::GetXYVal(int x, int y)
{
	unsigned char value = 0;
	m_mapOffset = (m_graphics.MapConstrain(y) * m_mapSize);
	m_mapOffset += m_graphics.MapConstrain(x);
	value = m_map[m_mapOffset];
	if (m_Party[2] != 0)
	{
		if (x < 0 || x >= m_mapSize || y < 0 || y >= m_mapSize)
		{
			value = 0x04;
		}
	}
	return value;
}

void U3Misc::PutXYTile(short value, short x, short y)
{
	if (x < 0 || x > 10 || y < 0 || y > 10)
	{
		return;
	}
	m_resources.m_TileArray[y * 11 + x] = (unsigned char)value;
}

bool U3Misc::CheckAlive(short member) /* $75BA */
{
	short rosNum;
	rosNum = m_Party[member + 6];
	if (m_Player[rosNum][17] == 'G')
	{
		return true;
	}
	if (m_Player[rosNum][17] == 'P')
	{
		return true;
	}
	return false;
}

// 0123456789A
// FCWTPBLIDAR
short U3Misc::MaxMana(char rosNum)
{
	short value, tempNum;

	value = m_Player[rosNum][23];
	tempNum = 0;
	if (value == m_careerTable[2])
	{
		tempNum = m_Player[rosNum][20]; // Wizard
	}
	else if (value == m_careerTable[1]) // Cleric
	{
		tempNum = m_Player[rosNum][21];
	}
	else if ((value == m_careerTable[6]) || (value == m_careerTable[9]))  // Lark Alchemist
	{
		tempNum = m_Player[rosNum][20] / 2;
	}
	else if ((value == m_careerTable[4]) || (value == m_careerTable[7]))  // Paladin Illus
	{
		tempNum = m_Player[rosNum][21] / 2;
	}
	else if (value == m_careerTable[8])  // Druid (greater of both halves)
	{
		tempNum = m_Player[rosNum][20] / 2;
		if ((m_Player[rosNum][21] / 2) > tempNum)
		{
			tempNum = m_Player[rosNum][21] / 2;
		}
	}
	else if (value == m_careerTable[10])  // Ranger (lesser of both halves)
	{
		tempNum = m_Player[rosNum][20] / 2;
		if ((m_Player[rosNum][21] / 2) < tempNum)
		{
			tempNum = m_Player[rosNum][21] / 2;
		}
	}
	return tempNum;
}

bool U3Misc::ValidTrans(char value)
{
	if (m_Party[15])
	{
		return true;
	}
	bool cango = true;
	bool includeWind;
	m_resources.GetPreference(U3PreferencesType::Include_Wind, includeWind);
	if (m_Party[1] == 0x16 && includeWind)
	{
		if ((value != m_WindDir) && (m_WindDir != 0))
		{
			cango = true;
		}
		else
		{
			cango = false;
		}
	}

	return cango;
}

bool U3Misc::ValidDir(unsigned char value) // $4702
{
	char byte;
	char byte2;
	bool GoodPlace = false;

	if (m_Party[0] == 0x16) // Ship
	{
		if ((value == 00) || (value == 48))
		{
			GoodPlace = true; // Water or whirlpool OK
		}
	}
	else
	{
		if (value == 128) // Forcefield
		{
			// InverseTiles();
			for (byte = 0; byte < 4; byte++)
			{
				if (!(m_Player[m_Party[byte + 6]][14] & 0x10))
				{
					byte = 5;
				}
				else
				{
					GoodPlace = true;
				}
			}
		}
		else if (value == 132) // Lava
		{
			GoodPlace = true;
			for (byte = 0; byte < 4; byte++)
			{
				byte2 = m_Party[byte + 6];
				if (!(m_Player[byte2][14] & 0x20))
				{
					if ((m_Player[byte2][17] == 'G') || (m_Player[byte2][17] == 'P'))
					{
					}
				}
			}
		}
		else if ((value == 136) || (value == 248))
		{
			value = 4;
		}

		if ((value < 48) && (value != 0) && (value != 16))
		{
			GoodPlace = true;
			if (m_Party[0] == 20)
			{
			}
			else
			{
			}
		}
	}

return GoodPlace;
}

void U3Misc::North()
{
	m_scrollArea.UPrintMessage(24);
	if (ValidTrans(1) == false)
	{
		NoGo();
	}
	else
	{
		if (!ValidDir(GetXYVal(m_xpos, m_ypos - 1)))
		{
			NoGo();
		}
		else
		{
			m_ypos--;
			m_ypos = m_graphics.MapConstrain(m_ypos);
		}
	}
}

void U3Misc::South()
{
	m_scrollArea.UPrintMessage(25);
	if (ValidTrans(3) == false)
	{
		NoGo();
	}
	else
	{
		if (!ValidDir(GetXYVal(m_xpos, m_ypos + 1)))
		{
			NoGo();
		}
		else
		{
			m_ypos++;
			m_ypos = m_graphics.MapConstrain(m_ypos);
		}
	}
}

void U3Misc::East()
{
	m_scrollArea.UPrintMessage(26);
	if (ValidTrans(2) == false)
	{
		NoGo();
	}
	else
	{
		if (!ValidDir(GetXYVal(m_xpos + 1, m_ypos)))
		{
			NoGo();
		}
		else
		{
			m_xpos++;
			m_xpos = m_graphics.MapConstrain(m_xpos);
		}
	}
}

void U3Misc::West()
{
	m_scrollArea.UPrintMessage(27);
	if (ValidTrans(4) == false)
	{
		NoGo();
	}
	else
	{
		if (!ValidDir(GetXYVal(m_xpos - 1, m_ypos)))
		{
			NoGo();
		}
		else
		{
			m_xpos--;
			m_xpos = m_graphics.MapConstrain(m_xpos);
		}
	}
}

void U3Misc::SouthEast()
{
	m_scrollArea.UPrintMessage(251);
	if (ValidTrans(2) == false || ValidTrans(3) == false)
	{
		NoGo();
	}
	else
	{
		if (!ValidDir(GetXYVal(m_xpos, m_ypos + 1)))
		{
			NoGo();
		}
		else
		{
			m_ypos++;
			m_ypos = m_graphics.MapConstrain(m_ypos);
		}
	}
}


void U3Misc::SouthWest()
{
	m_scrollArea.UPrintMessage(25);
	if (ValidTrans(3) == false || ValidTrans(4) == false)
	{
		NoGo();
	}
	else
	{
		if (!ValidDir(GetXYVal(m_xpos, m_ypos + 1)))
		{
			NoGo();
		}
		else
		{
			m_ypos++;
			m_ypos = m_graphics.MapConstrain(m_ypos);
		}
	}
}


void U3Misc::NorthEast()
{
	m_scrollArea.UPrintMessage(25);
	if (ValidTrans(1) == false || ValidTrans(2) == false)
	{
		NoGo();
	}
	else
	{
		if (!ValidDir(GetXYVal(m_xpos, m_ypos + 1)))
		{
			NoGo();
		}
		else
		{
			m_ypos++;
			m_ypos = m_graphics.MapConstrain(m_ypos);
		}
	}
}


void U3Misc::NorthWest()
{
	m_scrollArea.UPrintMessage(25);
	if (ValidTrans(1) == false || ValidTrans(4) == false)
	{
		NoGo();
	}
	else
	{
		if (!ValidDir(GetXYVal(m_xpos, m_ypos + 1)))
		{
			NoGo();
		}
		else
		{
			m_ypos++;
			m_ypos = m_graphics.MapConstrain(m_ypos);
		}
	}
}

void U3Misc::NoGo()
{
	m_scrollArea.UPrintMessage(116);
}

void U3Misc::LetterCommand(SDL_Keycode key)
{
	switch (key)
	{
	case SDLK_A:
		break;
	case SDLK_B:
		break;
	case SDLK_C:
		break;
	case SDLK_D:
		break;
	case SDLK_E:
		Enter();
		break;
	case SDLK_F:
		break;
	case SDLK_G:
		break;
	case SDLK_H:
		break;
	case SDLK_I:
		break;
	case SDLK_J:
		break;
	case SDLK_K:
		break;
	case SDLK_L:
		break;
	case SDLK_M:
		break;
	case SDLK_N:
		break;
	case SDLK_O:
		break;
	case SDLK_P:
		break;
	case SDLK_Q:
		break;
	case SDLK_R:
		break;
	case SDLK_S:
		break;
	case SDLK_T:
		break;
	case SDLK_U:
		break;
	case SDLK_V:
		break;
	case SDLK_W:
		break;
	case SDLK_X:
		break;
	case SDLK_Y:
		break;
	case SDLK_Z:
		break;
	default:
		break;
	}
}

void U3Misc::HandleKeyPress(SDL_Keycode key)
{
	if (key >= SDLK_A && key <= SDLK_Z)
	{
		LetterCommand(key);
	}
	else
	{
		switch (key)
		{
		case SDLK_UP:
			North();
			break;
		case SDLK_DOWN:
			South();
			break;
		case SDLK_LEFT:
			West();
			break;
		case SDLK_RIGHT:
			East();
			break;
		default:
			break;
		}

		Routine6E35();
	}
}

void U3Misc::ProcessEvent(SDL_Event event)
{
	bool quit = false;
	bool gInterrupt = false;
	bool updateMouse = false;
	int mouseState = 0;

	switch (event.type)
	{
	case SDL_EVENT_QUIT:
		quit = true;
		break;
	case SDL_EVENT_KEY_DOWN:
		if (!(event.key.mod & SDL_KMOD_ALT) && !(event.key.mod & SDL_KMOD_CTRL) && !(event.key.mod & SDL_KMOD_GUI))
		{
			HandleKeyPress(event.key.key);
		}
		break;
	case SDL_EVENT_MOUSE_BUTTON_DOWN:
		mouseState = 1;
		updateMouse = true;
		break;
	case SDL_EVENT_MOUSE_BUTTON_UP:
		mouseState = 2;
		updateMouse = true;
		break;
	case SDL_EVENT_MOUSE_MOTION:
		mouseState = 0;
		updateMouse = true;
		break;
	default:
		break;
	}
}

void U3Misc::What() // $5135
{
	m_scrollArea.UPrintMessage(106);
}

void U3Misc::What2() // $5279
{
	m_scrollArea.UPrintMessage(107);
}

void U3Misc::NotHere() // $5288
{
	m_scrollArea.UPrintMessage(108);
}

void U3Misc::Enter()
{
	short x;
	short placeNum;
	short tile;
	unsigned char newval = 0;
	std::string dispString;
	std::string addString;

	if (m_Party[2] != 0xFF && m_Party[2] != 0x00)
	{
		m_scrollArea.UPrintMessage(33);
		What();
		return;
	}
	else if (m_Party[2] == 0x00) // 32 was 19
	{
		m_Party[3] = m_xpos;
		m_Party[4] = m_ypos;
		placeNum = -1;
		for (x = 0; x < 32; x++)
		{
			if (m_xpos == m_LocationX[x] && m_ypos == m_LocationY[x])
			{
				placeNum = x;
			}
		}
		if (placeNum < 0)
		{
			m_scrollArea.UPrintMessage(33);
			What2();
			return;
		}
		m_zp[0xE3] = m_xpos;
		m_zp[0xE4] = m_ypos;

		tile = GetXYVal(m_xpos, m_ypos) / 2;
		switch (tile)
		{
		case 0x0A:// Dungeon
			dispString = m_resources.m_plistMap["Messages"][32];
			addString = m_resources.m_plistMap["Messages"][33];
			newval = 1;
			m_xpos = 1;
			m_ypos = 1;
			m_heading = 1;
			break;
		case 0x0C: // Town
			dispString = m_resources.m_plistMap["Messages"][32];
			addString = m_resources.m_plistMap["Messages"][34];
			newval = 2;
			m_xpos = 1;
			m_ypos = 32;
			m_heading = 2;
			break;
		case 0x0E: // Castle
			dispString = m_resources.m_plistMap["Messages"][32];
			addString = m_resources.m_plistMap["Messages"][35];
			newval = 3;
			m_xpos = 32;
			m_ypos = 62;
			m_zp[0x0F] = 2;
			break;
		default:
			What2();
			return;
		}
		dispString += addString;
		m_scrollArea.UPrintWin(dispString);
	}
	else if (m_Party[2] == 0xFF) // Ambrosia
	{
		if (GetXYVal(m_xpos, m_ypos) != 0xF8)
		{
			m_scrollArea.UPrintMessage(33);
			What2();
			return;
		}
		dispString = m_resources.m_plistMap["Messages"][32];
		addString = m_resources.m_plistMap["Messages"][36];
		dispString += addString;
		m_scrollArea.UPrintWin(dispString);

		return;
	}
	bool autosave;
	m_resources.GetPreference(U3PreferencesType::AutoSave, autosave);
	if (autosave)
	{
		PushSosaria();
	}
	else
	{
		PushSosaria();
	}

	if (placeNum > 18)
	{
		LoadUltimaMap(placeNum + 3);
	}
	else
	{
		LoadUltimaMap(placeNum);
	}
	m_Party[2] = newval;

	if (m_Party[2] == 1) // Dungeon
	{
		return;
	}
	if (m_Party[2] == 2) // Town
	{
		return;
	}
	if (placeNum != 1)
	{
		return;
	}
	if (m_Party[15] == 1)
	{
	}

	
}

void U3Misc::IncMoves() // $3AF
{
	m_Party[10] += m_Party[1];
	if (m_Party[10] > 99)
	{
		m_Party[10] -= 100;
		m_Party[11]++;
		if (m_Party[11] > 99)
		{
			m_Party[11] -= 100;
			m_Party[12]++;
			if (m_Party[12] > 99)
			{
				m_Party[12] -= 100;
				m_Party[13]++;
				if (m_Party[13] > 99)
				{
					m_Party[10] = m_Party[11] = m_Party[12] = m_Party[13] = 99;
				}
			}
		}
	}
}

void U3Misc::Routine6E6B()
{
	m_xpos = m_zp[0xE3];
	m_ypos = m_zp[0xE4];
	m_Party[2] = 0;    // back to surface
	//if (!gResurrect)
	{
		m_scrollArea.UPrintMessage(182);
	}
	m_Party[3] = m_xpos;
	m_Party[4] = m_ypos;

	bool autosave;
	m_resources.GetPreference(U3PreferencesType::AutoSave, autosave);
	if (autosave)
	{
		PullSosaria();
	}
	else
	{
		PullSosaria();
	}
}

void U3Misc::Routine6E35()
{
	short temp;
	IncMoves();

	//MoonGateUpdate();
	if (m_Party[2] == 1)
	{
		//DrawDungeon();
		//DungeonStart(1);
		return;
	}
	// if $E2=#$80 (combat?), see $6E5C.  Apparently unneccessary
	//   code, since the combat routines never touch this area while in effect.
	if (m_Party[2] > 1) // Town or castle, IOW
	{
		if (m_xpos == 0 || m_ypos == 0)
		{
			Routine6E6B();
		}
	}
	//AgeChars();
	//ShowChars(false);
	temp = GetXYVal(m_xpos, m_ypos);
	/*if (temp == 136)
	{
		HandleMoonStep();
	}
	if (temp == 48)
	{
		GoWhirlPool();
	}
	if (ExodusCastle() == 0)
	{
		gTimeNegate = 0;
		xs = RandNum(0, 11);
		ys = RandNum(0, 11);
		gBallTileBackground = GetXYTile(xs, ys);
		if (xs == 5 && ys == 5)
		{
			PutXYTile(0x7A, xs, ys);
			DrawTiles();
			DrawMapPause();
			BombTrap();
			DrawMap(xpos, ypos);
		}
		else
		{
			if (gBallTileBackground == 0x10)
			{
				PutXYTile(0x7A, xs, ys);
				DrawTiles();
				PlaySoundFile(CFSTR("Hit"), FALSE);    // was 0xF7
				DrawMap(xpos, ypos);
			}
		}
	}
	*/
	FinishAll();
}

void U3Misc::FinishAll() // $79DD
{
	//m_tx = 0x18;
	//m_ty = 0x17;
	if (m_Party[0] == 0x14 || m_Party[0] == 0x16)
	{
		m_zp[0xCD] = 255 - m_zp[0xCD];
		if (m_zp[0xCD] < 128)
		{
			return;
		}
	}
	/*if (gTimeNegate != 0)
	{
		gTimeNegate--;
		return;
	}
	SpawnMonster();
	MoveMonsters();*/
}

void U3Misc::PullSosaria()
{
	if (!m_saved_map.get()) // was never pushed!
	{
		GetSosaria();
	}
	else // oh, yes it was!  Don't tease!
	{
		m_map = m_saved_map->Map;
		memcpy(m_Monsters, m_saved_map->Monsters, sizeof(unsigned char) * 256);
		m_WhirlDX = m_saved_map->WhirlDX;
		m_WhirlDY = m_saved_map->WhirlDY;
		m_WhirlX = m_saved_map->WhirlX;
		m_WhirlY = m_saved_map->WhirlY;

		m_saved_map.release();
	}
}

void U3Misc::PushSosaria()
{
	m_saved_map = std::make_unique<SosariaHandle>();
	m_saved_map->Map = m_map;
	memcpy(m_saved_map->Monsters, m_Monsters, sizeof(unsigned char) * 256);
	m_saved_map->WhirlDX = m_WhirlDX & 0xFF;
	m_saved_map->WhirlDY = m_WhirlDY & 0xFF;
	m_saved_map->WhirlX = m_WhirlX & 0xFF;
	m_saved_map->WhirlY = m_WhirlY & 0xFF;
}

