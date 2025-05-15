#include <SDL3/SDL_video.h>
#include <SDL3/SDL.h>
#include <stdexcept>
#include <string>

#include "UltimaDungeon.h"
#include "UltimaGraphics.h"
#include "UltimaIncludes.h"
#include "UltimaMacIF.h"
#include "UltimaSpellCombat.h"
#include "U3Misc.h"
#include "U3Resources.h"
#include "U3ScrollArea.h"
#include "U3Utilities.h"

extern SDL_Window* window;
extern std::unique_ptr<U3Resources> m_resources;
extern std::unique_ptr<U3Misc> m_misc;
extern std::unique_ptr<U3Graphics> m_graphics;
extern std::unique_ptr<U3ScrollArea> m_scrollArea;
extern std::unique_ptr<U3Utilities> m_utilities;
extern std::unique_ptr<UltimaDungeon> m_dungeon;
extern std::unique_ptr<UltimaSpellCombat> m_spellCombat;

extern short screenOffsetX;
extern short screenOffsetY;

int gCurCursor;
Uint32 gCurMouseDir = 0;
Uint32 gTransactMouseDir = 0;
short gMouseState = 0;


static bool isInRect(float mousH, float mousV, SDL_FRect topOfRect)
{
	if (topOfRect.x <= mousH && mousH <= topOfRect.x + topOfRect.w &&
		topOfRect.y <= mousV && mousV <= topOfRect.y + topOfRect.h)
	{
		return true;
	}
	return false;
}

void HandleMouseDown(bool mouseclicked)
{
	SDL_FRect topOfRect{};
	short cleric;
	short left;

	short tilesWidth = (short)m_resources->m_blockSize * 24;
	float mousH;
	float mousV;
	short num = 0;
	SDL_GetMouseState(&mousH, &mousV);

	topOfRect.x = tilesWidth;
	topOfRect.y = (float)m_resources->m_blockSize;
	topOfRect.w = (float)(m_resources->m_blockSize * 15) - 1;
	topOfRect.h = (float)(m_resources->m_blockSize * 3) - 1;

	if (mouseclicked)
	{
		if (isInRect(mousH, mousV, topOfRect))
		{
			num = 1;
		}
		topOfRect.y += m_resources->m_blockSize + (m_resources->m_blockSize * 3);
		if (isInRect(mousH, mousV, topOfRect))
		{
			num = 2;
		}
		topOfRect.y += m_resources->m_blockSize + (m_resources->m_blockSize * 3);
		if (isInRect(mousH, mousV, topOfRect))
		{
			num = 3;
		}
		topOfRect.y += m_resources->m_blockSize + (m_resources->m_blockSize * 3);
		if (isInRect(mousH, mousV, topOfRect))
		{
			num = 4;
		}
		if (num > 0)
		{
			if (m_misc->m_inputType == InputType::Transact)
			{
				if (m_misc->m_inTransaction)
				{
					m_misc->m_InputDeque.push_back(gTransactMouseDir);
				}
				m_misc->m_InputDeque.push_back(SDLK_1 + (num - 1));
			}
			else
			{
				if (m_graphics->m_curMode != U3GraphicsMode::Combat)
				{
					m_misc->m_InputDeque.push_back(SDLK_1 + (num - 1));
					m_misc->m_InputDeque.push_back(SDLK_Z);
				}
				else
				{
					m_misc->m_InputDeque.push_back(SDLK_Z);
				}
			}
			return;
		}
	}

	if (gCurCursor >= 0)
	{
		if (mouseclicked)
		{
			switch (gCurCursor)
			{
			case 0:
				m_misc->m_InputDeque.push_back(SDLK_SPACE);
				break;
			case 1:
				m_misc->m_InputDeque.push_back(SDLK_UP);
				break;
			case 2:
				m_misc->m_InputDeque.push_back(SDLK_DOWN);
				break;
			case 3:
				m_misc->m_InputDeque.push_back(SDLK_LEFT);
				break;
			case 4:
				m_misc->m_InputDeque.push_back(SDLK_RIGHT);
				break;
			case 5:
				m_misc->m_InputDeque.push_back(gCurMouseDir);
				m_misc->m_InputDeque.push_back(SDLK_A);
				break;
			case 6:
				m_misc->m_inTransaction = true;
				gTransactMouseDir = gCurMouseDir;
				m_misc->m_InputDeque.push_back(SDLK_T);
				break;
			case 7:    // open chest
				cleric = 0;
				num = 0;
				while (!cleric && num < 4)
				{
					left = m_misc->m_Party[num + 6];
					if ((m_misc->m_Player[left][17] == 'G') || (m_misc->m_Player[left][17] == 'P'))
					{
						if (m_misc->m_Player[left][25] > 4)
						{
							if (m_misc->m_Player[left][23] == m_misc->m_careerTable[8] ||
								m_misc->m_Player[left][23] == m_misc->m_careerTable[10])
							{
								m_misc->m_InputDeque.push_back(SDLK_B);
								m_misc->m_InputDeque.push_back(SDLK_C);
								m_misc->m_InputDeque.push_back(SDLK_1 + num);
								m_misc->m_InputDeque.push_back(SDLK_C);
								cleric = 1;
							}
							if (m_misc->m_Player[left][23] == m_misc->m_careerTable[1] ||
								m_misc->m_Player[left][23] == m_misc->m_careerTable[4] ||
								m_misc->m_Player[left][23] == m_misc->m_careerTable[7])
							{
								m_misc->m_InputDeque.push_back(SDLK_B);
								m_misc->m_InputDeque.push_back(SDLK_1 + num);
								m_misc->m_InputDeque.push_back(SDLK_C);
								cleric = 1;
							}
						}
					}
					num++;
				}
				if (!cleric)
				{
					m_misc->m_InputDeque.push_back(SDLK_G);
				}
				break;
			case 8:
				m_misc->m_InputDeque.push_back(SDLK_E);
				break;
			case 9:
				m_misc->m_InputDeque.push_back(SDLK_B);
				break;
			case 10:
				m_misc->m_InputDeque.push_back(gCurMouseDir);
				m_misc->m_InputDeque.push_back(SDLK_F);
				break;
			case 11:
				m_misc->m_InputDeque.push_back(gCurMouseDir);
				m_misc->m_InputDeque.push_back(SDLK_U);
				break;
			case 12:
				m_misc->m_InputDeque.push_back(SDLK_X);
				break;
			case 13:
				break;
			case 14:
				m_misc->m_InputDeque.push_back(SDLK_K);
				break;
			case 15:
				m_misc->m_InputDeque.push_back(SDLK_D);
				break;
			case 16:
				m_misc->m_InputDeque.push_back(SDLK_LEFT);
				break;
			case 17:
				m_misc->m_InputDeque.push_back(SDLK_RIGHT);
				break;
			case 18:
				m_misc->m_InputDeque.push_back(SDLK_UP);
				break;
			case 19:
				m_misc->m_InputDeque.push_back(SDLK_DOWN);
				break;
			case 20:    // Ignite (cleric means who has a torch)
				cleric = -1;
				num = 0;
				while (cleric < 0 && num < 4)
				{
					left = m_misc->m_Party[num + 6];
					if ((m_misc->m_Player[left][17] == 'G') || (m_misc->m_Player[left][17] == 'P'))
					{
						if (m_misc->m_Player[left][15] > 0)
						{
							cleric = num;
						}
					}
					num++;
				}
				if (cleric > -1)
				{
					m_misc->m_InputDeque.push_back(SDLK_1 + cleric);
					m_misc->m_InputDeque.push_back(SDLK_I);
				}
				else   // no one had a torch, just press I and let 'em figure it out.
				{
					m_misc->m_InputDeque.push_back(SDLK_I);
				}
				break;
			case 22:
				m_misc->m_InputDeque.push_back(SDLK_KP_7);
				break;
			case 23:
				m_misc->m_InputDeque.push_back(SDLK_KP_9);
				break;
			case 24:
				m_misc->m_InputDeque.push_back(SDLK_KP_1);
				break;
			case 25:
				m_misc->m_InputDeque.push_back(SDLK_KP_3);
				break;
			default:
				break;
			}
		}
		else
		{
			// Only allow movement when holding down buttons.  Everything else requires a new click
			switch (gCurCursor)
			{
			case 1:
				m_misc->m_InputDeque.push_back(SDLK_UP);
				break;
			case 2:
				m_misc->m_InputDeque.push_back(SDLK_DOWN);
				break;
			case 3:
				m_misc->m_InputDeque.push_back(SDLK_LEFT);
				break;
			case 4:
				m_misc->m_InputDeque.push_back(SDLK_RIGHT);
				break;
			case 16:
				m_misc->m_InputDeque.push_back(SDLK_LEFT);
				break;
			case 17:
				m_misc->m_InputDeque.push_back(SDLK_RIGHT);
				break;
			case 18:
				m_misc->m_InputDeque.push_back(SDLK_UP);
				break;
			case 19:
				m_misc->m_InputDeque.push_back(SDLK_DOWN);
				break;
			case 22:
				m_misc->m_InputDeque.push_back(SDLK_KP_7);
				break;
			case 23:
				m_misc->m_InputDeque.push_back(SDLK_KP_9);
				break;
			case 24:
				m_misc->m_InputDeque.push_back(SDLK_KP_1);
				break;
			case 25:
				m_misc->m_InputDeque.push_back(SDLK_KP_3);
				break;
			default:
				break;
			}
		}
	}
}


void CursorUpdate(float mousH, float mousV) // figure what cursor to show where
{
	SDL_FRect topOfRect{};
	SDL_FRect gameRect{};
	short mouseTileX;
	short mouseTileY;
	short tileSiz;
	short tilesWidth;
	short cX;
	short cY;
	short cXBig;
	short cYBig;
	unsigned char value;
	int newCursor = -1;
	short offVal;
	bool aimedAt;
	unsigned char wpn;

	mousH -= screenOffsetX;
	mousV -= screenOffsetY;

	tileSiz = (short)m_resources->m_blockSize * 2;
	tilesWidth = (short)m_resources->m_blockSize * 24;
	gameRect.x = (float)m_resources->m_blockSize;
	gameRect.y = (float)m_resources->m_blockSize;
	gameRect.h = 22.0f * m_resources->m_blockSize;
	gameRect.w = 22.0f * m_resources->m_blockSize;

	SDL_Keycode keyEquiv[26] = { SDLK_SPACE, SDLK_UP,  SDLK_DOWN,  SDLK_LEFT, SDLK_RIGHT, SDLK_A, SDLK_T, SDLK_G, SDLK_E, SDLK_B, SDLK_F, SDLK_U, SDLK_X,
											  SDLK_Z, SDLK_K, SDLK_D, SDLK_LEFT, SDLK_RIGHT, SDLK_UP,  SDLK_DOWN,  SDLK_I, 0,   SDLK_KP_7, SDLK_KP_9, SDLK_KP_1, SDLK_KP_3 };

	if (m_resources->m_overrideImage == 9)
	{
	}
	else
	{
		if (isInRect(mousH, mousV, gameRect))
		{
			if (m_misc->m_Party[PARTY_LOCATION] == 1) // is in dungeon
			{
				if (m_misc->m_gTorch == 0 && m_dungeon->GetXYDng(m_misc->m_xs, m_misc->m_ys) != 3)   // no light but not on strange wind
				{
					newCursor = 20;    // ignite torch
				}
				else
				{
					gCurMouseDir = 0;
					value = m_dungeon->GetXYDng(m_misc->m_xpos, m_misc->m_ypos);
					if (mousH > mousV)
					{
						if ((tilesWidth - mousH) > mousV)
						{
							newCursor = 18;    // forward
							if ((value & 0x10) && mousV < ((short)m_resources->m_blockSize * 8))
							{
								newCursor = 14;    // klimb
							}
						}
						else
						{
							newCursor = 17;    // right
						}
					}
					else
					{
						if ((tilesWidth - mousH) > mousV)
						{
							newCursor = 16;    // left
						}
						else
						{
							newCursor = 19;    // backward
							if ((value < 128) && (value & 0x20) && mousV > ((short)m_resources->m_blockSize * 16))
							{
								newCursor = 15;    // descend
							}
							if ((value == 0x40) && mousV > ((short)m_resources->m_blockSize * 16))
							{
								newCursor = 7;    // get chest
							}
						}
					}
				}
			}
			else // not a dungeon
			{
				mouseTileX = (short)(mousH - (short)m_resources->m_blockSize) / tileSiz;
				mouseTileY = (short)(mousV - (short)m_resources->m_blockSize) / tileSiz;
				cX = cY = 5;
				if (m_misc->m_Party[PARTY_LOCATION] == 0x80)
				{   // in combat
					cX = m_misc->m_CharX[m_spellCombat->m_gChnum];
					cY = m_misc->m_CharY[m_spellCombat->m_gChnum];
				}

				if (cX == mouseTileX && cY == mouseTileY && m_misc->m_Party[PARTY_LOCATION] != 0x80)
				{
					value = m_misc->GetXYVal(m_misc->m_xpos, m_misc->m_ypos) / 2;
				}
				else
				{
					value = m_misc->GetXYTile(mouseTileX, mouseTileY);
				}

				topOfRect.x = cX * tileSiz + (float)m_resources->m_blockSize;
				topOfRect.w = tileSiz;
				topOfRect.y = cY * tileSiz + (float)m_resources->m_blockSize;
				topOfRect.h = tileSiz;
				cXBig = (cX - 5) * tileSiz;
				cYBig = (cY - 5) * tileSiz;
				if (mousH - cXBig > mousV - cYBig)
				{
					if ((tilesWidth - mousH) + cXBig > mousV - cYBig)
					{
						newCursor = 1;    // North
					}
					else {
						newCursor = 4;    // East
					}
				}
				else
				{
					if ((tilesWidth - mousH) + cXBig > mousV - cYBig)
					{
						newCursor = 3;    // West
					}
					else
					{
						newCursor = 2;    // South
					}
				}

				bool allowDiagonal;
				m_resources->GetPreference(U3PreferencesType::Allow_Diagonal, allowDiagonal);
				if (allowDiagonal)
				{
					offVal = (short)(m_resources->m_blockSize * 6);
					if (((mousV - cYBig) / m_resources->m_blockSize) > 12)
					{   // bottom half
						if ((((mousH - cXBig) - offVal) * 2 < tilesWidth - (mousV - cYBig)) &&
							((tilesWidth - (mousV - cYBig)) - offVal) * 2 < mousH - cXBig)
						{
							newCursor = 24;    // Southeast
						}
						if ((((tilesWidth - (mousH - cXBig)) - offVal) * 2 < tilesWidth - (mousV - cYBig)) &&
							((tilesWidth - (mousV - cYBig)) - offVal) * 2 < tilesWidth - (mousH - cXBig))
						{
							newCursor = 25;    // Southwest
						}
					}
					else                     // top half
					{
						if ((((mousH - cXBig) - offVal) * 2 < mousV - cYBig) && ((mousV - cYBig) - offVal) * 2 < mousH - cXBig)
						{
							newCursor = 22;    // Northwest
						}
						if ((((tilesWidth - (mousH - cXBig)) - offVal) * 2 < mousV - cYBig) &&
							((mousV - cYBig) - offVal) * 2 < tilesWidth - (mousH - cXBig))
						{
							newCursor = 23;    // Northeast
						}
					}
				}
				if (m_misc->m_Party[PARTY_ICON] != 1 && value >= 0x5C && value < 0x5E)
				{
					aimedAt = (cY == mouseTileY && (mouseTileX > cX - 2 && mouseTileX < cX + 2));    // only west or east!
					if (aimedAt)
					{
						gCurMouseDir = keyEquiv[newCursor];
						newCursor = 11;    // Key (unlock)
					}
				}

				if ((value >= 0x1A && value < 0x3E) || value == 0x7E || value >= 0xA0)   // a being; standard range or a variant tile.
				{
					gCurMouseDir = keyEquiv[newCursor];
					if (m_misc->m_Party[PARTY_LOCATION] == 0x80 && m_misc->m_inputType != InputType::GetDirection) // in combat & not GetDirection
					{
						aimedAt = (cX == mouseTileX || cY == mouseTileY);
						if (allowDiagonal)
						{
							aimedAt |= (m_utilities->Absolute(cX - mouseTileX) == m_utilities->Absolute(cY - mouseTileY));
						}
						if (aimedAt)
						{
							wpn = m_misc->m_Player[m_misc->m_Party[m_spellCombat->m_gChnum + 6]][48];
							if (wpn == 1 || wpn == 3 || wpn == 5 || wpn == 9 || wpn == 13)
							{
								newCursor = 5;
							}
							else
							{
								aimedAt = (cX == mouseTileX && (mouseTileY == cY - 1 || mouseTileY == cY + 1));
								if (aimedAt)
								{
									newCursor = 5;    // Attack
								}
								aimedAt = (cY == mouseTileY && (mouseTileX == cX - 1 || mouseTileX == cX + 1));
								if (aimedAt)
								{
									newCursor = 5;    // Attack
								}
								if (allowDiagonal)
								{
									if (mouseTileY > cY - 2 && mouseTileY < cY + 2 && mouseTileX > cX - 2 && mouseTileX < cX + 2)
									{
										newCursor = 5;    // Attack all 'round!
									}
								}
							}
						}
					}
					if (m_misc->m_Party[PARTY_LOCATION] > 1 && m_misc->m_Party[PARTY_LOCATION] < 4 && m_misc->m_inputType != InputType::GetDirection) // indoors & not GetDir
					{
						if (cX == mouseTileX && (mouseTileY == cY - 1 || mouseTileY == cY + 1))
						{
							newCursor = 6;    // Transact
						}
						if (cY == mouseTileY && (mouseTileX == cX - 1 || mouseTileX == cX + 1))
						{
							newCursor = 6;    // Transact
						}
						if (allowDiagonal)
						{
							if (mouseTileY > cY - 2 && mouseTileY < cY + 2 && mouseTileX > cX - 2 && mouseTileX < cX + 2)
							{
								newCursor = 6;    // Transact all 'round!
							}
						}
						if (cX == mouseTileX && mouseTileY == cY - 2 && m_misc->GetXYTile(cX, cY - 1) > 0x48)
						{
							newCursor = 6;    // Transact
						}
						if (cX == mouseTileX && mouseTileY == cY + 2 && m_misc->GetXYTile(cX, cY + 1) > 0x48)
						{
							newCursor = 6;    // Transact
						}
						if (cY == mouseTileY && mouseTileX == cX - 2 && m_misc->GetXYTile(cX - 1, cY) > 0x48)
						{
							newCursor = 6;    // Transact
						}
						if (cY == mouseTileY && mouseTileX == cX + 2 && m_misc->GetXYTile(cX + 1, cY) > 0x48)
						{
							newCursor = 6;    // Transact
						}
					}
					if (m_misc->m_Party[PARTY_ICON] == 0x16 && (m_misc->m_Party[PARTY_LOCATION] == 0 || m_misc->m_Party[PARTY_LOCATION] == 0xFF)) // Sosaria/Ambrosia Ship
					{
						aimedAt = ((cX == mouseTileX && (mouseTileY > cY - 4 && mouseTileY < cY + 4)) ||
							(cY == mouseTileY && (mouseTileX > cX - 4 && mouseTileX < cX + 4)));
						if (!aimedAt && allowDiagonal)
						{
							aimedAt = ((m_utilities->Absolute(cX - mouseTileX) == m_utilities->Absolute(cY - mouseTileY)) &&
								(mouseTileX > cX - 4 && mouseTileX < cX + 4 && mouseTileY > cY - 4 && mouseTileY < cY + 4));
						}
						if (aimedAt)
						{
							newCursor = 10;    // Fire Cannon
						}
					}
					if (m_misc->m_Party[PARTY_ICON] != 0x16 && (m_misc->m_Party[PARTY_LOCATION] == 0 || m_misc->m_Party[PARTY_LOCATION] == 0xFF)) // Sosaria/Ambrosia non-ship
					{
						aimedAt = (cX == mouseTileX && (mouseTileY > cY - 2 && mouseTileY < cY + 2));
						aimedAt |= (cY == mouseTileY && (mouseTileX > cX - 2 && mouseTileX < cX + 2));
						if (allowDiagonal)
						{
							aimedAt |= (mouseTileY > cY - 2 && mouseTileY < cY + 2 && mouseTileX > cX - 2 && mouseTileX < cX + 2);
						}
						if (aimedAt)
						{
							newCursor = 5;
						}
					}
				}

				
				if (isInRect(mousH, mousV, topOfRect))
				{
					newCursor = 0; // nothing
					if (m_misc->m_Party[PARTY_ICON] == 0x14 || m_misc->m_Party[PARTY_ICON] == 0x16)
					{
						newCursor = 12; // X-it
					}
					if (value > 0x11 && value < 0x14)
					{
						newCursor = 7; // Get Chest
					}
					if ((value > 0x09 && value < 0x10) || value == 0x7C)
					{
						newCursor = 8; // Enter
					}
					if (value == 0x14 || value == 0x16)
					{
						newCursor = 9; // Board/Mount
					}
					if (m_misc->m_Party[PARTY_LOCATION] == 0x80)
					{
						newCursor = 0;
					}
				}
			}
		}
		else
		{
			newCursor = -1; /* just arrow */
			//if (gUpdateWhere == 3 || gUpdateWhere == 4 || gUpdateWhere == 8 || gUpdateWhere == 9 || gUpdateWhere > 19)
			{
				//if (gMouseState != 4)
				{
					topOfRect.x = tilesWidth;
					topOfRect.y = (float)m_resources->m_blockSize;
					topOfRect.w = (float)(m_resources->m_blockSize * 15) - 1;
					topOfRect.h = (float)(m_resources->m_blockSize * 3) - 1;

					if (isInRect(mousH, mousV, topOfRect))
					{
						newCursor = 13;
						if (m_misc->m_Party[PARTY_LOCATION] == 0x80 && m_spellCombat->m_gChnum != 0)
						{
							newCursor = -1;
						}
					}
					topOfRect.y += m_resources->m_blockSize + (m_resources->m_blockSize * 3);
					if (isInRect(mousH, mousV, topOfRect))
					{
						newCursor = 13;
						if (m_misc->m_Party[PARTY_LOCATION] == 0x80 && m_spellCombat->m_gChnum != 1)
						{
							newCursor = -1;
						}
					}
					topOfRect.y += m_resources->m_blockSize + (m_resources->m_blockSize * 3);
					if (isInRect(mousH, mousV, topOfRect))
					{
						newCursor = 13;
						if (m_misc->m_Party[PARTY_LOCATION] == 0x80 && m_spellCombat->m_gChnum != 2)
						{
							newCursor = -1;
						}
					}
					topOfRect.y += m_resources->m_blockSize + (m_resources->m_blockSize * 3);
					if (isInRect(mousH, mousV, topOfRect))
					{
						newCursor = 13;
						if (m_misc->m_Party[PARTY_LOCATION] == 0x80 && m_spellCombat->m_gChnum != 3)
						{
							newCursor = -1;
						}
					}
				}
			}
		}
	}
	if (m_resources->HasAlert())
	{
		newCursor = -1;
	}
	if (m_graphics->m_staydead)
	{
		newCursor = 21;
	}

	ReflectNewCursor(newCursor);
}

void ReflectNewCursor(int newCursor)
{
	if (newCursor != gCurCursor)
	{
		if (newCursor >= 0)
		{
			SDL_SetCursor(m_resources->m_cursors[newCursor]);
		}
		else
		{
			SDL_SetCursor(SDL_GetDefaultCursor());
		}

		gCurCursor = newCursor;
		SDL_Cursor* cursorName;

		switch (gCurCursor)
		{
		case 0: cursorName = m_resources->m_cursors[CL_PASS]; break;
		case 1: cursorName = m_resources->m_cursors[CL_NORTH]; break;
		case 2: cursorName = m_resources->m_cursors[CL_SOUTH]; break;
		case 3: cursorName = m_resources->m_cursors[CL_WEST]; break;
		case 4: cursorName = m_resources->m_cursors[CL_EAST]; break;
		case 5: cursorName = m_resources->m_cursors[CL_ATTACK]; break;
		case 6: cursorName = m_resources->m_cursors[CL_TALK]; break;
		case 7: cursorName = m_resources->m_cursors[CL_CHEST]; break;
		case 8: cursorName = m_resources->m_cursors[CL_ENTER]; break;
		case 9: cursorName = m_resources->m_cursors[CL_BOARD]; break;
		case 10: cursorName = m_resources->m_cursors[CL_CANNON]; break;
		case 11: cursorName = m_resources->m_cursors[CL_UNLOCK]; break;
		case 12: cursorName = m_resources->m_cursors[CL_EXIT]; break;
		case 13: cursorName = m_resources->m_cursors[CL_LOOK]; break;
		case 14: cursorName = m_resources->m_cursors[CL_UP]; break;
		case 15: cursorName = m_resources->m_cursors[CL_DOWN]; break;
		case 16: cursorName = m_resources->m_cursors[CL_LEFT]; break;
		case 17: cursorName = m_resources->m_cursors[CL_RIGHT]; break;
		case 18: cursorName = m_resources->m_cursors[CL_FORWARD]; break;
		case 19: cursorName = m_resources->m_cursors[CL_BACKWARDS]; break;
		case 20: cursorName = m_resources->m_cursors[CL_TORCH]; break;
		case 21: cursorName = m_resources->m_cursors[CL_DEAD]; break;
		case 22: cursorName = m_resources->m_cursors[CL_NORTHWEST]; break;
		case 23: cursorName = m_resources->m_cursors[CL_NORTHEAST]; break;
		case 24: cursorName = m_resources->m_cursors[CL_SOUTHWEST]; break;
		case 25: cursorName = m_resources->m_cursors[CL_SOUTHEAST]; break;
		case 26: cursorName = m_resources->m_cursors[CL_USE]; break;
		default:
			cursorName = nullptr;
			break;
		}

		if (cursorName)
		{
			SDL_SetCursor(cursorName);
		}
		else
		{
			SDL_SetCursor(SDL_GetDefaultCursor());
		}
	}
}