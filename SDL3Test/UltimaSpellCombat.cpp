#include "UltimaSpellCombat.h"
#include "U3ScrollArea.h"
#include "U3Misc.h"
#include "UltimaDungeon.h"
#include "U3Resources.h"
#include "U3Utilities.h"
#include "UltimaGraphics.h"
#include "UltimaIncludes.h"

extern std::unique_ptr<U3Resources> m_resources;
extern std::unique_ptr<U3Misc> m_misc;
extern std::unique_ptr<U3ScrollArea> m_scrollArea;
extern std::unique_ptr<U3Utilities> m_utilities;
extern std::unique_ptr<UltimaDungeon> m_dungeon;
extern std::unique_ptr<U3Graphics> m_graphics;

bool UltimaSpellCombat::HandleDefaultKeyPress(SDL_Keycode key)
{
	/*if (key >= SDLK_A && key <= SDLK_Z)
	{
		LetterCommand(key);
	}
	else
	{
		switch (key)
		{
		case SDLK_UP:
			Forward();
			break;
		case SDLK_DOWN:
			Retreat();
			break;
		case SDLK_LEFT:
			Left();
			break;
		case SDLK_RIGHT:
			Right();
			break;
		case SDLK_SPACE:
			m_misc->Pass();
			break;
		default:
			break;
		}
	}*/
	return true;
}


UltimaSpellCombat::UltimaSpellCombat() :
	m_hitType(0),
	m_origValue(0),
	m_x(0),
	m_y(0),
	m_g835E(0),
	m_g5521(0),
	m_chNum(0),
	m_damage(0),
	m_g56E7(0),
	m_newMove(true),
	m_g835D(0),
	m_combatStart(true),
	m_count(0),
	m_count2(0)
{
}

void UltimaSpellCombat::drawCombat()
{
	m_resources->DrawTiles();
}

void UltimaSpellCombat::combatstart()
{
	m_resources->m_wasMove = true;
	m_newMove = true;
	m_g835D = 0;
	m_misc->AgeChars();
}


void UltimaSpellCombat::Combat()
{
	unsigned char monhpstart[16] = { 0x20, 0x20, 0xF0, 0xF0, 0xC0, 0x60, 0xA0, 0x80, 0x30, 0x50, 0x70, 0xA0, 0xC0, 0xE0, 0xF0, 0xF0 };
	m_graphics->m_lastMode = m_graphics->m_curMode;
	m_graphics->m_queuedMode = U3GraphicsMode::Combat;
	m_misc->m_gameMode = GameStateMode::Combat;

	m_g5521 = m_g56E7 = 0;

	for (short mon = 31; mon >= 0; mon--)
	{
		if (m_misc->m_Monsters[mon] == 0x4C || m_misc->m_Monsters[mon] == 0x48)
		{
			m_misc->m_Monsters[mon + HPMON] = 0xC0;
		}
	}
	m_scrollArea->UPrintMessage(133);
	short numMon = HowMany();
	PrintMonster(m_misc->m_gMonType, (numMon > 0), m_misc->m_gMonVarType);
	m_scrollArea->UPrintWin("\n\n");
	GetScreen(BackGround(m_misc->m_gMonType));
	m_g835E = m_misc->m_Party[2];
	m_misc->m_Party[2] = 0x80;
	short health;
	short mon;

	for (short chnum = m_misc->m_Party[1] - 1; chnum >= 0; chnum--)
	{
		health = m_misc->m_Player[m_misc->m_Party[6 + chnum]][17];
		if (health == 'G' || health == 'P')
		{
			// The game was originally written so that it reads both of the tiles for a character
			// then moves down to the next one, whereas I wrote it just reading down row by row.
			// Maybe for the future, to give a slight speed increase, change everything to match,
			// but for now, just finish
			int charTile = DetermineShape(m_misc->m_Player[m_misc->m_Party[6 + chnum]][23]);
			int offset = (charTile % 16);
			m_misc->m_CharShape[chnum] = charTile + offset;
			m_misc->m_CharTile[chnum] = m_misc->GetXYTile(m_misc->m_CharX[chnum], m_misc->m_CharY[chnum]);
			m_misc->PutXYTile(m_misc->m_CharShape[chnum], m_misc->m_CharX[chnum], m_misc->m_CharY[chnum]);
		}
		else
		{
			m_misc->m_CharX[chnum] = 0xFF;
			m_misc->m_CharY[chnum] = 0xFF;
		}
	}
	for (mon = 7; mon >= 0; mon--)
	{
		m_misc->m_MonsterHP[mon] = 0;
	}
	while (numMon >= 0)
	{
		mon = m_utilities->getRandom(0, 7);
		if (m_misc->m_MonsterHP[mon] == 0)
		{
			health = (m_misc->m_gMonType / 2) & 0x0F;
			int rngVal = m_utilities->getRandom(0, monhpstart[health]);
			m_misc->m_MonsterHP[mon] = rngVal | 0x0F;
			m_misc->m_MonsterTile[mon] = m_misc->GetXYTile(m_misc->m_MonsterX[mon], m_misc->m_MonsterY[mon]);
			unsigned char tileValue = (unsigned char)m_misc->m_gMonType;
			if (m_misc->m_gMonVarType && m_misc->m_gMonType >= 46 && m_misc->m_gMonType <= 63)
			{
				tileValue = (((m_misc->m_gMonType / 2) - 23) * 2 + 79 + m_misc->m_gMonVarType) * 2;
			}
			m_misc->PutXYTile(tileValue, m_misc->m_MonsterX[mon], m_misc->m_MonsterY[mon]);
			numMon--;
		}
	}
	m_newMove = true;
	m_combatStart = true;
}

void UltimaSpellCombat::GetScreen(short resid)
{
	memcpy(m_resources->m_TileArray, m_resources->m_cons_data[resid].data(), sizeof(unsigned char) * 121);
	memcpy(m_misc->m_MonsterX, m_resources->m_cons_data[resid].data() + 0x80, sizeof(unsigned char) * 8);
	memcpy(m_misc->m_MonsterY, m_resources->m_cons_data[resid].data() + 0x88, sizeof(unsigned char) * 8);
	memcpy(m_misc->m_MonsterTile, m_resources->m_cons_data[resid].data() + 0x90, sizeof(unsigned char) * 8);
	memcpy(m_misc->m_MonsterHP, m_resources->m_cons_data[resid].data() + 0x98, sizeof(unsigned char) * 8);
	memcpy(m_misc->m_CharX, m_resources->m_cons_data[resid].data() + 0xA0, sizeof(unsigned char) * 4);
	memcpy(m_misc->m_CharY, m_resources->m_cons_data[resid].data() + 0xA4, sizeof(unsigned char) * 4);
	memcpy(m_misc->m_CharTile, m_resources->m_cons_data[resid].data() + 0xA8, sizeof(unsigned char) * 4);
	memcpy(m_misc->m_CharShape, m_resources->m_cons_data[resid].data() + 0xAC, sizeof(unsigned char) * 4);
}

void UltimaSpellCombat::Incap()
{
	m_scrollArea->UPrintMessage(126);
}

void UltimaSpellCombat::Failed() // $586E
{
	m_scrollArea->UPrintMessage(86);
}

/* monster name depended upon player's x and y location.
even y locations contain the normal name (Thief, Orc, Skeleton, Giant ...)
odd y locations indicate a variant.
if x is even, it's variant one (Brigand, Goblin, Ghoul, Golem ...)
if x is odd, it's variant two (Cutpurse, Troll, Zombie, Titan ...)
*/
void UltimaSpellCombat::PrintMonster(short which, bool plural, char variant) // $8457
{
	if (which > 44 && variant > 0)   // Ö2 = >23
	{
		m_misc->PrintTile((which - 46) + 63 + variant, plural);
	}
	else
	{
		m_misc->PrintTile(which / 2, plural);
	}
}

unsigned char UltimaSpellCombat::GetXYDng(short x, short y)
{
	if (y < 0)
	{
		y += 16;
	}
	if (x < 0)
	{
		x += 16;
	}
	if (y > 15)
	{
		y -= 16;
	}
	if (x > 15)
	{
		x -= 16;
	}
	return m_misc->m_Dungeon[(m_dungeon->m_dungeonLevel * 256) + (y * 16) + x];
}

void UltimaSpellCombat::PutXYDng(unsigned char value, short x, short y) // ??
{
	m_misc->m_Dungeon[(m_dungeon->m_dungeonLevel * 256) + (y * 16) + x] = value;
}

void UltimaSpellCombat::ShowHit(short x, short y, unsigned char hitType, unsigned char tileUnder)
{
	bool sound_inactive;
	m_resources->GetPreference(U3PreferencesType::Sound_Inactive, sound_inactive);
	if (sound_inactive)
	{
		m_misc->m_inputType = InputType::Default;
		return;
	}
	m_x = x;
	m_y = y;

	m_origValue = m_misc->GetXYTile(x, y);
	m_misc->m_gBallTileBackground = tileUnder;
	m_misc->PutXYTile(hitType, x, y);
	m_resources->SwapShape(hitType);
	m_misc->DelayGame(200, std::bind(&UltimaSpellCombat::ShowHitCallback, this));
	m_hitType = hitType;

}

bool UltimaSpellCombat::ShowHitCallback() const
{
	if (m_misc->m_callbackStack.size() > 0)
	{
		m_misc->m_callbackStack.pop();
	}

	m_resources->SwapShape(m_hitType);
	m_misc->PutXYTile(m_origValue, m_x, m_y);
	m_misc->m_inputType = InputType::Default;

	return false;
}

void UltimaSpellCombat::Poison(short chnum) // $8881
{
	short rosNum;
	rosNum = m_misc->m_Party[6 + chnum];
	int rngNum = m_utilities->getRandom(0, 255);
	if ((rngNum & 0x03) != 0)
	{
		return;
	}
	if (m_misc->m_Player[rosNum][17] != 'G')
	{
		return;
	}
	m_misc->m_Player[rosNum][17] = 'P';
	m_scrollArea->UPrintMessage(141);
	std::string dispString = std::to_string(chnum + 1);
	m_scrollArea->UPrintWin(dispString);
	m_misc->m_wx++;
	m_scrollArea->UPrintMessage(149);
}

void UltimaSpellCombat::Missed() // $8414
{
	m_scrollArea->UPrintMessage(147);
}

void UltimaSpellCombat::Pilfer(short chnum) // $881F
{
	short rosNum, item;
	rosNum = m_misc->m_Party[6 + chnum];
	int rngNum = m_utilities->getRandom(0, 255);
	if (rngNum < 128)
	{
		rngNum = m_utilities->getRandom(0, 15);
		item = rngNum;
		if (item == 0)
		{
			return;
		}
		if (m_misc->m_Player[rosNum][48] == item)
			return;
		if (m_misc->m_Player[rosNum][48 + item] == 0)
			return;
		m_misc->m_Player[rosNum][48 + item] = 0;
	}
	else
	{
		rngNum = m_utilities->getRandom(0, 7);
		item = rngNum;
		if (item == 0)
		{
			return;
		}
		if (m_misc->m_Player[rosNum][40] == item)
		{
			return;
		}
		if (m_misc->m_Player[rosNum][40 + item] == 0)
		{
			return;
		}
		m_misc->m_Player[rosNum][40 + item] = 0;
	}
	m_scrollArea->UPrintMessage(141);
	std::string dispString = std::to_string(chnum + 1);
	m_scrollArea->UPrintWin(dispString);
	m_misc->m_wx++;
	m_scrollArea->UPrintMessage(148);
}

unsigned char UltimaSpellCombat::DetermineShape(short type) // $7F5D
{
	if (type == m_misc->m_careerTable[0])
	{
		return 0x80;
	}
	if (type == m_misc->m_careerTable[1])
	{
		return 0x81;
	}
	if (type == m_misc->m_careerTable[2])
	{
		return 0x82;
	}
	if (type == m_misc->m_careerTable[3])
	{
		return 0x83;
	}
	if (type == m_misc->m_careerTable[4])
	{
		return 0x80;
	}
	if (type == m_misc->m_careerTable[5])
	{
		return 0x80;
	}
	if (type == m_misc->m_careerTable[6])
	{
		return 0x21;
	}
	if (type == m_misc->m_careerTable[7])
	{
		return 0x82;
	}
	if (type == m_misc->m_careerTable[8])
	{
		return 0x81;
	}
	if (type == m_misc->m_careerTable[9])
	{
		return 0x82;
	}
	return 0x6F;
}

unsigned char UltimaSpellCombat::CombatCharacterHere(short x, short y)
{
	char i;
	for (i = 3; i >= 0; i--)
	{
		if (m_misc->m_CharX[i] == x && m_misc->m_CharY[i] == y)
		{
			return i;
		}
	}
	return 255;
}

unsigned char UltimaSpellCombat::CombatMonsterHere(short x, short y) // $7D24
{
	short mon;
	short result;
	result = 255;
	for (mon = 7; mon >= 0; mon--)
	{
		if (m_misc->m_MonsterHP[mon] != 0)
		{
			if (m_misc->m_MonsterX[mon] == x && m_misc->m_MonsterY[mon] == y)
			{
				result = mon;
			}
		}
	}
	return (unsigned char)result;
}

unsigned char UltimaSpellCombat::ExodusCastle() const // $6F43
{
	short value;
	value = m_misc->m_Party[2];
	if (m_misc->m_Party[15] == 1)
	{
		return 0xFF;
	}
	if (value == 0x80)
	{
		value = m_g835E;
	}
	if (value != 3)
	{
		return 0xFF;
	}
	if (m_misc->m_Party[3] != m_misc->m_LocationX[1])
	{
		return 0xFF;
	}
	return 0;    // In Exodus Castle!
}

unsigned char UltimaSpellCombat::HowMany() const // $80DE
{
	if (m_misc->m_gMonType == 0x26)
	{
		return 0;
	}
	if (m_misc->m_gMonType == 0x24)
	{
		return 7;
	}
	if (ExodusCastle() == 0)
	{
		return 7;
	}
	int rngNum = m_utilities->getRandom(0, 7);
	if (m_misc->m_Party[2] < 2)
	{
		return rngNum;    // Party[2]'s were g835E, which wasn't set yet!
	}
	if (m_misc->m_Party[2] > 3)
	{
		return rngNum;
	}
	return 0;
}

short UltimaSpellCombat::BackGround(short montype) // A, +1=B, +2=C, +3=F, +4=G, +5=M, +6=Q, +7=R, +8=S
{
	short tile;
	if (m_misc->m_Party[2] == 1)
	{
		return 2;
	}
	if (montype == 0x10)
	{
		return 2;    // fighting floor on floor
	}
	if (montype == 0x02)
	{
		return 4;    // fighting grass on grass [pass the bong!]
	}
	if (montype == 0x1E)       // Frigate filled with
	{
		m_misc->m_gMonType = 0x2E;    // Thieves
		if (m_misc->m_Party[0] == 0x16)
		{
			return 8;
		}
		return 0;
	}
	if (m_misc->m_Party[0] == 0x16)
	{
		if (montype < 0x20)
		{
			return 6;
		}
		return 7;
	}
	if ((montype < 0x20) && (montype > 0x14))
	{
		return 5;
	}
	tile = m_misc->GetXYVal(m_misc->m_xpos, m_misc->m_ypos) / 2;
	if (tile == 0x02)
	{
		return 4;
	}
	if (tile == 0x04)
	{
		return 1;
	}
	if (tile == 0x06)
	{
		return 3;
	}
	if (tile == 0x10)
	{
		return 2;
	}
	if (tile == 0x40)
	{
		return 2;
	}
	if (tile == 0x42)
	{
		return 2;
	}
	return 4;
}

void UltimaSpellCombat::Flashriek() // $5885
{
	short SpellSound[34] = { 0, 1, 2, 3, 4, 1, 5, 1, 2, 7, 0, 1, 7, 0, 0, 0, 0, 7, 6, 2, 4, 3, 5, 6, 5, 2, 6, 7, 1, 6, 0, 6, 7, 0 };
	m_misc->InverseTiles(true);
}


void UltimaSpellCombat::BigDeath(short damage, short chnum) // $5600
{
	if (m_misc->m_Party[2] != 0x80)
	{
		Failed();
		return;
	}
	m_damage = damage;
	m_chNum = chnum;
	m_misc->m_callbackStack.push(std::bind(&UltimaSpellCombat::BigDeathCallback, this));
	Flashriek();
}

bool UltimaSpellCombat::BigDeathCallback()
{
	if (m_misc->m_callbackStack.size() > 0)
	{
		m_misc->m_callbackStack.pop();
	}
	for (short mon = 7; mon >= 0; mon--)
	{
		m_misc->m_callbackStack.push(std::bind(&UltimaSpellCombat::BigDeathCallback1, this));
	}
	m_misc->m_opnum = 7;
	return false;
}

bool UltimaSpellCombat::BigDeathCallback1()
{
	if (m_misc->m_callbackStack.size() > 0)
	{
		m_misc->m_callbackStack.pop();
	}
	int rngNum = m_utilities->getRandom(0, 255);
	if (m_misc->m_MonsterHP[m_misc->m_opnum] != 0)
	{
		m_misc->m_opnum2 = m_misc->GetXYTile(m_misc->m_MonsterX[m_misc->m_opnum], m_misc->m_MonsterY[m_misc->m_opnum]);
		m_misc->m_gBallTileBackground = m_misc->m_MonsterTile[m_misc->m_opnum];
		m_misc->PutXYTile(0x78, m_misc->m_MonsterX[m_misc->m_opnum], m_misc->m_MonsterY[m_misc->m_opnum]);
		ShowHit(m_misc->m_MonsterX[m_misc->m_opnum], m_misc->m_MonsterY[m_misc->m_opnum], 0x78, m_misc->m_MonsterTile[m_misc->m_opnum]);
		m_misc->DelayGame(80, std::bind(&UltimaSpellCombat::BigDeathCallback2, this));
		return false;
	}
	m_misc->m_opnum--;
	return false;
}

void UltimaSpellCombat::AddExp(short chnum, short amount) // $7091
{
	short rosNum, experience;
	rosNum = m_misc->m_Party[6 + chnum];
	experience = (m_misc->m_Player[rosNum][30] * 100) + m_misc->m_Player[rosNum][31];
	int oldLvl = (experience / 100);
	experience += amount;
	if (experience > 9899)
	{
		experience = 9899;
	}
	int newLvl = (experience / 100);
	m_misc->m_Player[rosNum][30] = experience / 100;
	m_misc->m_Player[rosNum][31] = experience - (m_misc->m_Player[rosNum][30] * 100);
}

void UltimaSpellCombat::DamageMonster(short which, short damage, short chnum)
{
	short expnum;
	std::string numStr;

	if (m_misc->m_gMonType != 0x26) // not Lord British, har har
	{
		if ((m_misc->m_MonsterHP[which] - damage) < 1)
		{
			std::string dispString = m_resources->m_plistMap["Messages"][130];

			expnum = ((m_misc->m_gMonType / 2) & 0x0F);
			numStr = std::to_string((int)m_misc->m_Experience[expnum]);
			dispString += numStr;
			dispString += '\n';
			m_scrollArea->UPrintWin(dispString);
			AddExp(chnum, m_misc->m_Experience[expnum]);
			m_misc->PutXYTile(m_misc->m_MonsterTile[which], m_misc->m_MonsterX[which], m_misc->m_MonsterY[which]);
			m_misc->m_MonsterHP[which] = 0;
		}
		else
		{
			m_misc->m_MonsterHP[which] -= damage;
		}
	}
}

bool UltimaSpellCombat::BigDeathCallback2()
{
	if (m_misc->m_callbackStack.size() > 0)
	{
		m_misc->m_callbackStack.pop();
	}

	m_misc->PutXYTile(m_misc->m_opnum2, m_misc->m_MonsterX[m_misc->m_opnum], m_misc->m_MonsterY[m_misc->m_opnum]);
	DamageMonster(m_misc->m_opnum, m_damage, m_chNum);

	m_misc->m_opnum--;
	return false;
}

void UltimaSpellCombat::Spell(short chnum, short spellnum)
{
	short rosNum;
	int rngNum;

	rosNum = m_misc->m_Party[6 + chnum];
	switch (spellnum)
	{
	case 0: // Repond
		if ((m_misc->m_Party[2] != 0x80) || (m_misc->m_gMonType != 0x30) || (m_g5521 != 0))
		{
			Failed();
			return;
		}
		m_g5521 = (unsigned char)0xFF;
		rngNum = m_utilities->getRandom(0, 255);
		if (rngNum < 128)
		{
			Failed();
			return;
		}
		BigDeath(255, chnum);
		break;
	case 1: // Mittar
		break;
	case 2: // Lorum
		break;
	case 3: // Dor Acron
		break;
	case 4: // Sur Acron
		break;
	case 5: // Fulgar
		break;
	case 6: // Dag Acron
		break;
	case 7: // Mentar
		break;
	case 8: // Dag Lorum
		break;
	case 9: // Fal Divi
		break;
	case 10: // Noxum
		break;
	case 11: // Decorp
		break;
	case 12: // Altair
		break;
	case 13: // Dag Mentar
		break;
	case 14: // Necorp
		break;
	case 15: // nameless
		break;
	case 16: // Pontori
		break;
	case 17: // Appar Unem
		break;
	case 18: // Sanctu
		break;
	case 19: // Luminae
		break;
	case 20: // Rec Su
		break;
	case 21: // Rec Du
		break;
	case 22: // Lib Rec
		break;
	case 23: // Alcort
		break;
	case 24: // Sequitu
		break;
	case 25: // Sominae
		break;
	case 26: // Sanctu Mani
		break;
	case 27: // Vieda
		break;
	case 28: // Excuun
		break;
	case 29: // Surmandum
		break;
	case 30: // ZXKUQYB
		break;
	case 31: // Anju Sermani
		break;
	case 32: // Terramorph
		break;
	case 33: // Armageddon
		break;
	case 34: // Flotellum
		break;
	default:
		break;
	}
}

