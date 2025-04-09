#include "U3Misc.h"
#include "UltimaGraphics.h"
#include "U3ScrollArea.h"
#include "U3Resources.h"
#include "UltimaSpellCombat.h"
#include "UltimaIncludes.h"
#include "U3Utilities.h"
#include <SDL3/SDL.h>
#include <iostream>

extern U3Resources m_resources;
extern U3Graphics m_graphics;
extern U3ScrollArea m_scrollArea;
extern UltimaSpellCombat m_spellCombat;
extern U3Utilities m_utilities;

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
	m_heading(0),
	m_inputType(InputType::Default),
	m_xs(0),
	m_ys(0),
	m_dx(0),
	m_dy(0),
	m_transactNum(0),
	m_storedir(0),
	m_rosNum(0),
	m_numOnly(false),
	m_maxInputLength(2),
	m_input_num(0),
	m_opnum(0),
	m_opnum2(0),
	m_restrictedStart(0),
	m_gTorch(0),
	m_wx(0),
	m_wy(0)
{
	memset(m_gShapeSwapped, 0, sizeof(bool) * 256);
	memset(m_Player, NULL, sizeof(char) * (21 * 65));
	m_saved_map.reset();
	m_gMoonDisp[0] = 4;
	m_gMoonDisp[1] = 4;

	memset(m_zp, 0, sizeof(short) * 255);
	memset(m_MonsterX, 0, sizeof(unsigned char) * 8);
	memset(m_MonsterY, 0, sizeof(unsigned char) * 8);
	memset(m_MonsterTile, 0, sizeof(unsigned char) * 8);
	memset(m_MonsterHP, 0, sizeof(unsigned char) * 8);
	memset(m_CharX, 0, sizeof(unsigned char) * 4);
	memset(m_CharY, 0, sizeof(unsigned char) * 4);
	memset(m_CharTile, 0, sizeof(unsigned char) * 4);
	memset(m_CharShape, 0, sizeof(unsigned char) * 4);

	memset(m_MoonXTable, 0, sizeof(unsigned char) * 8);
	memset(m_MoonYTable, 0, sizeof(unsigned char) * 8);
	memset(m_careerTable, 0, sizeof(unsigned char) * 12);
	memset(m_wpnUseTable, 0, sizeof(unsigned char) * 12);
	memset(m_armUseTable, 0, sizeof(unsigned char) * 12);
	memset(m_LocationX, 0, sizeof(unsigned char) * 20);
	memset(m_LocationY, 0, sizeof(unsigned char) * 20);
	memset(m_Experience, 0, sizeof(unsigned char) * 17);
	memset(m_Monsters, 0, sizeof(unsigned char) * 256);
	memset(m_Talk, 0, sizeof(unsigned char) * 256);
	memset(m_Dungeon, 0, sizeof(unsigned char) * 2048);

	memset(careerTable, 0, sizeof(unsigned char) * 12);
	memset(m_Party, 0, sizeof(unsigned char) * 64);
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

void U3Misc::PutXYVal(unsigned char value, unsigned char x, unsigned char y)
{
	m_mapOffset = (m_graphics.MapConstrain(y) * m_mapSize);
	m_mapOffset += m_graphics.MapConstrain(x);
	m_map[m_mapOffset] = value;
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
			InverseTiles(true);
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
		Attack();
		break;
	case SDLK_B:
		Board();
		break;
	case SDLK_C:
		break;
	case SDLK_D:
		Descend();
		break;
	case SDLK_E:
		Enter();
		break;
	case SDLK_F:
		Fire();
		break;
	case SDLK_G:
		break;
	case SDLK_H:
		break;
	case SDLK_I:
		Ignite();
		break;
	case SDLK_J:
		JoinGold();
		break;
	case SDLK_K:
		Klimb();
		break;
	case SDLK_L:
		Look();
		break;
	case SDLK_M:
		break;
	case SDLK_N:
		break;
	case SDLK_O:
		break;
	case SDLK_P:
		PeerGem();
		break;
	case SDLK_Q:
		break;
	case SDLK_R:
		break;
	case SDLK_S:
		Steal();
		break;
	case SDLK_T:
		Transact();
		break;
	case SDLK_U:
		Unlock();
		break;
	case SDLK_V:
		break;
	case SDLK_W:
		break;
	case SDLK_X:
		Exit();
		break;
	case SDLK_Y:
		break;
	case SDLK_Z:
		break;
	default:
		break;
	}
}

void U3Misc::HandleTransactPress(SDL_Keycode key)
{
	m_input_num = -1;

	if (key >= SDLK_1 && key <= SDLK_4)
	{
		m_inputType = InputType::Default;
		m_input_num = key - SDLK_1;
		if (m_callbackStack.size() > 0)
		{
			auto callbackFunction = m_callbackStack.top();
			m_callbackStack.pop();
			if (callbackFunction)
			{
				callbackFunction();
			}
		}
	}
	else if (key == SDLK_SPACE || key == SDLK_RETURN)
	{
		m_inputType = InputType::Default;
		if (m_callbackStack.size() > 0)
		{
			auto callbackFunction = m_callbackStack.top();
			m_callbackStack.pop();
			if (callbackFunction)
			{
				callbackFunction();
			}
		}
	}
}

bool U3Misc::HandleDefaultKeyPress(SDL_Keycode key)
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
		case SDLK_SPACE:
			Pass();
			break;
		default:
			break;
		}

		Routine6E35();
	}
	return true;
}

void U3Misc::HandleDircetionKeyPress(SDL_Keycode key)
{
	short dirgot = false;

	switch (key)
	{
	case SDLK_UP:
		m_xs = m_xpos;
		m_ys = m_ypos - 1;
		m_dx = 0;
		m_dy = -1;
		dirgot = true;
		m_scrollArea.UPrintMessage(24);
		break;
	case SDLK_DOWN:
		m_xs = m_xpos;
		m_ys = m_ypos + 1;
		m_dx = 0;
		m_dy = 1;
		dirgot = true;
		m_scrollArea.UPrintMessage(25);
		break;
	case SDLK_LEFT:
		m_xs = m_xpos - 1;
		m_ys = m_ypos;
		m_dx = -1;
		m_dy = 0;
		dirgot = true;
		m_scrollArea.UPrintMessage(27);
		break;
	case SDLK_RIGHT:
		m_xs = m_xpos + 1;
		m_ys = m_ypos;
		m_dx = 1;
		m_dy = 0;
		dirgot = true;
		m_scrollArea.UPrintMessage(26);
		break;
	default:
		break;
	}
	if (dirgot)
	{
		m_inputType = InputType::Default;
		if (m_callbackStack.size() > 0)
		{
			auto callbackFunction = m_callbackStack.top();
			m_callbackStack.pop();
			if (callbackFunction)
			{
				callbackFunction();
			}
		}
	}
}

void U3Misc::HandleInputBuySell(SDL_Keycode key)
{
	bool handled = false;
	switch (key)
	{
	case SDLK_B:
		m_input_num = 1;
		m_input = std::string("B");
		handled = true;
		break;
	case SDLK_S:
		m_input_num = 0;
		m_input = std::string("S");
		handled = true;
		break;
	case SDLK_RETURN:
	case SDLK_SPACE:
		m_input_num = -1;
		m_input = std::string("");
		handled = true;
		break;
	default:
		m_input_num = 0;
		break;
	}
	if (handled)
	{
		m_inputType = InputType::Default;
		m_scrollArea.setInputString(m_input);
		m_scrollArea.setInput(false);
		if (m_callbackStack.size() > 0)
		{
			auto callbackFunction = m_callbackStack.top();
			m_callbackStack.pop();
			if (callbackFunction)
			{
				callbackFunction();
			}
		}
	}
}

void U3Misc::HandleInputGuild(SDL_Keycode key)
{
	bool handled = false;
	switch (key)
	{
	case SDLK_T:
		m_input_num = 0;
		m_input = std::string("T");
		handled = true;
		break;
	case SDLK_K:
		m_input_num = 1;
		m_input = std::string("K");
		handled = true;
		break;
	case SDLK_P:
		m_input_num = 2;
		m_input = std::string("P");
		handled = true;
		break;
	case SDLK_G:
		m_input_num = 3;
		m_input = std::string("G");
		handled = true;
		break;
	case SDLK_RETURN:
	case SDLK_SPACE:
		m_input_num = -1;
		m_input = std::string("");
		handled = true;
		break;
	default:
		m_input_num = 0;
		break;
	}
	if (handled)
	{
		m_inputType = InputType::Default;
		m_scrollArea.setInputString(m_input);
		m_scrollArea.setInput(false);
		if (m_callbackStack.size() > 0)
		{
			auto callbackFunction = m_callbackStack.top();
			m_callbackStack.pop();
			if (callbackFunction)
			{
				callbackFunction();
			}
		}
	}
}

void U3Misc::HandleInputYesNo(SDL_Keycode key)
{
	bool handled = false;
	switch (key)
	{
	case SDLK_Y:
		m_input_num = 1;
		m_input = std::string("Y");
		handled = true;
		break;
	case SDLK_N:
		m_input_num = 0;
		m_input = std::string("N");
		handled = true;
		break;
	case SDLK_RETURN:
	case SDLK_SPACE:
		m_input_num = -1;
		m_input = std::string("");
		handled = true;
		break;
	default:
		m_input_num = 0;
		break;
	}
	if (handled)
	{
		m_inputType = InputType::Default;
		m_scrollArea.setInputString(m_input);
		m_scrollArea.setInput(false);
		if (m_callbackStack.size() > 0)
		{
			auto callbackFunction = m_callbackStack.top();
			m_callbackStack.pop();
			if (callbackFunction)
			{
				callbackFunction();
			}
		}
	}
}

void U3Misc::HandleInputRestricted(SDL_Keycode key)
{
	int value = key - SDLK_A;
	if (value >= (int)((m_restrictedStart - 'A')) && value < (int)((m_opnum - 'A')))
	{
		m_input_num = value;
		if (m_callbackStack.size() > 0)
		{
			auto callbackFunction = m_callbackStack.top();
			m_callbackStack.pop();
			if (callbackFunction)
			{
				callbackFunction();
			}
		}
	}
	else if (key == SDLK_SPACE || key == SDLK_RETURN)
	{
		m_input_num = -1;
		if (m_callbackStack.size() > 0)
		{
			auto callbackFunction = m_callbackStack.top();
			m_callbackStack.pop();
			if (callbackFunction)
			{
				callbackFunction();
			}
		}
	}
}

void U3Misc::HandleNumImmediate(SDL_Keycode key)
{
	bool handled = false;

	if (key >= SDLK_0 && key <= SDLK_9)
	{
		m_input_num = key - SDLK_0;
		handled = true;
	}
	else if (key == SDLK_SPACE || key == SDLK_RETURN)
	{
		m_input_num = 0;
		handled = true;
	}
	if (handled)
	{
		if (m_callbackStack.size() > 0)
		{
			m_scrollArea.setInputString(std::to_string(m_input_num));
			auto callbackFunction = m_callbackStack.top();
			m_callbackStack.pop();
			if (callbackFunction)
			{
				callbackFunction();
			}
		}
	}
}

void U3Misc::HandleInputText(SDL_Keycode key)
{
	bool handled = false;
	if (key >= SDLK_A && key <= SDLK_Z)
	{
		if (!m_numOnly)
		{
			const char* value = SDL_GetKeyName(key);
			if (m_maxInputLength < m_input.size())
			{
				m_input += value;
				handled = true;
			}
		}
	}
	else if (key >= SDLK_0 && key <= SDLK_9)
	{
		const char* value = SDL_GetKeyName(key);
		if (m_maxInputLength > m_input.size())
		{
			m_input += value;
			handled = true;
		}
	}
	else
	{
		switch (key)
		{
		case SDLK_BACKSPACE:
			if (m_input.size() > 0)
			{
				m_input.pop_back();
				handled = true;
			}
			break;
		case SDLK_RETURN:
			m_scrollArea.setInput(false);
			handled = false;
			if (m_callbackStack.size() > 0)
			{
				auto callbackFunction = m_callbackStack.top();
				m_callbackStack.pop();
				if (callbackFunction)
				{
					callbackFunction();
				}
			}
			else
			{
				// This should never be called, but just incase, add a new line
				m_scrollArea.UPrintWin("\n");
			}
			break;
		default:
			break;
		}
	}
	if (handled)
	{
		m_scrollArea.setInputString(m_input);
	}
}

void U3Misc::HandleAnyKey()
{
	m_inputType = InputType::Default;
	if (m_callbackStack.size() > 0)
	{
		auto callbackFunction = m_callbackStack.top();
		m_callbackStack.pop();
		if (callbackFunction)
		{
			callbackFunction();
		}
	}
}

void U3Misc::InputTextCallback()
{
	if (m_callbackStack.size() > 0)
	{
		auto callbackFunction = m_callbackStack.top();
		m_callbackStack.pop();
		if (callbackFunction)
		{
			callbackFunction();
			return;
		}
	}
	m_inputType = InputType::Default;
}

void U3Misc::InputNumCallback()
{
	m_numOnly = false;
	m_inputType = InputType::Default;
	try
	{
		m_input_num = std::stoi(m_input);
	}
	catch ([[maybe_unused]] std::exception& e)
	{
		m_input_num = 0;
	}
	if (m_callbackStack.size() > 0)
	{
		auto callbackFunction = m_callbackStack.top();
		m_callbackStack.pop();
		if (callbackFunction)
		{
			callbackFunction();
		}
	}
}

void U3Misc::HandleCallback()
{
	// Most likely we threw up an alert message, so this is just to delay any followup until the alert is closed
	m_inputType = InputType::Default;
	if (m_callbackStack.size() > 0)
	{
		auto callbackFunction = m_callbackStack.top();
		m_callbackStack.pop();
		if (callbackFunction)
		{
			callbackFunction();
		}
	}
}

bool U3Misc::HandleKeyPress(SDL_Keycode key)
{
	switch (m_inputType)
	{
	case InputType::NumImmediate:
		HandleNumImmediate(key);
		break;
	case InputType::GuildVendor:
		HandleInputGuild(key);
		break;
	case InputType::Restricted:
		HandleInputRestricted(key);
		break;
	case InputType::BuySell:
		HandleInputBuySell(key);
		break;
	case InputType::YesNo:
		HandleInputYesNo(key);
		break;
	case InputType::InputText:
		HandleInputText(key);
		break;
	case InputType::Transact:
		HandleTransactPress(key);
		break;
	case InputType::GetDirection:
		HandleDircetionKeyPress(key);
		break;
	case InputType::AnyKey:
		HandleAnyKey();
		break;
	case InputType::Callback:
		HandleCallback();
		break;
	default:
		HandleDefaultKeyPress(key);
		return true;
	}
	return false;
}

bool U3Misc::ProcessAnyEvent(SDL_Event event)
{
	switch (event.type)
	{
	case SDL_EVENT_KEY_DOWN:
		return true;
	case SDL_EVENT_MOUSE_BUTTON_DOWN:
		return true;
	default:
		break;
	}
	return false;
}

bool U3Misc::ProcessEvent(SDL_Event event)
{
	bool quit = false;
	bool gInterrupt = false;
	bool updateMouse = false;
	int mouseState = 0;
	bool retVal = false;

	switch (event.type)
	{
	case SDL_EVENT_QUIT:
		quit = true;
		break;
	case SDL_EVENT_KEY_DOWN:
		if (!(event.key.mod & SDL_KMOD_ALT) && !(event.key.mod & SDL_KMOD_CTRL) && !(event.key.mod & SDL_KMOD_GUI))
		{
			retVal = HandleKeyPress(event.key.key);
		}
		break;
	case SDL_EVENT_MOUSE_BUTTON_DOWN:
		mouseState = 1;
		updateMouse = true;
		if (m_inputType == InputType::AnyKey)
		{
			HandleAnyKey();
			retVal = true;
		}
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
	return retVal;
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

void U3Misc::Pass()
{
	m_scrollArea.UPrintMessage(23);
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
	m_resources.GetPreference(U3PreferencesType::Auto_Save, autosave);
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
	m_resources.GetPreference(U3PreferencesType::Auto_Save, autosave);
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
	SpawnMonster();*/
	MoveMonsters();
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

void U3Misc::Look()
{
	m_scrollArea.UPrintMessage(69);
	m_inputType = InputType::GetDirection;
	m_callbackStack.push(std::bind(&U3Misc::LookCallback, this));
}

void U3Misc::PrintTile(short tile, bool plural)
{

	std::string theString;
	std::string strKey = plural ? std::string("TilesPlural") : std::string("Tiles");

	if (m_resources.m_plistMap.contains(strKey))
	{
		if (m_resources.m_plistMap[strKey].size() > tile)
		{
			theString = m_resources.m_plistMap[strKey][tile];
			m_scrollArea.UPrintWin(theString);
		}
	}
}

/* monster name depended upon player's x and y location.
even y locations contain the normal name (Thief, Orc, Skeleton, Giant ...)
odd y locations indicate a variant.
if x is even, it's variant one (Brigand, Goblin, Ghoul, Golem ...)
if x is odd, it's variant two (Cutpurse, Troll, Zombie, Titan ...)
*/
void U3Misc::PrintMonster(short which, bool plural, char variant) // $8457
{
	if (which > 44 && variant > 0)   // Ö2 = >23
	{
		PrintTile((which - 46) + 63 + variant, plural);
	}
	else
	{
		PrintTile(which / 2, plural);
	}
}

void U3Misc::LookCallback()
{
	short temp;
	short mon;
	m_scrollArea.UPrintWin("->");
	temp = (GetXYVal(m_xs, m_ys));
	mon = MonsterHere(m_xs, m_ys);
	bool handled = false;

	if (mon == 255)
	{
		if (temp / 4 == 46)
		{
			short offset;
			int xOff = (m_xs - m_xpos) + 5;
			int yOff = (m_ys - m_ypos) + 5;
			if (xOff > -1 && xOff < 11 && yOff > -1 && yOff < 11)
			{
				offset = yOff * 11 + xOff;
				unsigned char tileVal = m_resources.m_TileArray[offset];
				if (tileVal == 0x5D) // Door, which is not actually handled in the original game or the mac remake, but let's handle it
				{
					std::string strDoor(DoorString);
					m_scrollArea.UPrintWin(strDoor);
					handled = true;
				}
			}
			if (!handled)
			{
				PrintTile(temp / 4, false);
			}
		}
		else
		{
			PrintTile(temp / 4, false);
		}
		if (temp > 0x30 && temp < 0x7C) // try to make it real a la SpawnMonster()
		{
			bool madeReal = false;
			int i = 0;
			while (!madeReal && i < 32)
			{
				if (m_Monsters[i] == 0)
				{
					m_Monsters[i] = (unsigned char)temp;
					m_Monsters[i + TILEON] = (temp < 0x40) ? 0x00 : 0x04;
					m_Monsters[i + XMON] = m_xs;
					m_Monsters[i + YMON] = m_ys;
					m_Monsters[i + HPMON] = 0x40;    // Wander
					m_Monsters[i + VARMON] = 0;
					madeReal = true;
				}
				++i;
			}
			if (madeReal)
			{
				PutXYVal(GetXYVal(m_xs - 1, m_ys), m_xs, m_ys);
			}
		}
	}
	else // plural if not in town or castle.
	{
		PrintMonster(m_Monsters[mon] / 2, (m_Party[2] != 2 && m_Party[2] != 3), m_Monsters[mon + VARMON]);
	}
	m_scrollArea.UPrintWin("\n");
}

void U3Misc::Transact()
{
	m_storedir = 0;
	m_scrollArea.UPrintMessageRewrapped(88);
	m_inputType = InputType::Transact;
	m_callbackStack.push(std::bind(&U3Misc::TransactCallback, this));
}

void U3Misc::TransactCallback()
{
	if (m_input_num < 0 || m_input_num > 3)
	{
		m_scrollArea.UPrintWin("\n");
	}
	else
	{
		m_transactNum = m_input_num;
		m_rosNum = m_Party[6 + m_transactNum];
		std::string strRosNum = std::to_string(m_rosNum) + std::string("\n");
		m_scrollArea.UPrintWin(strRosNum);
		if (CheckAlive(m_transactNum) == 0)
		{
			m_spellCombat.Incap();
			return;
		}
		m_scrollArea.UPrintMessage(85);

		if (m_storedir)
		{
			//AddMacro(storeDir);
		}

		m_inputType = InputType::GetDirection;
		m_callbackStack.push(std::bind(&U3Misc::TransactCallback2, this));
	}
}

void U3Misc::TransactCallback2()
{
	short monNum;
	short tile;
	short shopNum;
	short perNum;
	short level;
	short hpmax;

	//m_scrollArea.UPrintWin("\n");
	monNum = MonsterHere(m_xs, m_ys);
	if (monNum > 127)
	{
		tile = GetXYVal(m_xs, m_ys);
		if (tile < 0x94 || tile >= 0xE8)
		{
			NotHere();
			return;
		}
		m_xs = m_xs + m_dx;
		m_ys = m_ys + m_dy;
		tile = GetXYVal(m_xs, m_ys);

		if (tile != 0x40) // merchant
		{
			NotHere();
			return;
		}
		shopNum = (m_ypos & 0x07);

		//gSongCurrent = gSongNext = 6;
		InverseChnum(m_transactNum, true);
		Shop(shopNum, m_transactNum);
		//InverseChnum(m_transactNum, false);
		//gSongNext = m_Party[2];
		return;
	}
	if (m_Monsters[monNum] != 0x4C)   // is not Lord British
	{
		perNum = (m_Monsters[(monNum + HPMON) % 256] & 0x0F);
		if (perNum == 0)
		{
			if (m_Party[15] == 1)
			{
				m_scrollArea.UPrintMessageRewrapped(262);
			}
			else
			{
				m_scrollArea.UPrintMessage(89);
			}
			return;
		}
		Speak(perNum, m_Monsters[monNum] / 4);
		return;
	}
	m_scrollArea.UPrintMessage(90);
	level = m_Player[m_rosNum][30];
	hpmax = (m_Player[m_rosNum][28] * 256) + m_Player[m_rosNum][29];

	if ((hpmax % 100) == 50)   // old 150/250 etc.
	{
		hpmax -= 50;
	}
	hpmax = hpmax / 100;
	if (level < hpmax)
	{
		if (m_Party[15] == 1)
		{
			m_scrollArea.UPrintMessageRewrapped(263);
		}
		else
		{
			m_scrollArea.UPrintMessage(91);
		}
		return;
	}
	if (hpmax >= 25 && m_Party[15] == 0)
	{
		m_scrollArea.UPrintMessage(92);
		return;
	}
	if (hpmax > 4 && (m_Player[m_rosNum][14] & 0x80) == 0)
	{
		m_scrollArea.UPrintMessage(93);
		return;
	}
	hpmax = ((m_Player[m_rosNum][28] * 256) + m_Player[m_rosNum][29]);
	hpmax += 100;
	if (hpmax > 9950)
	{
		hpmax = 9950;
	}
	m_Player[m_rosNum][28] = hpmax / 256;
	m_Player[m_rosNum][29] = hpmax - (m_Player[m_rosNum][28] * 256);
	m_scrollArea.UPrintMessage(94);
	InverseTiles(true);
}

void U3Misc::Speak(short perNum, short shnum)
{
	short talk;
	std::string speechStr;
	std::string outStr;
	short tlkptr = 0;

	while (perNum > 0 && tlkptr < 256)
	{
		while (m_Talk[tlkptr] != 0 && tlkptr < 256)
		{
			tlkptr++;
		}
		perNum--;
		tlkptr++;
	}
	while (m_Talk[tlkptr] != 0 && tlkptr < 256)
	{
		talk = m_Talk[tlkptr];
		if (talk == 0xFF) {
			outStr += '\n';
			speechStr += ' ';
		}
		else
		{
			talk = talk & 0x7F;
			outStr += (char)talk;
			if (talk >= 'A' && talk <= 'Z')
			{
				talk += 32;
			}
			if (talk == '-')
			{
				talk = ' ';
			}
			if ((talk >= 'a' && talk <= 'z') || talk == '?' || talk == '&' || talk == '!' || talk == ':' || talk == ' ' ||
				(talk >= '0' && talk <= '9'))
			{
				speechStr += (char)talk;
			}
		}
		tlkptr++;
	}
	bool classic;
	m_resources.GetPreference(U3PreferencesType::Classic_Appearance, classic);
	if (!classic)
	{
		outStr = m_scrollArea.RewrapString(outStr);
	}
	m_scrollArea.UPrintWin(outStr);
}

void U3Misc::InverseChnum(char which, bool value)
{
	if (which < 0 || which > 3)
	{
		return;
	}
	m_resources.m_inverses.character_num[which] = value;
}

void U3Misc::InverseTiles(bool value)
{
	m_resources.m_inverses.tiles = value;
	m_resources.m_inverses.inverseTileTime = 1000;
	m_resources.m_inverses.elapsedTileTime = 0;
	if (!value)
	{
		m_resources.m_inverses.char_details[0] = false;
		m_resources.m_inverses.char_details[1] = false;
		m_resources.m_inverses.char_details[2] = false;
		m_resources.m_inverses.char_details[3] = false;
	}
}

// Might need to rethink the inversing
void U3Misc::InverseCharDetails(short num, bool value)
{
	if (num >= 0 && num < 4)
	{
		m_resources.m_inverses.char_details[num] = value;
	}
}

void U3Misc::Shop(short shopNum, short chnum)
{
	short rosNum;

	/*if (shopNum == 0)
	{
		shopNum = 7;
	}*/

	rosNum = m_Party[6 + chnum];
	switch (shopNum)
	{
	case 0:
		m_scrollArea.UPrintMessageRewrapped(185);
		m_scrollArea.UPrintMessage(186);
		setInputTypeNum(std::bind(&U3Misc::tavernCallback, this));
		break;
	case 1:
		m_scrollArea.UPrintMessage(191);
		m_scrollArea.UPrintMessage(192);
		setInputTypeNum(std::bind(&U3Misc::grocerCallback, this), 4);
		break;
	case 2:
		m_scrollArea.UPrintMessage(196);
		m_scrollArea.UPrintMessage(197);
		m_inputType = InputType::Transact;
		m_scrollArea.setInput(true);
		m_callbackStack.push(std::bind(&U3Misc::healingCallback, this));
		break;
	case 3:
		m_opnum = 'I';
		if (m_Party[3] == 37)
		{
			m_opnum = 'P';    // x location of party on Sosaria
		}
		m_scrollArea.UPrintMessage(206);
		setInputTypeYesNo(std::bind(&U3Misc::weaponsListCallback, this));
		break;
	case 4:
		m_opnum = 'F';
		if (m_Party[3] == 37)
		{
			m_opnum = 'H';    // x location of party on Sosaria
		}
		m_scrollArea.UPrintMessage(216);
		setInputTypeYesNo(std::bind(&U3Misc::armorsListCallback, this));
		break;
	case 5:
		m_scrollArea.UPrintMessage(217);
		m_scrollArea.UPrintMessage(261);
		m_scrollArea.UPrintMessage(218);
		m_inputType = InputType::GuildVendor;
		m_scrollArea.setInput(true);
		m_callbackStack.push(std::bind(&U3Misc::guildCallback, this));
		break;
	case 6:
		m_scrollArea.UPrintMessage(221);
		m_scrollArea.UPrintMessageRewrapped(222);
		setInputTypeNumImmediate(std::bind(&U3Misc::oracleCallback, this));
		break;
	case 7:
		m_scrollArea.UPrintMessage(225);
		m_scrollArea.UPrintWin(std::to_string(m_Party[1]));
		m_scrollArea.UPrintMessage(226);
		m_opnum = m_Party[1] * 200;
		m_scrollArea.UPrintWin(std::to_string(m_opnum));
		m_scrollArea.UPrintMessage(227);
		setInputTypeYesNo(std::bind(&U3Misc::horseVendorCallback, this));
		m_scrollArea.setInput(true);
		break;
	default:
		InverseChnum(m_transactNum, false);
		break;
	}
}

void U3Misc::horseVendorCallback()
{
	short gold;
	m_scrollArea.setInput(false);
	if (m_input_num != 1)
	{
		if (m_input_num < 0)
		{
			m_scrollArea.UPrintWin("N");
		}
		m_scrollArea.UPrintWin("\n\n");
		m_scrollArea.UPrintMessageRewrapped(228);
		InverseChnum(m_transactNum, false);
		return;
	}
	gold = (m_Player[m_rosNum][35] * 256) + m_Player[m_rosNum][36];
	if (gold < m_opnum)
	{
		m_scrollArea.UPrintWin("\n\n");
		m_scrollArea.UPrintMessageRewrapped(229);
		//Speech(GetLocalizedPascalString("\pI'm sorry, but you haven't the gold!"),63);
		InverseChnum(m_transactNum, false);
		return;
	}
	gold -= m_opnum;
	m_Player[m_rosNum][35] = gold / 256;
	m_Player[m_rosNum][36] = gold - (m_Player[m_rosNum][35] * 256);
	m_scrollArea.UPrintWin("\n\n");
	m_scrollArea.UPrintMessageRewrapped(230);
	m_Party[0] = 0x14;
	InverseChnum(m_transactNum, false);
}

void U3Misc::setInputTypeNumImmediate(std::function<void()> func)
{
	m_scrollArea.setInput(true);
	m_callbackStack.push(func);
	m_input.clear();
	m_inputType = InputType::NumImmediate;
}

void U3Misc::oracleCallback()
{
	m_inputType = InputType::Default;
	short input;
	short gold;
	m_scrollArea.setInput(false);
	m_scrollArea.UPrintWin("\n\n");
	input = m_input_num * 100;
	gold = (m_Player[m_rosNum][35] * 256) + m_Player[m_rosNum][36];
	if (input > gold)
	{
		m_scrollArea.UPrintMessage(188);
		//Speech(GetLocalizedPascalString("\pWhat? Can't pay? Out, you scum!"),16);
		InverseChnum(m_transactNum, false);
		return;
	}
	gold -= input;
	m_Player[m_rosNum][35] = gold / 256;
	m_Player[m_rosNum][36] = gold - (m_Player[m_rosNum][35] * 256);

	if (m_resources.m_plistMap["Radrion"].size() <= m_input_num)
	{
		InverseChnum(m_transactNum, false);
		return;
	}
	std::string dispString = m_resources.m_plistMap["Radrion"][m_input_num];

	bool classic;
	m_resources.GetPreference(U3PreferencesType::Classic_Appearance, classic);
	if (!classic)
	{
		dispString = m_scrollArea.RewrapString(dispString);
	}
	m_scrollArea.UPrintWin(dispString);
	m_scrollArea.UPrintMessage(223);
	setInputTypeYesNo(std::bind(&U3Misc::oracleFinishCallback, this));
	m_scrollArea.setInput(true);
}

void U3Misc::oracleFinishCallback()
{
	m_scrollArea.setInput(false);
	if (m_input_num == 1)
	{
		m_scrollArea.UPrintMessageRewrapped(222);
		setInputTypeNumImmediate(std::bind(&U3Misc::oracleCallback, this));
	}
	else
	{
		if (m_input_num < 0)
		{
			m_scrollArea.UPrintWin("N");
		}
		m_scrollArea.UPrintWin("\n");
		m_scrollArea.UPrintMessageRewrapped(224);
		InverseChnum(m_transactNum, false);
		return;
	}
}

void U3Misc::guildCallback()
{
	short cost = 0;
	short attrib;
	short qty = 1;
	switch (m_input_num)
	{
	case 0:
		cost = 30;
		attrib = 15;
		qty = 5;
		break;
	case 1:
		cost = 50;
		attrib = 38;
		break;
	case 2:
		cost = 90;
		attrib = 39;
		break;
	case 3:
		cost = 75;
		attrib = 37;
		break;
	default:
		m_scrollArea.UPrintWin("N\n\n");
		m_scrollArea.UPrintMessageRewrapped(219);
		m_scrollArea.setInput(false);
		InverseChnum(m_transactNum, false);
		return;
	}

	m_scrollArea.setInput(false);
	m_scrollArea.UPrintWin("\n");
	if (m_Player[m_rosNum][attrib] + qty > 99)
	{
		m_scrollArea.UPrintMessageRewrapped(260);
		m_scrollArea.UPrintMessage(261);
		m_scrollArea.UPrintMessage(218);
		m_inputType = InputType::GuildVendor;
		m_scrollArea.setInput(true);
		m_callbackStack.push(std::bind(&U3Misc::guildCallback, this));
		return;
	}

	if (GuildPay(m_rosNum, cost) == 0)
	{
		GuildGive(m_rosNum, attrib, qty);
	}
	else
	{
		m_scrollArea.setInput(false);
		InverseChnum(m_transactNum, false);
		return;
	}
	m_scrollArea.UPrintMessage(220);
	setInputTypeYesNo(std::bind(&U3Misc::guildCompleteCallback, this));
}

void U3Misc::guildCompleteCallback()
{
	m_scrollArea.setInput(false);
	if (m_input_num == 1) // yes
	{
		m_scrollArea.UPrintMessage(261);
		m_scrollArea.UPrintMessage(218);
		m_inputType = InputType::GuildVendor;
		m_scrollArea.setInput(true);
		m_callbackStack.push(std::bind(&U3Misc::guildCallback, this));
	}
	else
	{
		m_scrollArea.UPrintWin("\n\n");
		m_scrollArea.UPrintMessage(219);
		InverseChnum(m_transactNum, false);
	}
}

bool U3Misc::GuildPay(short rosNum, short cost)
{
	short gold;
	gold = (m_Player[rosNum][35] * 256) + m_Player[rosNum][36];
	if (gold < cost)
	{
		m_scrollArea.UPrintMessageRewrapped(231);
		//Speech(GetLocalizedPascalString("\pI'm sorry, but you have not the funds!"),23);
		return true;
	}
	gold -= cost;
	m_Player[rosNum][35] = gold / 256;
	m_Player[rosNum][36] = gold - (m_Player[rosNum][35] * 256);
	return false;
}

void U3Misc::GuildGive(short rosNum, short item, short amount)
{
	m_Player[rosNum][item] += amount;
	if (m_Player[rosNum][item] > 99)
	{
		m_Player[rosNum][item] = 99;
	}
}

void U3Misc::WeaponList()
{
	m_scrollArea.blockPrompt(true);
	m_scrollArea.UPrintMessage(237);
	m_weaponsList.push(1);
	m_weaponsList.push(2);
	m_weaponsList.push(3);
	m_weaponsList.push(4);
	m_weaponsList.push(5);
	m_weaponsList.push(0);
	m_weaponsList.push(6);
	m_weaponsList.push(7);
	if (m_opnum == 'P')
	{
		m_weaponsList.push(8);
		m_weaponsList.push(9);
		m_weaponsList.push(10);
		m_weaponsList.push(11);
		m_weaponsList.push(12);
		m_weaponsList.push(13);
		m_weaponsList.push(14);
	}
	m_weaponsList.push(0);
	listWeaponsCallback();
}

void U3Misc::ArmorsList()
{
	m_scrollArea.blockPrompt(true);
	m_scrollArea.UPrintMessage(237);
	m_weaponsList.push(1);
	m_weaponsList.push(2);
	m_weaponsList.push(3);
	m_weaponsList.push(4);
	if (m_opnum == 'H')
	{
		m_weaponsList.push(5);
		m_weaponsList.push(6);
	}
	m_weaponsList.push(0);
	listArmorsCallback();
}

void U3Misc::PrintWeaponList(short weapon)
{
	char curWeapon = 'A' + weapon;
	std::string strWeapon;
	std::string dispString = m_resources.m_plistMap["WeaponsArmour"][weapon];
	strWeapon += curWeapon + std::string(":");
	if (!dispString.ends_with('s') && dispString.size() < 7)
	{
		dispString += 's';
	}
	strWeapon += dispString;
	dispString = m_resources.m_plistMap["WeaponsArmour"][weapon + 24];
	dispString += std::string("gp");
	dispString += std::string("\n");
	std::string strSpaces;
	for (int index = 0; index < 17 - (strWeapon.size() + dispString.size()); ++index)
	{
		strSpaces += ' ';
	}
	strWeapon += strSpaces + dispString;
	m_scrollArea.UPrintWin(strWeapon, true);
}

void U3Misc::PrintArmorList(short armor)
{
	char curWeapon = 'A' + armor;
	std::string strWeapon;
	std::string dispString = m_resources.m_plistMap["WeaponsArmour"][armor + 16];
	strWeapon += curWeapon + std::string(":");
	if (!dispString.ends_with('s') && dispString.size() < 7)
	{
		dispString += 's';
	}
	strWeapon += dispString;
	dispString = m_resources.m_plistMap["WeaponsArmour"][armor + 40];
	dispString += std::string("gp");
	dispString += std::string("\n");
	std::string strSpaces;
	for (int index = 0; index < 17 - (strWeapon.size() + dispString.size()); ++index)
	{
		strSpaces += ' ';
	}
	strWeapon += strSpaces + dispString;
	m_scrollArea.UPrintWin(strWeapon, true);
}

void U3Misc::listWeaponsCallback()
{
	bool anykey = false;
	std::string dispString;
	while (!m_weaponsList.empty())
	{
		int num = m_weaponsList.front();
		m_weaponsList.pop();
		if (num == 0)
		{
			anykey = true;
			break;
		}
		PrintWeaponList(num);
	}
	if (anykey)
	{
		m_inputType = InputType::AnyKey;
		m_callbackStack.push(std::bind(&U3Misc::listWeaponsCallback, this));
	}
	else
	{
		weaponsBuyOrSell();
	}
}

void U3Misc::listArmorsCallback()
{
	bool anykey = false;
	std::string dispString;
	while (!m_weaponsList.empty())
	{
		int num = m_weaponsList.front();
		m_weaponsList.pop();
		if (num == 0)
		{
			anykey = true;
			break;
		}
		PrintArmorList(num);
	}
	if (anykey)
	{
		m_inputType = InputType::AnyKey;
		m_callbackStack.push(std::bind(&U3Misc::listArmorsCallback, this));
	}
	else
	{
		armorsBuyOrSell();
	}
}

void U3Misc::weaponsListCallback()
{
	if (m_input_num == 1) // yes
	{
		WeaponList();
	}
	else
	{
		weaponsBuyOrSell();
	}
}

void U3Misc::armorsListCallback()
{
	if (m_input_num == 1) // yes
	{
		ArmorsList();
	}
	else
	{
		armorsBuyOrSell();
	}
}

void U3Misc::weaponsBuyOrSell()
{
	m_scrollArea.UPrintMessage(207);
	m_scrollArea.blockPrompt(false);
	setInputTypeBuySell(std::bind(&U3Misc::weaponsBuySellCallback, this));
}

void U3Misc::armorsBuyOrSell()
{
	m_scrollArea.UPrintMessage(207);
	m_scrollArea.blockPrompt(false);
	setInputTypeBuySell(std::bind(&U3Misc::armorsBuySellCallback, this));
}

void U3Misc::setInputTypeNum(std::function<void()> func, int inputLength)
{
	m_callbackStack.push(func);
	m_input.clear();
	m_numOnly = true;
	m_scrollArea.setInput(true);
	m_inputType = InputType::InputText;
	m_maxInputLength = inputLength;
	m_callbackStack.push(std::bind(&U3Misc::InputNumCallback, this));
	m_callbackStack.push(std::bind(&U3Misc::InputTextCallback, this));
}

void U3Misc::setInputTypeYesNo(std::function<void()> func)
{
	m_scrollArea.setInput(true);
	m_callbackStack.push(func);
	m_input.clear();
	m_inputType = InputType::YesNo;
}

void U3Misc::setInputTypeBuySell(std::function<void()> func)
{
	m_scrollArea.setInput(true);
	m_callbackStack.push(func);
	m_input.clear();
	m_inputType = InputType::BuySell;
}

void U3Misc::setInputTypeRestricted(std::function<void()> func, short start)
{
	m_restrictedStart = start;
	m_scrollArea.setInput(true);
	m_callbackStack.push(func);
	m_input.clear();
	m_inputType = InputType::Restricted;
}

void U3Misc::grocerCallback()
{
	short gold;
	if (m_input_num == 0)
	{
		InverseChnum(m_transactNum, false);
		m_scrollArea.UPrintWin("\n\n");
		return;
	}
	int existingFood = (m_Player[m_rosNum][32] * 100) + m_Player[m_rosNum][33];
	if (m_input_num > (9999 - existingFood))
	{
		m_scrollArea.UPrintMessageRewrapped(260);
		m_scrollArea.UPrintWin("\n\n");
		m_scrollArea.UPrintMessage(192);
		setInputTypeNum(std::bind(&U3Misc::grocerCallback, this), 4);
		return;
	}
	gold = (m_Player[m_rosNum][35] * 256) + m_Player[m_rosNum][36];
	if (gold < m_input_num)
	{
		InverseChnum(m_transactNum, false);
		m_scrollArea.UPrintMessageRewrapped(193);
		return;
	}
	gold -= m_input_num;
	m_Player[m_rosNum][35] = gold / 256;
	m_Player[m_rosNum][36] = gold - (m_Player[m_rosNum][35] * 256);
	existingFood += m_input_num;
	m_Player[m_rosNum][32] = existingFood / 100;
	m_Player[m_rosNum][33] = existingFood - (m_Player[m_rosNum][32] * 100);
	m_scrollArea.UPrintMessageRewrapped(194);
	setInputTypeYesNo(std::bind(&U3Misc::moreFoodCallback, this));
}

void U3Misc::moreFoodCallback()
{
	m_scrollArea.setInput(true);
	if (m_input_num == 1) // yes
	{
		m_scrollArea.UPrintWin("\n\n");
		m_scrollArea.UPrintMessage(192);
		setInputTypeNum(std::bind(&U3Misc::grocerCallback, this));
	}
	else
	{
		m_scrollArea.setInput(false);
		m_scrollArea.UPrintWin("\n\n");
		m_scrollArea.UPrintMessageRewrapped(195);
		InverseChnum(m_transactNum, false);
	}
}

void U3Misc::tavernCallback()
{
	short gold;
	std::string message;

	m_scrollArea.UPrintWin("\n\n");
	if (m_input_num < 7)
	{
		InverseChnum(m_transactNum, false);
		m_scrollArea.UPrintMessageRewrapped(187);
		return;
	}
	gold = (m_Player[m_rosNum][35] * 256) + m_Player[m_rosNum][36];
	if (gold < m_input_num)
	{
		InverseChnum(m_transactNum, false);
		m_scrollArea.UPrintMessageRewrapped(188);
		return;
	}
	gold -= m_input_num;
	m_Player[m_rosNum][35] = gold / 256;
	m_Player[m_rosNum][36] = gold - (m_Player[m_rosNum][35] * 256);

	message = m_resources.m_plistMap["Pub"][(m_input_num / 10)];
	bool classic;
	m_resources.GetPreference(U3PreferencesType::Classic_Appearance, classic);
	if (!classic)
	{
		message = m_scrollArea.RewrapString(message);
	}
	m_scrollArea.UPrintWin(message);
	m_scrollArea.UPrintMessage(189);
	setInputTypeYesNo(std::bind(&U3Misc::anotherDrinkCallback, this));
}

void U3Misc::anotherDrinkCallback()
{
	m_scrollArea.setInput(true);
	if (m_input_num == 1) // yes
	{
		m_scrollArea.UPrintMessage(186);
		setInputTypeNum(std::bind(&U3Misc::tavernCallback, this));
	}
	else
	{
		m_scrollArea.setInput(false);
		m_scrollArea.UPrintMessageRewrapped(190);
		InverseChnum(m_transactNum, false);
	}
}

void U3Misc::weaponsBuySellCallback()
{
	m_scrollArea.setInput(false);
	if (m_input_num == 1) // buy
	{
		m_scrollArea.UPrintMessage(208);
		setInputTypeRestricted(std::bind(&U3Misc::weaponsBuyCallback, this), 'B');
	}
	else if (m_input_num == 0) // sell
	{
		m_scrollArea.UPrintMessage(211);
		setInputTypeRestricted(std::bind(&U3Misc::weaponsSellCallback, this), 'A');
	}
	else // cancel
	{
		weaponsBuySellFinishedCallback();
	}
}

void U3Misc::armorsBuySellCallback()
{
	m_scrollArea.setInput(false);
	if (m_input_num == 1) // buy
	{
		m_scrollArea.UPrintMessage(208);
		setInputTypeRestricted(std::bind(&U3Misc::armorsBuyCallback, this), 'B');
	}
	else if (m_input_num == 0) // sell
	{
		m_scrollArea.UPrintMessage(211);
		setInputTypeRestricted(std::bind(&U3Misc::armorsSellCallback, this), 'B');
	}
	else // cancel
	{
		armorsBuySellFinishedCallback();
	}
}

void U3Misc::weaponsBuyCallback()
{
	if (m_input_num < 0)
	{
		weaponsBuySellFinishedCallback();
	}
	else
	{
		std::string strInput;
		strInput += m_input_num + 'A';
		m_scrollArea.setInputString(strInput);
		m_scrollArea.setInput(false);
		short gold = (m_Player[m_rosNum][35] * 256) + m_Player[m_rosNum][36];
		// m_input_num starts at b
		std::string dispString = m_resources.m_plistMap["WeaponsArmour"][m_input_num + 24];
		int amount;
		try
		{
			amount = std::stoi(dispString);
		}
		catch ([[maybe_unused]] std::exception& e)
		{
			amount = 0;
		}
		if (amount > gold)
		{
			weaponsBuyPoor();
			return;
		}
		if (m_Player[m_rosNum][48 + m_input_num] > 98)
		{
			m_scrollArea.UPrintMessage(260);
			m_scrollArea.UPrintWin("\n\n");
			m_scrollArea.setInput(false);
			m_scrollArea.blockPrompt(true);
			setInputTypeRestricted(std::bind(&U3Misc::weaponsBuyCallback, this), 'B');
			m_scrollArea.UPrintMessageRewrapped(208);
			return;
		}
		gold -= amount;
		m_Player[m_rosNum][35] = gold / 256;
		m_Player[m_rosNum][36] = gold - (m_Player[m_rosNum][35] * 256);
		m_Player[m_rosNum][48 + m_input_num]++;

		if (m_Player[m_rosNum][48 + m_input_num] > 99)
		{
			m_Player[m_rosNum][48 + m_input_num] = 99;
		}

		m_scrollArea.UPrintMessageRewrapped(210);
		m_scrollArea.setInput(false);
		m_scrollArea.blockPrompt(true);
		setInputTypeRestricted(std::bind(&U3Misc::weaponsBuyCallback, this), 'B');
		m_scrollArea.UPrintMessageRewrapped(208);
	}
}

void U3Misc::weaponsSellCallback()
{
	if (m_input_num == 0)
	{
		m_resources.CreateAlertMessage(9);
		m_inputType = InputType::Callback;
		m_callbackStack.push(std::bind(&U3Misc::weaponsBuySellFinishedCallback, this));
		return;
	}
	else if (m_input_num < 0)
	{
		weaponsBuySellFinishedCallback();
	}
	else
	{
		std::string strInput;
		strInput += m_input_num + 'A';
		m_scrollArea.setInputString(strInput);
		m_scrollArea.setInput(false);
		if (m_Player[m_rosNum][49 + m_input_num] < 1)
		{
			weaponsSellMissing();
			return;
		}
		std::string dispString = m_resources.m_plistMap["WeaponsArmour"][m_input_num + 25];
		int amount;
		try
		{
			amount = std::stoi(dispString);
		}
		catch ([[maybe_unused]] std::exception& e)
		{
			amount = 0;
		}
		if (AddGold(m_rosNum, amount, false))
		{
			m_Player[m_rosNum][49 + m_input_num]--;
			if (m_Player[m_rosNum][49 + m_input_num] < 1 && m_Player[m_rosNum][48] == m_input_num + 1)
			{
				m_Player[m_rosNum][48] = 0;
			}
			m_scrollArea.UPrintMessage(213);
		}
		else
		{
			m_scrollArea.UPrintMessageRewrapped(214);
		}
		m_input_num = 0;
		weaponsBuySellCallback();
	}
}

void U3Misc::armorsBuyCallback()
{
	if (m_input_num < 0)
	{
		armorsBuySellFinishedCallback();
	}
	else
	{
		std::string strInput;
		strInput += m_input_num + 'A';
		m_scrollArea.setInputString(strInput);
		m_scrollArea.setInput(false);
		short gold = (m_Player[m_rosNum][35] * 256) + m_Player[m_rosNum][36];
		// m_input_num starts at b
		std::string dispString = m_resources.m_plistMap["WeaponsArmour"][m_input_num + 40];
		int amount;
		try
		{
			amount = std::stoi(dispString);
		}
		catch ([[maybe_unused]] std::exception& e)
		{
			amount = 0;
		}
		if (amount > gold)
		{
			armorsBuyPoor();
			return;
		}
		if (m_Player[m_rosNum][48 + m_input_num] > 98)
		{
			m_scrollArea.UPrintMessage(260);
			m_scrollArea.UPrintWin("\n\n");
			m_scrollArea.setInput(false);
			m_scrollArea.blockPrompt(true);
			setInputTypeRestricted(std::bind(&U3Misc::armorsBuyCallback, this), 'B');
			m_scrollArea.UPrintMessageRewrapped(208);
			return;
		}
		gold -= amount;
		m_Player[m_rosNum][35] = gold / 256;
		m_Player[m_rosNum][36] = gold - (m_Player[m_rosNum][35] * 256);
		m_Player[m_rosNum][40 + m_input_num]++;

		if (m_Player[m_rosNum][40 + m_input_num] > 99)
		{
			m_Player[m_rosNum][40 + m_input_num] = 99;
		}

		m_scrollArea.UPrintMessageRewrapped(210);
		m_scrollArea.setInput(false);
		m_scrollArea.blockPrompt(true);
		setInputTypeRestricted(std::bind(&U3Misc::armorsBuyCallback, this), 'B');
		m_scrollArea.UPrintMessageRewrapped(208);
	}
}

void U3Misc::armorsSellCallback()
{
	if (m_input_num < 1)
	{
		armorsBuySellFinishedCallback();
	}
	else
	{
		std::string strInput;
		strInput += m_input_num + 'A';
		m_scrollArea.setInputString(strInput);
		m_scrollArea.setInput(false);
		if (m_Player[m_rosNum][49 + m_input_num] < 1)
		{
			armorsSellMissing();
			return;
		}
		std::string dispString = m_resources.m_plistMap["WeaponsArmour"][m_input_num + 40];
		int amount;
		try
		{
			amount = std::stoi(dispString);
		}
		catch ([[maybe_unused]] std::exception& e)
		{
			amount = 0;
		}
		if (AddGold(m_rosNum, amount, false))
		{
			m_Player[m_rosNum][40 + m_input_num]--;
			if (m_Player[m_rosNum][40 + m_input_num] < 1 && m_Player[m_rosNum][48] == m_input_num + 1)
			{
				m_Player[m_rosNum][39] = 0;
			}
			m_scrollArea.UPrintMessage(213);
		}
		else
		{
			m_scrollArea.UPrintMessageRewrapped(214);
		}
		m_input_num = 0;
		armorsBuySellCallback();
	}
}

void U3Misc::weaponsBuyPoor()
{
	m_scrollArea.UPrintMessageRewrapped(209);
	m_inputType = InputType::Default;
	m_scrollArea.blockPrompt(false);
	m_scrollArea.setInput(false);
	InverseChnum(m_transactNum, false);
}

void U3Misc::armorsBuyPoor()
{
	m_scrollArea.UPrintMessageRewrapped(209);
	m_inputType = InputType::Default;
	m_scrollArea.blockPrompt(false);
	m_scrollArea.setInput(false);
	InverseChnum(m_transactNum, false);
}

void U3Misc::weaponsSellMissing()
{
	m_scrollArea.UPrintMessageRewrapped(212);
	m_inputType = InputType::Default;
	m_scrollArea.blockPrompt(false);
	m_scrollArea.setInput(false);
	InverseChnum(m_transactNum, false);
}

void U3Misc::armorsSellMissing()
{
	m_scrollArea.UPrintMessageRewrapped(212);
	m_inputType = InputType::Default;
	m_scrollArea.blockPrompt(false);
	m_scrollArea.setInput(false);
	InverseChnum(m_transactNum, false);
}


void U3Misc::weaponsBuySellFinishedCallback()
{
	m_inputType = InputType::Default;
	m_scrollArea.blockPrompt(false);
	m_scrollArea.setInput(false);
	InverseChnum(m_transactNum, false);
	m_scrollArea.UPrintMessageRewrapped(215);
}

void U3Misc::armorsBuySellFinishedCallback()
{
	m_inputType = InputType::Default;
	m_scrollArea.blockPrompt(false);
	m_scrollArea.setInput(false);
	InverseChnum(m_transactNum, false);
	m_scrollArea.UPrintMessageRewrapped(215);
}

void U3Misc::healingCallback()
{
	m_scrollArea.setInput(false);
	switch (m_input_num)
	{
	case 0:
		m_scrollArea.UPrintMessage(198);
		break;
	case 1:
		m_scrollArea.UPrintMessage(200);
		break;
	case 2:
		m_scrollArea.UPrintMessage(202);
		break;
	case 3:
		m_scrollArea.UPrintMessage(204);
		break;
	default:
		InverseChnum(m_transactNum, false);
		m_scrollArea.UPrintWin("\n");
		return;
	}
	m_opnum = m_input_num;
	m_scrollArea.setInput(true);
	setInputTypeYesNo(std::bind(&U3Misc::healingPayCallback, this));
}

void U3Misc::healingPayCallback()
{
	short cost[4] = { 100, 200, 500, 900 };
	short gold;
	m_scrollArea.setInput(false);
	if (m_input_num != 1)
	{
		m_scrollArea.UPrintWin("\n\n");
		m_scrollArea.UPrintMessageRewrapped(232);
		InverseChnum(m_transactNum, false);
		return;
	}
	gold = (m_Player[m_rosNum][35] * 256) + m_Player[m_rosNum][36];
	if (cost[m_opnum] > gold)
	{
		m_scrollArea.UPrintWin("\n\n");
		m_scrollArea.UPrintMessageRewrapped(233);
		InverseChnum(m_transactNum, false);
		return;
	}

	gold -= cost[m_opnum];
	m_Player[m_rosNum][35] = gold / 256;
	m_Player[m_rosNum][36] = gold - (m_Player[m_rosNum][35] * 256);
	m_scrollArea.UPrintWin("\n");
	switch (m_opnum)
	{
	case 0:
		m_scrollArea.UPrintMessage(199);
		break;
	case 1:
		m_scrollArea.UPrintMessage(201);
		break;
	case 2:
		m_scrollArea.UPrintMessage(203);
		break;
	case 3:
		m_scrollArea.UPrintMessage(205);
		break;
	default:
		InverseChnum(m_transactNum, false);
		return;
	}
	m_scrollArea.setInput(true);
	m_inputType = InputType::Transact;
	m_callbackStack.push(std::bind(&U3Misc::healingCallback2, this));
}

void U3Misc::SpellNoize(short opnum, short opnum2)
{
	InverseTiles(true);
	InverseCharDetails(opnum, true);
	m_resources.m_inverses.func = std::bind(&U3Misc::FinalizeHealingCallback, this);
}

void U3Misc::FinalizeHealingCallback()
{
	switch (m_opnum)
	{
	case 0:
		if (m_Player[m_Party[6 + m_input_num]][17] == 'P')
		{
			m_Player[m_Party[6 + m_input_num]][17] = 'G';
		}
		break;
	case 1:
		m_Player[m_Party[6 + m_input_num]][26] = m_Player[m_Party[5 + m_input_num]][28];
		m_Player[m_Party[6 + m_input_num]][27] = m_Player[m_Party[5 + m_input_num]][29];
		break;
	case 2:
		if (m_Player[m_Party[6 + m_input_num]][17] == 'D')
		{
			m_Player[m_Party[6 + m_input_num]][17] = 'G';
		}
		break;
	case 3:
		if (m_Player[m_Party[6 + m_input_num]][17] == 'A')
		{
			m_Player[m_Party[6 + m_input_num]][17] = 'G';
		}
		break;
	default:
		break;
	}
	InverseChnum(m_transactNum, false);
}

void U3Misc::healingCallback2()
{
	if (m_input_num >= 0 && m_input_num < 4)
	{
		m_scrollArea.setInput(false);
		m_scrollArea.UPrintWin("\n");
		SpellNoize(m_input_num, m_opnum);
		return;
	}

	m_scrollArea.setInput(false);
	m_scrollArea.UPrintWin("\n");
	InverseChnum(m_transactNum, false);
}

bool U3Misc::AddGold(short rosNum, short gold, bool overflow) // $70BB
{
	short presentGold;
	bool retVal = true;
	presentGold = ((m_Player[rosNum][35]) * 256) + m_Player[rosNum][36];
	gold += presentGold;
	if (gold > 9999)
	{
		if (overflow == false)
		{
			return false;
		}
		retVal = false;
		gold = 9999;
	}
	m_Player[m_rosNum][35] = gold / 256;
	m_Player[m_rosNum][36] = gold - (m_Player[rosNum][35] * 256);
	return retVal;
}

void U3Misc::GetMonsterDir(short monNum) // $7C37
{
	if (m_Party[2] != 0)
	{
		m_zp[0xF5] = m_xpos - m_Monsters[monNum + XMON];
		m_dx = GetHeading(m_zp[0xF5]);
		m_xs = m_graphics.MapConstrain(m_dx + m_Monsters[monNum + XMON]);
		m_zp[0xF6] = m_ypos - m_Monsters[monNum + YMON];
		m_dy = GetHeading(m_zp[0xF6]);
		m_ys = m_graphics.MapConstrain(m_dy + m_Monsters[monNum + YMON]);
	}
	else
	{
		m_zp[0xF5] = m_xpos - m_Monsters[monNum + XMON];
		m_dx = GetHeading(m_zp[0xF5] * 4);
		m_xs = m_graphics.MapConstrain(m_dx + m_Monsters[monNum + XMON]);
		m_zp[0xF6] = m_ypos - m_Monsters[monNum + YMON];
		m_dy = GetHeading(m_zp[0xF6] * 4);
		m_ys = m_graphics.MapConstrain(m_dy + m_Monsters[monNum + YMON]);
	}
	m_zp[0xFB] = m_utilities.Absolute(m_zp[0xF5]);
	m_zp[0xFB] += m_utilities.Absolute(m_zp[0xF6]);
}

unsigned char ValidMonsterDir(short tile, short montype) // $7C0C
{
	if (montype > 0x28 && montype < 0x40) // pirate/sea monster
	{
		return (tile == 0) ? 0 : 255;
	}
	else
	{
		if (tile == 4 || tile == 8 || tile == 12 || tile == 32)
		{
			return 0;
		}
		else
		{
			return 255;
		}
	}
}

char U3Misc::GetHeading(short value) // $7DFC
{
	if (value == 0)
	{
		return 0;
	}
	if (value < 0)
	{
		return -1;
	}
	if (value > 127)
	{
		return -1;
	}
	return 1;
}

// TO DO:
// They're updating the graphics in the monster move loop.
// Rather, we want to rewrite this to handle being in the main loop
bool U3Misc::moveshoot(int offset) // $7B36
{
	int randnum = m_utilities.getRandom(0, 255);
	if (randnum > 128)
	{
		return false;
	}
	GetMonsterDir(offset);

	m_xs = 5 - m_zp[0xF5];
	if (m_xs > 10 || m_xs < 0)
	{
		return false;
	}
	m_ys = 5 - m_zp[0xF6];
	if (m_ys > 10 || m_ys < 0)
	{
		return false;
	}
	//DrawMap(xpos, ypos);
	//PlaySoundFile(CFSTR("Shoot"), TRUE);    // was 0xEA
	m_zp[0xFB] = 3;

	// moveshoot2

	return true;
}

void U3Misc::move7AAA(int offset)
{
	short value;
	// check if this is a valid place for the monster to walk on.

	value = ValidMonsterDir(GetXYVal(m_xs, m_ys), m_Monsters[offset]);
	if (value == 0 && MonsterHere(m_xs, m_ys) != 255)
	{
		value = 255;
	}
	if (value != 0)
	{
		m_xs = m_Monsters[offset + XMON];
		value = ValidMonsterDir(GetXYVal(m_xs, m_ys), m_Monsters[offset]);
		if (value == 0 && MonsterHere(m_xs, m_ys) != 255)
		{
			value = 255;
		}
		if (value != 0)
		{
			m_xs = m_graphics.MapConstrain(m_Monsters[offset + XMON] + m_dx);
			m_ys = m_graphics.MapConstrain(m_Monsters[offset + YMON]);    // no +dy!?
			value = ValidMonsterDir(GetXYVal(m_xs, m_ys), m_Monsters[offset]);
			if (value == 0 && MonsterHere(m_xs, m_ys) != 255)
			{
				value = 255;
			}
			if (value != 0)
			{
				if (m_Monsters[offset] == 0x3C || m_Monsters[offset] == 0x74)
				{
					moveshoot(offset);
				}
				return;
			}
		}
	}
	if (m_xpos == m_xs && m_ypos == m_ys)
	{
		return;
	}
	PutXYVal(m_Monsters[offset + TILEON], m_Monsters[offset + XMON], m_Monsters[offset + YMON]);
	m_Monsters[offset + XMON] = m_xs;
	m_Monsters[offset + YMON] = m_ys;
	m_Monsters[offset + TILEON] = GetXYVal(m_Monsters[offset + XMON], m_Monsters[offset + YMON]);
	unsigned char monsterTile = m_Monsters[offset];
	if (m_Monsters[offset + VARMON])
	{
		monsterTile += m_Monsters[offset + VARMON];
	}
	PutXYVal(monsterTile, m_Monsters[offset + XMON], m_Monsters[offset + YMON]);
	if (m_Monsters[offset] == 0x3C || m_Monsters[offset] == 0x74)
	{
		moveshoot(offset);
	}
}

bool U3Misc::moveoutside(int offset)
{
	GetMonsterDir(offset);
	if (m_xpos == m_xs && m_ypos == m_ys)
	{
		//AttackCode(offset);
		return false;
	}
	return true;
}

void U3Misc::MoveMonsters() // $7A81
{
	short value;

	for (int offset = 0; offset < 32; ++offset)
	{
		if (m_Monsters[offset] == 0)
		{
			continue;
		}
		if (m_Party[2] == 0 && m_Party[15] == 0) // // Player hasn't beaten Exodus.
		{
			if (!moveoutside(offset))
			{
				return;
			}
			move7AAA(offset);
		}
		else
		{
			value = m_Monsters[offset + 128];
			value = value & 0xC0;
			if (value == 0)
			{
				continue;
			}
			if (value == 0x40)
			{
				int randnum = m_utilities.getRandom(0, 255);
				if (randnum < 128)
				{
					continue;
				}
				randnum = m_utilities.getRandom(0, 255);
				m_xs = m_graphics.MapConstrain(m_Monsters[offset + XMON] + GetHeading(randnum));
				if (m_xs == 0)
				{
					continue;
				}
				randnum = m_utilities.getRandom(0, 255);
				m_ys = m_graphics.MapConstrain(m_Monsters[offset + YMON] + GetHeading(randnum));
				if (m_ys == 0)
				{
					continue;
				}

				// Sosaria, and user has already defeated Exodus.  Handle monsters running into one another.
				if (m_Party[15] != 0 && m_Party[2] == 0)
				{
				}
				move7AAA(offset);
			}
			else if (value == 0x80)
			{
				GetMonsterDir(offset);
				move7AAA(offset);
			}
			else
			{
				if (!moveoutside(offset))
				{
					return;
				}
				move7AAA(offset);
			}
		}
	}
}

void U3Misc::Attack()
{
	m_scrollArea.UPrintMessage(28);
	m_inputType = InputType::GetDirection;
	m_callbackStack.push(std::bind(&U3Misc::AttackCallback, this));
}

void U3Misc::AttackCallback()
{
	short monNum;
	monNum = MonsterHere(m_xs, m_ys);
	if (monNum == 255)
	{
		NotHere();
		return;
	}
	//AttackCode(monNum);
}


void U3Misc::Board()
{
	short tileOn;
	if (m_Party[0] != 0x7E) // Not 'Ranger' shape?
	{
		m_scrollArea.UPrintMessage(29); // Board
		What2();
	}
	else
	{
		tileOn = GetXYVal(m_xpos, m_ypos);
		if (tileOn == 0x28) // horse
		{
			PutXYVal(0x04, m_xpos, m_ypos); // replace with grass
			m_Party[0] = 0x14;
			m_scrollArea.UPrintMessage(30);
		}
		else if (tileOn == 0x2C) // ship
		{
			PutXYVal(0x00, m_xpos, m_ypos); // replace with water
			m_Party[0] = 0x16;
			m_scrollArea.UPrintMessage(31);
		}
		else
		{
			m_scrollArea.UPrintMessage(29); // Board
			What2();
		}
	}
}

void U3Misc::Descend()
{
	// Seems like a little easter egg, not going to implement yet
	//if (m_Party[15] == 0)
	{
		m_scrollArea.UPrintMessage(32);
		What2();
		return;
	}
	//std::string strDiorama = std::string(DioramaString);
	//m_scrollArea.UPrintWin(strDiorama);
	//DrawDioramaMap();
}

void U3Misc::Exit()
{
	short tileOn;
	if (m_Party[0] == 0x7E)
	{
		std::string dispString = m_resources.m_plistMap["Messages"][101];
		std::string addString = m_resources.m_plistMap["Messages"][106];
		dispString += addString;
		m_scrollArea.UPrintWin(dispString);
	}
	else
	{
		tileOn = GetXYVal(m_xpos, m_ypos);
		if (tileOn > 4) // not water or grass
		{
			m_scrollArea.UPrintMessage(102);    // X-it
			m_scrollArea.UPrintWin("\n");
			m_scrollArea.UPrintMessage(108);    // Not here!\n
			//ErrorTone();
		}
		else
		{
			PutXYVal(m_Party[0] * 2, m_xpos, m_ypos);
			if (m_Party[1] == 0x14)
			{
				m_scrollArea.UPrintMessage(17);    // Dismount\n
			}
			else
			{
				m_scrollArea.UPrintMessage(103);    // X-it craft
			}
			m_Party[0] = 0x7E;
		}
	}
}

void U3Misc::Fire()
{
}

void U3Misc::Ignite()
{
	m_scrollArea.UPrintMessage(64);
	if (m_Party[2] != 1)
	{
		NotHere();
		return;
	}
	m_scrollArea.UPrintMessage(65);
	m_inputType = InputType::Transact;
	m_callbackStack.push(std::bind(&U3Misc::IgniteCallback, this));
}

void U3Misc::IgniteCallback()
{
	short rosNum;
	if (m_input_num < 1 || m_input_num > 4)
	{
		return;
	}
	rosNum = m_Party[5 + m_input_num];
	if (m_Player[rosNum][15] < 1)
	{
		m_scrollArea.UPrintMessage(67);
		return;
	}
	m_Player[rosNum][15]--;
	m_gTorch = 255;
}

void U3Misc::JoinGold()
{
	m_scrollArea.UPrintMessage(66);
	m_inputType = InputType::Transact;
	m_callbackStack.push(std::bind(&U3Misc::JoinGoldCallback, this));
}

void U3Misc::JoinGoldCallback()
{
	if (m_input_num < 1 || m_input_num > 4)
	{
		m_scrollArea.UPrintWin("\n");
		m_scrollArea.UPrintMessage(41);
		return;
	}
	JoinGold(m_input_num);
}

void U3Misc::JoinGold(short chnum)
{
	std::string strOutVal = std::to_string(chnum);
	strOutVal += '\n';
	m_scrollArea.UPrintWin(strOutVal);
	short x, mainRosNum, rosNum, total, gold, transfer;

	mainRosNum = m_Party[6 + chnum];
	total = ((m_Player[mainRosNum][35]) * 256) + m_Player[mainRosNum][36];
	for (x = 1; x < 5; x++)
	{
		if (x != chnum)
		{
			rosNum = m_Party[5 + x];
			gold = ((m_Player[rosNum][35]) * 256) + m_Player[rosNum][36];
			transfer = gold;
			if (total + transfer > 9999)
			{
				transfer = 9999 - total;
			}
			if (transfer > 0)
			{
				total += transfer;
				gold -= transfer;
				m_Player[rosNum][35] = gold / 256;
				m_Player[rosNum][36] = gold - (m_Player[rosNum][35] * 256);
			}
		}
	}
	m_Player[mainRosNum][35] = total / 256;
	m_Player[mainRosNum][36] = total - (m_Player[mainRosNum][35] * 256);
}

void U3Misc::Klimb()
{
	// Another easter egg, implement later
	// Exodus not defeated or not on the overworld
	//if (m_Party[15] != 1 || m_Party[2] != 0)
	{
		m_scrollArea.UPrintMessage(68);
		What2();
	}
	//else
	//{
		// ?
	//}
}

void U3Misc::PeerGem()
{
	m_scrollArea.UPrintMessage(75);
	m_inputType = InputType::Transact;
	m_graphics.resetBlink();
	m_callbackStack.push(std::bind(&U3Misc::PeerGemCallback, this));
}

void U3Misc::PeerGemCallback()
{
	short rosnum;

	if (m_input_num > 4 || m_input_num < 0 || m_Party[6 + m_input_num] == 0)
	{
		m_scrollArea.UPrintWin("\n\n");
		return;
	}
	rosnum = m_Party[6 + m_input_num];
	std::string strRosNum = std::to_string(rosnum) + std::string("\n\n");
	/*if (m_Player[rosnum][37] < 1)
	{
		m_scrollArea.UPrintWin(strRosNum);
		m_scrollArea.UPrintMessage(67);
	}
	else*/
	{
		m_scrollArea.blockPrompt(true);
		m_scrollArea.UPrintWin(strRosNum);
		m_Player[rosnum][37]--;
		m_graphics.m_queuedMode = U3GraphicsMode::MiniMap;
	}
}

void U3Misc::Steal()
{
}

void U3Misc::Unlock()
{
	m_scrollArea.UPrintMessage(95);
	m_inputType = InputType::GetDirection;
	m_callbackStack.push(std::bind(&U3Misc::UnlockCallback, this));
}

void U3Misc::UnlockCallback()
{
	if ((m_ys != m_ypos) && (m_xs == m_xpos))
	{
		NotHere();
	}
	else
	{
		short temp = GetXYVal(m_xs, m_ys);
		if (temp != 184) // not a door
		{
			NotHere();
		}
		else
		{
			m_scrollArea.UPrintMessage(96);
			m_inputType = InputType::Transact;
			m_callbackStack.push(std::bind(&U3Misc::UnlockKeyCallback, this));
		}
	}
}
void U3Misc::UnlockKeyCallback()
{
	short rosNum;
	if (m_input_num < 0)
	{
		m_scrollArea.UPrintWin("\n");
		return;
	}
	if (m_input_num > 4 || m_Party[6 + m_input_num] == 0)
	{
		std::string dispString = std::to_string(m_input_num + 1) + std::string("\n");
		m_scrollArea.UPrintWin(dispString);
		m_scrollArea.UPrintMessage(41);
	}
	else
	{
		rosNum = m_Party[6 + m_input_num];
		if (m_Player[rosNum][38] < 1)
		{
			m_scrollArea.UPrintMessage(67);
		}
		else
		{
			m_Player[rosNum][38]--;
			// see HideMonsters for visual under-tile choice equivalent
			int mon = MonsterHere(m_xs - 1, m_ys);
			unsigned char value = (mon < 255) ? m_Monsters[(mon + TILEON) % 256] : GetXYVal(m_xs - 1, m_ys);
			if (value > 0x20)
			{
				value = 0x04;
			}
			PutXYVal(value, m_xs, m_ys);
		}
	}
}

bool stayingalive = false;
void U3Misc::CheckAllDead() // $71B4
{
	bool alive;
	char byte;

	for (byte = 0; byte < 4; byte++)
	{
		if (CheckAlive(byte) == true)
		{
			alive = true;
		}
	}
	alive = stayingalive;
	stayingalive = true;
	if (!alive)
	{
		m_wx = 25;
		m_wy = 23;
		m_scrollArea.UPrintMessage(109);    // ALL PLAYERS OUT!
		bool autosave;
		m_resources.GetPreference(U3PreferencesType::Auto_Save, autosave);
		if (autosave)
		{
			/*PutParty();
			PutRoster();
			PutSosaria();*/
		}
		m_inputType = InputType::AnyKey;
		m_callbackStack.push(std::bind(&U3Misc::CheckAllDeadPause, this));
	}
}

void U3Misc::CheckAllDeadPause()
{
	m_resources.CreateAlertMessage(416, DialogType::DITL);
}
