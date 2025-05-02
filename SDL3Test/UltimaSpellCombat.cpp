#include "UltimaSpellCombat.h"
#include "U3ScrollArea.h"
#include "U3Misc.h"
#include "UltimaDungeon.h"
#include "U3Resources.h"
#include "U3Utilities.h"
#include "UltimaGraphics.h"
#include "UltimaIncludes.h"
#include "UltimaSound.h"

extern std::unique_ptr<U3Resources> m_resources;
extern std::unique_ptr<U3Misc> m_misc;
extern std::unique_ptr<U3ScrollArea> m_scrollArea;
extern std::unique_ptr<U3Utilities> m_utilities;
extern std::unique_ptr<UltimaDungeon> m_dungeon;
extern std::unique_ptr<U3Graphics> m_graphics;
extern std::unique_ptr<U3Audio> m_audio;


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
	m_count2(0),
	m_activePlayer(0),
	m_g835F(0),
	m_mon(0),
	m_monster_turn(false),
	m_gChnum(0),
	m_wpn(0),
	m_shootX(0),
	m_shootY(0),
	m_shootRet(0),
	m_cHide(false),
	m_elapsedFlashTime(0),
	m_updateBlink(false),
	m_hit(0),
	m_destX(0),
	m_destY(0),
	m_spellnum(-1)
{
	memset(m_futureMonX, 0, sizeof(unsigned char) * 8);
	memset(m_futureMonY, 0, sizeof(unsigned char) * 8);
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
	m_monster_turn = false;
	m_mon = -1;
}


void UltimaSpellCombat::Combat()
{
	m_graphics->m_lastMode = m_graphics->m_curMode;
	m_misc->m_lastMode = m_misc->m_gameMode;
	m_graphics->m_queuedMode = U3GraphicsMode::Combat;
	m_misc->m_gameMode = GameStateMode::Combat;

	m_g835F = (unsigned char)m_audio->m_currentSong;
	m_g5521 = m_g56E7 = 0;
	m_audio->playSfx(SFX_COMBATSTART);
	m_audio->m_currentSong = 0;
	m_audio->m_nextSong = 5;
	m_audio->musicUpdate();
	m_misc->m_callbackStack.push(std::bind(&UltimaSpellCombat::CombatCallback, this));
}

bool UltimaSpellCombat::CombatCallback()
{
	unsigned char monhpstart[16] = { 0x20, 0x20, 0xF0, 0xF0, 0xC0, 0x60, 0xA0, 0x80, 0x30, 0x50, 0x70, 0xA0, 0xC0, 0xE0, 0xF0, 0xF0 };

	if (m_misc->m_callbackStack.size() > 0)
	{
		m_misc->m_callbackStack.pop();
	}

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
			m_misc->m_CharShape[chnum] = (unsigned char)(charTile + offset);
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
		mon = (short)m_utilities->getRandom(0, 7);
		if (m_misc->m_MonsterHP[mon] == 0)
		{
			health = (m_misc->m_gMonType / 2) & 0x0F;
			int rngVal = m_utilities->getRandom(0, monhpstart[health]);
			m_misc->m_MonsterHP[mon] = (unsigned char)(rngVal | 0x0F);
			m_misc->m_MonsterTile[mon] = m_misc->GetXYTile(m_misc->m_MonsterX[mon], m_misc->m_MonsterY[mon]);
			unsigned char tileValue = (unsigned char)m_misc->m_gMonType;
			if (m_misc->m_gMonVarType && m_misc->m_gMonType >= 46 && m_misc->m_gMonType <= 63)
			{
				tileValue = (unsigned char)((((m_misc->m_gMonType / 2) - 23) * 2 + 79 + m_misc->m_gMonVarType) * 2);
			}
			m_misc->PutXYTile(tileValue, m_misc->m_MonsterX[mon], m_misc->m_MonsterY[mon]);
			numMon--;
		}
	}
	m_monster_turn = false;
	m_elapsedFlashTime = 0;
	m_chNum = false;
	m_newMove = true;
	m_combatStart = true;

	return false;
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
	m_audio->playSfx(SFX_ERROR1);
	m_scrollArea->UPrintMessage(126);
}

void UltimaSpellCombat::Failed() // $586E
{
	m_scrollArea->UPrintMessage(86);
	m_audio->playSfx(SFX_FAILEDSPELL);
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
	m_audio->playSfx(SFX_OUCH);
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
		item = (short)rngNum;
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
		item = (short)rngNum;
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
	m_audio->playSfx(SFX_OUCH);
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
	unsigned char rngNum = (unsigned char)m_utilities->getRandom(0, 7);
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
	switch (SpellSound[m_spellnum]) {
	case 0:
		m_audio->playSfx(SFX_BIGDEATH);
		break;    // was 0xE0
	case 1:
		m_audio->playSfx(SFX_IMMOLATE);
		break;    // was 0xE9
	case 2:
		m_audio->playSfx(SFX_TORCHIGNITE);
		break;    // was 0xDF
	case 3:
		m_audio->playSfx(SFX_DOWNWARDS);
		break;    // was 0xDE
	case 4:
		m_audio->playSfx(SFX_UPWARDS);
		break;    // was 0xDD
	case 5:
		m_audio->playSfx(SFX_INVOCATION);
		break;    // was 0xF4
	case 6:
		m_audio->playSfx(SFX_HEAL);
		break;    // was 0xF3
	default:
		m_audio->playSfx(SFX_MISCSPELL);
		break;    // was 0xDC
	}
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

bool UltimaSpellCombat::BigDeathCallback3()
{
	if (m_misc->m_callbackStack.size() > 0)
	{
		m_misc->m_callbackStack.pop();
	}
	m_audio->playSfx(SFX_HIT);
	m_misc->DelayGame(200, std::bind(&UltimaSpellCombat::BigDeathCallback2, this));

	return false;
}

bool UltimaSpellCombat::BigDeathCallback1()
{
	if (m_misc->m_callbackStack.size() > 0)
	{
		m_misc->m_callbackStack.pop();
	}
	//unsigned char rngNum = (unsigned char)m_utilities->getRandom(0, 255);
	if (m_misc->m_MonsterHP[m_misc->m_opnum] != 0)
	{
		m_misc->m_opnum2 = m_misc->GetXYTile(m_misc->m_MonsterX[m_misc->m_opnum], m_misc->m_MonsterY[m_misc->m_opnum]);
		m_misc->m_gBallTileBackground = m_misc->m_MonsterTile[m_misc->m_opnum];
		m_misc->PutXYTile(0x78, m_misc->m_MonsterX[m_misc->m_opnum], m_misc->m_MonsterY[m_misc->m_opnum]);
		
		m_misc->m_callbackStack.push(std::bind(&UltimaSpellCombat::BigDeathCallback3, this));
		ShowHit(m_misc->m_MonsterX[m_misc->m_opnum], m_misc->m_MonsterY[m_misc->m_opnum], 0x78, m_misc->m_MonsterTile[m_misc->m_opnum]);
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
	if (oldLvl < newLvl)
	{
		m_audio->playSfx(SFX_EXPLEVELUP);
	}
	m_misc->m_Player[rosNum][30] = (unsigned char)(experience / 100);
	m_misc->m_Player[rosNum][31] = (unsigned char)(experience - (m_misc->m_Player[rosNum][30] * 100));
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
			m_misc->m_MonsterHP[which] -= (unsigned char)damage;
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

void UltimaSpellCombat::FinishCombatMonsterTurn()
{
	if (m_mon >= 7)
	{
		combatstart();
		return;
	}
}

bool UltimaSpellCombat::FinishCombatTurn()
{
	m_newMove = true;

	if (m_monster_turn)
	{
		FinishCombatMonsterTurn();
		return false;
	}

	m_misc->m_inputType = InputType::Default;

	if (m_g835F == 7)
	{
		m_misc->m_gTimeNegate = 0;
	}

	m_misc->InverseChnum(m_activePlayer, false);
	m_activePlayer++;
	m_activePlayer %= 4;

	short value = 1;
	for (unsigned char mon = 0; mon < 8; mon++)
	{
		if (m_misc->m_MonsterHP[mon] != 0)
		{
			value = 0;
			break;
		}
	}
	if (value == 1)
	{
		Victory();
		return false;
	}

	m_misc->m_wy = 0x17;
	m_misc->m_wx = 0x18;
	m_g835D++;

	if (m_g835D < m_misc->m_Party[1])
	{
		return false;
	}

	if (m_misc->m_gTimeNegate != 0)
	{
		m_misc->m_gTimeNegate--;
		combatstart();
		return false;
	}
	m_monster_turn = true;
	m_mon = -1;
	return false;
}

void UltimaSpellCombat::Victory() const
{
	
	m_scrollArea->UPrintMessage(132);
	m_audio->playSfx(SFX_COMBATVICTORY);
	m_misc->m_callbackStack.push(std::bind(&UltimaSpellCombat::VictoryCallback, this));
}

bool UltimaSpellCombat::VictoryCallback() const
{
	if (m_misc->m_callbackStack.size() > 0)
	{
		m_misc->m_callbackStack.pop();
	}

	m_misc->m_gTimeNegate = 0;
	m_audio->m_currentSong = m_g835F;
	m_audio->m_nextSong = m_g835F;
	m_audio->musicUpdate();
	m_misc->m_Party[2] = m_g835E;
	m_graphics->m_queuedMode = m_graphics->m_lastMode;
	m_misc->m_gameMode = m_misc->m_lastMode;
	m_dungeon->setForceRedraw();

	return false;
}

void UltimaSpellCombat::HandleMove()
{
	m_misc->AddFinishTurn();
	m_misc->m_callbackStack.push(std::bind(&UltimaSpellCombat::CommandHandleMove, this));
}

bool UltimaSpellCombat::CommandHandleMove() // $828D
{
	if (m_misc->m_callbackStack.size() > 0)
	{
		m_misc->m_callbackStack.pop();
	}

	short value;
	m_misc->m_xs = m_misc->m_CharX[m_activePlayer] + m_misc->m_dx;
	if (m_misc->m_xs < 0 || m_misc->m_xs > 10)
	{
		m_scrollArea->UPrintMessage(116);
		m_audio->playSfx(SFX_BUMP);
		return false;
	}
	m_misc->m_ys = m_misc->m_CharY[m_activePlayer] + m_misc->m_dy;
	if (m_misc->m_ys < 0 || m_misc->m_ys > 10)
	{
		m_scrollArea->UPrintMessage(116);
		m_audio->playSfx(SFX_BUMP);
		return false;
	}
	value = ValidMove(m_misc->GetXYTile(m_misc->m_xs, m_misc->m_ys));
	if (value != 0)
	{
		m_scrollArea->UPrintMessage(116);
		m_audio->playSfx(SFX_BUMP);
		return false;
	}
	m_misc->PutXYTile(m_misc->m_CharTile[m_activePlayer], m_misc->m_CharX[m_activePlayer], m_misc->m_CharY[m_activePlayer]);
	m_misc->m_CharX[m_activePlayer] = (unsigned char)m_misc->m_xs;
	m_misc->m_CharY[m_activePlayer] = (unsigned char)m_misc->m_ys;
	m_misc->m_CharTile[m_activePlayer] = m_misc->GetXYTile(m_misc->m_xs, m_misc->m_ys);
	m_misc->PutXYTile(m_misc->m_CharShape[m_activePlayer], m_misc->m_xs, m_misc->m_ys);

	return false;
}

unsigned char UltimaSpellCombat::ValidMove(short value) // $7E5B
{
	switch (value)
	{
	case 2:
	case 4:
	case 6:
	case 0x10:
		m_audio->playSfx(SFX_STEP);
		return 0;
	default:
		return 0xFF;
	}
}

void UltimaSpellCombat::LetterCommand(SDL_Keycode key)
{
	switch (key)
	{
	case SDLK_A:
		m_misc->AddFinishTurn();
		m_misc->m_callbackStack.push(std::bind(&UltimaSpellCombat::CombatAttack, this));
		break;
	case SDLK_C:
		m_misc->m_zp[0x1F] = 0x78;
		m_misc->Cast(m_activePlayer);
		break;
	case SDLK_N:
		m_misc->AddFinishTurn();
		m_scrollArea->UPrintMessage(137);
		m_misc->NegateTime(m_activePlayer);
		break;
	case SDLK_R:
		m_scrollArea->UPrintMessage(138);
		m_misc->ReadyWeapon(m_activePlayer);
		break;
	case SDLK_Z:
		m_scrollArea->UPrintMessage(139);
		m_misc->ZStats(1, m_activePlayer);
		break;
	default:
		m_misc->AddFinishTurn();
		m_scrollArea->UPrintMessage(140);
		m_audio->playSfx(SFX_ERROR2);
		break;
	}
}

bool UltimaSpellCombat::HandleDefaultKeyPress(SDL_Keycode key)
{
	std::string strError;
	bool allowDiagonal;
	m_resources->GetPreference(U3PreferencesType::Allow_Diagonal, allowDiagonal);
	if (key >= SDLK_A && key <= SDLK_Z)
	{
		LetterCommand(key);
	}
	else
	{
		switch (key)
		{
		case SDLK_TAB:
			m_graphics->m_showMenu = false;
			m_graphics->m_menu_stack.push(m_graphics->m_curMode);
			m_graphics->m_curMode = U3GraphicsMode::Menu;
			m_graphics->m_menuInit = false;
			break;
		case SDLK_KP_8:
		case SDLK_UP:
			m_scrollArea->UPrintMessage(24);
			m_misc->m_dy = -1;
			HandleMove();
			break;
		case SDLK_KP_2:
		case SDLK_DOWN:
			m_scrollArea->UPrintMessage(25);
			m_misc->m_dy = 1;
			HandleMove();
			break;
		case SDLK_KP_4:
		case SDLK_LEFT:
			m_scrollArea->UPrintMessage(27);
			m_misc->m_dx = -1;
			HandleMove();
			break;
		case SDLK_KP_6:
		case SDLK_RIGHT:
			m_scrollArea->UPrintMessage(26);
			m_misc->m_dx = 1;
			HandleMove();
			break;
		case SDLK_KP_1:
			if (allowDiagonal)
			{
				m_scrollArea->UPrintMessage(250);
				m_misc->m_dx = -1;
				m_misc->m_dy = 1;
				HandleMove();
			}
			else
			{
				m_misc->AddFinishTurn();
				m_misc->What2();
			}
			break;
		case SDLK_KP_3:
			if (allowDiagonal)
			{
				m_scrollArea->UPrintMessage(251);
				m_misc->m_dx = 1;
				m_misc->m_dy = 1;
				HandleMove();
			}
			else
			{
				m_misc->AddFinishTurn();
				m_misc->What2();
			}
			break;
		case SDLK_KP_7:
			if (allowDiagonal)
			{
				m_scrollArea->UPrintMessage(252);
				m_misc->m_dx = -1;
				m_misc->m_dy = -1;
				HandleMove();
			}
			else
			{
				m_misc->AddFinishTurn();
				m_misc->What2();
			}
			break;
		case SDLK_KP_9:
			if (allowDiagonal)
			{
				m_scrollArea->UPrintMessage(253);
				m_misc->m_dx = 1;
				m_misc->m_dy = -1;
				HandleMove();
			}
			else
			{
				m_misc->AddFinishTurn();
				m_misc->What2();
			}
			break;
		case SDLK_SPACE:
			m_misc->Pass();
			break;
		default:
			strError = std::string(SDL_GetKeyName(key));
			m_misc->AddFinishTurn();
			m_misc->What2();
			break;
		}
	}
	return true;
}

void UltimaSpellCombat::HandleMonsterMove()
{
	m_misc->m_callbackStack.push(std::bind(&UltimaSpellCombat::HandleMonsterMoveCallback, this));
}

bool UltimaSpellCombat::HandleMonsterMoveCallback()
{
	if (m_misc->m_callbackStack.size() > 0)
	{
		m_misc->m_callbackStack.pop();
	}

	int rngNum;
	m_gChnum = FigureNewMonPosition(m_mon);
	m_misc->m_zp[0x1F] = 0x7A;
	if (m_misc->m_zp[0xD0] == 0)
	{
		afternext();
		return false;
	}
	if (m_gChnum >= 0)
	{
		rngNum = m_utilities->getRandom(0, 255);
		if (rngNum < 128)
		{
			if (m_misc->m_gMonType == 0x3A)
			{
				monshoot();
				return false;
			}
		}
	}
	rngNum = m_utilities->getRandom(0, 0xC0);
	switch (m_misc->m_gMonType)
	{
	case 0x1A:
	case 0x1C:
	case 0x2C:
	case 0x36:
	case 0x3A:
	case 0x3C:
		monmagic();
		return false;
	case 0x26:
		monlb();
		return false;
	default:
		break;
	}
	nextplr();

	return false;
}

void UltimaSpellCombat::nextplr()
{
	if (m_misc->m_zp[0xD0] >= 0 && m_misc->m_zp[0xD0] < 0x80)
	{
		monlb();
	}
}

void UltimaSpellCombat::monmagic() // $864A
{
	int rngNum = m_utilities->getRandom(0, 255);
	m_gChnum = rngNum &= 3;

	if (m_misc->m_Player[m_misc->m_Party[6 + m_gChnum]][17] != 'G')
	{
		nextplr();
		return;
	}
	m_misc->m_callbackStack.push(std::bind(&UltimaSpellCombat::monmagicCallback, this));
	m_misc->InverseTiles(true);
	m_audio->playSfx(SFX_MONSTERSPELL);
}

bool UltimaSpellCombat::monmagicCallback()
{
	if (m_misc->m_callbackStack.size() > 0)
	{
		m_misc->m_callbackStack.pop();
	}
	m_misc->m_zp[0x1F] = 0x78;
	afternext();
	return false;
}

void UltimaSpellCombat::monlb() // $8672
{
	m_misc->PutXYTile(m_misc->m_MonsterTile[m_mon], m_misc->m_MonsterX[m_mon], m_misc->m_MonsterY[m_mon]);
	m_misc->m_MonsterX[m_mon] = (unsigned char)m_misc->m_zp[0xF7];
	m_misc->m_MonsterY[m_mon] = (unsigned char)m_misc->m_zp[0xF8];
	m_misc->m_MonsterTile[m_mon] = m_misc->GetXYTile(m_misc->m_MonsterX[m_mon], m_misc->m_MonsterY[m_mon]);
	unsigned char tileValue = (unsigned char)m_misc->m_gMonType;
	if (m_misc->m_gMonVarType && m_misc->m_gMonType >= 46 && m_misc->m_gMonType <= 63)
	{
		tileValue = (unsigned char)((((m_misc->m_gMonType / 2) - 23) * 2 + 79 + m_misc->m_gMonVarType) * 2);
	}
	m_misc->PutXYTile(tileValue, m_misc->m_MonsterX[m_mon], m_misc->m_MonsterY[m_mon]);
}

void UltimaSpellCombat::monshoot2() // $86B5
{
	m_misc->m_xs += m_misc->m_zp[0xF5];
	if (m_misc->m_xs > 10 || m_misc->m_xs < 0)
	{
		m_misc->DelayGame(80);
		return;
	}
	m_misc->m_ys += m_misc->m_zp[0xF6];
	if (m_misc->m_ys > 10 || m_misc->m_ys < 0)
	{
		m_misc->DelayGame(80);
		return;
	}
	for (short chnum = m_misc->m_Party[1] - 1; chnum >= 0; chnum--)
	{
		if (m_misc->m_xs == m_misc->m_CharX[chnum] && m_misc->m_ys == m_misc->m_CharY[chnum])
		{
			m_gChnum = chnum;
			c8777();
			return;
		}
	}
	m_misc->m_opnum = m_misc->GetXYTile(m_misc->m_xs, m_misc->m_ys);
	m_misc->m_gBallTileBackground = (unsigned char)m_misc->m_opnum;
	m_misc->PutXYTile(0x7A, m_misc->m_xs, m_misc->m_ys);
	m_misc->DelayGame(200, std::bind(&UltimaSpellCombat::monshootCallback, this));
}

void UltimaSpellCombat::monshoot() const // $86A4
{
	m_audio->playSfx(SFX_SHOOT);
	m_misc->m_xs = m_misc->m_MonsterX[m_mon];
	m_misc->m_ys = m_misc->m_MonsterY[m_mon];
}

bool UltimaSpellCombat::monshootCallback()
{
	if (m_misc->m_callbackStack.size() > 0)
	{
		m_misc->m_callbackStack.pop();
	}

	m_misc->PutXYTile(m_misc->m_opnum, m_misc->m_xs, m_misc->m_ys);
	monshoot2();
	return false;
}

void UltimaSpellCombat::afternext()
{
	if (m_misc->m_gMonType == 0x1C || m_misc->m_gMonType == 0x3C || m_misc->m_gMonType == 0x38)
	{
		Poison(m_gChnum);
	}
	else
	{
		if (m_misc->m_gMonType == 0x2E)
		{
			Pilfer(m_gChnum);
		}
	}
	m_scrollArea->UPrintMessage(141);
	std::string dispString = std::to_string(m_gChnum + 1);
	m_scrollArea->UPrintWin(dispString);
	m_misc->m_wx++;
	m_audio->playSfx(SFX_ATTACK);

	// If in Exodus Castle and the character is not wearing Exotic, it's an automatic hit.
	if (m_g835E == 3 && m_misc->m_Party[3] == m_misc->m_LocationX[1] && m_misc->m_Player[m_misc->m_Party[6 + m_gChnum]][40] != 7)
	{
		plrhit();
		return;
	}
	// Random from 0 to armour+16 -- less than 8 is a hit.
	int rngNum = m_utilities->getRandom(0, m_misc->m_Player[m_misc->m_Party[6 + m_gChnum]][40] + 0x10);
	if (rngNum < 8)
	{
		plrhit();
		return;
	}
	m_scrollArea->UPrintMessage(142);    // Missed
}

void UltimaSpellCombat::plrhit() // $876D
{
	m_scrollArea->UPrintMessage(143);    // Hit
	c8777();
}

void UltimaSpellCombat::c8777()
{
	unsigned char monhpstart[16] = { 0x20, 0x20, 0xF0, 0xF0, 0xC0, 0x60, 0xA0, 0x80, 0x30, 0x50, 0x70, 0xA0, 0xC0, 0xE0, 0xF0, 0xF0 };

	short temp;

	temp = ((m_misc->m_Player[m_misc->m_Party[6 + m_gChnum]][28] * 256) + m_misc->m_Player[m_misc->m_Party[6 + m_gChnum]][29]) / 100;
	temp = ((monhpstart[(m_misc->m_gMonType / 2) & 0x0F] / 8) + temp) | 1;
	int rngNum = m_utilities->getRandom(0, temp) + 1;
	temp = (short)rngNum;
	m_misc->HPSubtract(m_misc->m_Party[6 + m_gChnum], temp);
	m_misc->HPSubtract(m_misc->m_Party[6 + m_gChnum], (m_g835E & 3) * 16);

	m_misc->m_callbackStack.push(std::bind(&UltimaSpellCombat::c8777Callback, this));

	m_resources->m_inverses.inverseTileTime = m_misc->damage_time;
	m_resources->m_inverses.elapsedTileTime = 0;
	m_resources->setInversed(true);
	m_misc->InverseCharDetails(m_gChnum, true);
	m_misc->AddInverse();
	m_misc->m_gBallTileBackground = m_misc->m_CharTile[m_gChnum];
	m_misc->PutXYTile(m_misc->m_zp[0x1F], m_misc->m_CharX[m_gChnum], m_misc->m_CharY[m_gChnum]);
	m_audio->playSfx(SFX_HIT);
	ShowHit(m_misc->m_CharX[m_gChnum], m_misc->m_CharY[m_gChnum], 0x7A, m_misc->m_CharTile[m_gChnum]);
}

bool UltimaSpellCombat::c8777Callback()
{
	if (m_misc->m_callbackStack.size() > 0)
	{
		m_misc->m_callbackStack.pop();
	}

	m_misc->PutXYTile(m_misc->m_CharShape[m_gChnum], m_misc->m_CharX[m_gChnum], m_misc->m_CharY[m_gChnum]);
	if (m_misc->m_Player[m_misc->m_Party[6 + m_gChnum]][17] == 'D')
	{
		m_scrollArea->UPrintMessage(144);
		m_misc->PutXYTile(m_misc->m_CharTile[m_gChnum], m_misc->m_CharX[m_gChnum], m_misc->m_CharY[m_gChnum]);
		m_misc->m_CharX[m_gChnum] = 0xFF;
		m_misc->m_CharY[m_gChnum] = 0xFF;
		m_misc->CheckAllDead();
		if (!m_misc->m_checkDead)
		{
			return false;
		}
	}

	m_misc->m_wx = 0x18;
	m_misc->m_wy = 0x17;

	return false;
}

short UltimaSpellCombat::FigureNewMonPosition(short mon) // $7E85
{
	short newx;
	short newy;
	short health;
	short rosNum;
	m_misc->m_zp[0xD0] = 0xFF;
	short plrTarget = 0xFF;
	short plrNum = -1;
 
	for (plrNum = 0; plrNum < m_misc->m_Party[1]; ++plrNum)
	{
		rosNum = m_misc->m_Party[6 + plrNum];
		health = m_misc->m_Player[rosNum][17];
		if (health == 'D' || health == 'A')
		{
			continue;
		}
		m_misc->m_dx = m_misc->m_CharX[plrNum] - m_misc->m_MonsterX[mon];
		m_misc->m_zp[0xD1] = m_utilities->Absolute(m_misc->m_dx);
		m_misc->m_dy = m_misc->m_CharY[plrNum] - m_misc->m_MonsterY[mon];
		m_misc->m_zp[0xD2] = m_utilities->Absolute(m_misc->m_dy);

		if ((m_misc->m_zp[0xD2] < 2) && (m_misc->m_zp[0xD1] < 2))
		{
			// $7F44
			m_misc->m_zp[0xD0] = 0;
			plrTarget = plrNum;
			if (m_misc->m_zp[0xD1] + m_misc->m_zp[0xD2] < 2)
			{
				break;
			}
			continue;
		}
		if ((m_misc->m_zp[0xD1] + m_misc->m_zp[0xD2]) >= m_misc->m_zp[0xD0])
		{
			continue;
		}
		m_misc->m_zp[0xD1] += m_misc->m_zp[0xD2];
		m_misc->m_dx = m_misc->GetHeading(m_misc->m_dx);
		newx = m_misc->m_MonsterX[mon] + m_misc->m_dx;
		m_misc->m_dy = m_misc->GetHeading(m_misc->m_dy);
		newy = m_misc->m_MonsterY[mon] + m_misc->m_dy;

		if (CombatValidMove(m_misc->GetXYTile(newx, newy)) != 0)
		{
			newx = m_misc->m_MonsterX[mon];
			newy = m_misc->m_MonsterY[mon] + m_misc->m_dy;
			if (CombatValidMove(m_misc->GetXYTile(newx, newy)) != 0)
			{
				newx = m_misc->m_MonsterX[mon] + m_misc->m_dx;
				newy = m_misc->m_MonsterY[mon];
				if (CombatValidMove(m_misc->GetXYTile(newx, newy)) != 0)
				{
					continue;
				}
			}
		}
		plrTarget = plrNum;
		m_misc->m_zp[0xD0] = m_misc->m_zp[0xD1];
		m_misc->m_zp[0xF7] = newx;
		m_misc->m_zp[0xF8] = newy;
		m_misc->m_zp[0xF5] = m_misc->m_dx;
		m_misc->m_zp[0xF6] = m_misc->m_dy;
	}
	plrNum = plrTarget;
	return plrNum;
}

unsigned char UltimaSpellCombat::CombatValidMove(short value) // $7E31
{
	if (m_misc->m_gMonType < 0x16 || m_misc->m_gMonType >= 0x20)
	{
		switch (value)
		{
		case 2:
		case 4:
		case 6:
		case 0x10:
			return 0;
		default:
			return 0xFF;
		}
	}
	else
	{
		if (value == 0)
		{
			return 0;
		}
		return 0xFF;
	}
}

bool UltimaSpellCombat::CombatAttack() // $8360
{
	if (m_misc->m_callbackStack.size() > 0)
	{
		m_misc->m_callbackStack.pop();
	}

	m_misc->m_zp[0x1F] = 0x7A; // Red ball
	m_misc->m_rosNum = m_misc->m_Party[6 + m_activePlayer];
	m_chNum = m_activePlayer;
	m_wpn = m_misc->m_Player[m_misc->m_rosNum][48];

	std::string str1 = m_resources->m_plistMap["WeaponsArmour"][m_wpn];
	std::string str2 = m_resources->m_plistMap["Messages"][144];

	str1 += str2;
	m_scrollArea->UPrintWin(str1);
	m_misc->m_inputType = InputType::GetDirection;
	m_misc->m_callbackStack.push(std::bind(&UltimaSpellCombat::CombatAttackCallback, this));
	m_misc->AddProcessEvent();

	return false;
}

bool UltimaSpellCombat::CombatAttackCallback()
{
	if (m_misc->m_callbackStack.size() > 0)
	{
		m_misc->m_callbackStack.pop();
	}
	if (m_misc->m_input_num < 0)
	{
		return false;
	}
	if (m_chNum == 0)
	{
		m_audio->playSfx(SFX_SWISH1);
	}
	else if (m_chNum == 1)
	{
		m_audio->playSfx(SFX_SWISH2);
	}
	else if (m_chNum == 2)
	{
		m_audio->playSfx(SFX_SWISH3);
	}
	else
	{
		m_audio->playSfx(SFX_SWISH4);
	}
	if ((m_wpn == 3 || m_wpn == 5 || m_wpn == 9 || m_wpn == 13))
	{
		m_shootX = m_misc->m_CharX[m_activePlayer];
		m_shootY = m_misc->m_CharY[m_activePlayer];
		m_misc->m_callbackStack.push(std::bind(&UltimaSpellCombat::ShootCallback, this));
		m_misc->m_callbackStack.push(std::bind(&UltimaSpellCombat::Shoot, this));
		return false;
	}
	else
	{
		m_mon = CombatMonsterHere(m_misc->m_CharX[m_activePlayer] + m_misc->m_dx, m_misc->m_CharY[m_activePlayer] + m_misc->m_dy);
		if (m_mon > 127 && m_mon != 1)
		{
			Missed();
			return false;
		}
		if (m_mon > 127 && m_wpn == 1)   // Thrown a dagger
		{
			m_misc->m_Player[m_misc->m_rosNum][49]--;
			if (m_misc->m_Player[m_misc->m_rosNum][49] < 1 || m_misc->m_Player[m_misc->m_rosNum][49] > 250)
			{
				m_misc->m_Player[m_misc->m_rosNum][48] = 0;
				m_misc->m_Player[m_misc->m_rosNum][49] = 0;
			}
			m_shootX = m_misc->m_CharX[m_activePlayer];
			m_shootY = m_misc->m_CharY[m_activePlayer];
			m_misc->m_callbackStack.push(std::bind(&UltimaSpellCombat::ShootCallback, this));
			m_misc->m_callbackStack.push(std::bind(&UltimaSpellCombat::Shoot, this));
			return false;
		}
	}
	m_misc->m_callbackStack.push(std::bind(&UltimaSpellCombat::CombatAttackCallback1, this));
	return false;
}

bool UltimaSpellCombat::CombatAttackCallback1()
{
	if (m_misc->m_callbackStack.size() > 0)
	{
		m_misc->m_callbackStack.pop();
	}
	if (ExodusCastle() == 0 && m_wpn != 15)
	{
		Missed();
		return false;
	}
	int rngNum = m_utilities->getRandom(0, 255);
	if (rngNum < 128)
	{
		Missed();
		return false;
	}
	PrintMonster(m_misc->m_gMonType, false, m_misc->m_gMonVarType);
	m_scrollArea->UPrintMessage(146);
	m_misc->m_gBallTileBackground = m_misc->m_MonsterTile[m_mon];
	m_misc->PutXYTile(m_misc->m_zp[0x1F], m_misc->m_MonsterX[m_mon], m_misc->m_MonsterY[m_mon]);
	m_misc->DelayGame(200, std::bind(&UltimaSpellCombat::CombatAttackCallback2, this));
	return false;
}

bool UltimaSpellCombat::CombatAttackCallback2()
{
	if (m_misc->m_callbackStack.size() > 0)
	{
		m_misc->m_callbackStack.pop();
	}
	m_audio->playSfx(SFX_HIT);
	m_misc->m_callbackStack.push(std::bind(&UltimaSpellCombat::CombatAttackCallback3, this));
	ShowHit(m_misc->m_MonsterX[m_mon], m_misc->m_MonsterY[m_mon], 0x7A, m_misc->m_MonsterTile[m_mon]);

	return false;
}

bool UltimaSpellCombat::CombatAttackCallback3()
{
	if (m_misc->m_callbackStack.size() > 0)
	{
		m_misc->m_callbackStack.pop();
	}
	short damage;
	m_misc->PutXYTile(m_misc->m_gMonType, m_misc->m_MonsterX[m_mon], m_misc->m_MonsterY[m_mon]);

	damage = (short)m_utilities->getRandom(0, m_misc->m_Player[m_misc->m_rosNum][18] | 1);
	damage += m_misc->m_Player[m_misc->m_rosNum][18] / 2;
	damage += m_misc->m_Player[m_misc->m_rosNum][48] * 3;
	damage += 4;
	DamageMonster(m_mon, damage, m_activePlayer);
	return false;
}

bool UltimaSpellCombat::Shoot() // $7D41
{
	if (m_misc->m_callbackStack.size() > 0)
	{
		m_misc->m_callbackStack.pop();
	}
	m_shootX += m_misc->m_dx;
	m_shootY += m_misc->m_dy;
	if (m_shootX < 0 || m_shootX > 10 || m_shootY < 0 || m_shootY > 10)
	{
		m_shootRet = 255;
		return false;
	}

	m_misc->m_opnum = CombatMonsterHere(m_shootX, m_shootY);
	unsigned char character = CombatCharacterHere(m_shootX, m_shootY);
	m_origValue = m_misc->GetXYTile(m_shootX, m_shootY);
	m_misc->m_gBallTileBackground = m_origValue;
	if (m_misc->m_opnum != 255)
	{
		m_misc->m_gBallTileBackground = m_misc->m_MonsterTile[m_misc->m_opnum];
	}
	else if (character != 255)
	{
		m_misc->m_gBallTileBackground = m_misc->m_CharTile[character];
	}
	m_misc->PutXYTile(m_misc->m_zp[0x1F], m_shootX, m_shootY);
	m_misc->DelayGame(80, std::bind(&UltimaSpellCombat::ShootCallback1, this));

	return false;
}

bool UltimaSpellCombat::ShootCallback1()
{
	if (m_misc->m_callbackStack.size() > 0)
	{
		m_misc->m_callbackStack.pop();
	}
	m_misc->PutXYTile(m_origValue, m_shootX, m_shootY);
	m_origValue = (unsigned char)m_misc->m_opnum;
	if (m_origValue > 127)
	{
		m_misc->m_callbackStack.push(std::bind(&UltimaSpellCombat::Shoot, this));
		return false;
	}
	m_shootRet = m_origValue;

	return false;
}

bool UltimaSpellCombat::ShootCallback()
{
	if (m_misc->m_callbackStack.size() > 0)
	{
		m_misc->m_callbackStack.pop();
	}
	m_mon = m_shootRet;
	if (m_shootRet > 127)
	{
		Missed();
		return false;
	}
	m_misc->m_callbackStack.push(std::bind(&UltimaSpellCombat::CombatAttackCallback1, this));
	return false;
}

void UltimaSpellCombat::updateGameTime(Uint64 deltaTime)
{
	if (m_monster_turn)
	{
		return;
	}
	m_elapsedFlashTime += deltaTime;
	if (m_elapsedFlashTime > CombatBlink)
	{
		m_elapsedFlashTime %= deltaTime;
		m_cHide = !m_cHide;
	}
}

void UltimaSpellCombat::Projectile(short chnum, short damage) // $552B
{
	if (m_misc->m_Party[2] != 0x80)
	{
		Failed();
		return;
	}
	m_chNum = chnum;
	m_damage = damage;
	m_scrollArea->UPrintMessage(85);
	m_misc->m_inputType = InputType::GetDirection;
	m_misc->m_callbackStack.push(std::bind(&UltimaSpellCombat::ProjectileCallback, this));
	m_misc->AddProcessEvent();
}

bool UltimaSpellCombat::ProjectileCallback()
{
	if (m_misc->m_callbackStack.size() > 0)
	{
		m_misc->m_callbackStack.pop();
	}

	m_shootX = m_misc->m_CharX[m_chNum];
	m_shootY = m_misc->m_CharY[m_chNum];

	m_misc->m_callbackStack.push(std::bind(&UltimaSpellCombat::ProjectileCallback1, this));
	m_misc->m_callbackStack.push(std::bind(&UltimaSpellCombat::Shoot, this));
	Flashriek();

	return false;
}

bool UltimaSpellCombat::ProjectileCallback1()
{
	if (m_misc->m_callbackStack.size() > 0)
	{
		m_misc->m_callbackStack.pop();
	}
	m_hit = m_shootRet;
	if (m_hit > 127)
	{
		Failed();
		return false;
	}
	m_misc->m_callbackStack.push(std::bind(&UltimaSpellCombat::ProjectileCallback2, this));
	ShowHit(m_misc->m_MonsterX[m_hit & 7], m_misc->m_MonsterY[m_hit & 7], 0x78, m_misc->m_MonsterTile[m_hit & 8]);
	m_audio->playSfx(SFX_HIT);
	return false;
}

bool UltimaSpellCombat::ProjectileCallback2()
{
	if (m_misc->m_callbackStack.size() > 0)
	{
		m_misc->m_callbackStack.pop();
	}
	DamageMonster(m_hit, m_damage, m_chNum);

	return false;
}

void UltimaSpellCombat::RelocateDungeon() // $572B
{
	short value;
	value = -1;
	while (value != 0)
	{
		m_misc->m_xs = (short)m_utilities->getRandom(0, 16);
		m_misc->m_ys = (short)m_utilities->getRandom(0, 16);
		value = GetXYDng(m_misc->m_xs, m_misc->m_ys);
	}
	m_misc->m_xpos = m_misc->m_xs;
	m_misc->m_ypos = m_misc->m_ys;
}

void UltimaSpellCombat::DownLevel()
{
	if (m_misc->m_Party[2] != 1)
	{
		Failed();
		return;
	}
	m_misc->m_callbackStack.push(std::bind(&UltimaSpellCombat::DownLevelCallback, this));
	Flashriek();
}


bool UltimaSpellCombat::DownLevelCallback()
{
	if (m_misc->m_callbackStack.size() > 0)
	{
		m_misc->m_callbackStack.pop();
	}
	if (m_dungeon->m_dungeonLevel > 6)
	{
		Failed();
		return false;
	}
	m_dungeon->m_dungeonLevel++;
	RelocateDungeon();
	return false;
}

void UltimaSpellCombat::UpLevel()
{
	if (m_misc->m_Party[2] != 1)
	{
		Failed();
		return;
	}
	m_misc->m_callbackStack.push(std::bind(&UltimaSpellCombat::UpLevelCallback, this));
	Flashriek();
}

bool UltimaSpellCombat::UpLevelCallback()
{
	if (m_misc->m_callbackStack.size() > 0)
	{
		m_misc->m_callbackStack.pop();
	}
	m_dungeon->m_dungeonLevel--;
	if (m_dungeon->m_dungeonLevel < 0)
	{
		m_dungeon->m_dungeonLevel = 0;
		m_dungeon->m_gExitDungeon = true;

		return false;
	}
	RelocateDungeon();
	return false;
}

bool UltimaSpellCombat::DagAcronCallback()
{
	if (m_misc->m_callbackStack.size() > 0)
	{
		m_misc->m_callbackStack.pop();
	}
	if ((m_misc->m_Party[2] != 0) || (m_misc->m_Party[0] == 0x16 && m_misc->m_Party[15] == 0))
	{
		Failed();
		return false;
	}
	int matchValue = (m_misc->m_Party[0] == 0x16) ? 0 : 4;
	short value = -1;
	while (value != matchValue)
	{
		short rngNum = (short)m_utilities->getRandom(0, m_misc->m_mapSize - 1);
		m_misc->m_xs = rngNum;
		rngNum = (short)m_utilities->getRandom(0, m_misc->m_mapSize - 1);
		m_misc->m_ys = rngNum;
		value = m_misc->GetXYVal(m_misc->m_xs, m_misc->m_ys);
	}
	m_misc->m_xpos = m_misc->m_xs;
	m_misc->m_ypos = m_misc->m_ys;
	return false;
}

bool UltimaSpellCombat::FalDiviCallback()
{
	if (m_misc->m_callbackStack.size() > 0)
	{
		m_misc->m_callbackStack.pop();
	}
	m_misc->callClerickChoose();
	return false;
}

void UltimaSpellCombat::Necorp() // $5677
{
	if (m_misc->m_Party[2] != 0x80)
	{
		Failed();
		return;
	}
	m_misc->m_opnum = 7;
	for (short mon = 7; mon >= 0; mon--)
	{
		m_misc->m_callbackStack.push(std::bind(&UltimaSpellCombat::NecorpCallback1, this));
		m_misc->m_callbackStack.push(std::bind(&UltimaSpellCombat::NecorpCallback, this));
	}
	Flashriek();
}

bool UltimaSpellCombat::NecorpCallback1()
{
	if (m_misc->m_callbackStack.size() > 0)
	{
		m_misc->m_callbackStack.pop();
	}

	m_misc->m_opnum--;

	return false;
}

bool UltimaSpellCombat::NecorpCallback2()
{
	if (m_misc->m_callbackStack.size() > 0)
	{
		m_misc->m_callbackStack.pop();
	}
	// Maybe later create a queue, but right now, we can only have one delay in
	// the queue at a time
	m_misc->DelayGame(200, std::bind(&UltimaSpellCombat::NecorpCallback3, this));

	return false;
}

bool UltimaSpellCombat::NecorpCallback3()
{
	if (m_misc->m_callbackStack.size() > 0)
	{
		m_misc->m_callbackStack.pop();
	}

	short mon = m_misc->m_opnum;
	m_misc->PutXYTile(m_misc->m_opnum, m_misc->m_MonsterX[mon], m_misc->m_MonsterY[mon]);

	return false;
}


bool UltimaSpellCombat::NecorpCallback()
{
	if (m_misc->m_callbackStack.size() > 0)
	{
		m_misc->m_callbackStack.pop();
	}

	short mon = m_misc->m_opnum;

	if (m_misc->m_MonsterHP[mon] > 0)
	{
		m_misc->m_MonsterHP[mon] = 5;
		m_misc->m_opnum2 = m_misc->GetXYTile(m_misc->m_MonsterX[mon], m_misc->m_MonsterY[mon]);
		m_misc->PutXYTile(0x78, m_misc->m_MonsterX[mon], m_misc->m_MonsterY[mon]);
		m_misc->m_callbackStack.push(std::bind(&UltimaSpellCombat::NecorpCallback2, this));
		m_audio->playSfx(SFX_BUMP);
		ShowHit(m_misc->m_MonsterX[mon], m_misc->m_MonsterY[mon], 0x78, m_misc->m_MonsterTile[mon]);
	}
	
	return false;
}

bool UltimaSpellCombat::ApparUnemCallback()
{
	if (m_misc->m_callbackStack.size() > 0)
	{
		m_misc->m_callbackStack.pop();
	}

	int rngNum = m_utilities->getRandom(0, 255);
	if (0 == (rngNum & 03))
	{
		Failed();
		return false;
	}
	m_misc->m_m5BDC = false;
	m_misc->GetChest(1, m_chNum);

	return false;
}

void UltimaSpellCombat::Alcort()
{
	m_scrollArea->UPrintMessage(130);
	m_misc->m_inputType = InputType::Transact;
	m_misc->m_callbackStack.push(std::bind(&UltimaSpellCombat::AlcortCallback, this));
	m_misc->AddProcessEvent();
}

bool UltimaSpellCombat::AlcortCallback()
{
	if (m_misc->m_callbackStack.size() > 0)
	{
		m_misc->m_callbackStack.pop();
	}
	if (m_misc->m_input_num < 0 || m_misc->m_input_num > 3)
	{
		m_scrollArea->UPrintWin("\n");
		Failed();
		return false;
	}
	if (m_misc->m_Party[6 + m_misc->m_input_num] == 0)
	{
		m_scrollArea->UPrintWin("\n");
		Failed();
		return false;
	}
	std::string dispString(std::to_string(m_misc->m_input_num + 1) + std::string("\n"));
	m_scrollArea->UPrintWin(dispString);
	m_misc->m_callbackStack.push(std::bind(&UltimaSpellCombat::AlcortCallback1, this));
	m_misc->InverseCharDetails(m_misc->m_input_num, true);
	Flashriek();

	return false;
}

bool UltimaSpellCombat::AlcortCallback1()
{
	if (m_misc->m_callbackStack.size() > 0)
	{
		m_misc->m_callbackStack.pop();
	}

	if (m_misc->m_Player[m_misc->m_Party[6 + m_misc->m_input_num]][17] != 'P')
	{
		Failed();
		return false;
	}
	m_misc->m_Player[m_misc->m_Party[6 + m_misc->m_input_num]][17] = 'G';

	return false;
}

void UltimaSpellCombat::Heal(short damage) // $56F7
{
	m_scrollArea->UPrintMessage(130);

	m_misc->m_inputType = InputType::Transact;
	m_misc->m_callbackStack.push(std::bind(&UltimaSpellCombat::HealCallback, this));
	m_misc->AddProcessEvent();
	m_damage = damage;
}

bool UltimaSpellCombat::HealCallback()
{
	if (m_misc->m_callbackStack.size() > 0)
	{
		m_misc->m_callbackStack.pop();
	}

	if (m_misc->m_input_num < 0 || m_misc->m_input_num > 3)
	{
		m_scrollArea->UPrintWin("\n");
		Failed();
		return false;
	}
	if (m_misc->m_Party[6 + m_misc->m_input_num] == 0)
	{
		m_scrollArea->UPrintWin("\n");
		Failed();
		return false;
	}
	std::string dispString(std::to_string(m_misc->m_input_num + 1) + std::string("\n"));
	m_scrollArea->UPrintWin(dispString);
	
	m_misc->m_callbackStack.push(std::bind(&UltimaSpellCombat::HealCallback1, this));
	m_misc->InverseCharDetails(m_misc->m_input_num, true);
	Flashriek();

	return false;
}

bool UltimaSpellCombat::HealCallback1() const
{
	if (m_misc->m_callbackStack.size() > 0)
	{
		m_misc->m_callbackStack.pop();
	}

	m_misc->HPAdd(m_misc->m_Party[6 + m_misc->m_input_num], m_damage);

	return false;
}

bool UltimaSpellCombat::LibRekCallback()
{
	if (m_misc->m_callbackStack.size() > 0)
	{
		m_misc->m_callbackStack.pop();
	}
	if (m_misc->m_Party[2] != 1)
	{
		Failed();
		return false;
	}
	RelocateDungeon();

	return false;
}

bool UltimaSpellCombat::SequituCallback()
{
	if (m_misc->m_callbackStack.size() > 0)
	{
		m_misc->m_callbackStack.pop();
	}
	m_dungeon->m_dungeonLevel = 0;
	m_dungeon->m_gExitDungeon = 1;

	return false;
}

void UltimaSpellCombat::Surmandum()
{
	m_scrollArea->UPrintMessage(128);
	m_misc->m_inputType = InputType::Transact;
	m_misc->m_callbackStack.push(std::bind(&UltimaSpellCombat::SurmandumCallback, this));
	m_misc->AddProcessEvent();
}

bool UltimaSpellCombat::SurmandumCallback()
{
	if (m_misc->m_callbackStack.size() > 0)
	{
		m_misc->m_callbackStack.pop();
	}

	if (m_misc->m_input_num < 0 || m_misc->m_input_num > 3)
	{
		m_scrollArea->UPrintWin("\n");
		Failed();
		return false;
	}
	if (m_misc->m_Party[6 + m_misc->m_input_num] == 0)
	{
		m_scrollArea->UPrintWin("\n");
		Failed();
		return false;
	}
	std::string dispString(std::to_string(m_misc->m_input_num + 1) + std::string("\n"));
	m_scrollArea->UPrintWin(dispString);

	m_misc->m_callbackStack.push(std::bind(&UltimaSpellCombat::SurmandumCallback1, this));
	m_misc->InverseCharDetails(m_misc->m_input_num, true);
	Flashriek();

	return false;
}

bool UltimaSpellCombat::SurmandumCallback1()
{
	if (m_misc->m_callbackStack.size() > 0)
	{
		m_misc->m_callbackStack.pop();
	}

	if (m_misc->m_Player[m_misc->m_Party[6 + m_misc->m_input_num]][17] != 'D')
	{
		Failed();
		return false;
	}
	int rngNum = m_utilities->getRandom(0, 255) & 0x03;
	if (rngNum == 0)
	{
		m_misc->m_Player[m_misc->m_Party[6 + m_misc->m_input_num]][17] = 'A';
		Failed();
		return false;
	}
	m_misc->m_Player[m_misc->m_Party[6 + m_misc->m_input_num]][17] = 'G';

	return false;
}

void UltimaSpellCombat::AnjuSermani()
{
	m_scrollArea->UPrintMessage(129);
	m_misc->m_inputType = InputType::Transact;
	m_misc->m_callbackStack.push(std::bind(&UltimaSpellCombat::AnjuSermaniCallback, this));
	m_misc->AddProcessEvent();
}

bool UltimaSpellCombat::AnjuSermaniCallback()
{
	if (m_misc->m_callbackStack.size() > 0)
	{
		m_misc->m_callbackStack.pop();
	}

	if (m_misc->m_input_num < 0 || m_misc->m_input_num > 3)
	{
		m_scrollArea->UPrintWin("\n");
		Failed();
		return false;
	}
	if (m_misc->m_Party[6 + m_misc->m_input_num] == 0)
	{
		m_scrollArea->UPrintWin("\n");
		Failed();
		return false;
	}
	std::string dispString(std::to_string(m_misc->m_input_num + 1) + std::string("\n"));
	m_scrollArea->UPrintWin(dispString);

	m_misc->m_callbackStack.push(std::bind(&UltimaSpellCombat::AnjuSermaniCallback1, this));
	m_misc->InverseCharDetails(m_misc->m_input_num, true);
	Flashriek();

	return false;
}

bool UltimaSpellCombat::AnjuSermaniCallback1()
{
	if (m_misc->m_callbackStack.size() > 0)
	{
		m_misc->m_callbackStack.pop();
	}

	if (m_misc->m_Player[m_misc->m_Party[6 + m_misc->m_input_num]][17] != 'A')
	{
		Failed();
		return false;
	}

	m_misc->m_Player[m_misc->m_Party[6 + m_misc->m_input_num]][17] = 'G';
	if (m_misc->m_Player[m_misc->m_Party[6 + m_chNum]][21] > 5)
	{
		m_misc->m_Player[m_misc->m_Party[6 + m_chNum]][21] -= 5; // Caster's wisdom-5
	}
	if (m_misc->m_Player[m_misc->m_Party[6 + m_chNum]][21] < 5)
	{
		m_misc->m_Player[m_misc->m_Party[6 + m_chNum]][21] = 5;
	}

	return false;
}

void UltimaSpellCombat::Armageddon()
{
	m_misc->m_callbackStack.push(std::bind(&UltimaSpellCombat::ArmageddonCallback, this));
	Flashriek();
}

bool UltimaSpellCombat::ArmageddonCallback()
{
	if (m_misc->m_callbackStack.size() > 0)
	{
		m_misc->m_callbackStack.pop();
	}
	for (short mon = 0; mon < 32; mon++)
	{
		if (m_misc->m_Monsters[mon] != 0 && m_misc->m_Monsters[mon] != 0x4C) // Can't kill Lord British
		{
			// put a chest where the monster was
			short tileon = m_misc->m_Monsters[mon + TILEON];
			if (tileon != 0)
			{
				tileon = ((tileon / 4) & 0x03) + 0x24;
			}
			m_misc->PutXYVal((unsigned char)tileon, m_misc->m_Monsters[mon + XMON], m_misc->m_Monsters[mon + YMON]);
			m_misc->m_Monsters[mon] = 0;
		}
		else
		{
			m_misc->m_Monsters[mon + HPMON] = 0xC0;
		}
	}

	return false;
}

void UltimaSpellCombat::Terramorph()
{
	m_scrollArea->UPrintMessage(85);
	m_misc->m_inputType = InputType::GetDirection;
	m_misc->m_callbackStack.push(std::bind(&UltimaSpellCombat::TerramorphCallback, this));
	m_misc->AddProcessEvent();
}

bool UltimaSpellCombat::TerramorphCallback()
{
	if (m_misc->m_callbackStack.size() > 0)
	{
		m_misc->m_callbackStack.pop();
	}
	std::string_view temp = TypeNumStr;
	std::string dispString = std::string(temp);
	m_scrollArea->UPrintWin(dispString);
	m_misc->setInputTypeNum(std::bind(&UltimaSpellCombat::TerramorphCallback2, this));
	return false;
}

bool UltimaSpellCombat::TerramorphCallback2()
{
	if (m_misc->m_callbackStack.size() > 0)
	{
		m_misc->m_callbackStack.pop();
	}
	m_misc->m_callbackStack.push(std::bind(&UltimaSpellCombat::TerramorphCallback3, this));
	Flashriek();
	m_scrollArea->UPrintWin("\n");
	return false;
}

bool UltimaSpellCombat::TerramorphCallback3()
{
	if (m_misc->m_callbackStack.size() > 0)
	{
		m_misc->m_callbackStack.pop();
	}

	unsigned char value = (unsigned char)(m_misc->m_input_num % 64);
	short mon;

	if (value > 12 && value < 31)   // a creature
	{
		short openSpot = -1;
		for (mon = 0; mon < 32; mon++)
		{
			if (m_misc->m_Monsters[mon] == 0)
			{
				openSpot = mon;
			}
		}
		if (openSpot == -1)
		{
			m_misc->PutXYVal(value * 4, (unsigned char)(m_misc->m_xpos + m_misc->m_dx), (unsigned char)(m_misc->m_ypos + m_misc->m_dy));
		}
		else
		{
			m_misc->m_Monsters[openSpot] = value * 4;
			m_misc->m_Monsters[openSpot + TILEON] = m_misc->GetXYVal((unsigned char)(m_misc->m_xpos + m_misc->m_dx), (unsigned char)(m_misc->m_ypos + m_misc->m_dy));
			m_misc->m_Monsters[openSpot + XMON] = (unsigned char)(m_misc->m_xpos + m_misc->m_dx);
			m_misc->m_Monsters[openSpot + YMON] = (unsigned char)(m_misc->m_ypos + m_misc->m_dy);
			m_misc->m_Monsters[openSpot + HPMON] = 0x40;    // wander
			m_misc->PutXYVal(value * 4, (unsigned char)(m_misc->m_xpos + m_misc->m_dx), (unsigned char)(m_misc->m_ypos + m_misc->m_dy));
		}
	}
	else
	{
		m_misc->PutXYVal(value * 4, (unsigned char)(m_misc->m_xpos + m_misc->m_dx), (unsigned char)(m_misc->m_ypos + m_misc->m_dy));
	}

	return false;
}


void UltimaSpellCombat::Flotellum()
{
	int closestDist = 99;
	int destX = -1, destY = -1;
	int testY;
	for (testY = -5; testY <= 5; testY++)
	{
		int testX;
		for (testX = -5; testX <= 5; testX++)
		{
			if (!(testX == 0 && testY == 0) && m_misc->GetXYVal((short)(m_misc->m_xpos + testX), (short)(m_misc->m_ypos + testY)) == 0)
			{
				int thisDist = m_utilities->Absolute((short)testX) + m_utilities->Absolute((short)testY);
				if (thisDist < closestDist)
				{
					closestDist = thisDist;
					destX = m_misc->m_xpos + testX;
					destY = m_misc->m_ypos + testY;
				}
			}
		}
	}
	if (destX < 0)
	{
		Failed();
		return;
	}
	m_destX = (unsigned char)destX;
	m_destY = (unsigned char)destY;

	m_misc->m_callbackStack.push(std::bind(&UltimaSpellCombat::FlotellumCallback, this));
	Flashriek();
}

bool UltimaSpellCombat::FlotellumCallback() const
{
	if (m_misc->m_callbackStack.size() > 0)
	{
		m_misc->m_callbackStack.pop();
	}
	m_misc->PutXYVal(11 * 4, m_destX, m_destY);
	return false;
}

void UltimaSpellCombat::Spell(short chnum, short spellnum)
{
	short rosNum;
	short rngNum;
	m_chNum = chnum;
	m_spellnum = spellnum;

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
		rngNum = (short)m_utilities->getRandom(0, 255);
		if (rngNum < 128)
		{
			Failed();
			return;
		}
		BigDeath(255, chnum);
		break;
	case 1: // Mittar
		rngNum = (short)m_utilities->getRandom(0, 255) | 0x10;
		Projectile(chnum, rngNum);
		break;
	case 2: // Lorum
		m_misc->m_gTorch = 10;
		Flashriek();
		break;
	case 3: // Dor Acron
		DownLevel();
		break;
	case 4: // Sur Acron
		UpLevel();
		break;
	case 5: // Fulgar
		Projectile(chnum, 75);
		break;
	case 6: // Dag Acron
		m_misc->m_callbackStack.push(std::bind(&UltimaSpellCombat::DagAcronCallback, this));
		Flashriek();
		break;
	case 7: // Mentar
		Projectile(chnum, m_misc->m_Player[rosNum][20]);
		break;
	case 8: // Dag Lorum
		m_misc->m_gTorch = 250;
		Flashriek();
		break;
	case 9: // Fal Divi
		m_misc->m_callbackStack.push(std::bind(&UltimaSpellCombat::FalDiviCallback, this));
		Flashriek();
		break;
	case 10: // Noxum
		BigDeath(75, chnum);
		break;
	case 11: // Decorp
		Projectile(chnum, 255);
		break;
	case 12: // Altair
		m_misc->m_gTimeNegate = 20;
		break;
	case 13: // Dag Mentar
		BigDeath(m_misc->m_Player[rosNum][20] * 2, chnum);
		break;
	case 14: // Necorp
		Necorp();
		break;
	case 15: // nameless
		BigDeath(255, chnum);
		break;
	case 16: // Pontori
		if (m_misc->m_Party[2] != 0x80)
		{
			Failed();
			return;
		}
		if (m_misc->m_gMonType != 0x32)
		{
			Failed();
			return;
		}
		if (m_g56E7 != 0)
		{
			Failed();
			return;
		}
		m_g56E7 = 0xFF;
		rngNum = (short)m_utilities->getRandom(0, 255);
		if (rngNum < 128)
		{
			Failed();
			return;
		}
		BigDeath(255, chnum);
		break;
	case 17: // Appar Unem
		m_misc->m_callbackStack.push(std::bind(&UltimaSpellCombat::ApparUnemCallback, this));
		Flashriek();
		break;
	case 18: // Sanctu
		rngNum = (short)m_utilities->getRandom(0, 20) + 10;
		Heal(rngNum);
		break;
	case 19: // Luminae
		m_misc->m_gTorch = 10;
		Flashriek();
		break;
	case 20: // Rec Su
		UpLevel();
		break;
	case 21: // Rec Du
		DownLevel();
		break;
	case 22: // Lib Rec
		m_misc->m_callbackStack.push(std::bind(&UltimaSpellCombat::LibRekCallback, this));
		Flashriek();
		break;
	case 23: // Alcort
		Alcort();
		break;
	case 24: // Sequitu
		if (m_misc->m_Party[2] != 1)
		{
			Failed();
			return;
		}
		m_misc->m_callbackStack.push(std::bind(&UltimaSpellCombat::SequituCallback, this));
		Flashriek();
		break;
	case 25: // Sominae
		m_misc->m_gTorch = 250;
		Flashriek();
		break;
	case 26: // Sanctu Mani
		rngNum = (short)m_utilities->getRandom(0, 80) + 20;
		Heal(rngNum);
		break;
	case 27: // Vieda
		if (m_misc->m_Party[2] == 0x80)
		{
			// The original Ultima 3 doesn't allow casting this in combat, though the LairWare version does.
			// A little extra logic would have to be put in if keeping directly to that, so let's keep
			// closer to the original.
			Failed();
		}
		else
		{
			if (m_graphics->m_curMode == U3GraphicsMode::Map)
			{
				m_graphics->m_queuedMode = U3GraphicsMode::MiniMap;
				m_graphics->m_menuInit = false;
			}
			else if (m_graphics->m_curMode == U3GraphicsMode::Dungeon)
			{
				m_graphics->m_queuedMode = U3GraphicsMode::MiniMapDungeon;
				m_graphics->m_menuInit = false;
			}
			else
			{
				Failed();
			}
		}
		break;
	case 28: // Excuun
		Projectile(chnum, 255);
		break;
	case 29: // Surmandum
		if (m_misc->m_Party[2] == 0x80)
		{
			Failed();
			return;
		}
		Surmandum();
		break;
	case 30: // ZXKUQYB
		BigDeath(255, chnum);
		break;
	case 31: // Anju Sermani
		if (m_misc->m_Party[2] == 0x80)
		{
			Failed();
			return;
		}
		AnjuSermani();
		break;
	case 32: // Terramorph
		if (m_misc->m_Party[2] == 0x80)
		{
			Failed();
			return;
		}
		Terramorph();
		break;
	case 33: // Armageddon
		if (m_misc->m_Party[2] == 0x80)
		{
			Failed();
			return;
		}
		Armageddon();
		break;
	case 34: // Flotellum
		if (m_misc->m_Party[2] == 0x80)
		{
			Failed();
			return;
		}
		Flotellum();
		break;
	default:
		break;
	}
}
