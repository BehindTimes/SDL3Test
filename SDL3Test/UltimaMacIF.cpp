#include <SDL3/SDL_video.h>
#include <SDL3/SDL.h>
#include <stdexcept>
#include <string>
#include "UltimaMacIF.h"
#include "U3Misc.h"
#include "U3ScrollArea.h"
#include "UltimaGraphics.h"
#include "UltimaIncludes.h"
#include "U3Utilities.h"
#include "UltimaDungeon.h"
#include "UltimaSpellCombat.h"
#include "U3Resources.h"
#include "UltimaSpellCombat.h"

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

void CursorUpdate(float mousH, float mousV) // figure what cursor to show where
{
	SDL_FRect topOfRect{};
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

	tileSiz = (short)m_resources->m_blockSize * 2;
	tilesWidth = (short)m_resources->m_blockSize * 24;

	if (m_misc->m_Party[2] == 1) // is in dungeon
	{
	}
	else // not a dungeon
	{
		mouseTileX = (short)(mousH - (short)m_resources->m_blockSize) / tileSiz;
		mouseTileY = (short)(mousV - (short)m_resources->m_blockSize) / tileSiz;
		cX = cY = 5;
		if (m_misc->m_Party[3] == 0x80)
		{   // in combat
			cX = m_misc->m_CharX[m_spellCombat->m_gChnum];
			cY = m_misc->m_CharY[m_spellCombat->m_gChnum];
		}

		if (cX == mouseTileX && cY == mouseTileY && m_misc->m_Party[2] != 0x80)
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