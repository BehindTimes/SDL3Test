#include "U3Misc.h"
#include "UltimaGraphics.h"
#include "U3ScrollArea.h"
#include "U3Resources.h"
#include "UltimaSpellCombat.h"
#include "UltimaIncludes.h"
#include "U3Utilities.h"
#include "UltimaDungeon.h"
#include "UltimaSound.h"
#include <SDL3/SDL.h>
#include <iostream>

extern std::unique_ptr<U3Resources> m_resources;
extern std::unique_ptr<U3Misc> m_misc;
extern std::unique_ptr<U3Graphics> m_graphics;
extern std::unique_ptr<U3ScrollArea> m_scrollArea;
extern std::unique_ptr<U3Utilities> m_utilities;
extern std::unique_ptr<UltimaSpellCombat> m_spellCombat;
extern std::unique_ptr<UltimaDungeon> m_dungeon;
extern std::unique_ptr<U3Audio> m_audio;

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
	m_ox(0),
	m_oy(0),
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
	m_wy(0),
	m_YellStat(false),
	m_lastCard(0x1E),
	m_checkDead(false),
	m_elapsedSleepTime(0),
	m_sleepCheckTime(0),
	m_m5BDC(false),
	m_gameMode(GameStateMode::Map),
	m_map_id(0),
	m_chNum(0),
	m_freezeAnimation(false),
	m_currentEvent(0),
	m_gWhirlCtr(0),
	m_partyFormed(false),
	m_surpressTextDisplay(false),
	m_gTimeNegate(0),
	m_lastMode(GameStateMode::Map),
	m_GoodPlace(false),
	m_validDirValue(0),
	m_moveMonsterOffset(0),
	m_value(0),
	m_storeBool(false),
	m_lastSaveNumberOfMoves(0),
	m_demoSong(0)
{
	memset(m_gShapeSwapped, 0, sizeof(bool) * 256);
	memset(m_Player, 0, sizeof(char) * (1365)); // 21 * 65
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

	memset(m_careerTable, 0, sizeof(unsigned char) * 12);
	memset(m_Party, 0, sizeof(unsigned char) * 64);
	memset(m_gTime, 0, sizeof(long) * 2);

	m_gMoon[0] = 12;
	m_gMoon[1] = 4;
	m_gMoonDisp[0] = '4';
	m_gMoonDisp[1] = '4';
}

U3Misc::~U3Misc()
{
	m_saved_map.reset();
}

bool U3Misc::OpenRstr()
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
		"Sosaria_Monsters.ult",
		"Preferences.ult",
		"MainResources.rsrc_MAPS_420.bin",
		"Sosaria_Current.ult",
		"MainResources.rsrc_PRTY_500_Party.bin",
		"Party.ult",
		"MainResources.rsrc_MISC_400.bin",
		"Moongate_Locations.ult",
		/*"MainResources.rsrc_MISC_401.bin",
		"Type Initial Table.ult",
		"MainResources.rsrc_MISC_402.bin",
		"Weapon Use By class.ult",
		"MainResources.rsrc_MISC_403.bin",
		"Armour Use By class.ult",*/
		"MainResources.rsrc_MISC_404.bin",
		"Location_Table.ult",
		/*"MainResources.rsrc_MISC_405.bin",
		"Experience Table.ult",*/
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
				return false;
			}

		}
		// New game, so copy stuff
		std::vector<char> dummy;
		if (!std::filesystem::exists(rosterPath))
		{
			std::filesystem::path monsterFile = currentPath / orig_files[0];
			dummy.resize(256);
			std::string strTemp = m_utilities->PathToSDLString(monsterFile);
			if (strTemp.empty())
			{
				return false;
			}
			SDL_IOStream* file = SDL_IOFromFile(strTemp.c_str(), "wb");
			SDL_WriteIO(file, dummy.data(), dummy.size());
			SDL_CloseIO(file);
			dummy.clear();
			dummy.resize(32);
			memset(dummy.data(), 1, sizeof(char) * 8);
			std::filesystem::path preferencesFile = currentPath / orig_files[1];
			strTemp = m_utilities->PathToSDLString(preferencesFile);
			if (strTemp.empty())
			{
				return false;
			}
			file = SDL_IOFromFile(strTemp.c_str(), "wb");
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
	catch ([[maybe_unused]] const std::exception& e)
	{
		return false;
	}
	return true;
}

void U3Misc::PutMiscStuff() const
{
	std::filesystem::path moongatePath = std::filesystem::current_path();
	moongatePath /= ResourceLoc;
	moongatePath /= SaveLoc;
	moongatePath /= std::string("Moongate_Locations.ult");

	std::filesystem::path locationPath = std::filesystem::current_path();
	locationPath /= ResourceLoc;
	locationPath /= SaveLoc;
	locationPath /= std::string("Location_Table.ult");

	std::vector<unsigned char> vec_moongates;
	vec_moongates.resize(16);
	memcpy(vec_moongates.data(), m_MoonXTable, sizeof(unsigned char) * 8);
	memcpy(vec_moongates.data() + 8, m_MoonYTable, sizeof(unsigned char) * 8);

	std::string strTemp = m_utilities->PathToSDLString(moongatePath);
	if (strTemp.empty())
	{
		return;
	}
	SDL_IOStream* file = SDL_IOFromFile(strTemp.c_str(), "wb");
	if (!file)
	{
		return;
	}

	SDL_WriteIO(file, vec_moongates.data(), vec_moongates.size());
	SDL_CloseIO(file);

	std::vector<unsigned char> vec_locations;
	vec_locations.resize(64);
	// It appears as if some of the data is not being used.  Perhaps it was in the past?
	std::fill(vec_locations.begin(), vec_locations.end(), 0xFF);
	memcpy(vec_locations.data(), m_LocationX, sizeof(unsigned char) * 20);
	memcpy(vec_locations.data() + 32, m_LocationY, sizeof(unsigned char) * 20);

	strTemp = m_utilities->PathToSDLString(locationPath);
	if (strTemp.empty())
	{
		return;
	}
	file = SDL_IOFromFile(strTemp.c_str(), "wb");
	if (!file)
	{
		return;
	}

	SDL_WriteIO(file, vec_locations.data(), vec_locations.size());
	SDL_CloseIO(file);
}

void U3Misc::GetMiscStuff(bool defaultData)
{
	std::vector<std::string> orig_files = {
		"Moongate_Locations.ult",
		"Type Initial Table.ult",
		"Weapon Use By class.ult",
		"Armour Use By class.ult",
		"Location_Table.ult",
		"Experience Table.ult",
		"MainResources.rsrc_MISC_400.bin",
		"MainResources.rsrc_MISC_401.bin",
		"MainResources.rsrc_MISC_402.bin",
		"MainResources.rsrc_MISC_403.bin",
		"MainResources.rsrc_MISC_404.bin",
		"MainResources.rsrc_MISC_405.bin"
	};
	unsigned short byte;
	std::filesystem::path moongateFile;
	std::filesystem::path typeFile;
	std::filesystem::path weaponFile;
	std::filesystem::path armourFile;
	std::filesystem::path locationFile;
	std::filesystem::path experienceFile;

	std::filesystem::path currentPath = std::filesystem::current_path();
	std::filesystem::path currentPath1 = std::filesystem::current_path();

	currentPath /= ResourceLoc;
	currentPath1 /= ResourceLoc;
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
		currentPath1 /= BinLoc;
		moongateFile = currentPath / orig_files[0];
		typeFile = currentPath1 / orig_files[7];
		weaponFile = currentPath1 / orig_files[8];
		armourFile = currentPath1 / orig_files[9];
		locationFile = currentPath / orig_files[4];
		experienceFile = currentPath1 / orig_files[11];
	}

	std::vector<std::filesystem::path> m_filePaths = { typeFile, weaponFile, armourFile, experienceFile };
	std::vector<unsigned char*> m_fileData = { m_careerTable, m_wpnUseTable, m_armUseTable, m_Experience };
	std::vector<size_t> m_arraySize = {12, 12, 12, 17};

	std::uintmax_t file_size = std::filesystem::file_size(moongateFile);
	std::vector<unsigned char> dummy;

	dummy.resize(file_size);
	std::string strTemp = m_utilities->PathToSDLString(moongateFile);
	if (strTemp.empty())
	{
		return;
	}
	SDL_IOStream* file = SDL_IOFromFile(strTemp.c_str(), "rb");
	SDL_ReadIO(file, dummy.data(), dummy.size());
	SDL_CloseIO(file);

	for (byte = 0; byte < 8; byte++)
	{
		m_MoonXTable[byte] = dummy[byte];
		m_MoonYTable[byte] = dummy[static_cast<size_t>(byte + 8)];
	}

	for (size_t index = 0; index < m_filePaths.size(); ++index)
	{
		memset(m_fileData[index], 0, sizeof(unsigned char) * m_arraySize[index]);

		if (!std::filesystem::exists(m_filePaths[0]))
		{
			return;
		}

		file_size = std::filesystem::file_size(m_filePaths[index]);
		dummy.resize(file_size);
		strTemp = m_utilities->PathToSDLString(m_filePaths[index]);
		if (strTemp.empty())
		{
			return;
		}
		file = SDL_IOFromFile(strTemp.c_str(), "rb");
		SDL_ReadIO(file, dummy.data(), dummy.size());
		SDL_CloseIO(file);
		if (dummy.size() < m_arraySize[index])
		{
			memcpy(m_fileData[index], dummy.data(), m_arraySize[index]);
		}
	}


	file_size = std::filesystem::file_size(locationFile);
	dummy.resize(file_size);
	strTemp = m_utilities->PathToSDLString(locationFile);
	if (strTemp.empty())
	{
		return;
	}
	file = SDL_IOFromFile(strTemp.c_str(), "rb");
	SDL_ReadIO(file, dummy.data(), dummy.size());
	SDL_CloseIO(file);

	for (byte = 0; byte < 20; byte++)
	{
		m_LocationX[byte] = dummy[byte];
		m_LocationY[byte] = dummy[static_cast<size_t>(byte + 32)];
	}
}

bool U3Misc::PutRoster() const
{
	std::filesystem::path rosterPath = std::filesystem::current_path();
	rosterPath /= ResourceLoc;
	rosterPath /= SaveLoc;
	rosterPath /= std::string("Roster.ult");

	short player;
	//const size_t roster_size = 1280;

	std::string strTemp = m_utilities->PathToSDLString(rosterPath);
	if (strTemp.empty())
	{
		return false;
	}
	SDL_IOStream* file = SDL_IOFromFile(strTemp.c_str(), "wb");
	if (!file)
	{
		return false;
	}

	for (player = 0; player < 20; player++)
	{

		SDL_WriteIO(file, m_Player[player + 1], 64);
	}


	SDL_CloseIO(file);

	return true;
}

bool U3Misc::PutParty() const
{
	std::filesystem::path partyPath = std::filesystem::current_path();
	partyPath /= ResourceLoc;
	partyPath /= SaveLoc;
	partyPath /= std::string("Party.ult");

	std::string strTemp = m_utilities->PathToSDLString(partyPath);
	if (strTemp.empty())
	{
		return false;
	}
	SDL_IOStream* file = SDL_IOFromFile(strTemp.c_str(), "wb");
	if (!file)
	{
		return false;
	}

	SDL_WriteIO(file, m_Party, 64);

	SDL_CloseIO(file);

	return true;
}

bool U3Misc::PutSosaria()
{
	if (m_Party[2] != 0)
	{
		return false;
	}
	int mapLength = m_misc->m_mapSize * m_misc->m_mapSize;

	std::filesystem::path mapPath = std::filesystem::current_path();
	mapPath /= ResourceLoc;
	mapPath /= SaveLoc;
	mapPath /= std::string("Sosaria_Current.ult");

	std::string strTemp = m_utilities->PathToSDLString(mapPath);
	if (strTemp.empty())
	{
		return false;
	}
	SDL_IOStream* file = SDL_IOFromFile(strTemp.c_str(), "wb");
	if (!file)
	{
		return false;
	}

	unsigned char mSize = m_misc->m_mapSize & 0xFF;

	SDL_WriteIO(file, &mSize, 1);
	SDL_WriteIO(file, m_map.data(), mapLength);

	unsigned char whirlX = (unsigned char)m_misc->m_WhirlX;
	unsigned char whirlY = (unsigned char)m_misc->m_WhirlY;
	unsigned char whirlDX = (unsigned char)m_misc->m_WhirlDX;
	unsigned char whirlDY = (unsigned char)m_misc->m_WhirlDY;

	SDL_WriteIO(file, &whirlX, 1);
	SDL_WriteIO(file, &whirlY, 1);
	SDL_WriteIO(file, &whirlDX, 1);
	SDL_WriteIO(file, &whirlDY, 1);

	SDL_CloseIO(file);

	std::filesystem::path monsterPath = std::filesystem::current_path();
	monsterPath /= ResourceLoc;
	monsterPath /= SaveLoc;
	monsterPath /= std::string("Sosaria_Monsters.ult");

	strTemp = m_utilities->PathToSDLString(monsterPath);
	if (strTemp.empty())
	{
		return false;
	}
	file = SDL_IOFromFile(strTemp.c_str(), "wb");
	if (!file)
	{
		return false;
	}

	SDL_WriteIO(file, m_misc->m_Monsters, 256);

	SDL_CloseIO(file);

	return true;
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
	std::string strTemp = m_utilities->PathToSDLString(rosterPath);
	if (strTemp.empty())
	{
		return false;
	}
	SDL_IOStream* file = SDL_IOFromFile(strTemp.c_str(), "rb");
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
	std::string strTemp = m_utilities->PathToSDLString(partyPath);
	if (strTemp.empty())
	{
		return false;
	}
	SDL_IOStream* file = SDL_IOFromFile(strTemp.c_str(), "rb");
	if (!file)
	{
		return false;
	}
	SDL_ReadIO(file, m_Party, party_size);
	SDL_CloseIO(file);

	m_xpos = m_Party[3];
	m_ypos = m_Party[4];

	if (m_Party[6] == 0)   // none should belong to a party
	{
		m_partyFormed = false;
		for (short i = 1; i < 21; i++)
		{
			m_Player[i][16] = 0;
		}
	}
	else
	{
		m_partyFormed = true;
	}

	return true;
}

bool U3Misc::GetSosaria()
{
	LoadUltimaMap(19);
	BlockExodus();
	return true;
}

void U3Misc::BlockExodus()
{
	if (m_Party[2] != 0)
	{
		return;
	}
	if (GetXYVal(0x0A, 0x35) == 0x1C && GetXYVal(0x0B, 0x36) == 0x00 && GetXYVal(0x0C, 0x35) == 0x10)
	{
		bool allowDiagonols;
		m_resources->GetPreference(U3PreferencesType::Allow_Diagonal, allowDiagonols);
		if (!allowDiagonols)
		{
			PutXYVal(0x84, 0x09, 0x35);
			PutXYVal(0x84, 0x0B, 0x35);
		}
		else
		{
			PutXYVal(0x10, 0x09, 0x35);
			PutXYVal(0x10, 0x0B, 0x35);
		}
	}
}

void U3Misc::LoadUltimaMap(short map)
{
	std::map<int, std::string> mapList = {
		{ 0, "MainResources.rsrc_MAPS_400" },
		{ 1, "MainResources.rsrc_MAPS_401" },
		{ 2, "MainResources.rsrc_MAPS_402" },
		{ 3, "MainResources.rsrc_MAPS_403" },
		{ 4, "MainResources.rsrc_MAPS_404" },
		{ 5, "MainResources.rsrc_MAPS_405" },
		{ 6, "MainResources.rsrc_MAPS_406" },
		{ 7, "MainResources.rsrc_MAPS_407" },
		{ 8, "MainResources.rsrc_MAPS_408" },
		{ 9, "MainResources.rsrc_MAPS_409" },
		{ 10, "MainResources.rsrc_MAPS_410" },
		{ 11, "MainResources.rsrc_MAPS_411" },
		{ 12, "MainResources.rsrc_MAPS_412" },
		{ 13, "MainResources.rsrc_MAPS_413" },
		{ 14, "MainResources.rsrc_MAPS_414" },
		{ 15, "MainResources.rsrc_MAPS_415" },
		{ 16, "MainResources.rsrc_MAPS_416" },
		{ 17, "MainResources.rsrc_MAPS_417" },
		{ 18, "MainResources.rsrc_MAPS_418" },
		{ 19, "Sosaria_Current" },
		{ 20, "MainResources.rsrc_MAPS_420" },
		{ 21, "MainResources.rsrc_MAPS_421" },
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
		{ 19, "Sosaria_Monsters" },
		{ 20, "MainResources.rsrc_MONS_420" },
		{ 21, "MainResources.rsrc_MONS_421" },
	};

	std::map<int, std::string> talkList = {
		{ 0, "MainResources.rsrc_TLKS_400" },
		{ 1, "MainResources.rsrc_TLKS_401" },
		{ 2, "MainResources.rsrc_TLKS_402" },
		{ 3, "MainResources.rsrc_TLKS_403" },
		{ 4, "MainResources.rsrc_TLKS_404" },
		{ 5, "MainResources.rsrc_TLKS_405" },
		{ 6, "MainResources.rsrc_TLKS_406" },
		{ 7, "MainResources.rsrc_TLKS_407" },
		{ 8, "MainResources.rsrc_TLKS_408" },
		{ 9, "MainResources.rsrc_TLKS_409" },
		{ 10, "MainResources.rsrc_TLKS_410" },
		{ 11, "MainResources.rsrc_TLKS_411" },
		{ 12, "MainResources.rsrc_TLKS_412" },
		{ 13, "MainResources.rsrc_TLKS_413" },
		{ 14, "MainResources.rsrc_TLKS_414" },
		{ 15, "MainResources.rsrc_TLKS_415" },
		{ 16, "MainResources.rsrc_TLKS_416" },
		{ 17, "MainResources.rsrc_TLKS_417" },
		{ 18, "MainResources.rsrc_TLKS_418" },
		{ 21, "MainResources.rsrc_TLKS_421" },
	};

	if (mapList.find(map) == mapList.end())
	{
		return;
	}
	m_map_id = map;
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

		std::string strTemp = m_utilities->PathToSDLString(currentPath);
		if (strTemp.empty())
		{
			return;
		}
		SDL_IOStream* file = SDL_IOFromFile(strTemp.c_str(), "rb");
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
				strTemp = m_utilities->PathToSDLString(currentPath);
				if (strTemp.empty())
				{
					return;
				}
				file = SDL_IOFromFile(strTemp.c_str(), "wb");
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
				strTemp = m_utilities->PathToSDLString(monsterPath);
				if (strTemp.empty())
				{
					return;
				}
				file = SDL_IOFromFile(strTemp.c_str(), "rb");
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
				m_WhirlX = map_data[static_cast<size_t>(mapLength + 1)];
				m_WhirlY = map_data[static_cast<size_t>(mapLength + 2)];
				m_WhirlDX = map_data[static_cast<size_t>(mapLength + 3)];
				m_WhirlDY = map_data[static_cast<size_t>(mapLength + 4)];
			}
		}
		if (map < 19 || map == 21) // "<419" *was* "<420", big mistake.
		{
			talkPath /= BinLoc;
			talkPath /= talkList[map];
			talkPath += std::string(".bin");
			std::vector<unsigned char> talk_data;

			file_size = std::filesystem::file_size(talkPath);
			if (file_size != 256)
			{
				return;
			}
			talk_data.resize(file_size);

			strTemp = m_utilities->PathToSDLString(talkPath);
			if (strTemp.empty())
			{
				return;
			}
			file = SDL_IOFromFile(strTemp.c_str(), "rb");
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

short U3Misc::MonsterHere(short x, short y) const
{
	for (short index = 32; index >= 0; --index)
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
	return m_resources->m_TileArray[y * 11 + x];
}

void U3Misc::PutXYVal(unsigned char value, unsigned char x, unsigned char y)
{
	m_mapOffset = (m_graphics->MapConstrain(y) * m_mapSize);
	m_mapOffset += m_graphics->MapConstrain(x);
	m_map[m_mapOffset] = value;
}

unsigned char U3Misc::GetXYVal(short x, short y)
{
	unsigned char value = 0;
	m_mapOffset = (m_graphics->MapConstrain(y) * m_mapSize);
	m_mapOffset += m_graphics->MapConstrain(x);
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

	m_resources->m_TileArray[y * 11 + x] = (unsigned char)value;
}

bool U3Misc::CheckAlive(short member) const // $75BA
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
short U3Misc::MaxMana(char rosNum) const
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

bool U3Misc::ValidTrans(char value) const
{
	if (m_Party[15])
	{
		return true;
	}
	bool cango = true;
	bool includeWind;
	m_resources->GetPreference(U3PreferencesType::Include_Wind, includeWind);
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

// This was a bug in the Mac version, but not in the original PC version.
// I'm unaware if this was a bug in the original Apple II version.
// In the 1983 PC version, everyone needs the Mark of Force to get through
// a force field.  In the Mac version, as long as the leader has the
// mark, everyone can pass through, even though there will be damage.
// I'm changing this to the PC version

// A second bug?/quirk, but I'm leaving it here, as it exists in both games.
// Only one character can die to a force field.  If multiple people are
// missing the mark, only one character will ever take damage, even if
// they're already dead.

void U3Misc::HandleForceField()
{
	bool GoodPlace = true;

	for (char byte = 0; byte < 4; byte++)
	{
		if (m_Party[6 + byte] != 0)
		{
			if (!(m_Player[m_Party[byte + 6]][14] & 0x10))
			{
				InverseCharDetails(byte, true);
				HPSubtract(m_Party[byte + 6], 99);
				GoodPlace = false;
				break;
			}
		}
	}

	if (GoodPlace)
	{
		InverseTiles(true, 200);
	}
	else
	{
		InverseTiles(true, 500);
	}

	m_GoodPlace = GoodPlace;
}

void U3Misc::HandleLava()
{
	m_opnum = 0;
	m_GoodPlace = true;
	m_callbackStack.push(std::bind(&U3Misc::HandleLavaCallback, this));
}

bool U3Misc::HandleLavaCallback()
{
	if (m_callbackStack.size() > 0)
	{
		m_callbackStack.pop();
	}
	// Should never happen, but just keeping this in for paranoia
	if (m_opnum > 3)
	{
		return false;
	}
	m_callbackStack.push(std::bind(&U3Misc::HandleLavaCallback1, this));
	if (m_Party[6 + m_opnum] != 0)
	{
		char byte2 = m_Party[m_opnum + 6];
		if (!(m_Player[byte2][14] & 0x20))
		{
			if ((m_Player[byte2][17] == 'G') || (m_Player[byte2][17] == 'P'))
			{
				HPSubtract(m_Party[m_opnum + 6], 50);
				InverseCharDetails(m_opnum, true);
				m_resources->m_inverses.elapsedTileTime = 0;
				m_resources->m_inverses.inverseTileTime = 200;
				m_resources->setInversed(true);
				m_callbackStack.push(std::bind(&U3Misc::InverseCallback, this));
			}
		}
	}

	return false;
}

bool U3Misc::HandleLavaCallback1()
{
	if (m_callbackStack.size() > 0)
	{
		m_callbackStack.pop();
	}
	m_opnum++;
	if (m_opnum < 4)
	{
		m_callbackStack.push(std::bind(&U3Misc::HandleLavaCallback, this));
	}

	return false;
}

bool U3Misc::ValidDir()
{
	if (m_callbackStack.size() > 0)
	{
		m_callbackStack.pop();
	}

#ifdef NDEBUG
	m_GoodPlace = false;
	unsigned char value = m_validDirValue;

	if (m_Party[0] == 0x16) // Ship
	{
		if ((value == 00) || (value == 48))
		{
			m_GoodPlace = true; // Water or whirlpool OK
		}
	}
	else
	{
		if (value == 128) // Forcefield
		{
			HandleForceField();
			return false;
		}
		else if (value == 132) // Lava
		{
			HandleLava();
			return false;
		}
		else if ((value == 136) || (value == 248))
		{
			value = 4;
		}

		if ((value < 48) && (value != 0) && (value != 16))
		{
			m_GoodPlace = true;
			if (m_Party[0] == 20)
			{
			}
			else
			{
			}
		}
	}
#else
	m_GoodPlace = true;
#endif

	return false;
}

void U3Misc::NoGo()
{
	m_scrollArea->UPrintMessage(116);
}

bool U3Misc::HandleKeyPress(SDL_KeyboardEvent key)
{
	switch (m_inputType)
	{
	case InputType::None:
		return false;
	case InputType::LetterImmediate:
		HandleLetterImmediate(key.key);
		break;
	case InputType::NumImmediate:
		HandleNumImmediate(key.key);
		break;
	case InputType::GuildVendor:
		HandleInputGuild(key.key);
		break;
	case InputType::Restricted:
		HandleInputRestricted(key.key);
		break;
	case InputType::BuySell:
		HandleInputBuySell(key.key);
		break;
	case InputType::YesNo:
		HandleInputYesNo(key.key);
		break;
	case InputType::ChooseWizardCleric:
		HandleInputChooseWizardCleric(key.key);
		break;
	case InputType::InputText:
		HandleInputText(key);
		break;
	case InputType::Transact:
		HandleTransactPress(key.key);
		break;
	case InputType::GetDirection:
		HandleDircetionKeyPress(key.key);
		break;
	case InputType::ZStats:
	case InputType::AnyKey:
		HandleAnyKey();
		break;
	case InputType::AnyKeyEscape:
		HandleAnyKeyEscape(key.key);
		break;
	case InputType::SleepCallback:
		HandleCallback(true);
		break;
	default:
		if (m_gameMode == GameStateMode::Map)
		{
			HandleDefaultKeyPress(key.key);
		}
		else if (m_gameMode == GameStateMode::Dungeon)
		{
			m_dungeon->HandleDefaultKeyPress(key.key);
		}
		else if (m_gameMode == GameStateMode::Combat)
		{
			m_spellCombat->HandleDefaultKeyPress(key.key);
		}
		return true;
	}
	return false;
}

void U3Misc::HandleTransactPress(SDL_Keycode key)
{
	bool handled = false;

	m_input_num = -1;

	if (key >= SDLK_1 && key <= SDLK_4)
	{
		m_input_num = (short)(key - SDLK_1);
		handled = true;
	}
	else if (key == SDLK_SPACE || key == SDLK_RETURN)
	{
		handled = true;
	}

	if (handled)
	{
		m_inputType = InputType::Default;
		if (m_callbackStack.size() > 0)
		{
			m_callbackStack.pop();
		}
	}
}

void U3Misc::HandleDircetionKeyPress(SDL_Keycode key)
{
	short dirgot = false;
	m_input_num = -1;

	switch (key)
	{
	case SDLK_KP_8:
	case SDLK_UP:
		m_input_num = 0;
		m_xs = m_xpos;
		m_ys = m_ypos - 1;
		m_dx = 0;
		m_dy = -1;
		dirgot = true;
		m_scrollArea->UPrintMessage(24);
		break;
	case SDLK_KP_2:
	case SDLK_DOWN:
		m_input_num = 0;
		m_xs = m_xpos;
		m_ys = m_ypos + 1;
		m_dx = 0;
		m_dy = 1;
		dirgot = true;
		m_scrollArea->UPrintMessage(25);
		break;
	case SDLK_KP_4:
	case SDLK_LEFT:
		m_input_num = 0;
		m_xs = m_xpos - 1;
		m_ys = m_ypos;
		m_dx = -1;
		m_dy = 0;
		dirgot = true;
		m_scrollArea->UPrintMessage(27);
		break;
	case SDLK_KP_6:
	case SDLK_RIGHT:
		m_input_num = 0;
		m_xs = m_xpos + 1;
		m_ys = m_ypos;
		m_dx = 1;
		m_dy = 0;
		dirgot = true;
		m_scrollArea->UPrintMessage(26);
		break;
	case SDLK_KP_1:
		if (m_resources->m_preferences.allow_diagonal)
		{
			m_input_num = 0;
			m_xs = m_xpos - 1;
			m_ys = m_ypos + 1;
			m_dx = -1;
			m_dy = 1;
			dirgot = true;
			m_scrollArea->UPrintMessage(250);
		}
		break;
	case SDLK_KP_3:
		if (m_resources->m_preferences.allow_diagonal)
		{
			m_input_num = 0;
			m_xs = m_xpos + 1;
			m_ys = m_ypos + 1;
			m_dx = 1;
			m_dy = 1;
			dirgot = true;
			m_scrollArea->UPrintMessage(251);
		}
		break;
	case SDLK_KP_7:
		if (m_resources->m_preferences.allow_diagonal)
		{
			m_input_num = 0;
			m_xs = m_xpos - 1;
			m_ys = m_ypos - 1;
			m_dx = -1;
			m_dy = -1;
			dirgot = true;
			m_scrollArea->UPrintMessage(252);
		}
		break;
	case SDLK_KP_9:
		if (m_resources->m_preferences.allow_diagonal)
		{
			m_input_num = 0;
			m_xs = m_xpos + 1;
			m_ys = m_ypos - 1;
			m_dx = 1;
			m_dy = -1;
			dirgot = true;
			m_scrollArea->UPrintMessage(253);
		}
		break;
	case SDLK_RETURN:
	case SDLK_SPACE:
		dirgot = true;
		break;
	default:
		break;
	}
	if (dirgot)
	{
		m_inputType = InputType::Default;
		if (m_callbackStack.size() > 0)
		{
			m_callbackStack.pop();
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
		m_scrollArea->setInputString(m_input);
		m_scrollArea->setInput(false);
		if (m_callbackStack.size() > 0)
		{
			//auto callbackFunction = m_callbackStack.top();
			m_callbackStack.pop();
			/*if (callbackFunction)
			{
				callbackFunction();
			}*/
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
		m_input_num = -1;
		break;
	}
	if (handled)
	{
		m_inputType = InputType::Default;
		m_scrollArea->setInputString(m_input);
		m_scrollArea->setInput(false);
		if (m_callbackStack.size() > 0)
		{
			//auto callbackFunction = m_callbackStack.top();
			m_callbackStack.pop();
			/*if (callbackFunction)
			{
				callbackFunction();
			}*/
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
		m_input = std::string("N");
		handled = true;
		break;
	default:
		m_input_num = 0;
		break;
	}
	if (handled)
	{
		m_inputType = InputType::Default;
		m_scrollArea->setInputString(m_input);
		m_scrollArea->setInput(false);
		if (m_callbackStack.size() > 0)
		{
			m_callbackStack.pop();
		}
	}
}

void U3Misc::HandleInputChooseWizardCleric(SDL_Keycode key)
{
	bool handled = false;
	switch (key)
	{
	case SDLK_W:
		m_input_num = 1;
		m_input = std::string("W");
		handled = true;
		break;
	case SDLK_C:
		m_input_num = 0;
		m_input = std::string("C");
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
		m_scrollArea->setInputString(m_input);
		m_scrollArea->setInput(false);
		if (m_callbackStack.size() > 0)
		{
			m_callbackStack.pop();
		}
	}
}

void U3Misc::HandleInputRestricted(SDL_Keycode key)
{
	int value = key - SDLK_A;
	if (value >= (int)((m_restrictedStart - 'A')) && value < (int)((m_opnum - 'A')))
	{
		m_input_num = (short)value;
		m_input = std::string(SDL_GetKeyName(key));
		if (m_callbackStack.size() > 0)
		{
			//auto callbackFunction = m_callbackStack.top();
			m_callbackStack.pop();
		}
	}
	else if (key == SDLK_SPACE || key == SDLK_RETURN)
	{
		m_input_num = -1;
		if (m_callbackStack.size() > 0)
		{
			//auto callbackFunction = m_callbackStack.top();
			m_callbackStack.pop();
			/*if (callbackFunction)
			{
				callbackFunction();
			}*/
		}
	}
}

void U3Misc::HandleLetterImmediate(SDL_Keycode key)
{
	bool handled = false;

	m_input.clear();
	if (key >= SDLK_0 && key <= SDLK_9)
	{
		const char* value = SDL_GetKeyName(key);
		if (m_maxInputLength > m_input.size())
		{
			m_input += value;
			handled = true;
		}
	}
	else if (key >= SDLK_A && key <= SDLK_Z)
	{
		if (!m_numOnly)
		{
			const char* value = SDL_GetKeyName(key);
			if (m_maxInputLength > m_input.size())
			{
				m_input += value;
				handled = true;
			}
		}
	}
	else if (key == SDLK_SPACE || key == SDLK_RETURN)
	{
		handled = true;
	}
	if (handled)
	{
		m_inputType = InputType::Default;
		if (m_callbackStack.size() > 0)
		{
			m_scrollArea->setInputString(m_input);
			//auto callbackFunction = m_callbackStack.top();
			m_callbackStack.pop();
			/*if (callbackFunction)
			{
				callbackFunction();
			}*/
		}
	}
}

void U3Misc::HandleNumImmediate(SDL_Keycode key)
{
	bool handled = false;

	if (key >= SDLK_0 && key <= SDLK_9)
	{
		m_input_num = (short)(key - SDLK_0);
		handled = true;
	}
	else if (key == SDLK_SPACE || key == SDLK_RETURN)
	{
		m_input_num = 0;
		handled = true;
	}
	if (handled)
	{
		m_inputType = InputType::Default;
		if (m_callbackStack.size() > 0)
		{
			m_scrollArea->setInputString(std::to_string(m_input_num));
			//auto callbackFunction = m_callbackStack.top();
			m_callbackStack.pop();
			/*if (callbackFunction)
			{
				callbackFunction();
			}*/
		}
	}
}

void U3Misc::HandleInputText(SDL_KeyboardEvent key)
{
	bool handled = false;
	auto display_key = SDL_GetKeyFromScancode(key.scancode, key.mod, false);
	if (display_key >= SDLK_0 && display_key <= SDLK_9)
	{
		const char* value = SDL_GetKeyName(key.key);
		if (m_maxInputLength > m_input.size())
		{
			m_input += value;
			handled = true;
		}
	}
	else if (key.key >= SDLK_A && key.key <= SDLK_Z)
	{
		if (!m_numOnly)
		{
			const char* value = SDL_GetKeyName(key.key);
			if (m_maxInputLength > m_input.size())
			{
				m_input += value;
				handled = true;
			}
		}
	}
	else if (display_key > SDLK_SPACE && display_key <= SDLK_TILDE)
	{
		if (!m_numOnly)
		{
			if (display_key == SDLK_SPACE)
			{
				m_input += ' ';
				handled = true;
			}
			else
			{
				const char* value = SDL_GetKeyName(display_key);
				if (m_maxInputLength > m_input.size())
				{
					m_input += value;
					handled = true;
				}
			}
		}
	}
	else
	{
		switch (key.key)
		{
		case SDLK_BACKSPACE:
			if (m_input.size() > 0)
			{
				m_input.pop_back();
				handled = true;
			}
			break;
		case SDLK_RETURN:
			m_scrollArea->setInput(false);
			handled = false;
			if (m_callbackStack.size() > 0)
			{
				m_callbackStack.pop();
			}
			else
			{
				// This should never be called, but just incase, add a new line
				m_scrollArea->UPrintWin("\n");
			}
			break;
		default:
			break;
		}
	}
	if (handled)
	{
		m_scrollArea->setInputString(m_input);
	}
}

void U3Misc::HandleAnyKeyEscape(SDL_Keycode key)
{
	m_inputType = InputType::Default;
	if (m_callbackStack.size() > 0)
	{
		m_callbackStack.pop();
	}
	m_input_num = 0;
	if (key == SDLK_ESCAPE)
	{
		m_input_num = -1;
	}
}

void U3Misc::HandleAnyKey()
{
	m_inputType = InputType::Default;
	if (m_callbackStack.size() > 0)
	{
		//auto callbackFunction = m_callbackStack.top();
		m_callbackStack.pop();
		/*if (callbackFunction)
		{
			callbackFunction();
		}*/
	}
}

bool U3Misc::InputTextCallback()
{
	if (m_callbackStack.size() > 0)
	{
		//auto callbackFunction = m_callbackStack.top();
		m_callbackStack.pop();
		/*if (callbackFunction)
		{
			callbackFunction();
			return false;
		}*/
	}
	m_inputType = InputType::Default;
	return false;
}

bool U3Misc::InputNumCallback()
{
	m_numOnly = false;
	m_inputType = InputType::Default;
	try
	{
		m_input_num = (short)std::stoi(m_input);
	}
	catch ([[maybe_unused]] const std::exception& e)
	{
		m_input_num = 0;
	}
	if (m_callbackStack.size() > 0)
	{
		m_callbackStack.pop();
		/*auto callbackFunction = m_callbackStack.top();
		m_callbackStack.pop();
		if (callbackFunction)
		{
			callbackFunction();
		}*/
	}
	return true;
}

void U3Misc::HandleCallback(bool sleeping)
{
	m_inputType = InputType::Default;
	// Most likely we threw up an alert message, so this is just to delay any followup until the alert is closed	
	if (m_callbackStack.size() > 0)
	{
		//auto callbackFunction = m_callbackStack.top();
		if (!sleeping)
		{
			m_callbackStack.pop();
		}
		/*if (callbackFunction)
		{
			callbackFunction();
		}*/
	}
	return;
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
	//bool gInterrupt = false;
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
			retVal = HandleKeyPress(event.key);
		}
		break;
	case SDL_EVENT_MOUSE_BUTTON_DOWN:
		mouseState = 1;
		updateMouse = true;
		if (m_inputType == InputType::AnyKey)
		{
			HandleAnyKey();
			//retVal = true;
		}
		else if (m_inputType == InputType::AnyKeyEscape)
		{
			HandleAnyKeyEscape(0);
			//retVal = true;
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

	if (updateMouse)
	{
		if (m_inputType == InputType::ZStats)
		{
			bool isCaptured = false;

			if (m_resources->m_zstatbuttons.size() > 0)
			{
				for (auto& curButton : m_resources->m_zstatbuttons)
				{
					curButton.setMouseCapture(mouseState, event.motion.x, event.motion.y);
					isCaptured |= curButton.isCaptured();
				}
			}
			if (mouseState == 1 && !isCaptured)
			{
				HandleAnyKey();
			}
		}
	}
	return retVal;
}

void U3Misc::What() // $5135
{
	m_scrollArea->UPrintMessage(106);
}

void U3Misc::What2() // $5279
{
	m_scrollArea->UPrintMessage(107);
}

void U3Misc::NotHere() // $5288
{
	m_scrollArea->UPrintMessage(108);
}

void U3Misc::NoEffect()
{
	m_scrollArea->UPrintMessage(248);
}

int movenum = 0;
void U3Misc::IncMoves() // $3AF
{
	movenum++;
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
		m_scrollArea->UPrintMessage(182);
	}
	m_Party[3] = (unsigned char)m_xpos;
	m_Party[4] = (unsigned char)m_ypos;

	m_audio->m_currentSong = 0;
	m_audio->m_nextSong = 1;
	m_audio->musicUpdate();

	bool autosave;
	m_resources->GetPreference(U3PreferencesType::Auto_Save, autosave);
	if (autosave)
	{
		GetSosaria();
		PutRoster();
		PutParty();
		PutSosaria();
	}
	else
	{
		PullSosaria();
	}
}

void U3Misc::Routine6E35()
{
	//short temp;
	IncMoves();

	MoonGateUpdate();
	/*if (m_Party[2] == 1)
	{
		//DrawDungeon();
		m_gameMode = GameStateMode::Dungeon;
		return;
	}*/
	// if $E2=#$80 (combat?), see $6E5C.  Apparently unneccessary
	//   code, since the combat routines never touch this area while in effect.
	if (m_Party[2] > 1) // Town or castle, IOW
	{
		if (m_xpos == 0 || m_ypos == 0)
		{
			Routine6E6B();
		}
	}
	m_callbackStack.push(std::bind(&U3Misc::EndTurnCallback, this));
	m_callbackStack.push(std::bind(&U3Misc::FinishAge, this));
	AgeChars();
	//ShowChars(false);

	/*if (ExodusCastle() == 0)
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
}

bool U3Misc::FinishAge() // $79DD
{
	if (m_callbackStack.size() > 0)
	{
		m_callbackStack.pop();
	}

	short temp = GetXYVal(m_xpos, m_ypos);

	m_callbackStack.push(std::bind(&U3Misc::FinishAll2, this));
	m_callbackStack.push(std::bind(&U3Misc::ExodusCastleCallback, this));
	// These two never compete with each other
	if (temp == 136)
	{
		HandleMoonStep();
	}
	else if (temp == 48)
	{
		GoWhirlPool();
	}

	return false;
}

bool U3Misc::ExodusCastleCallback()
{
	if (m_callbackStack.size() > 0)
	{
		m_callbackStack.pop();
	}

	if (m_spellCombat->ExodusCastle() == 0)
	{
		m_gTimeNegate = 0;
		m_xs = (short)m_utilities->getRandom(0, 11);
		m_ys = (short)m_utilities->getRandom(0, 11);
		m_gBallTileBackground = GetXYTile(m_xs, m_ys);
		if (m_xs == 5 && m_ys == 5)
		{
			PutXYTile(0x7A, m_xs, m_ys);
			BombTrap();
		}
		else
		{
			if (m_gBallTileBackground == 0x10)
			{
				PutXYTile(0x7A, m_xs, m_ys);
			}
		}
	}

	return false;
}

bool U3Misc::FinishAll2()
{
	if (m_callbackStack.size() > 0)
	{
		m_callbackStack.pop();
	}
	//m_tx = 0x18;
	//m_ty = 0x17;
	if (m_Party[0] == 0x14 || m_Party[0] == 0x16)
	{
		m_zp[0xCD] = 255 - m_zp[0xCD];
		if (m_zp[0xCD] < 128)
		{
			return false;
		}
	}
	if (m_gTimeNegate != 0)
	{
		m_gTimeNegate--;
		return false;
	}
	// Spawn Monsters
	SpawnMonster();

	m_callbackStack.push(std::bind(&U3Misc::FinishAll1, this));
	m_callbackStack.push(std::bind(&U3Misc::MoveMonsters, this));

	return false;
}

bool U3Misc::FinishAll1()
{
	if (m_callbackStack.size() > 0)
	{
		m_callbackStack.pop();
	}

	m_resources->m_newMove = true;

	return false;
}

void U3Misc::PullSosaria()
{
	if (!m_saved_map.get()) // was never pushed!
	{
		GetSosaria();
		PutRoster();
		PutParty();
		PutSosaria();
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

	// If Exodus has been defeated, make land creatures placid.
	if (m_Party[15] == 1)
	{
		int i;
		for (i = 0; i < 32; i++)
		{
			if (m_Monsters[i] >= 0x40)
			{
				m_Monsters[i + HPMON] = 0x40;
			}
		}
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

void U3Misc::PrintTile(short tile, bool plural)
{

	std::string theString;
	std::string strKey = plural ? std::string("TilesPlural") : std::string("Tiles");

	if (m_resources->m_plistMap.contains(strKey))
	{
		if (m_resources->m_plistMap[strKey].size() > tile)
		{
			theString = m_resources->m_plistMap[strKey][tile];
			m_scrollArea->UPrintWin(theString);
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

void U3Misc::Speak(short perNum, [[maybe_unused]]short shnum) const
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
		if (talk == 0xFF)
		{
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
	m_resources->GetPreference(U3PreferencesType::Classic_Appearance, classic);
	if (!classic)
	{
		outStr = m_scrollArea->RewrapString(outStr);
	}
	m_scrollArea->UPrintWin(outStr);
}

void U3Misc::InverseChnum(char which, bool value)
{
	if (which < 0 || which > 3)
	{
		return;
	}
	m_resources->m_inverses.character_num[which] = value;
}

void U3Misc::InverseTiles(bool value, Uint64 tileTime)
{
	m_resources->m_inverses.tiles = value;
	m_resources->m_inverses.inverseTileTime = tileTime;
	m_resources->m_inverses.elapsedTileTime = 0;
	m_resources->setInversed(true);
	if (!value)
	{
		m_resources->m_inverses.char_details[0] = false;
		m_resources->m_inverses.char_details[1] = false;
		m_resources->m_inverses.char_details[2] = false;
		m_resources->m_inverses.char_details[3] = false;
	}
	m_callbackStack.push(std::bind(&U3Misc::InverseCallback, this));
}

// Might need to rethink the inversing
void U3Misc::InverseCharDetails(short num, bool value)
{
	if (num >= 0 && num < 4)
	{
		m_resources->m_inverses.char_details[num] = value;
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
		m_scrollArea->UPrintMessageRewrapped(185);
		m_scrollArea->UPrintMessage(186);
		setInputTypeNum(std::bind(&U3Misc::tavernCallback, this));
		break;
	case 1:
		m_scrollArea->UPrintMessage(191);
		m_scrollArea->UPrintMessage(192);
		setInputTypeNum(std::bind(&U3Misc::grocerCallback, this), 4);
		break;
	case 2:
		m_scrollArea->UPrintMessage(196);
		m_scrollArea->UPrintMessage(197);
		m_inputType = InputType::Transact;
		m_scrollArea->setInput(true);
		m_callbackStack.push(std::bind(&U3Misc::healingCallback, this));
		m_callbackStack.push(std::bind(&U3Misc::ProcessEventCallback, this));
		break;
	case 3:
		m_opnum = 'I';
		if (m_Party[3] == 37)
		{
			m_opnum = 'P';    // x location of party on Sosaria
		}
		m_scrollArea->UPrintMessage(206);
		setInputTypeYesNo(std::bind(&U3Misc::weaponsListCallback, this));
		break;
	case 4:
		m_opnum = 'F';
		if (m_Party[3] == 37)
		{
			m_opnum = 'H';    // x location of party on Sosaria
		}
		m_scrollArea->UPrintMessage(216);
		setInputTypeYesNo(std::bind(&U3Misc::armorsListCallback, this));
		break;
	case 5:
		m_scrollArea->UPrintMessage(217);
		m_scrollArea->UPrintMessage(261);
		m_scrollArea->UPrintMessage(218);
		m_inputType = InputType::GuildVendor;
		m_scrollArea->setInput(true);
		m_callbackStack.push(std::bind(&U3Misc::guildCallback, this));
		m_callbackStack.push(std::bind(&U3Misc::ProcessEventCallback, this));
		break;
	case 6:
		m_scrollArea->UPrintMessage(221);
		m_scrollArea->UPrintMessageRewrapped(222);
		setInputTypeNumImmediate(std::bind(&U3Misc::oracleCallback, this));
		break;
	case 7:
		m_scrollArea->UPrintMessage(225);
		m_scrollArea->UPrintWin(std::to_string(m_Party[1]));
		m_scrollArea->UPrintMessage(226);
		m_opnum = m_Party[1] * 200;
		m_scrollArea->UPrintWin(std::to_string(m_opnum));
		m_scrollArea->UPrintMessage(227);
		setInputTypeYesNo(std::bind(&U3Misc::horseVendorCallback, this));
		m_scrollArea->setInput(true);
		break;
	default:
		InverseChnum((char)m_transactNum, false);
		break;
	}
}

bool U3Misc::horseVendorCallback()
{
	if (m_callbackStack.size() > 0)
	{
		m_callbackStack.pop();
	}

	short gold;
	m_scrollArea->setInput(false);
	if (m_input_num != 1)
	{
		m_scrollArea->UPrintWin("\n\n");
		m_scrollArea->UPrintMessageRewrapped(228);
		InverseChnum((char)m_transactNum, false);
		return false;
	}
	gold = (m_Player[m_rosNum][35] * 256) + m_Player[m_rosNum][36];
	if (gold < m_opnum)
	{
		m_scrollArea->UPrintWin("\n\n");
		m_scrollArea->UPrintMessageRewrapped(229);
		//Speech(GetLocalizedPascalString("\pI'm sorry, but you haven't the gold!"),63);
		InverseChnum((char)m_transactNum, false);
		return false;
	}
	gold -= m_opnum;
	m_Player[m_rosNum][35] = (unsigned char)(gold / 256);
	m_Player[m_rosNum][36] = (unsigned char)(gold - (m_Player[m_rosNum][35] * 256));
	m_scrollArea->UPrintWin("\n\n");
	m_scrollArea->UPrintMessageRewrapped(230);
	m_Party[0] = 0x14;
	InverseChnum((char)m_transactNum, false);

	return false;
}

void U3Misc::setInputTypeNumImmediate(std::function<bool()> func)
{
	m_scrollArea->setInput(true);
	m_callbackStack.push(func);
	m_input.clear();
	m_inputType = InputType::NumImmediate;
	m_callbackStack.push(std::bind(&U3Misc::ProcessEventCallback, this));
}

bool U3Misc::oracleCallback()
{
	if (m_callbackStack.size() > 0)
	{
		m_callbackStack.pop();
	}

	m_inputType = InputType::Default;
	short input;
	short gold;
	m_scrollArea->setInput(false);
	m_scrollArea->UPrintWin("\n\n");
	input = m_input_num * 100;
	gold = (m_Player[m_rosNum][35] * 256) + m_Player[m_rosNum][36];
	if (input > gold)
	{
		m_scrollArea->UPrintMessage(188);
		//Speech(GetLocalizedPascalString("\pWhat? Can't pay? Out, you scum!"),16);
		InverseChnum((char)m_transactNum, false);
		return false;
	}
	gold -= input;
	m_Player[m_rosNum][35] = (unsigned char)(gold / 256);
	m_Player[m_rosNum][36] = (unsigned char)(gold - (m_Player[m_rosNum][35] * 256));

	if (m_resources->m_plistMap["Radrion"].size() <= m_input_num)
	{
		InverseChnum((char)m_transactNum, false);
		return false;
	}
	std::string dispString = m_resources->m_plistMap["Radrion"][m_input_num];

	bool classic;
	m_resources->GetPreference(U3PreferencesType::Classic_Appearance, classic);
	if (!classic)
	{
		dispString = m_scrollArea->RewrapString(dispString);
	}
	m_scrollArea->UPrintWin(dispString);
	m_scrollArea->UPrintMessage(223);
	setInputTypeYesNo(std::bind(&U3Misc::oracleFinishCallback, this));
	m_scrollArea->setInput(true);

	return false;
}

bool U3Misc::oracleFinishCallback()
{
	if (m_callbackStack.size() > 0)
	{
		m_callbackStack.pop();
	}

	m_scrollArea->setInput(false);
	if (m_input_num == 1)
	{
		m_scrollArea->UPrintMessageRewrapped(222);
		setInputTypeNumImmediate(std::bind(&U3Misc::oracleCallback, this));
	}
	else
	{
		m_scrollArea->UPrintWin("\n");
		m_scrollArea->UPrintMessageRewrapped(224);
		InverseChnum((char)m_transactNum, false);
		return false;
	}

	return false;
}

bool U3Misc::guildCallback()
{
	if (m_callbackStack.size() > 0)
	{
		m_callbackStack.pop();
	}

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
		m_scrollArea->UPrintWin("N\n\n");
		m_scrollArea->UPrintMessageRewrapped(219);
		m_scrollArea->setInput(false);
		InverseChnum((char)m_transactNum, false);
		return false;
	}

	m_scrollArea->setInput(false);
	m_scrollArea->UPrintWin("\n");
	if (m_Player[m_rosNum][attrib] + qty > 99)
	{
		m_scrollArea->UPrintMessageRewrapped(260);
		m_scrollArea->UPrintMessage(261);
		m_scrollArea->UPrintMessage(218);
		m_inputType = InputType::GuildVendor;
		m_scrollArea->setInput(true);
		m_callbackStack.push(std::bind(&U3Misc::guildCallback, this));
		m_callbackStack.push(std::bind(&U3Misc::ProcessEventCallback, this));
		return false;
	}

	if (GuildPay(m_rosNum, cost) == 0)
	{
		GuildGive(m_rosNum, attrib, qty);
	}
	else
	{
		m_scrollArea->setInput(false);
		InverseChnum((char)m_transactNum, false);
		return false;
	}
	m_scrollArea->UPrintMessage(220);
	setInputTypeYesNo(std::bind(&U3Misc::guildCompleteCallback, this));

	return false;
}

bool U3Misc::guildCompleteCallback()
{
	if (m_callbackStack.size() > 0)
	{
		m_callbackStack.pop();
	}

	m_scrollArea->setInput(false);
	if (m_input_num == 1) // yes
	{
		m_scrollArea->UPrintMessage(261);
		m_scrollArea->UPrintMessage(218);
		m_inputType = InputType::GuildVendor;
		m_scrollArea->setInput(true);
		m_callbackStack.push(std::bind(&U3Misc::guildCallback, this));
		m_callbackStack.push(std::bind(&U3Misc::ProcessEventCallback, this));
	}
	else
	{
		m_scrollArea->UPrintWin("\n\n");
		m_scrollArea->UPrintMessage(219);
		InverseChnum((char)m_transactNum, false);
	}

	return false;
}

bool U3Misc::GuildPay(short rosNum, short cost)
{
	short gold;
	gold = (m_Player[rosNum][35] * 256) + m_Player[rosNum][36];
	if (gold < cost)
	{
		m_scrollArea->UPrintMessageRewrapped(231);
		//Speech(GetLocalizedPascalString("\pI'm sorry, but you have not the funds!"),23);
		return true;
	}
	gold -= cost;
	m_Player[rosNum][35] = (unsigned char)(gold / 256);
	m_Player[rosNum][36] = (unsigned char)(gold - (m_Player[rosNum][35] * 256));
	return false;
}

void U3Misc::GuildGive(short rosNum, short item, short amount)
{
	m_Player[rosNum][item] += (unsigned char)amount;
	if (m_Player[rosNum][item] > 99)
	{
		m_Player[rosNum][item] = 99;
	}
}

void U3Misc::WeaponList()
{
	//m_scrollArea->blockPrompt(true);
	m_scrollArea->UPrintMessage(237);
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
	//listWeaponsCallback();
	m_callbackStack.push(std::bind(&U3Misc::listWeaponsCallback, this));
}

void U3Misc::ArmorsList()
{
	//m_scrollArea->blockPrompt(true);
	m_scrollArea->UPrintMessage(237);
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
	//listArmorsCallback();
	m_callbackStack.push(std::bind(&U3Misc::listArmorsCallback, this));
}

void U3Misc::PrintWeaponList(short weapon)
{
	char curWeapon = (char)('A' + weapon);
	std::string strWeapon;
	std::string dispString = m_resources->m_plistMap["WeaponsArmour"][weapon];
	strWeapon += curWeapon + std::string(":");
	if (!dispString.ends_with('s') && dispString.size() < 7)
	{
		dispString += 's';
	}
	strWeapon += dispString;
	dispString = m_resources->m_plistMap["WeaponsArmour"][static_cast<size_t>(weapon + 24)];
	dispString += std::string("gp");
	dispString += std::string("\n");
	std::string strSpaces;
	for (int index = 0; index < 17 - (strWeapon.size() + dispString.size()); ++index)
	{
		strSpaces += ' ';
	}
	strWeapon += strSpaces + dispString;
	m_scrollArea->UPrintWin(strWeapon, true);
}

void U3Misc::PrintArmorList(short armor)
{
	char curWeapon = (char)('A' + armor);
	std::string strWeapon;
	std::string dispString = m_resources->m_plistMap["WeaponsArmour"][armor + 16];
	strWeapon += curWeapon + std::string(":");
	if (!dispString.ends_with('s') && dispString.size() < 7)
	{
		dispString += 's';
	}
	strWeapon += dispString;
	dispString = m_resources->m_plistMap["WeaponsArmour"][static_cast<size_t>(armor) + 40];
	dispString += std::string("gp");
	dispString += std::string("\n");
	std::string strSpaces;
	for (int index = 0; index < 17 - (strWeapon.size() + dispString.size()); ++index)
	{
		strSpaces += ' ';
	}
	strWeapon += strSpaces + dispString;
	m_scrollArea->UPrintWin(strWeapon, true);
}

bool U3Misc::listWeaponsCallback()
{
	if (m_callbackStack.size() > 0)
	{
		m_callbackStack.pop();
	}
	bool anykey = false;
	std::string dispString;
	while (!m_weaponsList.empty())
	{
		short num = (short)m_weaponsList.front();
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
		m_callbackStack.push(std::bind(&U3Misc::ProcessEventCallback, this));
	}
	else
	{
		weaponsBuyOrSell();
	}
	return false;
}

bool U3Misc::listArmorsCallback()
{
	if (m_callbackStack.size() > 0)
	{
		m_callbackStack.pop();
	}
	bool anykey = false;
	std::string dispString;
	while (!m_weaponsList.empty())
	{
		short num = (short)m_weaponsList.front();
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
		m_callbackStack.push(std::bind(&U3Misc::ProcessEventCallback, this));
	}
	else
	{
		armorsBuyOrSell();
	}

	return false;
}

bool U3Misc::weaponsListCallback()
{
	if (m_callbackStack.size() > 0)
	{
		m_callbackStack.pop();
	}
	if (m_input_num == 1) // yes
	{
		WeaponList();
	}
	else
	{
		weaponsBuyOrSell();
	}
	return false;
}

bool U3Misc::armorsListCallback()
{
	if (m_callbackStack.size() > 0)
	{
		m_callbackStack.pop();
	}
	if (m_input_num == 1) // yes
	{
		ArmorsList();
	}
	else
	{
		armorsBuyOrSell();
	}
	return false;
}

void U3Misc::weaponsBuyOrSell()
{
	m_scrollArea->UPrintMessage(207);
	//m_scrollArea->blockPrompt(false);
	setInputTypeBuySell(std::bind(&U3Misc::weaponsBuySellCallback, this));
}

void U3Misc::armorsBuyOrSell()
{
	m_scrollArea->UPrintMessage(207);
	//m_scrollArea->blockPrompt(false);
	setInputTypeBuySell(std::bind(&U3Misc::armorsBuySellCallback, this));
}

void U3Misc::setInputTypeNum(std::function<bool()> func, int inputLength)
{
	//m_callbackStack.push(func);
	m_input.clear();
	m_numOnly = true;
	m_scrollArea->setInput(true);
	m_inputType = InputType::InputText;
	m_maxInputLength = (short)inputLength;
	m_input.clear();
	m_callbackStack.push(func);
	m_callbackStack.push(std::bind(&U3Misc::InputNumCallback, this));
	//m_callbackStack.push(std::bind(&U3Misc::InputTextCallback, this));
	m_callbackStack.push(std::bind(&U3Misc::ProcessEventCallback, this));
}

void U3Misc::setInputTypeYesNo(std::function<bool()> func)
{
	m_scrollArea->setInput(true);
	//m_callbackStack.push(func);
	m_input.clear();
	m_inputType = InputType::YesNo;
	m_callbackStack.push(func);
	m_callbackStack.push(std::bind(&U3Misc::ProcessEventCallback, this));
}

void U3Misc::setInputTypeBuySell(std::function<bool()> func)
{
	m_scrollArea->setInput(true);
	//m_callbackStack.push(func);
	m_input.clear();
	m_inputType = InputType::BuySell;
	m_callbackStack.push(func);
	m_callbackStack.push(std::bind(&U3Misc::ProcessEventCallback, this));
}

void U3Misc::setInputTypeRestricted(std::function<bool()> func, short start)
{
	m_restrictedStart = start;
	m_scrollArea->setInput(true);
	//m_callbackStack.push(func);
	m_input.clear();
	m_inputType = InputType::Restricted;
	m_callbackStack.push(func);
	m_callbackStack.push(std::bind(&U3Misc::ProcessEventCallback, this));
}

bool U3Misc::grocerCallback()
{
	if (m_callbackStack.size() > 0)
	{
		m_callbackStack.pop();
	}

	short gold;
	if (m_input_num == 0)
	{
		InverseChnum((char)m_transactNum, false);
		m_scrollArea->UPrintWin("\n\n");
		return false;
	}
	int existingFood = (m_Player[m_rosNum][32] * 100) + m_Player[m_rosNum][33];
	if (m_input_num > (9999 - existingFood))
	{
		m_scrollArea->UPrintMessageRewrapped(260);
		m_scrollArea->UPrintWin("\n\n");
		m_scrollArea->UPrintMessage(192);
		setInputTypeNum(std::bind(&U3Misc::grocerCallback, this), 4);
		return false;
	}
	gold = (m_Player[m_rosNum][35] * 256) + m_Player[m_rosNum][36];
	if (gold < m_input_num)
	{
		InverseChnum((char)m_transactNum, false);
		m_scrollArea->UPrintMessageRewrapped(193);
		return false;
	}
	gold -= m_input_num;
	m_Player[m_rosNum][35] = (unsigned char)(gold / 256);
	m_Player[m_rosNum][36] = (unsigned char)(gold - (m_Player[m_rosNum][35] * 256));
	existingFood += m_input_num;
	m_Player[m_rosNum][32] = (unsigned char)(existingFood / 100);
	m_Player[m_rosNum][33] = (unsigned char)(existingFood - (m_Player[m_rosNum][32] * 100));
	m_scrollArea->UPrintMessageRewrapped(194);
	setInputTypeYesNo(std::bind(&U3Misc::moreFoodCallback, this));

	return false;
}

bool U3Misc::moreFoodCallback()
{
	if (m_callbackStack.size() > 0)
	{
		m_callbackStack.pop();
	}

	m_scrollArea->setInput(true);
	if (m_input_num == 1) // yes
	{
		m_scrollArea->UPrintWin("\n\n");
		m_scrollArea->UPrintMessage(192);
		setInputTypeNum(std::bind(&U3Misc::grocerCallback, this));
	}
	else
	{
		m_scrollArea->setInput(false);
		m_scrollArea->UPrintWin("\n\n");
		m_scrollArea->UPrintMessageRewrapped(195);
		InverseChnum((char)m_transactNum, false);
	}

	return false;
}

bool U3Misc::tavernCallback()
{
	if (m_callbackStack.size() > 0)
	{
		m_callbackStack.pop();
	}
	short gold;
	std::string message;

	m_scrollArea->UPrintWin("\n\n");
	if (m_input_num < 7)
	{
		InverseChnum((char)m_transactNum, false);
		m_scrollArea->UPrintMessageRewrapped(187);
		return false;
	}
	gold = (m_Player[m_rosNum][35] * 256) + m_Player[m_rosNum][36];
	if (gold < m_input_num)
	{
		InverseChnum((char)m_transactNum, false);
		m_scrollArea->UPrintMessageRewrapped(188);
		return false;
	}
	gold -= m_input_num;
	m_Player[m_rosNum][35] = (unsigned char)(gold / 256);
	m_Player[m_rosNum][36] = (unsigned char)(gold - (m_Player[m_rosNum][35] * 256));

	message = m_resources->m_plistMap["Pub"][(m_input_num / 10)];
	bool classic;
	m_resources->GetPreference(U3PreferencesType::Classic_Appearance, classic);
	if (!classic)
	{
		message = m_scrollArea->RewrapString(message);
	}
	m_scrollArea->UPrintWin(message);
	m_scrollArea->UPrintMessage(189);
	setInputTypeYesNo(std::bind(&U3Misc::anotherDrinkCallback, this));

	return false;
}

bool U3Misc::anotherDrinkCallback()
{
	if (m_callbackStack.size() > 0)
	{
		m_callbackStack.pop();
	}

	m_scrollArea->setInput(true);
	if (m_input_num == 1) // yes
	{
		m_scrollArea->UPrintMessage(186);
		setInputTypeNum(std::bind(&U3Misc::tavernCallback, this));
	}
	else
	{
		m_scrollArea->setInput(false);
		m_scrollArea->UPrintMessageRewrapped(190);
		InverseChnum((char)m_transactNum, false);
	}
	return false;
}

bool U3Misc::weaponsBuySellCallback()
{
	if (m_callbackStack.size() > 0)
	{
		m_callbackStack.pop();
	}

	m_scrollArea->setInput(false);
	if (m_input_num == 1) // buy
	{
		m_scrollArea->UPrintMessage(208);
		setInputTypeRestricted(std::bind(&U3Misc::weaponsBuyCallback, this), 'B');
	}
	else if (m_input_num == 0) // sell
	{
		m_scrollArea->UPrintMessage(211);
		setInputTypeRestricted(std::bind(&U3Misc::weaponsSellCallback, this), 'A');
	}
	else // cancel
	{
		m_callbackStack.push(std::bind(&U3Misc::weaponsBuySellFinishedCallback, this));
		//weaponsBuySellFinishedCallback();
	}
	return false;
}

bool U3Misc::armorsBuySellCallback()
{
	if (m_callbackStack.size() > 0)
	{
		m_callbackStack.pop();
	}

	m_scrollArea->setInput(false);
	if (m_input_num == 1) // buy
	{
		m_scrollArea->UPrintMessage(208);
		setInputTypeRestricted(std::bind(&U3Misc::armorsBuyCallback, this), 'B');
	}
	else if (m_input_num == 0) // sell
	{
		m_scrollArea->UPrintMessage(211);
		setInputTypeRestricted(std::bind(&U3Misc::armorsSellCallback, this), 'B');
	}
	else // cancel
	{
		m_callbackStack.push(std::bind(&U3Misc::armorsBuySellFinishedCallback, this));
		//armorsBuySellFinishedCallback();
	}

	return false;
}

bool U3Misc::weaponsBuyCallback()
{
	if (m_callbackStack.size() > 0)
	{
		m_callbackStack.pop();
	}

	if (m_input_num < 0)
	{
		m_callbackStack.push(std::bind(&U3Misc::weaponsBuySellFinishedCallback, this));
	}
	else
	{
		std::string strInput;
		strInput += (char)(m_input_num + 'A');
		m_scrollArea->setInputString(strInput);
		m_scrollArea->setInput(false);
		short gold = (m_Player[m_rosNum][35] * 256) + m_Player[m_rosNum][36];
		// m_input_num starts at b
		std::string dispString = m_resources->m_plistMap["WeaponsArmour"][m_input_num + 24];
		short amount;
		try
		{
			amount = (short)std::stoi(dispString);
		}
		catch ([[maybe_unused]] const std::exception& e)
		{
			amount = 0;
		}
		if (amount > gold)
		{
			weaponsBuyPoor();
			return false;
		}
		if (m_Player[m_rosNum][48 + m_input_num] > 98)
		{
			m_scrollArea->UPrintMessage(260);
			m_scrollArea->UPrintWin("\n\n");
			m_scrollArea->setInput(false);
			//m_scrollArea->blockPrompt(true);
			setInputTypeRestricted(std::bind(&U3Misc::weaponsBuyCallback, this), 'B');
			m_scrollArea->UPrintMessageRewrapped(208);
			return false;
		}
		gold -= amount;
		m_Player[m_rosNum][35] = (unsigned char)(gold / 256);
		m_Player[m_rosNum][36] = (unsigned char)(gold - (m_Player[m_rosNum][35] * 256));
		m_Player[m_rosNum][48 + m_input_num]++;

		if (m_Player[m_rosNum][48 + m_input_num] > 99)
		{
			m_Player[m_rosNum][48 + m_input_num] = 99;
		}

		m_scrollArea->UPrintMessageRewrapped(210);
		m_scrollArea->setInput(false);
		//m_scrollArea->blockPrompt(true);
		m_input_num = 1;
		setInputTypeRestricted(std::bind(&U3Misc::weaponsBuyCallback, this), 'B');
		m_scrollArea->UPrintMessageRewrapped(208);
	}
	return false;
}

bool U3Misc::weaponsSellCallback()
{
	if (m_callbackStack.size() > 0)
	{
		m_callbackStack.pop();
	}

	if (m_input_num == 0)
	{
		m_resources->CreateAlertMessage(9);
		//m_inputType = InputType::Callback;
		m_inputType = InputType::None;
		m_callbackStack.push(std::bind(&U3Misc::weaponsBuySellFinishedCallback, this));
		m_callbackStack.push(std::bind(&U3Misc::ShowAlert, this));
		return false;
	}
	else if (m_input_num < 0)
	{
		m_callbackStack.push(std::bind(&U3Misc::weaponsBuySellFinishedCallback, this));
		//weaponsBuySellFinishedCallback();
	}
	else
	{
		std::string strInput;
		strInput += (char)(m_input_num + 'A');
		m_scrollArea->setInputString(strInput);
		m_scrollArea->setInput(false);
		if (m_Player[m_rosNum][48 + m_input_num] < 1)
		{
			weaponsSellMissing();
			return false;
		}
		std::string dispString = m_resources->m_plistMap["WeaponsArmour"][m_input_num + 25];
		short amount;
		try
		{
			amount = (short)std::stoi(dispString);
		}
		catch ([[maybe_unused]] const std::exception& e)
		{
			amount = 0;
		}
		if (AddGold(m_rosNum, amount, false))
		{
			m_Player[m_rosNum][48 + m_input_num]--;
			if (m_Player[m_rosNum][48 + m_input_num] < 1 && m_Player[m_rosNum][48] == m_input_num)
			{
				m_Player[m_rosNum][48] = 0;
			}
			m_scrollArea->UPrintMessage(213);
		}
		else
		{
			m_scrollArea->UPrintMessageRewrapped(214);
		}
		m_input_num = 0;
		//setInputTypeRestricted(std::bind(&U3Misc::weaponsBuyCallback, this), 'B');
		m_callbackStack.push(std::bind(&U3Misc::weaponsBuySellCallback, this));
		//weaponsBuySellCallback();
	}
	return false;
}

bool U3Misc::armorsBuyCallback()
{
	if (m_callbackStack.size() > 0)
	{
		m_callbackStack.pop();
	}

	if (m_input_num < 0)
	{
		//armorsBuySellFinishedCallback();
		m_callbackStack.push(std::bind(&U3Misc::armorsBuySellFinishedCallback, this));
	}
	else
	{
		std::string strInput;
		strInput += (char)(m_input_num + 'A');
		m_scrollArea->setInputString(strInput);
		m_scrollArea->setInput(false);
		short gold = (m_Player[m_rosNum][35] * 256) + m_Player[m_rosNum][36];
		// m_input_num starts at b
		std::string dispString = m_resources->m_plistMap["WeaponsArmour"][m_input_num + 40];
		short amount;
		try
		{
			amount = (short)std::stoi(dispString);
		}
		catch ([[maybe_unused]] const std::exception& e)
		{
			amount = 0;
		}
		if (amount > gold)
		{
			armorsBuyPoor();
			return false;
		}
		if (m_Player[m_rosNum][40 + m_input_num] > 98)
		{
			m_scrollArea->UPrintMessage(260);
			m_scrollArea->UPrintWin("\n\n");
			m_scrollArea->setInput(false);
			//m_scrollArea->blockPrompt(true);
			setInputTypeRestricted(std::bind(&U3Misc::armorsBuyCallback, this), 'B');
			m_scrollArea->UPrintMessageRewrapped(208);
			return false;
		}
		gold -= amount;
		m_Player[m_rosNum][35] = (unsigned char)(gold / 256);
		m_Player[m_rosNum][36] = (unsigned char)(gold - (m_Player[m_rosNum][35] * 256));
		m_Player[m_rosNum][40 + m_input_num]++;

		if (m_Player[m_rosNum][40 + m_input_num] > 99)
		{
			m_Player[m_rosNum][40 + m_input_num] = 99;
		}

		m_scrollArea->UPrintMessageRewrapped(210);
		m_scrollArea->setInput(false);
		//m_scrollArea->blockPrompt(true);
		setInputTypeRestricted(std::bind(&U3Misc::armorsBuyCallback, this), 'B');
		m_scrollArea->UPrintMessageRewrapped(208);
	}
	return false;
}

bool U3Misc::armorsSellCallback()
{
	if (m_callbackStack.size() > 0)
	{
		m_callbackStack.pop();
	}
	if (m_input_num < 1)
	{
		//armorsBuySellFinishedCallback();
		m_callbackStack.push(std::bind(&U3Misc::armorsBuySellFinishedCallback, this));
	}
	else
	{
		std::string strInput;
		strInput += (char)(m_input_num + 'A');
		m_scrollArea->setInputString(strInput);
		m_scrollArea->setInput(false);
		if (m_Player[m_rosNum][40 + m_input_num] < 1)
		{
			armorsSellMissing();
			return false;
		}
		std::string dispString = m_resources->m_plistMap["WeaponsArmour"][m_input_num + 40];
		short amount;
		try
		{
			amount = (short)std::stoi(dispString);
		}
		catch ([[maybe_unused]] const std::exception& e)
		{
			amount = 0;
		}
		if (AddGold(m_rosNum, amount, false))
		{
			m_Player[m_rosNum][40 + m_input_num]--;
			if (m_Player[m_rosNum][40 + m_input_num] < 1 && m_Player[m_rosNum][40] == m_input_num)
			{
				m_Player[m_rosNum][40] = 0;
			}
			m_scrollArea->UPrintMessage(213);
		}
		else
		{
			m_scrollArea->UPrintMessageRewrapped(214);
		}
		m_input_num = 0;
		m_callbackStack.push(std::bind(&U3Misc::armorsBuySellCallback, this));
		//m_callbackStack.push(std::bind(&U3Misc::armorsBuySellCallback, this));
		//setInputTypeBuySell(std::bind(&U3Misc::armorsBuySellCallback, this));
		//armorsBuySellCallback();
	}
	return false;
}

void U3Misc::weaponsBuyPoor()
{
	m_scrollArea->UPrintMessageRewrapped(209);
	m_inputType = InputType::Default;
	//m_scrollArea->blockPrompt(false);
	m_scrollArea->setInput(false);
	InverseChnum((char)m_transactNum, false);
}

void U3Misc::armorsBuyPoor()
{
	m_scrollArea->UPrintMessageRewrapped(209);
	m_inputType = InputType::Default;
	//m_scrollArea->blockPrompt(false);
	m_scrollArea->setInput(false);
	InverseChnum((char)m_transactNum, false);
}

void U3Misc::weaponsSellMissing()
{
	m_scrollArea->UPrintMessageRewrapped(212);
	m_inputType = InputType::Default;
	//m_scrollArea->blockPrompt(false);
	m_scrollArea->setInput(false);
	InverseChnum((char)m_transactNum, false);
}

void U3Misc::armorsSellMissing()
{
	m_scrollArea->UPrintMessageRewrapped(212);
	m_inputType = InputType::Default;
	//m_scrollArea->blockPrompt(false);
	m_scrollArea->setInput(false);
	InverseChnum((char)m_transactNum, false);
}


bool U3Misc::weaponsBuySellFinishedCallback()
{
	if (m_callbackStack.size() > 0)
	{
		m_callbackStack.pop();
	}

	m_inputType = InputType::Default;
	//m_scrollArea->blockPrompt(false);
	m_scrollArea->setInput(false);
	InverseChnum((char)m_transactNum, false);
	m_scrollArea->UPrintMessageRewrapped(215);

	return false;
}

bool U3Misc::armorsBuySellFinishedCallback()
{
	if (m_callbackStack.size() > 0)
	{
		m_callbackStack.pop();
	}

	m_inputType = InputType::Default;
	//m_scrollArea->blockPrompt(false);
	m_scrollArea->setInput(false);
	InverseChnum((char)m_transactNum, false);
	m_scrollArea->UPrintMessageRewrapped(215);

	return false;
}

bool U3Misc::healingCallback()
{
	if (m_callbackStack.size() > 0)
	{
		m_callbackStack.pop();
	}

	m_scrollArea->setInput(false);
	switch (m_input_num)
	{
	case 0:
		m_scrollArea->UPrintMessage(198);
		break;
	case 1:
		m_scrollArea->UPrintMessage(200);
		break;
	case 2:
		m_scrollArea->UPrintMessage(202);
		break;
	case 3:
		m_scrollArea->UPrintMessage(204);
		break;
	default:
		InverseChnum((char)m_transactNum, false);
		m_scrollArea->UPrintWin("\n");
		//m_scrollArea->blockPrompt(false);
		return false;
	}
	m_opnum = m_input_num;
	m_scrollArea->setInput(true);
	setInputTypeYesNo(std::bind(&U3Misc::healingPayCallback, this));

	return false;
}

bool U3Misc::healingPayCallback()
{
	if (m_callbackStack.size() > 0)
	{
		m_callbackStack.pop();
	}

	short cost[4] = { 100, 200, 500, 900 };
	short gold;
	m_scrollArea->setInput(false);
	if (m_input_num != 1)
	{
		m_scrollArea->UPrintWin("\n\n");
		m_scrollArea->UPrintMessageRewrapped(232);
		InverseChnum((char)m_transactNum, false);
		m_scrollArea->blockPrompt(false);
		return false;
	}
	gold = (m_Player[m_rosNum][35] * 256) + m_Player[m_rosNum][36];
	if (cost[m_opnum] > gold)
	{
		m_scrollArea->UPrintWin("\n\n");
		m_scrollArea->UPrintMessageRewrapped(233);
		InverseChnum((char)m_transactNum, false);
		//m_scrollArea->blockPrompt(false);
		return false;
	}

	gold -= cost[m_opnum];
	m_Player[m_rosNum][35] = (unsigned char)(gold / 256);
	m_Player[m_rosNum][36] = (unsigned char)(gold - (m_Player[m_rosNum][35] * 256));
	m_scrollArea->UPrintWin("\n");
	switch (m_opnum)
	{
	case 0:
		m_scrollArea->UPrintMessage(199);
		break;
	case 1:
		m_scrollArea->UPrintMessage(201);
		break;
	case 2:
		m_scrollArea->UPrintMessage(203);
		break;
	case 3:
		m_scrollArea->UPrintMessage(205);
		break;
	default:
		InverseChnum((char)m_transactNum, false);
		m_scrollArea->blockPrompt(false);
		return false;
	}
	//m_scrollArea->setInput(true);
	m_inputType = InputType::Transact;
	m_callbackStack.push(std::bind(&U3Misc::healingCallback2, this));
	m_callbackStack.push(std::bind(&U3Misc::ProcessEventCallback, this));
	return false;
}

void U3Misc::SpellNoize(short opnum, [[maybe_unused]]short opnum2)
{
	//m_resources->m_inverses.func = std::bind(&U3Misc::FinalizeHealingCallback, this);
	m_callbackStack.push(std::bind(&U3Misc::FinalizeHealingCallback, this));
	InverseTiles(true);
	InverseCharDetails(opnum, true);

}

bool U3Misc::FinalizeHealingCallback()
{
	if (m_callbackStack.size() > 0)
	{
		m_callbackStack.pop();
	}

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
	//m_scrollArea->blockPrompt(false);
	InverseChnum((char)m_transactNum, false);
	return false;
}

bool U3Misc::healingCallback2()
{
	if (m_callbackStack.size() > 0)
	{
		m_callbackStack.pop();
	}

	if (m_input_num >= 0 && m_input_num < 4)
	{
		m_scrollArea->UPrintWin(std::to_string(m_input_num + 1));
		m_scrollArea->setInput(false);
		m_scrollArea->UPrintWin("\n");
		SpellNoize(m_input_num, m_opnum);
		return false;
	}

	m_scrollArea->setInput(false);
	m_scrollArea->UPrintWin("\n");
	InverseChnum((char)m_transactNum, false);

	return false;
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
	m_Player[m_rosNum][35] = (unsigned char)(gold / 256);
	m_Player[m_rosNum][36] = (unsigned char)(gold - (m_Player[rosNum][35] * 256));
	return retVal;
}

void U3Misc::GetMonsterDir(short monNum) // $7C37
{
	if (m_Party[2] != 0)
	{
		m_zp[0xF5] = m_xpos - m_Monsters[monNum + XMON];
		m_dx = GetHeading(m_zp[0xF5]);
		m_xs = m_graphics->MapConstrain(m_dx + m_Monsters[monNum + XMON]);
		m_zp[0xF6] = m_ypos - m_Monsters[monNum + YMON];
		m_dy = GetHeading(m_zp[0xF6]);
		m_ys = m_graphics->MapConstrain(m_dy + m_Monsters[monNum + YMON]);
	}
	else
	{
		m_zp[0xF5] = m_xpos - m_Monsters[monNum + XMON];
		m_dx = GetHeading(m_zp[0xF5] * 4);
		m_xs = m_graphics->MapConstrain(m_dx + m_Monsters[monNum + XMON]);
		m_zp[0xF6] = m_ypos - m_Monsters[monNum + YMON];
		m_dy = GetHeading(m_zp[0xF6] * 4);
		m_ys = m_graphics->MapConstrain(m_dy + m_Monsters[monNum + YMON]);
	}
	m_zp[0xFB] = m_utilities->Absolute(m_zp[0xF5]);
	m_zp[0xFB] += m_utilities->Absolute(m_zp[0xF6]);
}

static unsigned char ValidMonsterDir(short tile, short montype) // $7C0C
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
/*bool U3Misc::moveshoot(short offset) // $7B36
{
	int randnum = m_utilities->getRandom(0, 255);
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
}*/

/*void U3Misc::move7AAA(short offset)
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
			m_xs = m_graphics->MapConstrain(m_Monsters[offset + XMON] + m_dx);
			m_ys = m_graphics->MapConstrain(m_Monsters[offset + YMON]);    // no +dy!?
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
	m_Monsters[offset + XMON] = (unsigned char)m_xs;
	m_Monsters[offset + YMON] = (unsigned char)m_ys;
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
}*/

/*bool U3Misc::moveoutside(short offset)
{
	GetMonsterDir(offset);
	if (m_xpos == m_xs && m_ypos == m_ys)
	{
		AttackCode(offset);
		return false;
	}
	return true;
}*/

short U3Misc::GetXY(short x, short y) // $7E18
{
	return m_resources->m_TileArray[y * 11 + x];
}

void U3Misc::PutXY(short a, short x, short y)
{
	m_resources->m_TileArray[y * 11 + x] = (unsigned char)a;
}

bool U3Misc::moveshoot2Callback()
{
	if (m_callbackStack.size() > 0)
	{
		m_callbackStack.pop();
	}
	PutXY(m_value, m_xs, m_ys);
	if (m_xs == 5 && m_ys == 5)
	{
		m_callbackStack.push(std::bind(&U3Misc::movemon, this));
		BombTrap();
		return false;
	}
	m_zp[0xFB]--;
	if (m_zp[0xFB] > 0)
	{
		m_callbackStack.push(std::bind(&U3Misc::moveshoot2, this));
		return false;
	}
	m_callbackStack.push(std::bind(&U3Misc::movemon, this));

	return false;
}

bool U3Misc::moveshoot2() // $7B60
{
	if (m_callbackStack.size() > 0)
	{
		m_callbackStack.pop();
	}
	m_xs += m_dx;
	if (m_xs > 10 || m_xs < 0)
	{
		m_callbackStack.push(std::bind(&U3Misc::movemon, this));
		return false;
	}
	m_ys += m_dy;
	if (m_ys > 10 || m_ys < 0)
	{
		m_callbackStack.push(std::bind(&U3Misc::movemon, this));
		return false;
	}
	m_value = GetXY(m_xs, m_ys);
	if (m_value == 0x08 || m_value == 0x46 || m_value == 0x48)
	{
		m_callbackStack.push(std::bind(&U3Misc::movemon, this));
		return false;
	}
	m_gBallTileBackground = (unsigned char)m_value;
	PutXY(0x7A, m_xs, m_ys);
	DelayGame(200, std::bind(&U3Misc::moveshoot2Callback, this));

	return false;
}

bool U3Misc::moveshoot() // $7B36
{
	if (m_callbackStack.size() > 0)
	{
		m_callbackStack.pop();
	}
	int rngNum = m_utilities->getRandom(0, 255);
	if (rngNum > 127)
	{
		m_callbackStack.push(std::bind(&U3Misc::movemon, this));
	}
	else
	{
		GetMonsterDir(m_moveMonsterOffset);
		m_xs = 5 - m_zp[0xF5];
		if (m_xs > 10 || m_xs < 0)
		{
			m_callbackStack.push(std::bind(&U3Misc::movemon, this));
			return false;
		}
		m_ys = 5 - m_zp[0xF6];
		if (m_ys > 10 || m_ys < 0)
		{
			m_callbackStack.push(std::bind(&U3Misc::movemon, this));
			return false;
		}
		m_zp[0xFB] = 3;
		m_callbackStack.push(std::bind(&U3Misc::moveshoot2, this));
	}
	return false;
}

bool U3Misc::move7AAA()
{
	if (m_callbackStack.size() > 0)
	{
		m_callbackStack.pop();
	}

	short value;
	// check if this is a valid place for the monster to walk on.
	value = ValidMonsterDir(GetXYVal(m_xs, m_ys), m_Monsters[m_moveMonsterOffset]);
	if (value == 0 && MonsterHere(m_xs, m_ys) != 255)
		value = 255;
	if (value != 0)
	{
		m_xs = m_Monsters[m_moveMonsterOffset + XMON];
		value = ValidMonsterDir(GetXYVal(m_xs, m_ys), m_Monsters[m_moveMonsterOffset]);
		if (value == 0 && MonsterHere(m_xs, m_ys) != 255)
		{
			value = 255;
		}
		if (value != 0)
		{
			m_xs = m_graphics->MapConstrain(m_Monsters[m_moveMonsterOffset + XMON] + m_dx);
			m_ys = m_graphics->MapConstrain(m_Monsters[m_moveMonsterOffset + YMON]);    // no +dy!?
			value = ValidMonsterDir(GetXYVal(m_xs, m_ys), m_Monsters[m_moveMonsterOffset]);
			if (value == 0 && MonsterHere(m_xs, m_ys) != 255)
			{
				value = 255;
			}
			if (value != 0)
			{
				if (m_Monsters[m_moveMonsterOffset] == 0x3C || m_Monsters[m_moveMonsterOffset] == 0x74)
				{
					m_callbackStack.push(std::bind(&U3Misc::moveshoot, this));
					return false;
				}
				m_callbackStack.push(std::bind(&U3Misc::movemon, this));
				return false;
			}
		}
	}
	if (m_xpos == m_xs && m_ypos == m_ys)
	{
		m_callbackStack.push(std::bind(&U3Misc::movemon, this));
		return false;
	}

	PutXYVal(m_Monsters[m_moveMonsterOffset + TILEON], m_Monsters[m_moveMonsterOffset + XMON], m_Monsters[m_moveMonsterOffset + YMON]);
	m_Monsters[m_moveMonsterOffset + XMON] = (unsigned char)m_xs;
	m_Monsters[m_moveMonsterOffset + YMON] = (unsigned char)m_ys;
	m_Monsters[m_moveMonsterOffset + TILEON] = GetXYVal(m_Monsters[m_moveMonsterOffset + XMON], m_Monsters[m_moveMonsterOffset + YMON]);
	unsigned char monsterTile = m_Monsters[m_moveMonsterOffset];
	if (m_Monsters[m_moveMonsterOffset + VARMON])
	{
		monsterTile += m_Monsters[m_moveMonsterOffset + VARMON];
	}
	PutXYVal(monsterTile, m_Monsters[m_moveMonsterOffset + XMON], m_Monsters[m_moveMonsterOffset + YMON]);

	if (m_Monsters[m_moveMonsterOffset] == 0x3C || m_Monsters[m_moveMonsterOffset] == 0x74)
	{
		m_callbackStack.push(std::bind(&U3Misc::moveshoot, this));
	}
	else
	{
		m_callbackStack.push(std::bind(&U3Misc::movemon, this));
	}

	return false;
}

bool U3Misc::moveoutside()
{
	if (m_callbackStack.size() > 0)
	{
		m_callbackStack.pop();
	}
	GetMonsterDir(m_moveMonsterOffset);
	if (m_xpos == m_xs && m_ypos == m_ys)
	{
		AttackCode(m_moveMonsterOffset);
		return false;
	}
	m_callbackStack.push(std::bind(&U3Misc::move7AAA, this));
	return false;
}

bool U3Misc::movemon() // $7A85
{
	if (m_callbackStack.size() > 0)
	{
		m_callbackStack.pop();
	}
	m_moveMonsterOffset--;
	if (m_moveMonsterOffset < 0)
	{
		return false;
	}
	if (m_Monsters[m_moveMonsterOffset] == 0)
	{
		m_callbackStack.push(std::bind(&U3Misc::movemon, this));
		return false;
	}
	if (m_Party[2] == 0 && m_Party[15] == 0)
	{
		m_callbackStack.push(std::bind(&U3Misc::moveoutside, this));
		return false;
	}
	else
	{
		m_value = m_Monsters[m_moveMonsterOffset + 128];
		m_value = m_value & 0xC0;
		if (m_value == 0)
		{
			m_callbackStack.push(std::bind(&U3Misc::movemon, this));
			return false;
		}
		if (m_value == 0x40)
		{
			int rngNum = m_utilities->getRandom(0, 255);
			if (rngNum < 128)
			{
				m_callbackStack.push(std::bind(&U3Misc::movemon, this));
				return false;
			}
			rngNum = m_utilities->getRandom(0, 255);
			m_xs = m_graphics->MapConstrain(m_Monsters[m_moveMonsterOffset + XMON] + GetHeading((short)rngNum));
			if (m_xs == 0)
			{
				m_callbackStack.push(std::bind(&U3Misc::movemon, this));
				return false;
			}
			rngNum = m_utilities->getRandom(0, 255);
			m_ys = m_graphics->MapConstrain(m_Monsters[m_moveMonsterOffset + YMON] + GetHeading((short)rngNum));
			if (m_ys == 0)
			{
				m_callbackStack.push(std::bind(&U3Misc::movemon, this));
				return false;
			}
			// Sosaria, and user has already defeated Exodus.  Handle monsters running into one another.
			if (m_Party[15] != 0 && m_Party[2] == 0)
			{
				m_value = GetXYVal(m_xs, m_ys);
				if (m_value > 0x27)
				{
					m_value = MonsterHere(m_xs, m_ys);
					if (m_value != 255 && m_Monsters[m_moveMonsterOffset] != m_Monsters[m_value])
					{
						if (m_xs > (m_xpos - 6) && m_ys > (m_ypos - 6) && m_xs < (m_xpos + 6) && m_ys < (m_ypos + 6))
						{
							m_gBallTileBackground = m_Monsters[m_value + TILEON] / 2;
							PutXYVal(0xF4, (unsigned char)m_xs, (unsigned char)m_ys);
							DelayGame(200, std::bind(&U3Misc::movemonCallback1, this));
							return false;
						}

						m_callbackStack.push(std::bind(&U3Misc::movemonCallback, this));
						return false;
					}
				}
			}

			m_callbackStack.push(std::bind(&U3Misc::move7AAA, this));
			return false;
		}
		if (m_value == 0x80)
		{
			GetMonsterDir(m_moveMonsterOffset);
			m_callbackStack.push(std::bind(&U3Misc::move7AAA, this));
			return false;
		}
		if (m_value != 0x40 && m_value != 0x80)
		{
			m_callbackStack.push(std::bind(&U3Misc::moveoutside, this));
			return false;
		}
	}

	return false;
}

bool U3Misc::movemonCallback1()
{
	if (m_callbackStack.size() > 0)
	{
		m_callbackStack.pop();
	}
	PutXYVal(m_Monsters[m_value], (unsigned char)m_xs, (unsigned char)m_ys);
	m_callbackStack.push(std::bind(&U3Misc::movemonCallback, this));
	return false;
}

bool U3Misc::movemonCallback()
{
	if (m_callbackStack.size() > 0)
	{
		m_callbackStack.pop();
	}

	short expnum1 = (m_Experience[m_Monsters[m_moveMonsterOffset] / 4 & 0x0F]);
	short expnum2 = (m_Experience[m_Monsters[m_value] / 4 & 0x0F]);
	if (expnum1 >= expnum2)
	{
		m_Monsters[m_value] = 0;
		PutXYVal(m_Monsters[m_value + TILEON], (unsigned char)m_xs, (unsigned char)m_ys);
	}

	m_callbackStack.push(std::bind(&U3Misc::move7AAA, this));

	return false;
}

bool U3Misc::MoveMonsters() // $7A81
{
	if (m_callbackStack.size() > 0)
	{
		m_callbackStack.pop();
	}

	m_moveMonsterOffset = 32;

	m_callbackStack.push(std::bind(&U3Misc::movemon, this));

	return false;
}

bool U3Misc::IgniteCallback()
{
	if (m_callbackStack.size() > 0)
	{
		m_callbackStack.pop();
	}
	short rosNum;
	m_chNum = m_input_num;
	if (m_chNum < 0 || m_chNum > 3)
	{
		m_scrollArea->UPrintWin("\n");
		return false;
	}
	std::string dispString = std::to_string(m_chNum + 1) + std::string("\n");
	m_scrollArea->UPrintWin(dispString);
	if (m_Party[6 + m_chNum] == 0)
	{
		m_scrollArea->UPrintMessage(41);
		return false;
	}

	rosNum = m_Party[6 + m_chNum];

	if (m_Player[rosNum][15] < 1)
	{
		m_scrollArea->UPrintMessage(67);
		return false;
	}
	m_Player[rosNum][15]--;
	m_gTorch = 255;
	m_dungeon->setForceRedraw();

	return false;
}

void U3Misc::JoinGold()
{
	m_scrollArea->UPrintMessage(66);
	m_inputType = InputType::Transact;
	m_callbackStack.push(std::bind(&U3Misc::JoinGoldCallback, this));
	m_callbackStack.push(std::bind(&U3Misc::ProcessEventCallback, this));
}

bool U3Misc::JoinGoldCallback()
{
	if (m_callbackStack.size() > 0)
	{
		m_callbackStack.pop();
	}
	if (m_input_num < 0 || m_input_num > 3)
	{
		m_scrollArea->UPrintWin("\n");
		m_scrollArea->UPrintMessage(41);
		return false;
	}
	JoinGold(m_input_num);

	return false;
}

void U3Misc::JoinGold(short chnum)
{
	chnum++;
	if (!m_surpressTextDisplay)
	{
		std::string strOutVal = std::to_string(chnum);
		strOutVal += '\n';
		m_scrollArea->UPrintWin(strOutVal);
	}
	short x, mainRosNum, rosNum, total, gold, transfer;

	mainRosNum = m_Party[5 + chnum];
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
				m_Player[rosNum][35] = (unsigned char)(gold / 256);
				m_Player[rosNum][36] = (unsigned char)(gold - (m_Player[rosNum][35] * 256));
			}
		}
	}
	m_Player[mainRosNum][35] = (unsigned char)(total / 256);
	m_Player[mainRosNum][36] = (unsigned char)(total - (m_Player[mainRosNum][35] * 256));
}

bool U3Misc::PeerGemCallback()
{
	if (m_callbackStack.size() > 0)
	{
		m_callbackStack.pop();
	}
	short rosnum;

	if (m_input_num > 3 || m_input_num < 0)
	{
		m_scrollArea->UPrintWin("\n\n");
		return false;
	}

	if (m_Party[6 + m_input_num] == 0)
	{
		m_scrollArea->UPrintWin("\n");
		m_scrollArea->UPrintMessage(41);
		m_scrollArea->UPrintWin("\n");
		return false;
	}

	m_chNum = m_input_num;
	rosnum = m_Party[6 + m_input_num];
	std::string strRosNum = std::to_string(m_chNum + 1) + std::string("\n\n");
	m_scrollArea->UPrintWin(strRosNum);
	/*if (m_Player[rosnum][37] < 1)
	{
		
		m_scrollArea->UPrintMessage(67);
	}
	else*/
	{
		//m_Player[rosnum][37]--;
		m_scrollArea->forceRedraw();
		m_graphics->m_queuedMode = U3GraphicsMode::MiniMap;
		m_graphics->m_menuInit = false;
	}
	return false;
}

void U3Misc::StealFail()
{
	int rngVal = m_utilities->getRandom(0, 255);
	if ((rngVal & 0x03) != 0)
	{
		m_scrollArea->UPrintMessage(86);
		return;
	}
	for (short byte = 63; byte >= 0; byte--)
	{
		if (m_Monsters[byte] == 0x48)
		{
			m_Monsters[byte + HPMON] = 0xC0;
		}
	}
	m_scrollArea->UPrintMessage(87);
}

bool U3Misc::StealCallback()
{
	if (m_callbackStack.size() > 0)
	{
		m_callbackStack.pop();
	}
	if (m_input_num < 0 || m_input_num > 3)
	{
		m_scrollArea->UPrintWin("\n");
		return false;
	}
	std::string strDisp = std::to_string(m_input_num + 1);
	strDisp += '\n';
	m_scrollArea->UPrintWin(strDisp);

	m_inputType = InputType::GetDirection;
	m_callbackStack.push(std::bind(&U3Misc::StealCallback1, this));
	m_callbackStack.push(std::bind(&U3Misc::ProcessEventCallback, this));

	m_rosNum = m_input_num;

	m_scrollArea->UPrintMessage(85);

	return false;
}

bool U3Misc::StealCallback1()
{
	if (m_callbackStack.size() > 0)
	{
		m_callbackStack.pop();
	}
	if (m_input_num < 0)
	{
		m_scrollArea->UPrintWin("\n");
		return false;
	}

	short byte = 0;

	if (m_Party[6 + m_rosNum] == 0)
	{
		m_scrollArea->UPrintMessage(41);
		return false;
	}
	if (CheckAlive(m_rosNum) == false)
	{
		m_spellCombat->Incap();
		return false;
	}

	if (StealDisarmFail(m_rosNum))
	{
		StealFail();
	}
	else
	{
		byte = GetXYVal(m_xs, m_ys);
		if (byte < 0x94 || byte > 0xE4)
		{
			StealFail();
			return false;
		}
		m_xs = m_xs + m_dx + m_dx;
		m_ys = m_ys + m_dy + m_dy;
		byte = GetXYVal(m_xs, m_ys);
		if (byte != 0x24)
		{
			StealFail();
			return false;
		}
		PutXYVal(0x20, (unsigned char)m_xs, (unsigned char)m_ys);
		GetChest(2, m_rosNum);
	}
	return false;
}

bool U3Misc::StealDisarmFail(short rosNum) const // $75CF - result true = failed
{
	short classType, factor;
	bool result;
	factor = m_Player[rosNum][19];
	classType = m_Player[rosNum][23];
	if (classType == m_careerTable[3])
	{
		factor += 0x80;    // thief
	}
	// Barbarian, Illusionist, Ranger
	if (classType == m_careerTable[5] || classType == m_careerTable[7] || classType == m_careerTable[10])
	{
		factor += 0x40;
	}

	// Alchemists now get steal&disarm bonus also. Apple II version does not.
	if (classType == m_careerTable[9])
	{
		factor += 0x40;
	}

	result = false;
	int rngVal = m_utilities->getRandom(0, 255);
	if (rngVal > factor)
	{
		result = true;
	}
	return result;
}

bool U3Misc::UnlockCallback()
{
	if (m_callbackStack.size() > 0)
	{
		m_callbackStack.pop();
	}
	m_resources->SetPreference(U3PreferencesType::Allow_Diagonal, m_storeBool);
	if (m_input_num < 0)
	{
		m_scrollArea->UPrintWin("\n");
		return false;
	}
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
			m_scrollArea->UPrintMessage(96);
			m_inputType = InputType::Transact;
			m_callbackStack.push(std::bind(&U3Misc::UnlockKeyCallback, this));
			m_callbackStack.push(std::bind(&U3Misc::ProcessEventCallback, this));
		}
	}
	return false;
}

bool U3Misc::UnlockKeyCallback()
{
	if (m_callbackStack.size() > 0)
	{
		m_callbackStack.pop();
	}
	short rosNum;
	if (m_input_num < 0)
	{
		m_scrollArea->UPrintWin("\n");
		return false;
	}
	if (m_input_num > 3 || m_Party[6 + m_input_num] == 0)
	{
		std::string dispString = std::to_string(m_input_num + 1) + std::string("\n");
		m_scrollArea->UPrintWin(dispString);
		m_scrollArea->UPrintMessage(41);
		return false;
	}
	else
	{
		rosNum = m_Party[6 + m_input_num];
		if (m_Player[rosNum][38] < 1)
		{
			m_scrollArea->UPrintMessage(67);
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
			PutXYVal(value, (unsigned char)m_xs, (unsigned char)m_ys);
		}
	}
	return false;
}

void U3Misc::CheckAllDead() // $71B4
{
	bool alive = false;
	char byte;

	if (m_checkDead)
	{
		return;
	}

	for (byte = 0; byte < 4; byte++)
	{
		if (CheckAlive(byte) == true)
		{
			alive = true;
			break;
		}
	}

	if (!alive)
	{
		m_resources->m_wasMove = false;
		m_resources->m_elapsedMoveTime = 0;
		m_checkDead = true;
		m_wx = 25;
		m_wy = 23;
		//m_scrollArea->blockPrompt(true);
		m_scrollArea->UPrintMessage(109);    // ALL PLAYERS OUT!
		m_resources->m_overrideImage = -1;

		m_resources->m_inverses.elapsedTileTime = 0;
		m_resources->m_inverses.inverseTileTime = 0;
		m_resources->m_inverses.tiles = false;
		m_resources->m_inverses.additive = false;
		m_resources->m_inverses.fill = false;
		for (int index = 0; index < 4; ++index)
		{
			m_resources->m_inverses.char_details[index] = false;
			m_resources->m_inverses.character_num[index] = false;
		}

		bool autosave;
		m_resources->GetPreference(U3PreferencesType::Auto_Save, autosave);
		if (autosave)
		{
			PutParty();
			PutRoster();
			PutSosaria();
		}

		m_inputType = InputType::AnyKey;
		while (m_callbackStack.size() > 0)
		{
			m_callbackStack.pop();
		}
		m_callbackStack.push(std::bind(&U3Misc::CheckAllDeadPause, this));
		m_callbackStack.push(std::bind(&U3Misc::ProcessEventCallback, this));
	}
}

bool U3Misc::ShowAlert()
{
	if (!m_resources->HandleAlert(m_currentEvent))
	{
		if (m_callbackStack.size() > 0)
		{
			m_callbackStack.pop();
		}

		return false;
	}
	return true;
}

void U3Misc::AddOptions()
{
	m_callbackStack.push(std::bind(&U3Misc::ShowOptions, this));
}

bool U3Misc::ShowOptions()
{
	if (!m_resources->HandleOptions(m_currentEvent))
	{
		if (m_callbackStack.size() > 0)
		{
			m_callbackStack.pop();
		}

		return false;
	}
	return true;
}

bool U3Misc::CheckAllDeadPause()
{
	if (m_callbackStack.size() > 0)
	{
		m_callbackStack.pop();
	}

	m_resources->CreateAlertMessage(416, DialogType::DITL);
	m_callbackStack.push(std::bind(&U3Misc::HandleDeadResponse, this));
	m_callbackStack.push(std::bind(&U3Misc::ShowAlert, this));
	return false;
}

bool U3Misc::HandleDeadResponse()
{
	if (m_callbackStack.size() > 0)
	{
		m_callbackStack.pop();
	}
	int resurrectChoice = m_resources->AlertReturn();
	if (resurrectChoice == 1)
	{
		m_scrollArea->UPrintMessage(249);    // Resurrecting!
		//m_inputType = InputType::Callback;
		m_callbackStack.push(std::bind(&U3Misc::HandleDeadResponse1, this));
	}
	else
	{
		m_graphics->m_staydead = true;
	}
	return false;
}

bool  U3Misc::HandleDeadResponse1()
{
	if (m_callbackStack.size() > 0)
	{
		m_callbackStack.pop();
	}
	m_resources->m_inverses.color.r = 255;
	m_resources->m_inverses.color.g = 0;
	m_resources->m_inverses.color.b = 0;
	m_resources->m_inverses.color.a = 128;
	m_resources->m_inverses.fill = true;
	m_resources->m_inverses.elapsedTileTime = 0;
	m_resources->m_inverses.inverseTileTime = 5000;
	m_resources->setInversed(true);
	m_callbackStack.push(std::bind(&U3Misc::ResurrectCallback, this));
	m_callbackStack.push(std::bind(&U3Misc::InverseCallback, this));

	return false;
}

void U3Misc::ResetSosaria()
{
	std::filesystem::path currentPath = std::filesystem::current_path();
	currentPath /= ResourceLoc;
	currentPath /= SaveLoc;

	std::filesystem::path resourcePath = std::filesystem::current_path();
	resourcePath /= ResourceLoc;
	resourcePath /= BinLoc;

	std::vector<std::string> orig_files = {
		"MainResources.rsrc_MAPS_420.bin",
		"Sosaria_Current.ult",
		"MainResources.rsrc_MISC_400.bin",
		"Moongate_Locations.ult",
		"MainResources.rsrc_MISC_404.bin",
		"Location_Table.ult",
	};

	// Resetting the map, so copy stuff
	for (int index = 0; index < orig_files.size(); index += 2)
	{
		std::filesystem::path sosariaFileOrig = resourcePath / orig_files[index];
		std::filesystem::path sosariaFileCurrent = currentPath / orig_files[index + 1];
		std::filesystem::copy(sosariaFileOrig, sosariaFileCurrent, std::filesystem::copy_options::overwrite_existing);
	}

	std::vector<char> dummy;

	std::filesystem::path monsterFile = currentPath / std::string("Sosaria_Monsters.ult");
	dummy.resize(256);
	std::string strTemp = m_utilities->PathToSDLString(monsterFile);
	if (strTemp.empty())
	{
		return;
	}
	SDL_IOStream* file = SDL_IOFromFile(strTemp.c_str(), "wb");
	SDL_WriteIO(file, dummy.data(), dummy.size());
	SDL_CloseIO(file);
}

bool U3Misc::ResurrectCallback()
{
	if (m_callbackStack.size() > 0)
	{
		m_callbackStack.pop();
	}

	char chNum;
	char byte;
	m_inputType = InputType::Default;
	m_graphics->m_curMode = U3GraphicsMode::Map;
	m_graphics->m_queuedMode = U3GraphicsMode::None;
	m_gameMode = GameStateMode::Map;

	for (chNum = 0; chNum < 4; chNum++)
	{
		m_rosNum = m_Party[chNum + 6];
		for (byte = 35; byte < 64; byte++)
		{
			m_Player[m_rosNum][byte] = 0;
		}
		m_Player[m_rosNum][15] = 0;    // no torches too
		if (m_Player[m_rosNum][32] < 1)
		{
			m_Player[m_rosNum][32] = 1;    // Some food
		}
		m_Player[m_rosNum][36] = 150;      // Gold Pieces
		m_Player[m_rosNum][41] = 1;        // Cloth
		m_Player[m_rosNum][40] = 1;        //  in use
		m_Player[m_rosNum][49] = 1;        // Dagger
		m_Player[m_rosNum][48] = 1;        //  in use
		m_Player[m_rosNum][17] = 'G';      // Good Health
		m_Player[m_rosNum][27] = 100;      // Current Hit Points
		m_Player[m_rosNum][26] = 0;        // Current Hit Points
	}
	m_Party[2] = 0;
	m_Party[0] = 0x7E;
	m_xpos = 42;
	m_ypos = 20;
	m_Party[3] = (unsigned char)m_xpos;
	m_Party[4] = (unsigned char)m_ypos;
	m_zp[0xE3] = (unsigned char)m_xpos;
	m_zp[0xE4] = (unsigned char)m_ypos;

	PutParty();
	PutRoster();
	ResetSosaria();
	GetMiscStuff(0);
	PutMiscStuff();
	GetSosaria();

	m_ox = m_xpos - 1;
	m_oy = m_ypos - 1;
	m_gTimeNegate = 0;

	m_audio->m_currentSong = 1;
	m_audio->m_nextSong = 1;
	m_audio->musicUpdate();

	m_scrollArea->UPrintWin("\n\n\n\n\n\n\n\n");
	//m_scrollArea->blockPrompt(false);
	m_checkDead = false;
	m_resources->m_elapsedMoveTime = 0;
	//m_graphics->m_curMode = U3GraphicsMode::Map;
	//m_gameMode = GameStateMode::Map;

	return false;
}

void U3Misc::SafeExodus(void)
{
	unsigned char x, y;

	for (x = 0; x < 0x20; x++)
	{
		if (m_Monsters[x + HPMON] > 0)
		{
			PutXYVal(m_Monsters[x + TILEON], m_Monsters[x + XMON], m_Monsters[x + YMON]);
			m_Monsters[x + HPMON] = 0;
			m_Monsters[x] = 0;
		}
	}
	for (y = 1; y < 12; y++)
	{
		for (x = 0x1E; x < 0x22; x++)
		{
			PutXYVal(0x20, x, y);
		}
	}
}

void U3Misc::OtherCommand(short yell)
{
	if (yell == 0)
	{
		m_YellStat = true;
		m_scrollArea->UPrintMessage(236);
	}
	m_inputType = InputType::Transact;
	m_callbackStack.push(std::bind(&U3Misc::OtherCallback, this));
	m_callbackStack.push(std::bind(&U3Misc::ProcessEventCallback, this));
}

bool U3Misc::OtherCallback()
{
	if (m_callbackStack.size() > 0)
	{
		m_callbackStack.pop();
	}
	m_chNum = m_input_num;
	std::string strchNum;
	if (m_chNum >= 0)
	{
		strchNum = std::to_string(m_chNum + 1) + std::string("\n");
	}
	else
	{
		strchNum = std::string("\n");
	}
	m_scrollArea->UPrintWin(strchNum);

	if (m_chNum < 0 || m_chNum > 3)
	{
		return false;
	}
	if (m_Party[6 + m_chNum] == 0)
	{
		m_scrollArea->UPrintMessage(41);
		return false;
	}
	m_rosNum = m_Party[6 + m_chNum];
	if (CheckAlive(m_chNum) == false)
	{
		m_scrollArea->UPrintMessage(126);
		return false;
	}
	m_scrollArea->UPrintMessage(238);
	m_scrollArea->setInput(true);
	m_inputType = InputType::InputText;
	m_maxInputLength = 8;
	m_numOnly = false;
	m_input.clear();
	m_callbackStack.push(std::bind(&U3Misc::OtherCallback1, this));
	m_callbackStack.push(std::bind(&U3Misc::ProcessEventCallback, this));

	return false;
}

bool U3Misc::OtherCallback1()
{
	if (m_callbackStack.size() > 0)
	{
		m_callbackStack.pop();
	}

	short chnum;

	m_inputType = InputType::Default;
	m_scrollArea->setInput(false);
	m_scrollArea->UPrintWin("\n");
	if (0 == m_input.compare("PAXUM"))
	{
		m_scrollArea->UPrintMessage(254);
		for (chnum = 0; chnum < 32; chnum++)
		{
			if (m_Monsters[chnum] && m_Party[2] != 0)
			{
				if (m_Monsters[chnum + XMON] > m_xpos - 6 && m_Monsters[chnum + XMON] < m_xpos + 6 && m_Monsters[chnum + YMON] > m_ypos - 6 &&
					m_Monsters[chnum + YMON] < m_ypos + 6 && m_Monsters[chnum + HPMON] == 0xC0)
				{
					m_Monsters[chnum + HPMON] = 0x40;
				}
			}
		}
		return false;
	}
	else if (0 == m_input.compare("SEARCH"))
	{
		unsigned char bits[8] = { 1, 2, 4, 8, 16, 32, 64, 128 };

		if (GetXYVal(m_xpos, m_ypos) != 0xF8)
		{
			NotHere();
			return false;
		}
		m_Player[m_rosNum][14] = m_Player[m_rosNum][14] | bits[m_xpos & 0x03];
		m_scrollArea->UPrintMessageRewrapped(244);
		return false;
	}
	else if (0 == m_input.compare("SCREAM"))
	{
		std::string dispString(ScreamString);
		m_scrollArea->UPrintWin(dispString);
		return false;
	}
	else if (0 == m_input.compare("INSERT"))
	{
		m_scrollArea->UPrintMessage(239);
		m_inputType = InputType::GetDirection;
		m_callbackStack.push(std::bind(&U3Misc::InsertCallback, this));
		m_callbackStack.push(std::bind(&U3Misc::ProcessEventCallback, this));
		return false;
	}
	else if (0 == m_input.compare("DIG"))
	{
		if (m_Party[2] != 0)
		{
			NotHere();
			return false;
		}
		if ((m_xpos == 0x21 && m_ypos == 0x03) || (m_xpos == 0x13 && m_ypos == 0x2C))
		{
			if (m_xpos == 0x21)
			{
				m_Player[m_rosNum][63] = 1;
			}
			if (m_xpos == 0x13)
			{
				m_Player[m_rosNum][47] = 1;
			}
			m_scrollArea->UPrintMessage(243);
			return false;
		}
		NotHere();
		return false;
	}
	else if (0 == m_input.compare("BRIBE")) // Bribe
	{
		m_scrollArea->UPrintMessage(239);
		m_inputType = InputType::GetDirection;
		m_callbackStack.push(std::bind(&U3Misc::BribeCallback, this));
		m_callbackStack.push(std::bind(&U3Misc::ProcessEventCallback, this));
		return false;
	}
	else if (0 == m_input.compare("PRAY"))
	{
		if (m_Party[2] != 2)
		{
			NoEffect();
			return false;
		}
		if (m_Party[3] != m_LocationX[4])
		{
			NoEffect();
			return false;
		}
		if (m_xpos != 0x30 || m_ypos != 0x30)
		{
			NoEffect();
			return false;
		}
		m_scrollArea->UPrintMessage(246);
		return false;
	}
	else if (0 == m_input.compare("EVOCARE"))
	{
		Yell(1 + m_rosNum);
		return false;
	}
	else if (0 == m_input.compare("PISSOFF"))
	{
		m_scrollArea->UPrintMessage(247);
		for (chnum = 0; chnum < 32; chnum++)
		{
			m_Monsters[chnum + HPMON] = 0xC0;
		}
		return false;
	}
	NoEffect();
	return false;
}

bool U3Misc::InsertCallback()
{
	if (m_callbackStack.size() > 0)
	{
		m_callbackStack.pop();
	}
	if (m_input_num < 0)
	{
		m_scrollArea->UPrintWin("\n");
		return false;
	}
	if (GetXYVal(m_xs, m_ys) != 0x7C)
	{
		NotHere();
		return false;
	}
	m_scrollArea->UPrintMessage(240);
	m_inputType = InputType::LetterImmediate;
	m_callbackStack.push(std::bind(&U3Misc::InsertCallback1, this));
	m_callbackStack.push(std::bind(&U3Misc::ProcessEventCallback, this));

	return false;
}

bool U3Misc::InsertCallback1()
{
	if (m_callbackStack.size() > 0)
	{
		m_callbackStack.pop();
	}
	unsigned char bits[8] = { 1, 2, 4, 8, 16, 32, 64, 128 };
	short bytes;
	short object;
	m_scrollArea->setInput(false);
	m_scrollArea->UPrintWin(m_input);
	m_scrollArea->UPrintWin("\n");

	if (m_input.c_str()[0] == 'L')
	{
		object = 0x1E;
	}
	else if (m_input.c_str()[0] == 'S')
	{
		object = 0x1F;
	}
	else if (m_input.c_str()[0] == 'M')
	{
		object = 0x20;
	}
	else if (m_input.c_str()[0] == 'D')
	{
		object = 0x21;
	}
	else
	{
		What2();
		return false;
	}

	bytes = bits[object - 0x1E];
	if ((m_Player[m_rosNum][14] & bytes) == 0)
	{
		m_scrollArea->UPrintMessage(67);
		return false;
	}

	if (m_xs != object || object != m_lastCard)
	{
		InverseCharDetails(m_chNum, true);
		m_resources->m_inverses.elapsedTileTime = 0;
		m_resources->m_inverses.inverseTileTime = 250;
		m_resources->setInversed(true);
		m_callbackStack.push(std::bind(&U3Misc::InsertCallback2, this));
		m_callbackStack.push(std::bind(&U3Misc::InverseCallback, this));
		return false;
	}
	m_lastCard++;
	m_gBallTileBackground = 0x3E; // Exodus

	//m_scrollArea->blockPrompt(true);
	m_callbackStack.push(std::bind(&U3Misc::ExodusDieCallback1, this));
	m_callbackStack.push(std::bind(&U3Misc::ExodusDieCallback, this));
	m_callbackStack.push(std::bind(&U3Misc::ExodusDieCallback, this));
	m_callbackStack.push(std::bind(&U3Misc::ExodusDieCallback, this));
	m_callbackStack.push(std::bind(&U3Misc::ExodusDieCallback, this));

	return false;
}

bool U3Misc::ExodusDieCallback()
{
	if (m_callbackStack.size() > 0)
	{
		m_callbackStack.pop();
	}
	m_elapsedSleepTime = 0;
	m_sleepCheckTime = exodus_death_time;
	PutXYVal(0xF0, (unsigned char)m_xs, (unsigned char)m_ys);
	m_inputType = InputType::SleepCallback;
	m_callbackStack.push(std::bind(&U3Misc::ExodusDieCallback2, this));
	m_callbackStack.push(std::bind(&U3Misc::SleepCallback, this));

	return false;
}

bool U3Misc::ExodusDieCallback2()
{
	if (m_callbackStack.size() > 0)
	{
		m_callbackStack.pop();
	}
	m_elapsedSleepTime = 0;
	m_sleepCheckTime = exodus_death_time;
	PutXYVal(0x7C, (unsigned char)m_xs, (unsigned char)m_ys);
	m_inputType = InputType::SleepCallback;
	m_callbackStack.push(std::bind(&U3Misc::SleepCallback, this));

	return false;
}


bool U3Misc::ExodusDieCallback1()
{
	if (m_callbackStack.size() > 0)
	{
		m_callbackStack.pop();
	}

	PutXYVal(0x20, (unsigned char)m_xs, (unsigned char)m_ys);

	m_inputType = InputType::Default;

	if (m_lastCard != 0x22)
	{
		//m_scrollArea->blockPrompt(false);
		return false;
	}

	//exodus_death

	m_Party[15] = 1;
	bool classic;
	std::string dispString;
	m_audio->m_currentSong = 9;
	m_audio->m_nextSong = 9;
	m_audio->musicUpdate();
	m_resources->GetPreference(U3PreferencesType::Classic_Appearance, classic);
	if (classic)
	{
		dispString = m_resources->m_plistMap["Messages"][240];
	}
	else
	{
		dispString = m_resources->m_plistMap["Messages"][254];
		dispString = m_scrollArea->RewrapString(dispString);
	}
	m_scrollArea->UPrintWin(dispString);
	int time = m_Party[10] + m_Party[11] * 100 + m_Party[12] * 10000 + m_Party[13] * 1000000;
	dispString = std::to_string(time);
	if (!classic)
	{
		m_scrollArea->UPrintWin("\n");
	}
	m_scrollArea->UPrintWin(dispString);
	m_scrollArea->UPrintMessage(242);

	m_inputType = InputType::None;
	m_input_num = 0;

	m_resources->m_inverses.color.r = (Uint8)m_utilities->getRandom(0, 255);
	m_resources->m_inverses.color.g = (Uint8)m_utilities->getRandom(0, 255);
	m_resources->m_inverses.color.b = (Uint8)m_utilities->getRandom(0, 255);
	m_resources->m_inverses.color.a = 255;
	m_resources->m_inverses.fill = false;
	m_resources->m_inverses.additive = true;
	m_resources->m_inverses.elapsedTileTime = 0;
	m_resources->m_inverses.inverseTileTime = screen_flicker_time;
	m_resources->setInversed(true);
	m_callbackStack.push(std::bind(&U3Misc::ExodusDieCallback3, this));
	m_callbackStack.push(std::bind(&U3Misc::InverseCallback, this));

	return false;
}

bool U3Misc::ExodusDieCallback3()
{
	if (m_callbackStack.size() > 0)
	{
		m_callbackStack.pop();
	}
	m_input_num++;
	m_resources->m_inverses.color.r = (Uint8)m_utilities->getRandom(0, 255);
	m_resources->m_inverses.color.g = (Uint8)m_utilities->getRandom(0, 255);
	m_resources->m_inverses.color.b = (Uint8)m_utilities->getRandom(0, 255);
	m_resources->m_inverses.elapsedTileTime = 0;
	m_resources->m_inverses.inverseTileTime = screen_flicker_time;
	m_resources->m_inverses.additive = true;
	m_resources->setInversed(true);
	if (m_input_num > 6)
	{
		m_callbackStack.push(std::bind(&U3Misc::ExodusDieCallback4, this));
		m_callbackStack.push(std::bind(&U3Misc::InverseCallback, this));
	}
	else
	{
		m_callbackStack.push(std::bind(&U3Misc::ExodusDieCallback3, this));
		m_callbackStack.push(std::bind(&U3Misc::InverseCallback, this));
	}

	return false;
}

bool U3Misc::ExodusDieCallback4()
{
	if (m_callbackStack.size() > 0)
	{
		m_callbackStack.pop();
	}
	m_resources->m_inverses.additive = false;
	m_graphics->setForceRedraw();
	m_graphics->m_queuedMode = U3GraphicsMode::WinScreen;
	m_graphics->setFade(false);

	return false;
}

void U3Misc::BombTrap() // $5C63
{
	m_inputType = InputType::None;
	m_input_num = 0;

	if (0 < m_Party[1])
	{
		if (CheckAlive(0))
		{
			InverseCharDetails(0, true);
			m_resources->m_inverses.elapsedTileTime = 0;
			m_resources->m_inverses.inverseTileTime = damage_time;
			m_resources->setInversed(true);
			m_callbackStack.push(std::bind(&U3Misc::BombTrapCallback, this));
			m_callbackStack.push(std::bind(&U3Misc::InverseCallback, this));
		}
		else
		{
			//BombTrapCallback();
			m_callbackStack.push(std::bind(&U3Misc::BombTrapCallback, this));
		}
	}
}

bool U3Misc::BombTrapCallback()
{
	if (m_callbackStack.size() > 0)
	{
		m_callbackStack.pop();
	}
	if (CheckAlive(m_input_num))
	{
		int val = m_utilities->getRandom(0, 255);
		HPSubtract(m_Party[6 + m_input_num], val & 0x77);
		HPSubtract(m_Party[6 + m_input_num], (m_zp[0x13] + 1) * 8);
	}

	m_input_num++;
	if (m_input_num < m_Party[1])
	{
		if (CheckAlive(m_input_num))
		{
			InverseCharDetails(m_input_num, true);
			m_resources->m_inverses.elapsedTileTime = 0;
			m_resources->m_inverses.inverseTileTime = damage_time;
			m_resources->setInversed(true);
			m_callbackStack.push(std::bind(&U3Misc::BombTrapCallback, this));
			m_callbackStack.push(std::bind(&U3Misc::InverseCallback, this));
		}
		else
		{
			m_callbackStack.push(std::bind(&U3Misc::BombTrapCallback, this));
		}
	}
	else
	{
		m_inputType = InputType::Default;
	}
	return false;
}


bool U3Misc::SleepCallback()
{
	m_elapsedSleepTime += m_resources->m_delta_time;
	if (m_elapsedSleepTime < m_sleepCheckTime)
	{
		m_inputType = InputType::None;
		return true;
	}
	if (m_callbackStack.size() > 0)
	{
		m_callbackStack.pop();
	}
	m_elapsedSleepTime = 0;

	return false;
}

bool U3Misc::HPSubtract(short rosNum, short amount) // $7181
{
	int originalHP;
	int hp;
	hp = (m_Player[rosNum][26] * 256 + m_Player[rosNum][27]);
	originalHP = hp;
	hp -= amount;
	if (hp < 1)
	{
		m_Player[rosNum][26] = 0;
		m_Player[rosNum][27] = 0;
		m_Player[rosNum][17] = 'D';
		bool autosave;
		m_resources->GetPreference(U3PreferencesType::Auto_Save, autosave);
		if (autosave)
		{
			m_Party[3] = (unsigned char)m_xpos;
			m_Party[4] = (unsigned char)m_ypos;
			PutParty();
			PutRoster();
		}
		if (originalHP > 0)
		{
			if (m_Player[rosNum][24] == 'F')
			{
				//PlaySoundFile(CFSTR("DeathFemale"), TRUE);    // was 0xE6, TRUE
			}
			else
			{
				//PlaySoundFile(CFSTR("DeathMale"), TRUE);    // was 0xE5, TRUE
			}
		}
		return true;
	}
	m_Player[rosNum][26] = (unsigned char)(hp / 256);
	m_Player[rosNum][27] = (unsigned char)(hp - (m_Player[rosNum][26] * 256));
	return false;
}

bool U3Misc::InsertCallback2()
{
	if (m_callbackStack.size() > 0)
	{
		m_callbackStack.pop();
	}
	m_Player[m_rosNum][26] = 0;
	m_Player[m_rosNum][27] = 1;
	HPSubtract(m_rosNum, 255);
	return false;
}

bool U3Misc::BribeCallback()
{
	if (m_callbackStack.size() > 0)
	{
		m_callbackStack.pop();
	}
	if (m_input_num < 0)
	{
		m_scrollArea->UPrintWin("\n");
		return false;
	}
	short object;
	short gold;

	object = MonsterHere(m_xs, m_ys);
	if (object > 127)
	{
		NotHere();
		return false;
	}
	gold = (m_Player[m_rosNum][35] * 256) + m_Player[m_rosNum][36];
	if (gold < 100)
	{
		m_scrollArea->UPrintMessage(245);
		return false;
	}
	gold -= 100;
	m_Player[m_rosNum][35] = (unsigned char)(gold / 256);
	m_Player[m_rosNum][36] = (unsigned char)(gold - (m_Player[m_rosNum][35] * 256));
	if (m_Monsters[object] != 0x48) // Guard
	{
		NoEffect();
		return false;
	}
	PutXYVal(m_Monsters[object + TILEON], m_Monsters[object + XMON], m_Monsters[object + YMON]);
	m_Monsters[object] = 0;

	return false;
}

void U3Misc::Yell(short mode)
{
	short chnum;
	short oy;
	short rosNum;

	if (mode == 0)
	{
		m_YellStat = false;
		m_scrollArea->UPrintMessage(104);
		OtherCommand(1);
		return;
	}
	if (m_YellStat)
	{
		NoEffect();
		return;
	}

	chnum = mode - 2;
	rosNum = m_Party[6 + chnum];
	if ((m_Player[rosNum][14] & 0x40) == 0)
	{
		NoEffect();
		return;
	}
	oy = m_ypos;
	if (GetXYVal(m_xpos, m_ypos - 1) == 236)
	{
		oy -= 3;
	}
	if (GetXYVal(m_xpos, m_ypos + 1) == 232)
	{
		oy += 3;
	}
	if (m_ypos == oy)
	{
		NoEffect();
		return;
	}
	m_ypos = oy;
	m_resources->m_inverses.color.r = 0;
	m_resources->m_inverses.color.g = 0;
	m_resources->m_inverses.color.b = 0;
	m_resources->m_inverses.color.a = 255;
	m_resources->m_inverses.fill = true;
	m_resources->m_inverses.elapsedTileTime = 0;
	m_resources->m_inverses.inverseTileTime = 1000;
	m_resources->setInversed(true);
	m_callbackStack.push(std::bind(&U3Misc::InverseCallback, this));
}

void U3Misc::ClearTiles()
{
	//m_scrollArea->blockPrompt(true);
	m_resources->m_inverses.color.r = 0;
	m_resources->m_inverses.color.g = 0;
	m_resources->m_inverses.color.b = 0;
	m_resources->m_inverses.color.a = 0;
	m_resources->m_inverses.fill = true;
	m_resources->m_inverses.elapsedTileTime = 0;
	m_resources->m_inverses.inverseTileTime = 1000;
	//m_resources->m_inverses.func = std::bind(&U3Misc::NoOpCallback, this);
	m_resources->setInversed(true);
	m_callbackStack.push(std::bind(&U3Misc::InverseCallback, this));
}

void U3Misc::GetChest(short spell, short chnum)
{
	if (spell == 2)
	{
		m_rosNum = m_Party[6 + chnum];
		GetChestBooty();
		return;
	}
	else if (spell == 0)
	{
		m_m5BDC = true;
		m_scrollArea->UPrintMessage(40);
		//m_scrollArea->blockPrompt(true);
		m_inputType = InputType::Transact;
		m_callbackStack.push(std::bind(&U3Misc::GetChestCallback, this));
		m_callbackStack.push(std::bind(&U3Misc::ProcessEventCallback, this));
		return;
	}
	GetChest1(chnum);
}

bool U3Misc::GetChestCallback()
{
	if (m_callbackStack.size() > 0)
	{
		m_callbackStack.pop();
	}
	if (m_input_num < 0 || m_input_num > 3)
	{
		m_scrollArea->UPrintWin("\n");
		//m_scrollArea->blockPrompt(false);
		return false;
	}
	std::string dispString = std::to_string(m_input_num + 1) + std::string("\n");
	m_scrollArea->UPrintWin(dispString);
	if (m_Party[6 + m_input_num] == 0)
	{
		m_scrollArea->UPrintWin("\n");
		m_scrollArea->UPrintMessage(41);
		//m_scrollArea->blockPrompt(false);
		return false;
	}
	if (CheckAlive(m_input_num) == false)
	{
		m_scrollArea->UPrintMessage(126);
		//m_scrollArea->blockPrompt(false);
		return false;
	}

	GetChest1(m_input_num);

	return false;
}

void U3Misc::GetChest1(short chnum)
{
	char tile;
	m_rosNum = m_Party[6 + chnum];
	m_xs = m_xpos;
	m_ys = m_ypos;
	if (m_Party[2] != 1) // party not in dungeon
	{
		tile = GetXYVal(m_xpos, m_ypos);
		if ((tile < 0x24) || (tile > 0x27)) // izzit not a chest?
		{
			NotHere();
			//m_scrollArea->blockPrompt(false);
			return;
		}
		tile = (tile & 0x3) * 4;
		if (tile == 0)
		{
			tile = 0x20;
		}
		PutXYVal(tile, (unsigned char)m_xpos, (unsigned char)m_ypos);
	}
	else // party in dungeon
	{
		if (m_spellCombat->GetXYDng(m_xpos, m_ypos) != 0x40)
		{
			NotHere();
			//m_scrollArea->blockPrompt(false);
			return;
		}
		m_spellCombat->PutXYDng(0, (unsigned char)m_xpos, (unsigned char)m_ypos);
	}
	int rngnum = m_utilities->getRandom(0, 255);

	if ((m_m5BDC == false) || rngnum > 127)
	{
		GetChestBooty();
		return;
	}
	int rngnum1 = m_utilities->getRandom(0, 255);
	int rngnum2 = m_utilities->getRandom(0, 255);
	rngnum = (rngnum1 & rngnum2) & 0x03;
	switch (rngnum)
	{
	case 0:
		m_scrollArea->UPrintMessage(42);
		if (StealDisarmFail(m_rosNum) == false)
		{
			m_scrollArea->UPrintMessage(46);
			GetChestBooty();
			return;
		}
		else
		{
			m_inputType = InputType::None;
			InverseCharDetails(chnum, true);
			//m_resources->m_inverses.func = std::bind(&U3Misc::ChestAcidCallback, this);
			m_resources->m_inverses.elapsedTileTime = 0;
			m_resources->m_inverses.inverseTileTime = damage_time;
			m_resources->setInversed(true);
			m_callbackStack.push(std::bind(&U3Misc::ChestAcidCallback, this));
			m_callbackStack.push(std::bind(&U3Misc::InverseCallback, this));
			return;
		}
		break;
	case 1:
		m_scrollArea->UPrintMessage(43);
		if (StealDisarmFail(m_rosNum) == false)
		{
			m_scrollArea->UPrintMessage(46);
			GetChestBooty();
			return;
		}
		else
		{
			m_inputType = InputType::None;
			InverseCharDetails(chnum, true);
			//m_resources->m_inverses.func = std::bind(&U3Misc::ChestPoisonCallback, this);
			m_resources->m_inverses.elapsedTileTime = 0;
			m_resources->m_inverses.inverseTileTime = damage_time;
			m_resources->setInversed(true);
			m_callbackStack.push(std::bind(&U3Misc::ChestPoisonCallback, this));
			m_callbackStack.push(std::bind(&U3Misc::InverseCallback, this));
			return;
		}
		break;
	case 2:
		m_scrollArea->UPrintMessage(44);
		if (StealDisarmFail(m_rosNum) == false)
		{
			m_scrollArea->UPrintMessage(46);
			GetChestBooty();
			return;
		}
		else
		{
			BombTrap();
			m_dungeon->setForceRedraw();
			//m_scrollArea->blockPrompt(false);
			return;
		}
		break;
	case 3:
		m_scrollArea->UPrintMessage(45);
		if (StealDisarmFail(m_rosNum) == false)
		{
			m_scrollArea->UPrintMessage(46);
			GetChestBooty();
			return;
		}
		else
		{
			m_inputType = InputType::None;
			m_input_num = 0;

			if (CheckAlive(0))
			{
				InverseCharDetails(0, true);
				//m_resources->m_inverses.func = std::bind(&U3Misc::ChestGasCallback, this);
				m_resources->m_inverses.elapsedTileTime = 0;
				m_resources->m_inverses.inverseTileTime = damage_time;
				m_resources->setInversed(true);
				m_callbackStack.push(std::bind(&U3Misc::ChestGasCallback, this));
				m_callbackStack.push(std::bind(&U3Misc::InverseCallback, this));
				return;
			}
			else
			{
				m_callbackStack.push(std::bind(&U3Misc::ChestGasCallback, this));
				return;
			}
		}
		break;
	default:
		break;
	}
}

bool U3Misc::ChestGasCallback()
{
	if (m_callbackStack.size() > 0)
	{
		m_callbackStack.pop();
	}
	m_Player[m_Party[6 + m_input_num]][17] = 'P';
	m_input_num++;

	if (m_input_num < 4)
	{
		if (CheckAlive(m_input_num))
		{
			InverseCharDetails(m_input_num, true);
			//m_resources->m_inverses.func = std::bind(&U3Misc::ChestGasCallback, this);
			m_resources->m_inverses.elapsedTileTime = 0;
			m_resources->m_inverses.inverseTileTime = damage_time;
			m_resources->setInversed(true);
			m_callbackStack.push(std::bind(&U3Misc::ChestGasCallback, this));
			m_callbackStack.push(std::bind(&U3Misc::InverseCallback, this));
		}
		else
		{
			m_callbackStack.push(std::bind(&U3Misc::ChestGasCallback, this));
		}
	}
	else
	{
		GetChestBooty();
	}
	return false;
}

bool U3Misc::ChestAcidCallback()
{
	if (m_callbackStack.size() > 0)
	{
		m_callbackStack.pop();
	}
	int rngNum = m_utilities->getRandom(0, 255);
	HPSubtract(m_rosNum, rngNum & 0x37);
	GetChestBooty();

	return false;
}

bool U3Misc::ChestPoisonCallback()
{
	if (m_callbackStack.size() > 0)
	{
		m_callbackStack.pop();
	}
	m_Player[m_rosNum][17] = 'P';
	GetChestBooty();

	return false;
}

void U3Misc::GetChestBooty()
{
	m_dungeon->setForceRedraw();
	short gold;
	short wpn;
	short arm;

	m_inputType = InputType::Default;
	std::string dispString = m_resources->m_plistMap["Messages"][46];
	gold = (short)m_utilities->getRandom(0, 100);
	if (gold < 30)
	{
		gold += 30;
	}
	dispString += std::to_string(gold);
	dispString += '\n';
	m_scrollArea->UPrintWin(dispString);
	if (AddGold(m_rosNum, gold, true) == false)
	{
		m_scrollArea->UPrintMessage(48);
	}
	short rngNum = (short)m_utilities->getRandom(0, 255);
	if (rngNum > 63)
	{
		//m_scrollArea->blockPrompt(false);
		return;
	}
	wpn = (short)m_utilities->getRandom(0, 255);
	if (wpn < 128)
	{
		rngNum = (short)m_utilities->getRandom(0, 255);
		wpn = (rngNum & wpn) & 0x07;
		if (wpn != 0)
		{
			std::string str1 = m_resources->m_plistMap["Messages"][48];
			std::string str2 = m_resources->m_plistMap["WeaponsArmour"][wpn];

			str1 += str2;
			str1 += '\n';

			m_scrollArea->UPrintWin(str1);
			AddItem(m_rosNum, 48 + wpn, 1);
			//m_scrollArea->blockPrompt(false);
			return;
		}
	}
	arm = (short)m_utilities->getRandom(0, 255);
	if (arm < 128)
	{
		rngNum = (short)m_utilities->getRandom(0, 255);
		arm = (rngNum & arm) & 0x03;
		if (arm != 0)
		{
			std::string str1 = m_resources->m_plistMap["Messages"][49];
			std::string str2 = m_resources->m_plistMap["WeaponsArmour"][arm + 16];

			str1 += str2;
			str1 += '\n';

			m_scrollArea->UPrintWin(str1);
			AddItem(m_rosNum, 40 + arm, 1);
			//m_scrollArea->blockPrompt(false);
			return;
		}
	}
	//m_scrollArea->blockPrompt(false);
}

void U3Misc::AddItem(short rosNum, short item, short amount) // $7145
{
	m_Player[rosNum][item] += (unsigned char)amount;
	if (m_Player[rosNum][item] > 99)
	{
		m_Player[rosNum][item] = 99;
	}
}

// Right now, there can only be one delay game per queue, otherwise these
// values will overwrite the previous one.  This can be changed by adding
// another queue, but as a temporary fix, make sure you only call
// this at the end of a function (or series of functions), so that no
// other items can on the queue.
void U3Misc::DelayGame(Uint64 delay_time)
{
	m_elapsedSleepTime = 0;
	m_sleepCheckTime = delay_time;
	m_callbackStack.push(std::bind(&U3Misc::SleepCallback, this));
	//m_callbackStack.push(std::bind(&U3Misc::ProcessEventCallback, this));
}

void U3Misc::DelayGame(Uint64 delay_time, std::function<bool()> callback)
{
	m_elapsedSleepTime = 0;
	m_sleepCheckTime = delay_time;
	m_inputType = InputType::None;
	m_callbackStack.push(callback);
	m_callbackStack.push(std::bind(&U3Misc::SleepCallback, this));
	//m_callbackStack.push(std::bind(&U3Misc::ProcessEventCallback, this));
}

void U3Misc::AgeChars() // $7470
{
	if (m_Party[2] > 0)
	{
		m_gTime[0]--;
		if (m_gTime[0] > 0)
		{
			return;
		}
		else
		{
			m_gTime[0] = 4;
		}
	}
	m_gTime[1]--;
	if (m_gTime[1] < 0)
	{
		m_gTime[1] = 9;
	}

	m_chNum = 3;
	//m_scrollArea->blockPrompt(true);
	m_freezeAnimation = true;
	m_callbackStack.push(std::bind(&U3Misc::AgeCallback, this));
	m_callbackStack.push(std::bind(&U3Misc::AgeCallback, this));
	m_callbackStack.push(std::bind(&U3Misc::AgeCallback, this));
	m_callbackStack.push(std::bind(&U3Misc::AgeCallback, this));
}

bool U3Misc::EndTurnCallback()
{
	if (m_callbackStack.size() > 0)
	{
		m_callbackStack.pop();
	}

	m_freezeAnimation = false;

	if (m_graphics->m_queuedMode != U3GraphicsMode::None)
	{
		//m_scrollArea->blockPrompt(true);
		m_graphics->m_curMode = m_graphics->m_queuedMode;
		m_graphics->m_mode_switch = true;
		m_resources->m_wasMove = true;

		if (m_graphics->m_curMode == U3GraphicsMode::Dungeon)
		{
			m_dungeon->DungeonStart(1);
			m_gameMode = GameStateMode::Dungeon;
		}
		else if (m_graphics->m_curMode == U3GraphicsMode::Combat)
		{
			m_gameMode = GameStateMode::Combat;
		}
		m_graphics->m_queuedMode = U3GraphicsMode::None;

	}
	else
	{
		//m_scrollArea->blockPrompt(false);
	}

	return false;
}

bool U3Misc::AgeCallback()
{
	if (m_callbackStack.size() > 0)
	{
		m_callbackStack.pop();
	}

	int temp;
	char cType;

	m_rosNum = m_Party[m_chNum + 6];
	cType = m_Player[m_rosNum][23];

	// Wizard, full Int
	if (cType == m_careerTable[2])
	{
		if (m_Player[m_rosNum][25] < m_Player[m_rosNum][20])
		{
			m_Player[m_rosNum][25]++;
		}
	}

	// Cleric, full Wis
	if (cType == m_careerTable[1])
	{
		if (m_Player[m_rosNum][25] < m_Player[m_rosNum][21])
		{
			m_Player[m_rosNum][25]++;
		}
	}

	// Lark Druid Alchemist, half Int
	if ((cType == m_careerTable[6]) || (cType == m_careerTable[8]) || (cType == m_careerTable[9]))
	{
		if (m_Player[m_rosNum][25] < (m_Player[m_rosNum][20] / 2))
		{
			m_Player[m_rosNum][25]++;
		}
	}

	// Paladin Illusionist Druid, half Wis
	if ((cType == m_careerTable[4]) || (cType == m_careerTable[7]) || (cType == m_careerTable[8]))
	{
		if (m_Player[m_rosNum][25] < (m_Player[m_rosNum][21] / 2))
		{
			m_Player[m_rosNum][25]++;
		}
	}

	// Ranger lesser of half Wis or half Int
	if (cType == m_careerTable[10])
	{
		temp = m_Player[m_rosNum][25];
		if ((temp < (m_Player[m_rosNum][20] / 2)) && (temp < (m_Player[m_rosNum][21] / 2)))
		{
			m_Player[m_rosNum][25]++;
		}
	}
	temp = m_Player[m_rosNum][17];
	if ((temp == 'D') || (temp == 'A') || (temp == 0))
	{
		m_chNum--;
	}
	else
	{
		EatFood(m_chNum, 10, std::bind(&U3Misc::FinishEatFood, this));
	}
	return false;
}

bool U3Misc::FinishEatFood()
{
	/*if (m_callbackStack.size() > 0)
	{
		m_callbackStack.pop();
	}*/

	/*if (m_Player[m_rosNum][17] == 'P')
	{
		HPSubtract(m_rosNum, 1);
		InverseCharDetails(m_chNum, true);
		m_resources->m_inverses.elapsedTileTime = 0;
		m_resources->m_inverses.inverseTileTime = damage_time;
		m_resources->setInversed(true);
		m_resources->m_inverses.func = std::bind(&U3Misc::FinishTurnCallback, this);
		m_scrollArea->UPrintMessage(183);
		return false;
	}*/
	int temp = m_Player[m_rosNum][17];
	if ((temp == 'D') || (temp == 'A') || (temp == 0))
	{
		m_chNum--;
	}
	else
	{
		m_callbackStack.push(std::bind(&U3Misc::FinishTurnCallback, this));

		if (m_Player[m_rosNum][17] == 'P')
		{
			HPSubtract(m_rosNum, 1);
			m_callbackStack.push(std::bind(&U3Misc::InverseCallback, this));
			m_resources->setInversed(true);
			m_scrollArea->UPrintMessage(183);
			InverseCharDetails(m_chNum, true);
			m_resources->m_inverses.elapsedTileTime = 0;
			m_resources->m_inverses.inverseTileTime = damage_time;
		}

	}
	return false;
}

bool U3Misc::FinishTurnCallback()
{
	if (m_callbackStack.size() > 0)
	{
		m_callbackStack.pop();
	}
	if (m_gTime[1] == 0)
	{
		HPAdd(m_rosNum, 1);
	}
	//m_inputType = InputType::Callback;
	m_chNum--;

	return false;
}

bool U3Misc::InverseCallback()
{
	if (m_resources->m_inverses.elapsedTileTime > m_resources->m_inverses.inverseTileTime)
	{
		if (m_callbackStack.size() > 0)
		{
			m_callbackStack.pop();
		}

		m_resources->setInversed(false);
		return true;
	}

	return true;
}

void U3Misc::EatFood(short member, short amount, std::function<bool()> callback) // member = 0-3 $761D
{
	short rosNum;
	rosNum = m_Party[6 + member];
	m_rosNum = rosNum;
	m_Player[rosNum][34] -= (unsigned char)amount;
	if (m_Player[rosNum][34] > 127)
	{
		m_Player[rosNum][34] -= 157;
		m_Player[rosNum][33] -= 1;
		if (m_Player[rosNum][33] > 127)
		{
			m_Player[rosNum][33] -= 157;
			m_Player[rosNum][32] -= 1;
			if (m_Player[rosNum][32] > 127)
			{
				m_Player[rosNum][32] = m_Player[rosNum][33] = m_Player[rosNum][34] = 0;
				m_scrollArea->UPrintMessage(184);

				m_callbackStack.push(std::bind(&U3Misc::EatFoodCallback, this));
				m_callbackStack.push(std::bind(&U3Misc::InverseCallback, this));
				m_resources->setInversed(true);
				InverseCharDetails(member, true);
				m_resources->m_inverses.elapsedTileTime = 0;
				m_resources->m_inverses.inverseTileTime = damage_time;

				/*m_resources->setInversed(true);
				InverseCharDetails(member, true);
				m_resources->m_inverses.elapsedTileTime = 0;
				m_resources->m_inverses.inverseTileTime = damage_time;
				m_resources->m_inverses.func = std::bind(&U3Misc::EatFoodCallback, this);*/
				return;
			}
		}
	}
	FinishEatFood();
}

bool U3Misc::EatFoodCallback()
{
	if (m_callbackStack.size() > 0)
	{
		m_callbackStack.pop();
	}

	HPSubtract(m_rosNum, 5);
	FinishEatFood();

	return false;
}

void U3Misc::HPAdd(short member, short amount)
{
	long hp, maxhp;
	hp = (m_Player[member][26] * 256 + m_Player[member][27]);
	maxhp = (m_Player[member][28] * 256 + m_Player[member][29]);
	hp += amount;
	if (hp > maxhp)
	{
		hp = maxhp;
	}
	m_Player[member][26] = (unsigned char)(hp / 256);
	m_Player[member][27] = (unsigned char)(hp - (m_Player[member][26] * 256));
}

void U3Misc::AddProcessEvent()
{
	m_callbackStack.push(std::bind(&U3Misc::ProcessEventCallback, this));
}

void U3Misc::AddInverse()
{
	m_callbackStack.push(std::bind(&U3Misc::InverseCallback, this));
}

void U3Misc::AddFinishTurn()
{
	m_callbackStack.push(std::bind(&U3Misc::CommandFinishTurn, this));
}

void U3Misc::Whirl1()
{
	unsigned char byte;

	byte = (unsigned char)m_utilities->getRandom(0, 7);
	m_WhirlDX = WhirlXtable[byte];
	m_WhirlDY = WhirlYtable[byte];
}

void U3Misc::Whirl2(short newx, short newy)
{
	PutXYVal(0x30, (unsigned char)newx, (unsigned char)newy);
	PutXYVal(0, (unsigned char)m_WhirlX, (unsigned char)m_WhirlY);
	m_WhirlX = newx;
	m_WhirlY = newy;
	Whirl3();
}

void U3Misc::Whirl3() // $771A
{
	if ((m_WhirlX == m_xpos) && (m_WhirlY == m_ypos)) // Whirlpool hit party
	{
		GoWhirlPool();
	}
}

void U3Misc::WhirlPool() // $7665
{
	unsigned char byte;
	short swap, newx, newy;

	m_gWhirlCtr--;
	if (m_gWhirlCtr > 0)
	{
		return;
	}
	m_gWhirlCtr = 4;

	if (m_Party[2] != 0)
	{
		return;
	}
	byte = (unsigned char)m_utilities->getRandom(0, 7);

	if (byte == 0)
	{
		Whirl1();
		return;
	}

	newx = m_graphics->MapConstrain(m_WhirlX + m_WhirlDX);
	newy = m_graphics->MapConstrain(m_WhirlY + m_WhirlDY);
	byte = GetXYVal(newx, newy);

	if (byte == 0)
	{
		Whirl2(newx, newy);
		return;
	}
	if (byte != 0x2C)
	{
		Whirl1();
		return;
	}
	PutXYVal(0x30, (unsigned char)newx, (unsigned char)newy);
	swap = m_WhirlX;
	m_WhirlX = newx;
	newx = swap;
	swap = m_WhirlY;
	m_WhirlY = newy;
	newy = swap;
	PutXYVal(0, (unsigned char)newx, (unsigned char)newy);
	m_scrollArea->UPrintMessageRewrapped(110);
	m_scrollArea->UPrintWin("\n");
}

void U3Misc::GoWhirlPool() // 772D
{
	m_Party[0] = 24; // Be one with the whirlpool

	bool classic;
	m_resources->GetPreference(U3PreferencesType::Classic_Appearance, classic);
	if (classic)
	{
		m_scrollArea->UPrintMessage(111);
	}
	else
	{
		m_scrollArea->UPrintMessageRewrapped(256);
	}
	m_audio->m_cachedSong = m_audio->m_currentSong;
	m_audio->m_currentSong = 0;
	m_audio->m_nextSong = 0;
	m_audio->musicUpdate();

	m_elapsedSleepTime = 0;
	m_sleepCheckTime = whirlpool_time;
	m_inputType = InputType::SleepCallback;
	//m_scrollArea->blockPrompt(true);
	m_callbackStack.push(std::bind(&U3Misc::GoWhirlPoolCallback, this));
	m_callbackStack.push(std::bind(&U3Misc::SleepCallback, this));
}

bool U3Misc::GoWhirlPoolCallback()
{
	if (m_callbackStack.size() > 0)
	{
		m_callbackStack.pop();
	}

	if (m_Party[2] == 0)
	{
		m_Party[3] = (unsigned char)m_xpos;
		m_xs = m_xpos;
		m_Party[4] = (unsigned char)m_ypos;
		m_ys = m_ypos;
		PutXYVal(0, (unsigned char)m_xpos, (unsigned char)m_ypos);
		m_WhirlX = 2;
		m_WhirlY = 0x3E;
		m_Party[0] = 0x16; // frigate
		//gSongCurrent=gSongNext=0;
		PushSosaria();
		bool autosave;
		bool classic;
		m_resources->GetPreference(U3PreferencesType::Auto_Save, autosave);
		if (autosave)
		{
			PutRoster();
			PutParty();
			PutSosaria();
		}
		m_resources->GetPreference(U3PreferencesType::Classic_Appearance, classic);
		if (classic)
		{
			m_scrollArea->UPrintMessage(112);
		}
		else
		{
			m_scrollArea->UPrintMessageRewrapped(257);
		}
		LoadUltimaMap(21);

		m_resources->m_inverses.color.r = 0;
		m_resources->m_inverses.color.g = 0;
		m_resources->m_inverses.color.b = 0;
		m_resources->m_inverses.color.a = 255;
		m_resources->m_inverses.fill = true;
		m_resources->m_inverses.elapsedTileTime = 0;
		m_resources->m_inverses.inverseTileTime = whirlpool_time;
		m_Party[0] = 0x7E;
		m_xpos = 32;
		m_ypos = 54;
		m_Party[2] = 255;
		
		m_resources->setInversed(true);
		m_callbackStack.push(std::bind(&U3Misc::GoWhirlPoolCallback1, this));
		m_callbackStack.push(std::bind(&U3Misc::InverseCallback, this));
	}
	else if (m_Party[2] == 0xFF)
	{
		PullSosaria();
		m_scrollArea->UPrintMessage(114);
		m_resources->m_inverses.color.r = 0;
		m_resources->m_inverses.color.g = 0;
		m_resources->m_inverses.color.b = 0;
		m_resources->m_inverses.color.a = 255;
		m_resources->m_inverses.fill = true;
		m_resources->m_inverses.elapsedTileTime = 0;
		m_resources->m_inverses.inverseTileTime = whirlpool_time;
		m_callbackStack.push(std::bind(&U3Misc::GoWhirlPoolCallback2, this));
		m_callbackStack.push(std::bind(&U3Misc::InverseCallback, this));
		m_xpos = m_Party[3];
		m_ypos = m_Party[4];
		m_Party[2] = 0;
		m_Party[0] = 0x16;
		bool autosave;
		m_resources->GetPreference(U3PreferencesType::Auto_Save, autosave);
		if (autosave)
		{
			PutRoster();
			PutParty();
		}
		
	}
	else if (m_Party[2] < 128)
	{
		short temp;

		temp = 0xFF;
		m_utilities->getRandom(0, m_mapSize - 1);
		m_xpos = (short)m_utilities->getRandom(0, m_mapSize - 1);
		m_ypos = (short)m_utilities->getRandom(0, m_mapSize - 1);
		temp = GetXYVal(m_xpos, m_ypos);
		m_audio->m_nextSong = m_audio->m_cachedSong;
		m_audio->musicUpdate();
	}
	return false;
}

bool U3Misc::GoWhirlPoolCallback2()
{
	if (m_callbackStack.size() > 0)
	{
		m_callbackStack.pop();
	}
	m_audio->m_nextSong = 1;
	m_audio->musicUpdate();
	m_scrollArea->UPrintMessage(115);
	m_inputType = InputType::Default;
	//m_scrollArea->blockPrompt(false);
	return false;
}

bool U3Misc::GoWhirlPoolCallback1()
{
	if (m_callbackStack.size() > 0)
	{
		m_callbackStack.pop();
	}

	m_audio->m_nextSong = 10;
	m_audio->musicUpdate();
	bool classic;
	m_resources->GetPreference(U3PreferencesType::Classic_Appearance, classic);
	if (classic)
	{
		m_scrollArea->UPrintMessage(113);
	}
	else
	{
		m_scrollArea->UPrintMessageRewrapped(258);
	}
	m_inputType = InputType::Default;
	//m_scrollArea->blockPrompt(false);
	return false;
}

void U3Misc::MoonGateUpdate() // $6F5D
{
	if (m_Party[2] != 0)
	{
		return; // only if on surface!
	}
	m_gMoon[0]--;
	if (m_gMoon[0] < 1)
	{
		m_gMoon[0] = 12;
		m_gMoonDisp[0]++;
		if (m_gMoonDisp[0] > '7')
		{
			m_gMoonDisp[0] = '0';
		}
	}
	m_gMoon[1]--;
	if (m_gMoon[1] < 1)
	{
		m_gMoon[1] = 4;
		m_gMoonDisp[1]++;
		if (m_gMoonDisp[1] > '7')
		{
			m_gMoonDisp[1] = '0';
		}
	}
}

void U3Misc::HandleMoonStep()
{
	m_callbackStack.push(std::bind(&U3Misc::HandleMoonStepCallback, this));
	InverseTiles(true);
	m_resources->m_inverses.stayInversed = true;
}

bool U3Misc::HandleMoonStepCallback()
{
	if (m_callbackStack.size() > 0)
	{
		m_callbackStack.pop();
	}
	short value;
	if (m_Party[2] == 0)
	{
		value = m_gMoonDisp[1] - '0';
		m_xpos = m_MoonXTable[value];
		m_ypos = m_MoonYTable[value];
	}
	else
	{
		value = 0;
		while (value != 0x04)
		{
			m_xpos = (short)m_utilities->getRandom(0, m_mapSize - 1);
			m_ypos = (short)m_utilities->getRandom(0, m_mapSize - 1);
			value = GetXYVal(m_xpos, m_ypos);
		}
	}

	m_callbackStack.push(std::bind(&U3Misc::HandleMoonStepCallback1, this));
	InverseTiles(true);
	m_resources->m_inverses.stayInversed = false;

	return false;
}

bool U3Misc::HandleMoonStepCallback1()
{
	if (m_callbackStack.size() > 0)
	{
		m_callbackStack.pop();
	}

	return false;
}

bool U3Misc::EnterShrineCallback()
{
	if (m_callbackStack.size() > 0)
	{
		m_callbackStack.pop();
	}

	m_chNum = m_input_num;

	if (m_chNum < 0 || m_chNum > 3)
	{
		m_scrollArea->UPrintWin("\n");
		return false;
	}
	std::string dispString(std::to_string(m_chNum + 1) + std::string("\n"));
	m_scrollArea->UPrintWin(dispString);
	if (m_Party[6 + m_chNum] == 0)
	{
		m_scrollArea->UPrintMessage(41);
		return false;
	}
	if (CheckAlive(m_chNum) == false)
	{
		m_spellCombat->Incap();
		return false;
	}

	Shrine(m_chNum);

	return false;
}

void U3Misc::Shrine(short chnum)
{
	short shtype = m_xpos & 0x03;
	short key = 0;

	m_resources->m_overrideImage = 7;
	std::string attributeName = m_resources->m_plistMap["Messages"][173 + shtype];

	bool classic;
	m_resources->GetPreference(U3PreferencesType::Classic_Appearance, classic);
	m_rosNum = m_Party[6 + chnum];

	if (classic)
	{
		m_scrollArea->UPrintMessage(235);
		for (key = 0; key < ((16 - attributeName[0]) / 2); key++)
		{
			m_scrollArea->UPrintWin(" ");
		}
		m_scrollArea->UPrintWin(attributeName);
		m_scrollArea->UPrintWin("\n\n");
	}
	else
	{
		std::string msg = m_resources->m_plistMap["Messages"][258];
		msg += attributeName;
		m_scrollArea->RewrapString(msg);
		m_scrollArea->UPrintWin(msg);
		m_scrollArea->UPrintWin("\n\n");
	}

	m_scrollArea->UPrintMessage(178);
	setInputTypeNumImmediate(std::bind(&U3Misc::shrineCallback, this));
}

bool U3Misc::shrineCallback()
{
	if (m_callbackStack.size() > 0)
	{
		m_callbackStack.pop();
	}
	short race = m_Player[m_rosNum][22];
	short shtype = m_xpos & 0x03;
	short maxval = 0;
	short statnum = 0;
	short shMax[22] = { 75, 75, 99, 75, 25, 75, 99, 75, 50, 99, 75, 50, 75, 99, 75, 75, 75, 50, 75, 99 };
	//short voiceTile = 0;    // correspond to class which ability is most apropos for

	switch (shtype)
	{
	case 0:
		maxval = shMax[ShrineRace(race)];
		statnum = 18;
		//voiceTile = 20;
		break;
	case 1:
		maxval = shMax[ShrineRace(race) + 5];
		statnum = 19;
		//voiceTile = 23;
		break;
	case 2:
		maxval = shMax[ShrineRace(race) + 15];
		statnum = 20;
		//voiceTile = 22;
		break;
	case 3:
		maxval = shMax[ShrineRace(race) + 10];
		statnum = 21;
		//voiceTile = 21;
		break;
	}

	short key = m_input_num;

	m_scrollArea->setInput(false);

	if (key == 0)
	{
		m_scrollArea->UPrintMessage(179);
		m_resources->m_overrideImage = -1;
		return false;
	}
	short gold = ((m_Player[m_rosNum][35]) * 256) + m_Player[m_rosNum][36];
	if (gold - (key * 100) < 0)
	{
		m_scrollArea->UPrintMessage(180);
		m_resources->m_overrideImage = -1;
		return false;
	}
	gold -= (key * 100);
	m_Player[m_rosNum][35] = (unsigned char)(gold / 256);
	m_Player[m_rosNum][36] = (unsigned char)(gold - (m_Player[m_rosNum][35] * 256));
	m_scrollArea->UPrintMessage(181);

	if (statnum > 0)
	{
		m_Player[m_rosNum][statnum] += (unsigned char)key;
		if (m_Player[m_rosNum][statnum] > 99)
		{
			m_Player[m_rosNum][statnum] = 99;
		}
		if ((m_Player[m_rosNum][statnum] > maxval) && (m_Party[15] == 0))
		{
			m_Player[m_rosNum][statnum] = (unsigned char)maxval;
		}
	}
	m_callbackStack.push(std::bind(&U3Misc::shrineCallback1, this));
	InverseCharDetails(m_chNum, true);
	InverseTiles(true);
	return false;
}

bool U3Misc::shrineCallback1()
{
	if (m_callbackStack.size() > 0)
	{
		m_callbackStack.pop();
	}
	m_resources->m_overrideImage = -1;
	m_audio->m_nextSong = 10;
	return false;
}

short U3Misc::ShrineRace(short race)
{
	short byte, result;
	std::string tempStr;

	result = 0;
	for (byte = 0; byte < 5; byte++)
	{
		tempStr = m_resources->m_plistMap["Races"][byte];
		if (race == tempStr[0])
		{
			result = byte;
		}
	}
	return result;
}

void U3Misc::SpawnMonster() // $7A0C
{
	const char              MonTypes[14] = { 24,23,25,20,26,27,13,28,22,14,15,29,30,24 };
	const char              MonBegin[13] = { 4,4,4,4,4,4,0,4,4,0,0,4,4 };
	bool allFirst;
	long hpmax;
	short type, chnum;
	if (m_Party[2] != 0)
	{
		return;
	}
	int rngNum = m_utilities->getRandom(0, 134);
	if (rngNum < 128)
	{
		return;
	}
	for (int offset = 31; offset >= 0; offset--)
	{
		if (m_Monsters[offset] != 0)
		{
			continue;
		}
		allFirst = true;
		for (chnum = 0; chnum < 4; chnum++)   // is everyone hpmax = 150?
		{
			hpmax = m_Player[m_Party[chnum + 6]][28] * 256 + m_Player[m_Party[chnum + 6]][29];
			if (hpmax > 150)
			{
				allFirst = false;
			}
		}
		if (allFirst)
		{
			type = (short)m_utilities->getRandom(0, 2); // then only thiefs/orx/skeletons
		}
		else
		{
			rngNum = (short)m_utilities->getRandom(0, 12);
			type = (short)m_utilities->getRandom(0, 12);
			type &= rngNum;
		}
		rngNum = m_utilities->getRandom(0, m_mapSize - 1);
		m_Monsters[offset] = MonTypes[type] * 4;
		m_Monsters[offset + TILEON] = MonBegin[type];
		m_Monsters[offset + XMON] = (unsigned char)rngNum;
		if (m_Monsters[offset + XMON] == m_xpos)
		{
			m_Monsters[offset] = 0;
			continue;
		}
		rngNum = m_utilities->getRandom(0, m_mapSize - 1);
		m_Monsters[offset + YMON] = (unsigned char)rngNum;
		if (m_Monsters[offset + YMON] == m_xpos)
		{
			m_Monsters[offset] = 0;
			continue;
		}
		if (GetXYVal(m_Monsters[offset + XMON], m_Monsters[offset + YMON]) != m_Monsters[offset + TILEON])
		{
			m_Monsters[offset] = 0;
			continue;
		}
		m_Monsters[offset + HPMON] = 0xC0;
		unsigned char var = 0;
		rngNum = m_utilities->getRandom(0, 1);
		if (rngNum)
		{
			var = (unsigned char)m_utilities->getRandom(1, 2);
		}
		m_Monsters[offset + VARMON] = var;
		if (m_Party[15] != 0 && m_Monsters[offset] != 0x3C)
		{
			m_Monsters[offset + HPMON] = 0x40;
		}
		PutXYVal(m_Monsters[offset], m_Monsters[offset + XMON], m_Monsters[offset + YMON]);
		break;
	}
}

void U3Misc::AttackCode(short whichMon) // $52B3
{
	unsigned char tileon;
	m_xs = m_Monsters[whichMon + XMON];
	m_ys = m_Monsters[whichMon + YMON];
	tileon = m_Monsters[whichMon + TILEON];
	if (tileon != 0)
	{
		tileon = ((tileon / 4) & 0x03) + 0x24;
	}
	PutXYVal(tileon, (unsigned char)m_xs, (unsigned char)m_ys);
	m_gMonType = m_Monsters[whichMon] / 2;
	m_gMonVarType = m_Monsters[VARMON + whichMon];
	m_Monsters[whichMon] = 0;
	if (m_gMonType == 0x1E) // Pirate
	{
		if (m_Party[0] != 0x16)
		{
			PutXYVal(0x2C, (unsigned char)m_xs, (unsigned char)m_ys);
		}
	}
	m_spellCombat->Combat();
}

bool U3Misc::ProcessMenuEvent(SDL_Event event)
{
	bool quit = false;
	//bool gInterrupt = false;
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
			if (event.key.key == SDLK_TAB)
			{
				m_graphics->returnToGame(0);
			}
		}
		break;
	case SDL_EVENT_MOUSE_BUTTON_DOWN:
		mouseState = 1;
		updateMouse = true;
		/*if (m_inputType == InputType::AnyKey)
		{
			HandleAnyKey();
			retVal = true;
		}
		else if (m_inputType == InputType::AnyKeyEscape)
		{
			HandleAnyKeyEscape(0);
			retVal = true;
		}*/
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

	if (updateMouse)
	{
		//if (m_inputType == InputType::ZStats)
		{
			bool isCaptured = false;

			if (m_graphics->m_buttons.size() > 0)
			{
				for (auto& curButton : m_graphics->m_buttons)
				{
					curButton->setMouseCapture(mouseState, event.motion.x, event.motion.y);
					isCaptured |= curButton->isCaptured();
				}
			}
			if (mouseState == 1 && !isCaptured)
			{
				//HandleAnyKey();
			}
		}
	}
	return retVal;
}
