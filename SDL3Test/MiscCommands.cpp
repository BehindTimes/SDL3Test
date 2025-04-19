#include "U3Misc.h"
#include "UltimaGraphics.h"
#include "U3ScrollArea.h"
#include "U3Resources.h"
#include "UltimaSpellCombat.h"
#include "UltimaIncludes.h"
#include "U3Utilities.h"
#include "UltimaDungeon.h"
#include <SDL3/SDL.h>
#include <iostream>

extern U3Resources m_resources;
extern U3Graphics m_graphics;
extern U3ScrollArea m_scrollArea;
extern UltimaSpellCombat m_spellCombat;
extern U3Utilities m_utilities;
extern UltimaSpellCombat m_spellCombat;
extern UltimaDungeon m_dungeon;

void U3Misc::LetterCommand(SDL_Keycode key)
{
	switch (key)
	{
	case SDLK_A:
		//Attack();
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
		//Fire();
		break;
	case SDLK_G:
		GetChest();
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
		OtherCommand();
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
		Yell();
		break;
	case SDLK_Z:
		break;
	default:
		break;
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
	}
	return true;
}

bool U3Misc::CommandNorth()
{
	if (m_callbackStack.size() > 0)
	{
		m_callbackStack.pop();
	}

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

	return false;
}

bool U3Misc::CommandSouth()
{
	if (m_callbackStack.size() > 0)
	{
		m_callbackStack.pop();
	}

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

	return false;
}

bool U3Misc::CommandEast()
{
	if (m_callbackStack.size() > 0)
	{
		m_callbackStack.pop();
	}

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

	return false;
}

bool U3Misc::CommandWest()
{
	if (m_callbackStack.size() > 0)
	{
		m_callbackStack.pop();
	}

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

	return false;
}

bool U3Misc::CommandSouthEast()
{
	if (m_callbackStack.size() > 0)
	{
		m_callbackStack.pop();
	}

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

	return false;
}

bool U3Misc::CommandSouthWest()
{
	if (m_callbackStack.size() > 0)
	{
		m_callbackStack.pop();
	}

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

	return false;
}

bool U3Misc::CommandNorthEast()
{
	if (m_callbackStack.size() > 0)
	{
		m_callbackStack.pop();
	}

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

	return false;
}

bool U3Misc::CommandNorthWest()
{
	if (m_callbackStack.size() > 0)
	{
		m_callbackStack.pop();
	}

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

	return false;
}

bool U3Misc::CommandFinishTurn()
{
	if (m_callbackStack.size() > 0)
	{
		m_callbackStack.pop();
	}
	m_resources.m_wasMove = true;

	if (m_graphics.m_curMode == U3GraphicsMode::Dungeon)
	{
		m_dungeon.dungeonmech();
	}
	else
	{
		Routine6E35();
	}

	return false;
}

void U3Misc::North()
{
	m_callbackStack.push(std::bind(&U3Misc::CommandFinishTurn, this));
	m_callbackStack.push(std::bind(&U3Misc::CommandNorth, this));
}

void U3Misc::South()
{
	m_callbackStack.push(std::bind(&U3Misc::CommandFinishTurn, this));
	m_callbackStack.push(std::bind(&U3Misc::CommandSouth, this));
}

void U3Misc::East()
{
	m_callbackStack.push(std::bind(&U3Misc::CommandFinishTurn, this));
	m_callbackStack.push(std::bind(&U3Misc::CommandEast, this));
}

void U3Misc::West()
{
	m_callbackStack.push(std::bind(&U3Misc::CommandFinishTurn, this));
	m_callbackStack.push(std::bind(&U3Misc::CommandWest, this));
}

void U3Misc::SouthEast()
{
	m_callbackStack.push(std::bind(&U3Misc::CommandFinishTurn, this));
	m_callbackStack.push(std::bind(&U3Misc::CommandSouthEast, this));
}


void U3Misc::SouthWest()
{
	m_callbackStack.push(std::bind(&U3Misc::CommandFinishTurn, this));
	m_callbackStack.push(std::bind(&U3Misc::CommandSouthWest, this));
}


void U3Misc::NorthEast()
{
	m_callbackStack.push(std::bind(&U3Misc::CommandFinishTurn, this));
	m_callbackStack.push(std::bind(&U3Misc::CommandNorthEast, this));
}


void U3Misc::NorthWest()
{
	m_callbackStack.push(std::bind(&U3Misc::CommandFinishTurn, this));
	m_callbackStack.push(std::bind(&U3Misc::CommandNorthWest, this));
}

bool U3Misc::CommandPass()
{
	if (m_callbackStack.size() > 0)
	{
		m_callbackStack.pop();
	}
	m_scrollArea.UPrintMessage(23);

	return false;
}

void U3Misc::Pass()
{
	m_callbackStack.push(std::bind(&U3Misc::CommandFinishTurn, this));
	m_callbackStack.push(std::bind(&U3Misc::CommandPass, this));
}

bool U3Misc::CommandGetChest()
{
	if (m_callbackStack.size() > 0)
	{
		m_callbackStack.pop();
	}
	GetChest(0, 0);
	return false;
}

void U3Misc::GetChest()
{
	m_callbackStack.push(std::bind(&U3Misc::CommandFinishTurn, this));
	m_callbackStack.push(std::bind(&U3Misc::CommandGetChest, this));
}

bool U3Misc::CommandEnter()
{
	if (m_callbackStack.size() > 0)
	{
		m_callbackStack.pop();
	}

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
		return false;
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
			return false;
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
			m_dungeon.m_dungeonLevel = 0;
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
			m_dungeon.m_dungeonLevel = 0;
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
			return false;
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
			return false;
		}
		dispString = m_resources.m_plistMap["Messages"][32];
		addString = m_resources.m_plistMap["Messages"][36];
		dispString += addString;
		m_scrollArea.UPrintWin(dispString);

		m_inputType = InputType::Transact;
		m_scrollArea.blockPrompt(true);
		m_callbackStack.push(std::bind(&U3Misc::EnterShrineCallback, this));
		m_callbackStack.push(std::bind(&U3Misc::ProcessEventCallback, this));

		return false;
	}
	bool autosave;
	m_resources.GetPreference(U3PreferencesType::Auto_Save, autosave);
	if (autosave)
	{
		PutRoster();
		PutParty();
		PutSosaria();
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
		m_dungeon.DungeonStart(0);
		return false;
	}
	if (m_Party[2] == 2) // Town
	{
		return false;
	}
	if (placeNum != 1)
	{
		return false;
	}
	if (m_Party[15] == 1)
	{
		SafeExodus();
	}

	return false;
}

void U3Misc::Enter()
{
	m_callbackStack.push(std::bind(&U3Misc::CommandFinishTurn, this));
	m_callbackStack.push(std::bind(&U3Misc::CommandEnter, this));
}

bool U3Misc::CommandLook()
{
	if (m_callbackStack.size() > 0)
	{
		m_callbackStack.pop();
	}

	m_scrollArea.UPrintMessage(69);
	m_inputType = InputType::GetDirection;
	m_callbackStack.push(std::bind(&U3Misc::LookCallback, this));
	m_callbackStack.push(std::bind(&U3Misc::ProcessEventCallback, this));

	return false;
}

void U3Misc::Look()
{
	m_callbackStack.push(std::bind(&U3Misc::CommandFinishTurn, this));
	m_callbackStack.push(std::bind(&U3Misc::CommandLook, this));
}

bool U3Misc::LookCallback()
{
	if (m_callbackStack.size() > 0)
	{
		m_callbackStack.pop();
	}

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

	return false;
}

bool U3Misc::ProcessEventCallback()
{
	ProcessEvent(m_currentEvent);
	return true;
}

bool U3Misc::CommandTransact()
{
	if (m_callbackStack.size() > 0)
	{
		m_callbackStack.pop();
	}
	m_storedir = 0;
	m_scrollArea.UPrintMessageRewrapped(88);
	m_inputType = InputType::Transact;
	m_scrollArea.blockPrompt(true);
	m_callbackStack.push(std::bind(&U3Misc::TransactCallback, this));
	m_callbackStack.push(std::bind(&U3Misc::ProcessEventCallback, this));
	return false;
}


void U3Misc::Transact()
{
	m_callbackStack.push(std::bind(&U3Misc::CommandFinishTurn, this));
	m_callbackStack.push(std::bind(&U3Misc::CommandTransact, this));
}

bool U3Misc::TransactCallback()
{
	if (m_callbackStack.size() > 0)
	{
		m_callbackStack.pop();
	}

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
			return false;
		}
		m_scrollArea.UPrintMessage(85);

		if (m_storedir)
		{
			//AddMacro(storeDir);
		}

		m_inputType = InputType::GetDirection;
		m_callbackStack.push(std::bind(&U3Misc::TransactCallback2, this));
		m_callbackStack.push(std::bind(&U3Misc::ProcessEventCallback, this));
	}

	return false;
}

bool U3Misc::TransactCallback2()
{
	if (m_callbackStack.size() > 0)
	{
		m_callbackStack.pop();
	}

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
			return false;
		}
		m_xs = m_xs + m_dx;
		m_ys = m_ys + m_dy;
		tile = GetXYVal(m_xs, m_ys);

		if (tile != 0x40) // merchant
		{
			NotHere();
			return false;
		}
		shopNum = (m_ypos & 0x07);

		//gSongCurrent = gSongNext = 6;
		InverseChnum(m_transactNum, true);
		Shop(shopNum, m_transactNum);
		//InverseChnum(m_transactNum, false);
		//gSongNext = m_Party[2];
		return false;
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
			m_scrollArea.blockPrompt(false);
			return false;
		}
		Speak(perNum, m_Monsters[monNum] / 4);
		m_scrollArea.blockPrompt(false);
		return false;
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
		return false;
	}
	if (hpmax >= 25 && m_Party[15] == 0)
	{
		m_scrollArea.UPrintMessage(92);
		return false;
	}
	if (hpmax > 4 && (m_Player[m_rosNum][14] & 0x80) == 0)
	{
		m_scrollArea.UPrintMessage(93);
		return false;
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
	m_scrollArea.blockPrompt(false);

	return false;
}

bool U3Misc::CommandBoard()
{
	if (m_callbackStack.size() > 0)
	{
		m_callbackStack.pop();
	}

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
	return false;
}

void U3Misc::Board()
{
	m_callbackStack.push(std::bind(&U3Misc::CommandFinishTurn, this));
	m_callbackStack.push(std::bind(&U3Misc::CommandBoard, this));
}

bool U3Misc::CommandExit()
{
	if (m_callbackStack.size() > 0)
	{
		m_callbackStack.pop();
	}
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
	return false;
}

void U3Misc::Exit()
{
	m_callbackStack.push(std::bind(&U3Misc::CommandFinishTurn, this));
	m_callbackStack.push(std::bind(&U3Misc::CommandExit, this));
}

bool U3Misc::CommandPeerGem()
{
	if (m_callbackStack.size() > 0)
	{
		m_callbackStack.pop();
	}

	m_scrollArea.UPrintMessage(75);
	m_inputType = InputType::Transact;
	m_graphics.setFade(false);
	m_callbackStack.push(std::bind(&U3Misc::PeerGemCallback, this));
	m_callbackStack.push(std::bind(&U3Misc::ProcessEventCallback, this));

	return false;
}

void U3Misc::PeerGem()
{
	m_callbackStack.push(std::bind(&U3Misc::CommandFinishTurn, this));
	m_callbackStack.push(std::bind(&U3Misc::CommandPeerGem, this));
}

bool U3Misc::CommandKlimb()
{
	if (m_callbackStack.size() > 0)
	{
		m_callbackStack.pop();
	}

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
	return false;
}

void U3Misc::Klimb()
{
	m_callbackStack.push(std::bind(&U3Misc::CommandFinishTurn, this));
	m_callbackStack.push(std::bind(&U3Misc::CommandKlimb, this));
}

bool U3Misc::CommandDescend()
{
	if (m_callbackStack.size() > 0)
	{
		m_callbackStack.pop();
	}

	// Seems like a little easter egg, not going to implement yet
	//if (m_Party[15] == 0)
	{
		m_scrollArea.UPrintMessage(32);
		What2();
		return false;
	}
	//std::string strDiorama = std::string(DioramaString);
	//m_scrollArea.UPrintWin(strDiorama);
	//DrawDioramaMap();

	return false;
}

void U3Misc::Descend()
{
	m_callbackStack.push(std::bind(&U3Misc::CommandFinishTurn, this));
	m_callbackStack.push(std::bind(&U3Misc::CommandDescend, this));
}

bool U3Misc::CommandIgnite()
{
	if (m_callbackStack.size() > 0)
	{
		m_callbackStack.pop();
	}

	m_scrollArea.UPrintMessage(64);
	if (m_Party[2] != 1)
	{
		NotHere();
		return false;
	}
	m_scrollArea.UPrintMessage(65);
	m_inputType = InputType::Transact;
	m_callbackStack.push(std::bind(&U3Misc::IgniteCallback, this));
	m_callbackStack.push(std::bind(&U3Misc::ProcessEventCallback, this));

	return false;
}

void U3Misc::Ignite()
{
	m_callbackStack.push(std::bind(&U3Misc::CommandFinishTurn, this));
	m_callbackStack.push(std::bind(&U3Misc::CommandIgnite, this));
}

bool U3Misc::CommandUnlock()
{
	if (m_callbackStack.size() > 0)
	{
		m_callbackStack.pop();
	}

	m_scrollArea.UPrintMessage(95);
	m_inputType = InputType::GetDirection;
	m_callbackStack.push(std::bind(&U3Misc::UnlockCallback, this));
	m_callbackStack.push(std::bind(&U3Misc::ProcessEventCallback, this));

	return false;
}

void U3Misc::Unlock()
{
	m_callbackStack.push(std::bind(&U3Misc::CommandFinishTurn, this));
	m_callbackStack.push(std::bind(&U3Misc::CommandUnlock, this));
}

bool U3Misc::CommandSteal()
{
	if (m_callbackStack.size() > 0)
	{
		m_callbackStack.pop();
	}

	m_scrollArea.UPrintMessage(84);
	m_inputType = InputType::Transact;
	m_callbackStack.push(std::bind(&U3Misc::StealCallback, this));
	m_callbackStack.push(std::bind(&U3Misc::ProcessEventCallback, this));

	return false;
}

void U3Misc::Steal()
{
	m_callbackStack.push(std::bind(&U3Misc::CommandFinishTurn, this));
	m_callbackStack.push(std::bind(&U3Misc::CommandSteal, this));
}

bool U3Misc::CommandOther()
{
	if (m_callbackStack.size() > 0)
	{
		m_callbackStack.pop();
	}

	OtherCommand(0);

	return false;
}

void U3Misc::OtherCommand()
{
	m_callbackStack.push(std::bind(&U3Misc::CommandFinishTurn, this));
	m_callbackStack.push(std::bind(&U3Misc::CommandOther, this));
}

bool U3Misc::CommandYell()
{
	if (m_callbackStack.size() > 0)
	{
		m_callbackStack.pop();
	}

	Yell(0);

	return false;
}


void U3Misc::Yell()
{
	m_callbackStack.push(std::bind(&U3Misc::CommandFinishTurn, this));
	m_callbackStack.push(std::bind(&U3Misc::CommandYell, this));
}