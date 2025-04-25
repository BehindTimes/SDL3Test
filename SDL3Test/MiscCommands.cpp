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

extern std::unique_ptr<U3Resources> m_resources;
extern std::unique_ptr<U3Graphics> m_graphics;
extern std::unique_ptr<U3ScrollArea> m_scrollArea;
extern std::unique_ptr<U3Utilities> m_utilities;
extern std::unique_ptr<UltimaSpellCombat> m_spellCombat;
extern std::unique_ptr<UltimaDungeon> m_dungeon;

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
		//Cast();
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
		HandEquip();
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
		ModifyOrder();
		break;
	case SDLK_N:
		NegateTime();
		break;
	case SDLK_O:
		OtherCommand();
		break;
	case SDLK_P:
		PeerGem();
		break;
	case SDLK_Q:
		//QuitSave
		break;
	case SDLK_R:
		ReadyWeapon();
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
		WearArmor();
		break;
	case SDLK_X:
		Exit();
		break;
	case SDLK_Y:
		Yell();
		break;
	case SDLK_Z:
		ZStats();
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

	m_scrollArea->UPrintMessage(24);
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
			m_ypos = m_graphics->MapConstrain(m_ypos);
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

	m_scrollArea->UPrintMessage(25);
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
			m_ypos = m_graphics->MapConstrain(m_ypos);
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

	m_scrollArea->UPrintMessage(26);
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
			m_xpos = m_graphics->MapConstrain(m_xpos);
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

	m_scrollArea->UPrintMessage(27);
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
			m_xpos = m_graphics->MapConstrain(m_xpos);
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

	m_scrollArea->UPrintMessage(251);
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
			m_ypos = m_graphics->MapConstrain(m_ypos);
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

	m_scrollArea->UPrintMessage(25);
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
			m_ypos = m_graphics->MapConstrain(m_ypos);
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

	m_scrollArea->UPrintMessage(25);
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
			m_ypos = m_graphics->MapConstrain(m_ypos);
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

	m_scrollArea->UPrintMessage(25);
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
			m_ypos = m_graphics->MapConstrain(m_ypos);
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
	m_resources->m_wasMove = true;

	if (m_graphics->m_curMode == U3GraphicsMode::Dungeon)
	{
		m_dungeon->dungeonmech();
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
	m_scrollArea->UPrintMessage(23);

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
		m_scrollArea->UPrintMessage(33);
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
			m_scrollArea->UPrintMessage(33);
			What2();
			return false;
		}
		m_zp[0xE3] = m_xpos;
		m_zp[0xE4] = m_ypos;

		tile = GetXYVal(m_xpos, m_ypos) / 2;
		switch (tile)
		{
		case 0x0A:// Dungeon
			dispString = m_resources->m_plistMap["Messages"][32];
			addString = m_resources->m_plistMap["Messages"][33];
			newval = 1;
			m_dungeon->m_dungeonLevel = 0;
			m_xpos = 1;
			m_ypos = 1;
			m_heading = 1;
			break;
		case 0x0C: // Town
			dispString = m_resources->m_plistMap["Messages"][32];
			addString = m_resources->m_plistMap["Messages"][34];
			newval = 2;
			m_xpos = 1;
			m_ypos = 32;
			m_heading = 2;
			m_dungeon->m_dungeonLevel = 0;
			break;
		case 0x0E: // Castle
			dispString = m_resources->m_plistMap["Messages"][32];
			addString = m_resources->m_plistMap["Messages"][35];
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
		m_scrollArea->UPrintWin(dispString);
	}
	else if (m_Party[2] == 0xFF) // Ambrosia
	{
		if (GetXYVal(m_xpos, m_ypos) != 0xF8)
		{
			m_scrollArea->UPrintMessage(33);
			What2();
			return false;
		}
		dispString = m_resources->m_plistMap["Messages"][32];
		addString = m_resources->m_plistMap["Messages"][36];
		dispString += addString;
		m_scrollArea->UPrintWin(dispString);

		m_inputType = InputType::Transact;
		m_scrollArea->blockPrompt(true);
		m_callbackStack.push(std::bind(&U3Misc::EnterShrineCallback, this));
		m_callbackStack.push(std::bind(&U3Misc::ProcessEventCallback, this));

		return false;
	}
	bool autosave;
	m_resources->GetPreference(U3PreferencesType::Auto_Save, autosave);
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
		m_dungeon->DungeonStart(0);
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

	m_scrollArea->UPrintMessage(69);
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
	m_scrollArea->UPrintWin("->");
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
				unsigned char tileVal = m_resources->m_TileArray[offset];
				if (tileVal == 0x5D) // Door, which is not actually handled in the original game or the mac remake, but let's handle it
				{
					std::string strDoor(DoorString);
					m_scrollArea->UPrintWin(strDoor);
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
	m_scrollArea->UPrintWin("\n");

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
	m_scrollArea->UPrintMessageRewrapped(88);
	m_inputType = InputType::Transact;
	m_scrollArea->blockPrompt(true);
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
		m_scrollArea->UPrintWin("\n");
	}
	else
	{
		m_transactNum = m_input_num;
		m_rosNum = m_Party[6 + m_transactNum];
		std::string strRosNum = std::to_string(m_rosNum) + std::string("\n");
		m_scrollArea->UPrintWin(strRosNum);
		if (CheckAlive(m_transactNum) == 0)
		{
			m_spellCombat->Incap();
			return false;
		}
		m_scrollArea->UPrintMessage(85);

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

	//m_scrollArea->UPrintWin("\n");
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
				m_scrollArea->UPrintMessageRewrapped(262);
			}
			else
			{
				m_scrollArea->UPrintMessage(89);
			}
			m_scrollArea->blockPrompt(false);
			return false;
		}
		Speak(perNum, m_Monsters[monNum] / 4);
		m_scrollArea->blockPrompt(false);
		return false;
	}
	m_scrollArea->UPrintMessage(90);
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
			m_scrollArea->UPrintMessageRewrapped(263);
		}
		else
		{
			m_scrollArea->UPrintMessage(91);
		}
		return false;
	}
	if (hpmax >= 25 && m_Party[15] == 0)
	{
		m_scrollArea->UPrintMessage(92);
		return false;
	}
	if (hpmax > 4 && (m_Player[m_rosNum][14] & 0x80) == 0)
	{
		m_scrollArea->UPrintMessage(93);
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
	m_scrollArea->UPrintMessage(94);
	InverseTiles(true);
	m_scrollArea->blockPrompt(false);

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
		m_scrollArea->UPrintMessage(29); // Board
		What2();
	}
	else
	{
		tileOn = GetXYVal(m_xpos, m_ypos);
		if (tileOn == 0x28) // horse
		{
			PutXYVal(0x04, m_xpos, m_ypos); // replace with grass
			m_Party[0] = 0x14;
			m_scrollArea->UPrintMessage(30);
		}
		else if (tileOn == 0x2C) // ship
		{
			PutXYVal(0x00, m_xpos, m_ypos); // replace with water
			m_Party[0] = 0x16;
			m_scrollArea->UPrintMessage(31);
		}
		else
		{
			m_scrollArea->UPrintMessage(29); // Board
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
		std::string dispString = m_resources->m_plistMap["Messages"][101];
		std::string addString = m_resources->m_plistMap["Messages"][106];
		dispString += addString;
		m_scrollArea->UPrintWin(dispString);
	}
	else
	{
		tileOn = GetXYVal(m_xpos, m_ypos);
		if (tileOn > 4) // not water or grass
		{
			m_scrollArea->UPrintMessage(102);    // X-it
			m_scrollArea->UPrintWin("\n");
			m_scrollArea->UPrintMessage(108);    // Not here!\n
			//ErrorTone();
		}
		else
		{
			PutXYVal(m_Party[0] * 2, m_xpos, m_ypos);
			if (m_Party[1] == 0x14)
			{
				m_scrollArea->UPrintMessage(17);    // Dismount\n
			}
			else
			{
				m_scrollArea->UPrintMessage(103);    // X-it craft
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

	m_scrollArea->UPrintMessage(75);
	m_inputType = InputType::Transact;
	m_graphics->setFade(false);
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
		m_scrollArea->UPrintMessage(68);
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
		m_scrollArea->UPrintMessage(32);
		What2();
		return false;
	}
	//std::string strDiorama = std::string(DioramaString);
	//m_scrollArea->UPrintWin(strDiorama);
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

	m_scrollArea->UPrintMessage(64);
	if (m_Party[2] != 1)
	{
		NotHere();
		return false;
	}
	m_scrollArea->UPrintMessage(65);
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

	m_scrollArea->UPrintMessage(95);
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

	m_scrollArea->UPrintMessage(84);
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

bool U3Misc::CommandZStats()
{
	if (m_callbackStack.size() > 0)
	{
		m_callbackStack.pop();
	}

	Stats(0, 0);

	return false;
}

void U3Misc::ZStats()
{
	m_callbackStack.push(std::bind(&U3Misc::CommandFinishTurn, this));
	m_callbackStack.push(std::bind(&U3Misc::CommandZStats, this));
}

void U3Misc::Stats(short mode, short chnum)
{
	if (mode == 0)
	{
		m_scrollArea->UPrintMessage(105);
		m_inputType = InputType::Transact;
		m_scrollArea->blockPrompt(true);
		m_callbackStack.push(std::bind(&U3Misc::StatsCallback, this));
		m_callbackStack.push(std::bind(&U3Misc::ProcessEventCallback, this));
	}
}

bool U3Misc::StatsCallback()
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

	bool classic;
	m_resources->GetPreference(U3PreferencesType::Classic_Appearance, classic);

	short temp;
	short x;
	if (!classic)
	{
		m_surpressTextDisplay = true;
		m_resources->m_overrideImage = 9;
		m_inputType = InputType::ZStats;
		m_callbackStack.push(std::bind(&U3Misc::StatsCallback2, this));
		m_callbackStack.push(std::bind(&U3Misc::ProcessEventCallback, this));
		m_rosNum = m_Party[6 + m_chNum];
		m_resources->GenerateZStatImage(m_rosNum);
		m_resources->createZStatButtons();
	}
	else
	{
		m_rosNum = m_Party[6 + m_chNum];
		for (x = 0; x < 13; x++)
		{
			temp = m_Player[m_rosNum][x];
			if (temp > 0)
			{
				std::string strValue;
				strValue += (char)temp;
				m_scrollArea->UPrintWin(strValue);
			}
			else
			{
				break;
			}
		}
		m_scrollArea->UPrintWin(std::string(StrString));
		auto padded = std::to_string(m_Player[m_rosNum][18]);
		padded.insert(0, 2U - std::min(std::string::size_type(2), padded.length()), '0');
		m_scrollArea->UPrintWin(padded);
		m_scrollArea->UPrintWin(std::string(DexString));
		padded = std::to_string(m_Player[m_rosNum][19]);
		padded.insert(0, 2U - std::min(std::string::size_type(2), padded.length()), '0');
		m_scrollArea->UPrintWin(padded);
		m_scrollArea->UPrintWin(std::string(IntString));
		padded = std::to_string(m_Player[m_rosNum][20]);
		padded.insert(0, 2U - std::min(std::string::size_type(2), padded.length()), '0');
		m_scrollArea->UPrintWin(padded);
		m_scrollArea->UPrintWin(std::string(WisString));
		padded = std::to_string(m_Player[m_rosNum][21]);
		padded.insert(0, 2U - std::min(std::string::size_type(2), padded.length()), '0');
		m_scrollArea->UPrintWin(padded);

		m_inputType = InputType::AnyKeyEscape;
		m_opnum = 0;
		m_callbackStack.push(std::bind(&U3Misc::StatsCallback1, this));
		m_callbackStack.push(std::bind(&U3Misc::ProcessEventCallback, this));
	}

	return false;
}

bool U3Misc::StatsCallback2()
{
	if (m_callbackStack.size() > 0)
	{
		m_callbackStack.pop();
	}
	m_resources->m_zstatbuttons.clear();
	m_resources->m_overrideImage = -1;
	m_surpressTextDisplay = false;
	return false;
}

bool U3Misc::StatsCallback1()
{
	unsigned char value;
	bool skip = false;
	short x;

	if (m_callbackStack.size() > 0)
	{
		m_callbackStack.pop();
	}
	if (m_input_num < 0)
	{
		m_scrollArea->UPrintWin("\n");
		return false;
	}
	short temp;
	switch (m_opnum)
	{
	case 0:
	{
		m_scrollArea->UPrintWin(std::string(HpString));
		temp = m_Player[m_rosNum][26] * 256 + m_Player[m_rosNum][27];
		std::string dispString = m_utilities->GetPaddedNum(temp, 4);
		m_scrollArea->UPrintWin(dispString);
	}
	break;
	case 1:
	{
		m_scrollArea->UPrintWin(std::string(HmString));
		temp = m_Player[m_rosNum][28] * 256 + m_Player[m_rosNum][29];
		std::string dispString = m_utilities->GetPaddedNum(temp, 4);
		m_scrollArea->UPrintWin(dispString);
	}
	break;
	case 2:
	{
		m_scrollArea->UPrintWin(std::string(GoldString));
		temp = m_Player[m_rosNum][35] * 256 + m_Player[m_rosNum][36];
		std::string dispString = m_utilities->GetPaddedNum(temp, 4);
		m_scrollArea->UPrintWin(dispString);
	}
	break;
	case 3:
	{
		m_scrollArea->UPrintWin(std::string(ExpString));
		temp = m_Player[m_rosNum][30] * 256 + m_Player[m_rosNum][31];
		std::string dispString = m_utilities->GetPaddedNum(temp, 4);
		m_scrollArea->UPrintWin(dispString);
	}
	break;
	case 4:
	{
		m_scrollArea->UPrintWin(std::string(GemString));
		temp = m_Player[m_rosNum][37];
		std::string dispString = m_utilities->GetPaddedNum(temp, 2);
		m_scrollArea->UPrintWin(dispString);
	}
	break;
	case 5:
	{
		m_scrollArea->UPrintWin(std::string(KeyString));
		temp = m_Player[m_rosNum][38];
		std::string dispString = m_utilities->GetPaddedNum(temp, 2);
		m_scrollArea->UPrintWin(dispString);
	}
	break;
	case 6:
	{
		m_scrollArea->UPrintWin(std::string(PowdString));
		temp = m_Player[m_rosNum][39];
		std::string dispString = m_utilities->GetPaddedNum(temp, 2);
		m_scrollArea->UPrintWin(dispString);
	}
	break;
	case 7:
	{
		m_scrollArea->UPrintWin(std::string(TrchString));
		temp = m_Player[m_rosNum][15];
		std::string dispString = m_utilities->GetPaddedNum(temp, 2);
		m_scrollArea->UPrintWin(dispString);
	}
	break;
	case 8:
		value = m_Player[m_rosNum][14];
		if (value & 0x08)
		{
			m_scrollArea->UPrintWin(std::string(CoDString));
		}
		else
		{
			skip = true;
		}
		break;
	case 9:
		value = m_Player[m_rosNum][14];
		if (value & 0x02)
		{
			m_scrollArea->UPrintWin(std::string(CoSString));
		}
		else
		{
			skip = true;
		}
		break;
	case 10:
		value = m_Player[m_rosNum][14];
		if (value & 0x01)
		{
			m_scrollArea->UPrintWin(std::string(CoLString));
		}
		else
		{
			skip = true;
		}
		break;
	case 11:
		value = m_Player[m_rosNum][14];
		if (value & 0x04)
		{
			m_scrollArea->UPrintWin(std::string(CoMString));
		}
		else
		{
			skip = true;
		}
		break;
	case 12:
		value = m_Player[m_rosNum][14];
		if (value & 0x04)
		{
			m_scrollArea->UPrintWin(std::string(MoForceString));
		}
		else
		{
			skip = true;
		}
		break;
	case 13:
		value = m_Player[m_rosNum][14];
		if (value & 0x04)
		{
			m_scrollArea->UPrintWin(std::string(MoFireString));
		}
		else
		{
			skip = true;
		}
		break;
	case 14:
		value = m_Player[m_rosNum][14];
		if (value & 0x04)
		{
			m_scrollArea->UPrintWin(std::string(MoSString));
		}
		else
		{
			skip = true;
		}
		break;
	case 15:
		value = m_Player[m_rosNum][14];
		if (value & 0x04)
		{
			m_scrollArea->UPrintWin(std::string(MoKString));
		}
		else
		{
			skip = true;
		}
		break;
	case 16:
	{
		m_scrollArea->UPrintWin(std::string(WeaponString));
		std::string dispString = m_resources->m_plistMap["WeaponsArmour"][m_Player[m_rosNum][48]];
		m_scrollArea->UPrintWin(std::string(dispString));
	}
		break;
	case 17:
	{
		m_scrollArea->UPrintWin(std::string(ArmorString));
		std::string dispString = m_resources->m_plistMap["WeaponsArmour"][m_Player[m_rosNum][40] + 16];
		m_scrollArea->UPrintWin(std::string(dispString));
	}
	break;
	case 18:
		m_scrollArea->UPrintWin(std::string(WeaponsString));
		skip = true;
		break;
	case 19:
	case 20:
	case 21:
	case 22:
	case 23:
	case 24:
	case 25:
	case 26:
	case 27:
	case 28:
	case 29:
	case 30:
	case 31:
	case 32:
	case 33:
	{
		x = 15 - (m_opnum - 19);
		if (m_Player[m_rosNum][x + 48])
		{
			m_scrollArea->UPrintWin("\n");
			std::string dispString = m_utilities->GetPaddedNum(m_Player[m_rosNum][x + 48], 2);
			m_scrollArea->UPrintWin(dispString);
			m_scrollArea->UPrintWin("-");
			dispString = m_resources->m_plistMap["WeaponsArmour"][x];
			m_scrollArea->UPrintWin(dispString);
			m_scrollArea->UPrintWin("(");
			dispString.clear();
			dispString += (char)(x + 65);
			m_scrollArea->UPrintWin(dispString);
			m_scrollArea->UPrintWin(")");
		}
		else
		{
			skip = true;
		}
	}
		break;
	case 34:
		m_scrollArea->UPrintWin(std::string(NoWeaponsString));
		skip = true;
		break;
	case 35:
	case 36:
	case 37:
	case 38:
	case 39:
	case 40:
	case 41:
		x = 7 - (m_opnum - 35);
		if (m_Player[m_rosNum][x + 40])
		{
			m_scrollArea->UPrintWin("\n");
			std::string dispString = m_utilities->GetPaddedNum(m_Player[m_rosNum][x + 40], 2);
			m_scrollArea->UPrintWin(dispString);
			m_scrollArea->UPrintWin("-");
			dispString = m_resources->m_plistMap["WeaponsArmour"][x + 16];
			m_scrollArea->UPrintWin(dispString);
			m_scrollArea->UPrintWin("(");
			dispString.clear();
			dispString += (char)(x + 65);
			m_scrollArea->UPrintWin(dispString);
			m_scrollArea->UPrintWin(")");
		}
		else
		{
			skip = true;
		}
		break;
	case 42:
		m_scrollArea->UPrintWin(std::string(NoArmorString));
		skip = true;
		break;
	default:
		break;
	}
	if (m_opnum < 42)
	{
		m_opnum++;
		if (!skip)
		{
			m_inputType = InputType::AnyKeyEscape;
			m_callbackStack.push(std::bind(&U3Misc::StatsCallback1, this));
			m_callbackStack.push(std::bind(&U3Misc::ProcessEventCallback, this));
		}
		else
		{
			m_callbackStack.push(std::bind(&U3Misc::StatsCallback1, this));
		}
	}
	else
	{
		m_scrollArea->UPrintWin("\n");
	}
	return false;
}

void U3Misc::WearArmour(short chnum, char armour, bool preset)
{
	short typeNum;
	short x;

	m_rosNum = m_Party[6 + chnum];

	typeNum = 0;
	for (x = 0; x < 12; x++)
	{
		if (m_Player[m_rosNum][23] == m_careerTable[x])
		{
			typeNum = x;
		}
	}
	if (armour != 'H' && armour >= m_armUseTable[typeNum])
	{
		m_scrollArea->UPrintMessage(82);
		return;
	}
	armour -= 'A';
	if ((armour > 0) && (m_Player[m_rosNum][armour + 40] < 1))
	{
		m_scrollArea->UPrintMessage(81);
		return;
	}
	m_Player[m_rosNum][40] = armour;
	if (!preset)
	{
		m_scrollArea->UPrintWin("\n");
		std::string outStr = m_resources->m_plistMap["WeaponsArmour"][armour + 16];
		std::string readyStr = m_resources->m_plistMap["Messages"][82];
		outStr += readyStr;
		m_scrollArea->UPrintWin(outStr);
	}
}

bool U3Misc::WearArmourCallback()
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

	m_scrollArea->UPrintMessage(101);
	m_inputType = InputType::LetterImmediate;
	m_callbackStack.push(std::bind(&U3Misc::WearArmourCallback1, this));
	m_callbackStack.push(std::bind(&U3Misc::ProcessEventCallback, this));

	return false;
}

bool U3Misc::WearArmourCallback1()
{
	if (m_callbackStack.size() > 0)
	{
		m_callbackStack.pop();
	}
	m_scrollArea->UPrintWin(m_input);
	m_scrollArea->UPrintWin("\n");

	if (m_input.empty())
	{
		return false;
	}
	else if (m_input[0] < 'A' || m_input[0] > 'H')
	{
		m_scrollArea->UPrintMessage(81);
		return false;
	}

	WearArmour(m_chNum, m_input[0], false);

	return false;
}

bool U3Misc::CommandWearArmor()
{
	if (m_callbackStack.size() > 0)
	{
		m_callbackStack.pop();
	}
	m_scrollArea->UPrintMessage(100);
	m_inputType = InputType::Transact;
	m_scrollArea->blockPrompt(true);
	m_callbackStack.push(std::bind(&U3Misc::WearArmourCallback, this));
	m_callbackStack.push(std::bind(&U3Misc::ProcessEventCallback, this));

	return false;
}

void U3Misc::WearArmor()
{
	m_callbackStack.push(std::bind(&U3Misc::CommandFinishTurn, this));
	m_callbackStack.push(std::bind(&U3Misc::CommandWearArmor, this));
}

void U3Misc::ReadyWeapon(short chnum, char weapon, bool preset)
{
	short typeNum;
	short x;

	m_rosNum = m_Party[6 + chnum];

	typeNum = 0;
	for (x = 0; x < 12; x++)
	{
		if (m_Player[m_rosNum][23] == m_careerTable[x])
		{
			typeNum = x;
		}
	}
	if (weapon != 'P' && weapon >= m_wpnUseTable[typeNum])
	{
		m_scrollArea->UPrintMessage(82);
		return;
	}
	weapon -= 'A';
	if ((weapon > 0) && (m_Player[m_rosNum][weapon + 48] < 1))
	{
		m_scrollArea->UPrintMessage(81);
		return;
	}
	m_Player[m_rosNum][48] = weapon;
	if (!preset)
	{
		m_scrollArea->UPrintWin("\n");
		std::string outStr = m_resources->m_plistMap["WeaponsArmour"][weapon];
		std::string readyStr = m_resources->m_plistMap["Messages"][82];
		outStr += readyStr;
		m_scrollArea->UPrintWin(outStr);
	}
}

bool U3Misc::ReadyWeaponCallback()
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

	m_scrollArea->UPrintMessage(80);
	m_inputType = InputType::LetterImmediate;
	m_callbackStack.push(std::bind(&U3Misc::ReadyWeaponCallback1, this));
	m_callbackStack.push(std::bind(&U3Misc::ProcessEventCallback, this));

	return false;
}

bool U3Misc::ReadyWeaponCallback1()
{
	if (m_callbackStack.size() > 0)
	{
		m_callbackStack.pop();
	}
	m_scrollArea->UPrintWin(m_input);
	m_scrollArea->UPrintWin("\n");

	if (m_input.empty())
	{
		return false;
	}
	else if (m_input[0] < 'A' || m_input[0] > 'P')
	{
		m_scrollArea->UPrintMessage(81);
		return false;
	}

	ReadyWeapon(m_chNum, m_input[0], false);

	return false;
}

bool U3Misc::CommandReadyWeapon()
{
	if (m_callbackStack.size() > 0)
	{
		m_callbackStack.pop();
	}
	m_scrollArea->UPrintMessage(79);
	m_inputType = InputType::Transact;
	m_scrollArea->blockPrompt(true);
	m_callbackStack.push(std::bind(&U3Misc::ReadyWeaponCallback, this));
	m_callbackStack.push(std::bind(&U3Misc::ProcessEventCallback, this));

	return false;
}

void U3Misc::ReadyWeapon()
{
	m_callbackStack.push(std::bind(&U3Misc::CommandFinishTurn, this));
	m_callbackStack.push(std::bind(&U3Misc::CommandReadyWeapon, this));
}

bool U3Misc::NegateTimeCallback()
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

	NegateTime(m_chNum);

	return false;
}

void U3Misc::NegateTime(short chnum)
{
	short rosNum;

	rosNum = m_Party[6 + chnum];
	if (m_Player[rosNum][39] < 1)
	{
		m_scrollArea->UPrintMessage(67);
		return;
	}
	m_Player[rosNum][39]--;
	m_gTimeNegate = 10;
}

bool U3Misc::CommandNegateTime()
{
	if (m_callbackStack.size() > 0)
	{
		m_callbackStack.pop();
	}
	m_scrollArea->UPrintMessage(74);
	m_inputType = InputType::Transact;
	m_scrollArea->blockPrompt(true);
	m_callbackStack.push(std::bind(&U3Misc::NegateTimeCallback, this));
	m_callbackStack.push(std::bind(&U3Misc::ProcessEventCallback, this));

	return false;
}

void U3Misc::NegateTime()
{
	m_callbackStack.push(std::bind(&U3Misc::CommandFinishTurn, this));
	m_callbackStack.push(std::bind(&U3Misc::CommandNegateTime, this));
}

bool U3Misc::CommandModifyOrder()
{
	if (m_callbackStack.size() > 0)
	{
		m_callbackStack.pop();
	}
	m_scrollArea->UPrintMessage(70);
	m_inputType = InputType::Transact;
	m_scrollArea->blockPrompt(true);
	m_callbackStack.push(std::bind(&U3Misc::ModifyOrderCallback, this));
	m_callbackStack.push(std::bind(&U3Misc::ProcessEventCallback, this));

	return false;
}

bool U3Misc::ModifyOrderCallback()
{
	if (m_callbackStack.size() > 0)
	{
		m_callbackStack.pop();
	}

	m_chNum = m_input_num;
	if (m_chNum < 0 || m_chNum > 3)
	{
		m_scrollArea->UPrintMessage(71);
		return false;
	}

	std::string dispString(std::to_string(m_chNum + 1) + std::string("\n"));
	m_scrollArea->UPrintWin(dispString);
	if (m_Party[6 + m_chNum] == 0)
	{
		m_scrollArea->UPrintMessage(41);
		return false;
	}
	m_scrollArea->UPrintMessage(72);

	m_inputType = InputType::Transact;
	m_callbackStack.push(std::bind(&U3Misc::ModifyOrderCallback1, this));
	m_callbackStack.push(std::bind(&U3Misc::ProcessEventCallback, this));

	return false;
}

bool U3Misc::ModifyOrderCallback1()
{
	if (m_callbackStack.size() > 0)
	{
		m_callbackStack.pop();
	}

	if (m_input_num < 0 || m_input_num > 3 || m_input_num == m_chNum || m_Party[6 + m_input_num] == 0)
	{
		m_scrollArea->UPrintMessage(71);
		return false;
	}

	std::string dispString(std::to_string(m_input_num + 1) + std::string("\n"));
	m_scrollArea->UPrintWin(dispString);

	unsigned char temp;
	temp = m_Party[6 + m_chNum];
	m_Party[6 + m_chNum] = m_Party[6 + m_input_num];
	m_Party[6 + m_input_num] = temp;

	return false;
}

void U3Misc::ModifyOrder()
{
	m_callbackStack.push(std::bind(&U3Misc::CommandFinishTurn, this));
	m_callbackStack.push(std::bind(&U3Misc::CommandModifyOrder, this));
}

bool U3Misc::CommandHandEquip()
{
	if (m_callbackStack.size() > 0)
	{
		m_callbackStack.pop();
	}

	m_scrollArea->UPrintMessage(51);

	m_inputType = InputType::Transact;
	m_callbackStack.push(std::bind(&U3Misc::HandEquipCallback, this));
	m_callbackStack.push(std::bind(&U3Misc::ProcessEventCallback, this));

	return false;
}

void U3Misc::HandEquip()
{
	m_callbackStack.push(std::bind(&U3Misc::CommandFinishTurn, this));
	m_callbackStack.push(std::bind(&U3Misc::CommandHandEquip, this));
}


bool U3Misc::HandEquipCallback()
{
	if (m_callbackStack.size() > 0)
	{
		m_callbackStack.pop();
	}

	m_chNum = m_input_num;
	if (m_chNum < 0 || m_chNum > 3)
	{
		m_scrollArea->UPrintMessage(71);
		return false;
	}

	std::string dispString(std::to_string(m_chNum + 1) + std::string("\n"));
	m_scrollArea->UPrintWin(dispString);
	if (m_Party[6 + m_chNum] == 0)
	{
		m_scrollArea->UPrintMessage(41);
		return false;
	}

	if (!m_Player[m_Party[6 + m_chNum]][17])
	{
		m_scrollArea->UPrintMessage(41);
		return false;
	}

	m_scrollArea->UPrintMessage(52);

	m_inputType = InputType::Transact;
	m_callbackStack.push(std::bind(&U3Misc::HandEquipCallback1, this));
	m_callbackStack.push(std::bind(&U3Misc::ProcessEventCallback, this));

	return false;
}

bool U3Misc::HandEquipCallback1()
{
	if (m_callbackStack.size() > 0)
	{
		m_callbackStack.pop();
	}
	m_opnum2 = m_input_num;
	if (m_opnum2 < 0 || m_opnum2 > 3)
	{
		m_scrollArea->UPrintMessage(71);
		return false;
	}

	std::string dispString(std::to_string(m_opnum2 + 1) + std::string("\n"));
	m_scrollArea->UPrintWin(dispString);
	if (m_Party[6 + m_opnum2] == 0)
	{
		m_scrollArea->UPrintMessage(41);
		return false;
	}

	if (!m_Player[m_Party[6 + m_opnum2]][17])
	{
		m_scrollArea->UPrintMessage(41);
		return false;
	}
	if (m_chNum == m_opnum2)
	{
		return false;
	}
	m_scrollArea->UPrintMessage(53);

	m_inputType = InputType::LetterImmediate;
	m_callbackStack.push(std::bind(&U3Misc::HandEquipCallback2, this));
	m_callbackStack.push(std::bind(&U3Misc::ProcessEventCallback, this));

	return false;
}

bool U3Misc::HandEquipCallback2()
{
	if (m_callbackStack.size() > 0)
	{
		m_callbackStack.pop();
	}
	m_scrollArea->UPrintWin(m_input);
	m_scrollArea->UPrintWin("\n");

	if (m_input.empty())
	{
		m_scrollArea->UPrintMessage(59);
		return false;
	}

	char item = m_input[0];
	m_input.clear();
	m_scrollArea->setInputString("");
	switch (item)
	{
	case 'F':
		m_scrollArea->UPrintMessage(54);
		setInputTypeNum(std::bind(&U3Misc::handFoodCallback, this), 4);
		break;
	case 'G':
		m_scrollArea->UPrintMessage(54);
		setInputTypeNum(std::bind(&U3Misc::handGoldCallback, this), 4);
		break;
	case 'E':
		m_scrollArea->UPrintMessage(58);
		m_inputType = InputType::GuildVendor;
		m_scrollArea->setInput(true);
		m_callbackStack.push(std::bind(&U3Misc::handItemCallback, this));
		m_callbackStack.push(std::bind(&U3Misc::ProcessEventCallback, this));
		break;
	case 'W':
		m_opnum = 'P';
		m_scrollArea->UPrintMessage(61);
		setInputTypeRestricted(std::bind(&U3Misc::handWeaponCallback, this), 'B');
		break;
	case 'A':
		m_opnum = 'H';
		m_scrollArea->UPrintMessage(62);
		setInputTypeRestricted(std::bind(&U3Misc::handArmorCallback, this), 'B');
		break;
	default:
		m_scrollArea->UPrintMessage(59);
		break;
	}

	return false;
}

bool U3Misc::handWeaponCallback()
{
	if (m_callbackStack.size() > 0)
	{
		m_callbackStack.pop();
	}
	m_scrollArea->setInputString(m_input);
	m_scrollArea->setInput(false);
	m_inputType = InputType::Default;
	m_scrollArea->UPrintWin("\n");
	if (m_input.empty())
	{
		m_scrollArea->UPrintMessage(59);
		return false;
	}
	if (m_input[0] < 'B' || m_input[0] > 'P')
	{
		m_scrollArea->UPrintMessage(59);
		return false;
	}
	short rosNum1 = m_Party[6 + m_chNum];
	short rosNum2 = m_Party[6 + m_opnum2];

	if (m_Player[rosNum1][48] == m_input_num && m_Player[rosNum1][48 + m_input_num] < 2)
	{
		m_scrollArea->UPrintMessage(63);
		return false;
	}
	if (m_Player[rosNum1][48 + m_input_num] == 0)
	{
		m_scrollArea->UPrintMessage(59);
		return false;
	}
	m_Player[rosNum1][48 + m_input_num]--;
	m_Player[rosNum2][48 + m_input_num]++;

	if (m_Player[rosNum2][48 + m_input_num] > 99)
	{
		m_Player[rosNum2][48 + m_input_num] = 99;
	}
	m_scrollArea->UPrintMessage(57);
	return false;
}

bool U3Misc::handArmorCallback()
{
	if (m_callbackStack.size() > 0)
	{
		m_callbackStack.pop();
	}
	m_scrollArea->setInputString(m_input);
	m_scrollArea->setInput(false);
	m_inputType = InputType::Default;
	m_scrollArea->UPrintWin("\n");
	if (m_input.empty())
	{
		m_scrollArea->UPrintMessage(59);
		return false;
	}
	if (m_input[0] < 'B' || m_input[0] > 'H')
	{
		m_scrollArea->UPrintMessage(59);
		return false;
	}
	short rosNum1 = m_Party[6 + m_chNum];
	short rosNum2 = m_Party[6 + m_opnum2];

	if (m_Player[rosNum1][40] == m_input_num && m_Player[rosNum1][40 + m_input_num] < 2)
	{
		m_scrollArea->UPrintMessage(63);
		return false;
	}
	if (m_Player[rosNum1][40 + m_input_num] == 0)
	{
		m_scrollArea->UPrintMessage(59);
		return false;
	}
	m_Player[rosNum1][40 + m_input_num]--;
	m_Player[rosNum2][40 + m_input_num]++;

	if (m_Player[rosNum2][40 + m_input_num] > 99)
	{
		m_Player[rosNum2][40 + m_input_num] = 99;
	}
	m_scrollArea->UPrintMessage(57);
	return false;
}

bool U3Misc::handFoodCallback()
{
	if (m_callbackStack.size() > 0)
	{
		m_callbackStack.pop();
	}
	m_scrollArea->UPrintWin("\n");
	short rosNum1 = m_Party[6 + m_chNum];
	short rosNum2 = m_Party[6 + m_opnum2];
	short fromAmount = (m_Player[rosNum1][32] * 100) + m_Player[rosNum1][33];
	short toAmount = (m_Player[rosNum2][32] * 100) + m_Player[rosNum2][33];
	if (m_input_num == 0)
	{
		return false;
	}
	if (m_input_num > fromAmount)
	{
		m_scrollArea->UPrintMessage(55);
		return false;
	}
	if (toAmount + m_input_num > 9999)
	{
		m_scrollArea->UPrintMessage(56);
		return false;
	}
	fromAmount -= m_input_num;
	toAmount += m_input_num;
	m_Player[rosNum1][32] = fromAmount / 100;
	m_Player[rosNum1][33] = fromAmount - (m_Player[rosNum1][32] * 100);
	m_Player[rosNum2][32] = toAmount / 100;
	m_Player[rosNum2][33] = toAmount - (m_Player[rosNum2][32] * 100);

	m_scrollArea->UPrintMessage(57);

	return false;
}

bool U3Misc::handGoldCallback()
{
	if (m_callbackStack.size() > 0)
	{
		m_callbackStack.pop();
	}
	m_scrollArea->UPrintWin("\n");
	short rosNum1 = m_Party[6 + m_chNum];
	short rosNum2 = m_Party[6 + m_opnum2];

	short fromAmount = (m_Player[rosNum1][35] * 256) + m_Player[rosNum1][36];
	short toAmount = (m_Player[rosNum2][35] * 256) + m_Player[rosNum2][36];
	if (m_input_num == 0)
	{
		return false;
	}
	if (m_input_num > fromAmount)
	{
		m_scrollArea->UPrintMessage(55);
		return false;
	}
	if (toAmount + m_input_num > 9999)
	{
		m_scrollArea->UPrintMessage(56);
		return false;
	}
	fromAmount -= m_input_num;
	toAmount += m_input_num;
	m_Player[rosNum1][35] = fromAmount / 256;
	m_Player[rosNum1][36] = fromAmount - (m_Player[rosNum1][35] * 256);
	m_Player[rosNum2][35] = toAmount / 256;
	m_Player[rosNum2][36] = toAmount - (m_Player[rosNum2][35] * 256);

	m_scrollArea->UPrintMessage(57);

	return false;
}

bool U3Misc::handItemCallback()
{
	if (m_callbackStack.size() > 0)
	{
		m_callbackStack.pop();
	}
	m_scrollArea->UPrintWin("\n");
	if (m_input_num < 0)
	{
		return false;
	}
	m_opnum = 0;
	
	if (m_input_num == 0) // T
	{
		m_opnum = 15;
	}
	else if (m_input_num == 1) // K
	{
		m_opnum = 38;
	}
	else if (m_input_num == 2) // P
	{
		m_opnum = 39;
	}
	else if (m_input_num == 3) // G
	{
		m_opnum = 37;
	}

	if (m_opnum == 0)
	{
		m_scrollArea->UPrintMessage(59);
		return false;
	}
	m_scrollArea->UPrintMessage(60);
	setInputTypeNum(std::bind(&U3Misc::handItemCallback1, this), 2);

	return false;
}

bool U3Misc::handItemCallback1()
{
	if (m_callbackStack.size() > 0)
	{
		m_callbackStack.pop();
	}
	m_scrollArea->UPrintWin("\n");
	if (m_input_num == 0)
	{
		return false;
	}
	short rosNum1 = m_Party[6 + m_chNum];
	short rosNum2 = m_Party[6 + m_opnum2];

	if (m_input_num > m_Player[rosNum1][m_opnum])
	{
		m_scrollArea->UPrintMessage(55);
		return false;
	}

	if (m_input_num + m_Player[rosNum2][m_opnum] > 99)
	{
		m_scrollArea->UPrintMessage(56);
		return false;
	}

	m_Player[rosNum1][m_opnum] -= m_input_num;
	m_Player[rosNum2][m_opnum] += m_input_num;
	if (m_Player[rosNum2][m_opnum] > 99)
	{
		m_Player[rosNum2][m_opnum] = 99;
	}

	m_scrollArea->UPrintMessage(57);

	return false;
}

void U3Misc::Attack()
{
	m_callbackStack.push(std::bind(&U3Misc::CommandFinishTurn, this));
	m_callbackStack.push(std::bind(&U3Misc::CommandAttack, this));
}

bool U3Misc::CommandAttack()
{
	if (m_callbackStack.size() > 0)
	{
		m_callbackStack.pop();
	}

	m_scrollArea->UPrintMessage(28);
	m_inputType = InputType::GetDirection;
	m_callbackStack.push(std::bind(&U3Misc::AttackCallback, this));
	m_callbackStack.push(std::bind(&U3Misc::ProcessEventCallback, this));

	return false;
}

bool U3Misc::AttackCallback()
{
	if (m_callbackStack.size() > 0)
	{
		m_callbackStack.pop();
	}
	short monNum;
	monNum = MonsterHere(m_xs, m_ys);
	if (monNum == 255)
	{
		NotHere();
		return false;
	}
	AttackCode(monNum);
	return false;
}
