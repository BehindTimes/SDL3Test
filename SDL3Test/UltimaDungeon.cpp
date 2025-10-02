#include <SDL3_image/SDL_image.h>

#include "UltimaDungeon.h"
#include "UltimaGraphics.h"
#include "UltimaIncludes.h"
#include "UltimaSound.h"
#include "UltimaSpellCombat.h"
#include "U3Misc.h"
#include "U3Resources.h"
#include "U3ScrollArea.h"
#include "U3Utilities.h"

extern std::unique_ptr<U3Resources> m_resources;
extern std::unique_ptr<U3Graphics> m_graphics;
extern std::unique_ptr<U3ScrollArea> m_scrollArea;
extern std::unique_ptr<U3Utilities> m_utilities;
extern std::unique_ptr<UltimaSpellCombat> m_spellCombat;
extern std::unique_ptr<U3Misc> m_misc;
extern std::unique_ptr<U3Audio> m_audio;

/*                           0    1    2    3    4    5    6    7    8    9   10   11   12   13   14   15   16   17   18   19   20   21   22   23   24   25   26   27   28   29   30   31 */
const short     dOfX[37] = { 600,   0,   0,1200,1200,1200, 300,   0,   0, 300,1200,1200,1200,1200,1200, 300, 300,   0,   0, 300, 300,1200,1200,1200,1200,1200,1200,1200, 300,   0,   0, 300 };
const short     dOfY[37] = { 0,   0,   0, -64, -64, -64, -64,   0,   0, -64,  34,  34,  34,  34,  34,  34, -64,   0,   0, -64,  34,  84,  84,  84,  84,  84,  84,  84, -64,   0,   0, -64 };
const bool   dMask[37] = { 0,   1,   1,   0,   0,   0,   1,   1,   1,   1,   0,   0,   0,   0,   0,   1,   1,   1,   1,   1,   1,   0,   0,   0,   0,   0,   0,   0,   1,   1,   1,   1 };

/*                          0   1   2   3   4   5   6   7   8   9  10  11  12  13  14  15  16  17  18  19  20  21  22  23  24  25  26  27  28  29  30  31  32  33  34  35 */
const short     dngL[37] = { 0,  0,225,  0, 75,225,  0, 75,187,225,  0, 75,113,187,225,  0, 75,113,170,188,226,  0, 75,112,130,170,188,225,112,130,152,168,  0,300,375,377 };
const short     dngT[37] = { 0,  0,  0, 64, 64, 64, 64, 65, 65, 64, 96, 96, 96, 96, 96, 96, 96, 97, 97, 96, 96,112,112,112,112,112,112,112,112,112,112,112,  0,208,208,208 };
const short     dngR[37] = { 300, 75,300, 75,225,300, 75,113,225,300, 75,113,187,225,300, 74,112,130,187,225,300, 75,112,130,170,188,225,300,132,148,170,188,300,375,377,381 };
const short     dngB[37] = { 256,256,256,192,192,192,192,191,191,192,160,160,160,160,160,160,160,159,159,160,160,144,144,144,144,144,144,144,144,144,144,144,256,240,212,256 };
/*                 0   1   2   3   4   5   6   7   8   9  10  11  12  13  14  15  16  17  18  19  20 */
short   chL[21] = { 112,  0,264, 56,131,226,  0, 75,207,272, 66,103,141,188,226, 28, 75,113,179,216,254 };
short   chT[21] = { 208,208,208,168,168,168,168,168,168,168,148,148,148,148,148,148,148,148,148,148,148 };
short   chR[21] = { 186, 36,300, 74,169,244, 18, 93,225,300, 74,112,159,197,234, 46, 84,121,187,225,272 };
short   chB[21] = { 240,240,240,184,184,184,184,184,184,184,156,156,156,156,156,156,156,156,156,156,156 };
short   chSide[21] = { 0,2,1,1,0,2,2,2,1,1,1,1,0,2,2,0,2,2,1,1,0 };

UltimaDungeon::UltimaDungeon() :
	m_dungeonLevel(0),
	m_texDungeonShapes(nullptr),
	m_forceRedraw(true),
	m_texDungeonPort(nullptr),
	m_gExitDungeon(false),
	m_isChunk0Wall0(false)
{
	m_HeadX[0] = 0;
	m_HeadX[1] = 1;
	m_HeadX[2] = 0;
	m_HeadX[3] = -1;

	m_HeadY[0] = -1;
	m_HeadY[1] = 0;
	m_HeadY[2] = 1;
	m_HeadY[3] = 0;

	for (int index = 0; index < 32; ++index)
	{
		m_texDungeonDoors[index] = nullptr;
	}
	for (int index = 0; index < 21; ++index)
	{
		m_texSecrets[index] = nullptr;
	}
}

UltimaDungeon::~UltimaDungeon()
{
	for (int index = 0; index < 32; ++index)
	{
		if (m_texDungeonDoors[index])
		{
			SDL_DestroyTexture(m_texDungeonDoors[index]);
			m_texDungeonDoors[index] = nullptr;
		}
	}
	for (int index = 0; index < 21; ++index)
	{
		if (m_texSecrets[index])
		{
			SDL_DestroyTexture(m_texSecrets[index]);
			m_texSecrets[index] = nullptr;
		}
	}

	if (m_texDungeonPort)
	{
		SDL_DestroyTexture(m_texDungeonPort);
		m_texDungeonPort = nullptr;
	}
	if (m_texDungeonShapes)
	{
		SDL_DestroyTexture(m_texDungeonShapes);
		m_texDungeonShapes = nullptr;
	}
}

void UltimaDungeon::LetterCommand(SDL_Keycode key)
{
	switch (key)
	{
	case SDLK_C:
		m_misc->Cast(-1);
		break;
	case SDLK_D:
		Descend();
		break;
	case SDLK_G:
		m_misc->GetChest();
		break;
	case SDLK_H:
		m_misc->HandEquip();
		break;
	case SDLK_I:
		m_misc->Ignite();
		break;
	case SDLK_J:
		m_misc->JoinGold();
		break;
	case SDLK_K:
		Klimb();
		break;
	case SDLK_M:
		m_misc->ModifyOrder();
		break;
	case SDLK_N:
		m_misc->NegateTime();
		break;
	case SDLK_O:
		m_misc->OtherCommand(0);
		break;
	case SDLK_P:
		PeerGem();
		break;
	case SDLK_R:
		m_misc->ReadyWeapon(-1);
		break;
	case SDLK_W:
		m_misc->WearArmor();
		break;
	case SDLK_Y:
		m_misc->Yell();
		break;
	case SDLK_Z:
		m_misc->ZStats(0, 0);
		break;
	default:
		NotDngCmd();
		break;
	}
}

bool UltimaDungeon::HandleDefaultKeyPress(SDL_Keycode key)
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
	}
	return true;
}

bool UltimaDungeon::loadGraphics()
{
	std::filesystem::path currentPath = m_resources->m_exePath;
	currentPath /= ResourceLoc;
	currentPath /= ImagesLoc;
	currentPath /= "DungeonShapes.png";

	m_texDungeonShapes = IMG_LoadTexture(m_resources->m_renderer, currentPath.string().c_str());
	if (!m_texDungeonShapes)
	{
		return false;
	}
	SDL_SetTextureScaleMode(m_texDungeonShapes, SDL_SCALEMODE_NEAREST);

	/*currentPath = m_resources->m_exePath;
	currentPath /= ResourceLoc;
	currentPath /= ImagesLoc;
	currentPath /= "Fountain.jpg";

	m_texFountain = IMG_LoadTexture(m_resources->m_renderer, currentPath.string().c_str());
	if (!m_texFountain)
	{
		return false;
	}
	SDL_SetTextureScaleMode(m_texFountain, SDL_SCALEMODE_NEAREST);

	currentPath = m_resources->m_exePath;
	currentPath /= ResourceLoc;
	currentPath /= ImagesLoc;
	currentPath /= "Shrine.jpg";

	m_texShrine = IMG_LoadTexture(m_resources->m_renderer, currentPath.string().c_str());
	if (!m_texShrine)
	{
		return false;
	}
	SDL_SetTextureScaleMode(m_texShrine, SDL_SCALEMODE_NEAREST);

	currentPath = m_resources->m_exePath;
	currentPath /= ResourceLoc;
	currentPath /= ImagesLoc;
	currentPath /= "TimeLord.jpg";

	m_texTimeLord = IMG_LoadTexture(m_resources->m_renderer, currentPath.string().c_str());
	if (!m_texTimeLord)
	{
		return false;
	}

	SDL_SetTextureScaleMode(m_texTimeLord, SDL_SCALEMODE_NEAREST);

	currentPath = m_resources->m_exePath;
	currentPath /= ResourceLoc;
	currentPath /= ImagesLoc;
	currentPath /= "Rod.jpg";

	m_texRod = IMG_LoadTexture(m_resources->m_renderer, currentPath.string().c_str());

	if (!m_texRod)
	{
		return false;
	}

	SDL_SetTextureScaleMode(m_texRod, SDL_SCALEMODE_NEAREST);*/

	m_texDungeonPort = SDL_CreateTexture(m_resources->m_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 600, 512);

	createDoorPolygons();
	createTextureSecrets();

	return true;
}

void UltimaDungeon::renderLine(unsigned char* canvas, short x1, short y1, short x2, short y2)
{
	short pitch = 2400;

	if (x1 != x2)
	{
		if (x2 < x1)
		{
			short temp = y1;
			y1 = y2;
			y2 = temp;

			temp = x1;
			x1 = x2;
			x2 = temp;
		}

		float d = (float)(y2 - y1) / (float)(x2 - x1);
		float fy1 = y1;
		for (short i = x1; i < x2; i++)
		{
			if (x1 >= 600 || (short)fy1 >= 512)
			{
				x1++;
				fy1 +=d;
				continue;
			}

			canvas[(pitch * (short)fy1) + (x1 * 4) + ((std::endian::native == std::endian::big) ? 3 : 0)] = 0xFF;
			x1++;
			fy1 += d;
		}
	}
	else // just draw straight down
	{
		if (y2 < y1)
		{
			short temp = y1;
			y1 = y2;
			y2 = temp;
		}
		for (short i = y1; i < y2; i++)
		{
			if (y1 >= 512 || x2 >= 600)
			{
				break;
			}

			canvas[(pitch * i) + (x1 * 4) + ((std::endian::native == std::endian::big) ? 3 : 0)] = 0xFF;
		}
	}
}

int UltimaDungeon::getLeftMost(unsigned char* canvas, int startX, int endX)
{
	int endianbyte = std::endian::native == std::endian::big ? 3 : 0;
	for (int index = (startX * 4); index < (endX * 4); index += 4)
	{
		if (*(canvas + index + endianbyte) != 0)
		{
			return index / 4;
		}
	}
	return -1;
}

int UltimaDungeon::getRightMost(unsigned char* canvas, int startX, int endX)
{
	int endianbyte = std::endian::native == std::endian::big ? 3 : 0;
	for (int index = (startX * 4); index >= (endX * 4); index -= 4)
	{
		if (*(canvas + index + endianbyte) != 0)
		{
			return index / 4;
		}
	}
	return -1;
}

void UltimaDungeon::fillDoorPoly(unsigned char* canvas, short minX, short maxX, short minY, short maxY)
{
	short endianbyte = std::endian::native == std::endian::big ? 3 : 0;
	short pitch = 2400;
	short drawMode = 0;
	if (maxX >= 600)
	{
		drawMode = 1; // Just draw right
	}
	else if (minX <= 0)
	{
		drawMode = 2; // Just draw left
	}
	for (short index = minY; index <= maxY; ++index)
	{
		if (index >= 512)
		{
			break;
		}
		if (drawMode == 1)
		{
			if (minX < 600)
			{
				short startPos = (short)getLeftMost(canvas + (pitch * index), minX, maxX);
				if (startPos >= 0)
				{
					for (short tempIndex = startPos; tempIndex < 600; ++tempIndex)
					{
						canvas[(pitch * index) + (tempIndex * 4) + endianbyte] = 0xFF;
					}
				}
			}
		}
		else if (drawMode == 2)
		{
			short endPos = (short)getRightMost(canvas + (pitch * index), maxX, minX);
			for (int tempIndex = endPos; tempIndex >= 0; --tempIndex)
			{
				canvas[(pitch * index) + (tempIndex * 4) + endianbyte] = 0xFF;
			}
		}
		else
		{
			short startPos = (short)getLeftMost(canvas + (pitch * index), minX, maxX);
			short endPos = (short)getRightMost(canvas + (pitch * index), maxX, minX);

			if (startPos >= 0 && endPos >= 0)
			{
				for (int tempIndex = startPos; tempIndex <= endPos; ++tempIndex)
				{
					canvas[(pitch * index) + (tempIndex * 4) + endianbyte] = 0xFF;
				}
			}
		}
	}
}

void UltimaDungeon::createDoorPoly(unsigned char* canvas, short dx1, short dy1, short dx2, short dy2, short dy3, short dy4)
{
	int pitch = 2400;
	memset(canvas, 0, sizeof(unsigned char) * pitch * 512);

	short maxX = std::max<short>(dx1 * 2, dx2 * 2);
	short maxY = std::max<short>(dy1 * 2, dy2 * 2);
	short minX = std::min<short>(dx1 * 2, dx2 * 2);
	short minY = std::min<short>(dy1 * 2, dy2 * 2);

	short tempMax = std::max<short>(dy3 * 2, dy4 * 2);
	short tempMin = std::min<short>(dy3 * 2, dy4 * 2);

	maxY = std::max<short>(maxY, tempMax);
	minY = std::min<short>(minY, tempMin);

	renderLine(canvas, dx1 * 2, dy1 * 2, dx1 * 2, dy2 * 2);
	renderLine(canvas, dx1 * 2, dy2 * 2, dx2 * 2, dy3 * 2);
	renderLine(canvas, dx2 * 2, dy3 * 2, dx2 * 2, dy4 * 2);
	renderLine(canvas, dx2 * 2, dy4 * 2, dx1 * 2, dy1 * 2);

	fillDoorPoly(canvas, minX, maxX, minY, maxY);
}

void UltimaDungeon::createDoorPolygons()
{
	// 0   1   2   3   4   5   6   7   8   9  10  11  12  13  14  15  16  17  18  19  20  21  22  23  24  25  26  27  28  29  30  31
	short dx1[32] = { 74, 0,   300, 0,   114, 300, 20, 88,  212, 280, 36,  79,  135, 221, 264, 24,
					 87, 118, 182, 213, 276, 40,  87, 114, 143, 186, 213, 260, 118, 136, 164, 182 };
	short dx2[32] = { 226, 40,  260, 37,  186, 263, 48,  102, 198, 252, 64,  108, 164, 192, 236, 48,
					 101, 124, 176, 199, 252, 54,  100, 127, 156, 173, 200, 246, 126, 140, 160, 174 };

	short dy1[32] = { 256, 256, 256, 191, 191, 191, 182, 179, 179, 182, 159, 159, 159, 159, 159, 154,
					 153, 153, 153, 153, 154, 143, 143, 143, 143, 143, 143, 143, 140, 138, 138, 140 };
	short dy2[32] = { 64,  75,  75,  95,  95,  95,  96,  100, 100, 96,  112, 112, 112, 112, 112, 112,
					 110, 110, 110, 110, 112, 120, 120, 120, 120, 120, 120, 120, 122, 124, 124, 122 };
	short dy3[32] = { 64,  92,  92,  95,  95,  95,  102, 106, 106, 102, 112, 112, 112, 112, 112, 116,
					 114, 115, 115, 114, 116, 120, 120, 120, 120, 120, 120, 120, 124, 126, 126, 124 };
	short dy4[32] = { 256, 220, 220, 191, 191, 191, 170, 168, 168, 170, 159, 159, 159, 159, 159, 149,
					 147, 148, 148, 147, 149, 143, 143, 143, 143, 143, 143, 143, 137, 134, 134, 137 };

	std::vector<unsigned char> canvas;
	size_t canvas_size = 1228800; // 600 * 512 * 4;
	canvas.resize(canvas_size);

	for (int index = 0; index < 32; ++index)
	{
		createDoorPoly(canvas.data(), dx1[index], dy1[index], dx2[index], dy2[index], dy3[index], dy4[index]);

		m_texDungeonDoors[index] = SDL_CreateTexture(m_resources->m_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STATIC, 600, 512);

		SDL_GetTextureProperties(m_texDungeonDoors[index]);

		SDL_UpdateTexture(m_texDungeonDoors[index], NULL, canvas.data(), 2400);
	}
}

void UltimaDungeon::DungeonStart(short mode)
{
	m_gExitDungeon = false;
	m_graphics->m_queuedMode = U3GraphicsMode::Dungeon;
	//m_misc->m_gameMode = GameStateMode::Dungeon;

	m_audio->m_currentSong = 0;
	m_audio->m_nextSong = 4;
	m_audio->musicUpdate();

	if (mode == 1)
	{
		dungeonmech();
		return;
	}

	m_misc->m_gTorch = 0;

	m_misc->m_wx = 0x18;
	m_misc->m_wy = 0x17;

	bool autosave;
	m_resources->GetPreference(U3PreferencesType::Auto_Save, autosave);
	if (autosave)
	{
		m_misc->GetSosaria();
		m_misc->PutRoster();
		m_misc->PutParty();
		m_misc->PutSosaria();
	}
	m_forceRedraw = true;
}

void UltimaDungeon::Routine6E6B()
{
	m_misc->m_xpos = m_misc->m_zp[0xE3];
	m_misc->m_ypos = m_misc->m_zp[0xE4];
	m_graphics->m_curMode = U3GraphicsMode::Map;
	m_misc->m_gameMode = GameStateMode::Map;
	m_graphics->m_mode_switch = true;
	m_resources->m_wasMove = true;
	m_misc->m_Party[PARTY_LOCATION] = 0;    // back to surface

	if (!m_misc->m_checkDead)
	{
		m_scrollArea->UPrintMessage(182);
	}

	m_misc->m_Party[PARTY_XPOS] = (unsigned char)m_misc->m_xpos;
	m_misc->m_Party[PARTY_YPOS] = (unsigned char)m_misc->m_ypos;

	bool autosave;
	m_resources->GetPreference(U3PreferencesType::Auto_Save, autosave);

	if (autosave)
	{
		m_misc->GetSosaria();
		m_misc->PutRoster();
		m_misc->PutParty();
		m_misc->PutSosaria();
	}
	else
	{
		m_misc->PullSosaria();
	}
	m_audio->m_nextSong = 1;
	m_audio->m_currentSong = 1;
	m_audio->musicUpdate();
}

void UltimaDungeon::DngInfo() const
{
    m_graphics->DrawFramePiece(12, 8, 0);
    m_graphics->DrawFramePiece(13, 15, 0);
    m_graphics->DrawFramePiece(10, 7, 0);
    m_graphics->DrawFramePiece(10, 16, 0);

	SDL_FRect myRect{};

	myRect.x = (float)(9 * m_resources->m_blockSize);
	myRect.y = 0;
	myRect.w = (float)(6 * m_resources->m_blockSize);
	myRect.h = (float)(m_resources->m_blockSize);

	m_resources->adjustRect(myRect);

	SDL_SetRenderDrawColor(m_resources->m_renderer, 0, 0, 0, 255);
	SDL_RenderFillRect(m_resources->m_renderer, &myRect);

	SDL_SetRenderDrawColor(m_resources->m_renderer, 0, 0, 0, 0);

	std::string dispString = m_resources->m_plistMap["MoreMessages"][47];
	dispString += std::to_string(m_dungeonLevel + 1);

	m_resources->renderString(dispString, 9, 0);

	if (m_graphics->m_curMode != U3GraphicsMode::Combat)
	{
		m_graphics->DrawFramePiece(12, 6, 23);
		m_graphics->DrawFramePiece(13, 17, 23);

		myRect.x = (float)(7 * m_resources->m_blockSize);
		myRect.y = (float)(23 * m_resources->m_blockSize);
		myRect.w = (float)(10 * m_resources->m_blockSize);
		m_resources->adjustRect(myRect);
		SDL_RenderFillRect(m_resources->m_renderer, &myRect);

		dispString = m_resources->m_plistMap["MoreMessages"][48];
		m_resources->renderString(dispString, 7, 23);
		std::string strHeading;
		switch (m_misc->m_heading)
		{
		case 0:
			strHeading = m_resources->m_plistMap["MoreMessages"][49];
			break;
		case 1:
			strHeading = m_resources->m_plistMap["MoreMessages"][50];
			break;
		case 2:
			strHeading = m_resources->m_plistMap["MoreMessages"][51];
			break;
		case 3:
			strHeading = m_resources->m_plistMap["MoreMessages"][52];
			break;
		default:
			break;
		}
		m_resources->renderString(strHeading, 12, 23);
	}
}

void UltimaDungeon::Chunk2() // $183A
{
	if (m_ladderRect.size() > 0)
	{
		for (auto& curPair : m_ladderRect)
		{
			SDL_RenderTexture(m_resources->m_renderer, m_texDungeonShapes, &curPair.first, &curPair.second);
		}
		m_ladderRect.clear();
	}
	if (DungeonBlock(2) > 127)
	{
		Chunk3();
		return;
	}
	if (DungeonBlock(5) > 127)
	{
		Chunk3();
		return;
	}
	if (DungeonBlock(9) > 127)
	{
		Chunk3();
		return;
	}
	if (DungeonBlock(14) > 127)
	{
		Chunk3();
		return;
	}
	if (DungeonBlock(20) > 127)
	{
		Chunk3();
		return;
	}
	DungeonBlock(27);
	Chunk3();
}

void UltimaDungeon::Chunk3() // $1862
{
	if (m_ladderRect.size() > 0)
	{
		for (auto& curPair : m_ladderRect)
		{
			SDL_RenderTexture(m_resources->m_renderer, m_texDungeonShapes, &curPair.first, &curPair.second);
		}
		m_ladderRect.clear();
	}

	if (DungeonBlock(4) != 0)
	{
		return;
	}
	if (DungeonBlock(7) > 127)
	{
		Chunk4();
		return;
	}
	if (DungeonBlock(11) > 127)
	{
		Chunk4();
		return;
	}
	if (DungeonBlock(16) > 127)
	{
		Chunk4();
		return;
	}
	DungeonBlock(22);
	Chunk4();
}

void UltimaDungeon::Chunk4() // $1884
{
	if (m_ladderRect.size() > 0)
	{
		for (auto& curPair : m_ladderRect)
		{
			SDL_RenderTexture(m_resources->m_renderer, m_texDungeonShapes, &curPair.first, &curPair.second);
		}
		m_ladderRect.clear();
	}
	if (DungeonBlock(8) > 127)
	{
		Chunk5();
		return;
	}
	if (DungeonBlock(13) > 127)
	{
		Chunk5();
		return;
	}
	if (DungeonBlock(19) > 127)
	{
		Chunk5();
		return;
	}
	DungeonBlock(26);
	Chunk5();
}

void UltimaDungeon::Chunk5() // $189E
{
	if (m_ladderRect.size() > 0)
	{
		for (auto& curPair : m_ladderRect)
		{
			SDL_RenderTexture(m_resources->m_renderer, m_texDungeonShapes, &curPair.first, &curPair.second);
		}
		m_ladderRect.clear();
	}
	if (DungeonBlock(12) != 0)
	{
		return;
	}
	if (DungeonBlock(17) > 127)
	{
		Chunk6();
		return;
	}
	if (DungeonBlock(23) > 127)
	{
		Chunk6();
		return;
	}
	DungeonBlock(28);
	Chunk6();
}

void UltimaDungeon::Chunk6() // $18B9
{
	if (DungeonBlock(18) > 127)
	{
		Chunk7();
		return;
	}
	if (DungeonBlock(25) > 127)
	{
		Chunk7();
		return;
	}
	Chunk7();
}

void UltimaDungeon::Chunk7() // $18CC
{
	if (DungeonBlock(24) > 127)
	{
		return;
	}
	DungeonBlock(29);
	DungeonBlock(30);
}

void UltimaDungeon::Chunk1()
{
	short value;

	m_isChunk0Wall0 = false;

	value = DungeonBlock(0);

	if (value == 1)
	{
		m_isChunk0Wall0 = true;
	}

	if (value < 0 || value > 127)
	{
		return;
	}
	if (value != 0)
	{
		Chunk3();
		return;
	}
	if (DungeonBlock(1) > 127)
	{
		Chunk2();
		return;
	}
	if (DungeonBlock(3) > 127)
	{
		Chunk2();
		return;
	}
	if (DungeonBlock(6) > 127)
	{
		Chunk2();
		return;
	}
	if (DungeonBlock(10) > 127)
	{
		Chunk2();
		return;
	}
	if (DungeonBlock(15) > 127)
	{
		Chunk2();
		return;
	}
	DungeonBlock(21);
	Chunk2();
}

void UltimaDungeon::RenderDungeon()
{
	SDL_SetRenderDrawColor(m_resources->m_renderer, 0, 0, 0, 255);
	SDL_SetRenderTarget(m_resources->m_renderer, m_texDungeonPort);
	SDL_RenderClear(m_resources->m_renderer);

	if (m_misc->m_gTorch < 1)
	{
		SDL_SetRenderTarget(m_resources->m_renderer, NULL);
		SDL_SetRenderDrawColor(m_resources->m_renderer, 0, 0, 0, 0);
		return;
	}

	DrawDungeonBackGround();
	m_ladderRect.clear();
	Chunk1();

	DrawSecretMessage();

	if (m_misc->m_gTorch < 3)
	{
		SDL_SetRenderDrawBlendMode(m_resources->m_renderer, SDL_BLENDMODE_BLEND);
		SDL_SetRenderDrawColor(m_resources->m_renderer, 0, 0, 0, 128);
		SDL_RenderFillRect(m_resources->m_renderer, NULL);
		SDL_SetRenderDrawBlendMode(m_resources->m_renderer, SDL_BLENDMODE_NONE);
	}

	SDL_SetRenderTarget(m_resources->m_renderer, NULL);
	SDL_SetRenderDrawColor(m_resources->m_renderer, 0, 0, 0, 0);
}

unsigned char UltimaDungeon::GetXYDng(short x, short y) const // $93DE
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
	return m_misc->m_Dungeon[(m_dungeonLevel * 256) + (y * 16) + x];
}

short UltimaDungeon::DungeonBlock(short location)
{
	short dx, dy, misc, swap;
	short tile = 0;
	char offsetX[32] = { 0,  -1, 1, -1, 0, 1,  -2, -1, 1, 2, -2, -1, 0,  1,  2, -3,
						-2, -1, 1, 2,  3, -3, -2, -1, 0, 1, 2,  3,  -2, -1, 1, 2 };
	char offsetY[32] = { 0,  0,  0,  -1, -1, -1, -1, -1, -1, -1, -2, -2, -2, -2, -2, -2,
						-2, -2, -2, -2, -2, -3, -3, -3, -3, -3, -3, -3, -3, -3, -3, -3 };
	dx = offsetX[location];
	dy = offsetY[location];
	misc = m_misc->m_heading;
	while (misc > 0)
	{
		misc--;
		swap = dx;
		dx = (-dy);
		dy = swap;
	}
	m_misc->m_xs = (m_misc->m_xpos + dx) & 0x0F;
	m_misc->m_ys = (m_misc->m_ypos + dy) & 0x0F;
	if (m_misc->m_xs < 0)
	{
		m_misc->m_xs += 16;
	}
	if (m_misc->m_ys < 0)
	{
		m_misc->m_ys += 16;
	}
	tile = GetXYDng(m_misc->m_xs, m_misc->m_ys);
	if (tile < 128)
	{
		if ((tile & 0x10) != 0)
		{
			DrawLadder(location, 0);
		}
		if ((tile & 0x20) != 0)
		{
			DrawLadder(location, 1);
		}
		if ((tile & 0x40) != 0)
		{
			DrawChest(location);
		}
		return 0;
	}
	if (tile < 0xC0)
	{
		DrawWall(location);
		return 0xFF;
	}
	misc = DrawWall(location);
	misc = DrawWall(misc + 32);
	if (misc == 0x20)
	{
		return 1;
	}
	return 0xFF;
}

short UltimaDungeon::DrawWall(short location) // $19B4
{
	SDL_FRect FromRect{};
	SDL_FRect ToRect{};
	if (location > 31)
	{
		DrawDoor(location - 32);
		return location;
	}

	ToRect.x = (dngL[location] * 2.0f);
	ToRect.y = (dngT[location] * 2.0f);
	ToRect.w = (dngR[location] * 2.0f);
	ToRect.h = (dngB[location] * 2.0f);
	FromRect.x = ToRect.x + (dOfX[location] * 2);
	FromRect.y = ToRect.y + (dOfY[location] * 2);
	FromRect.w = ToRect.w + (dOfX[location] * 2);
	FromRect.h = ToRect.h + (dOfY[location] * 2);

	ToRect.w -= ToRect.x;
	ToRect.h -= ToRect.y;

	FromRect.w -= FromRect.x;
	FromRect.h -= FromRect.y;

	SDL_RenderTexture(m_resources->m_renderer, m_texDungeonShapes, &FromRect, &ToRect);
	return location;
}

void UltimaDungeon::DrawDoor(short location) const
{
	if (m_texDungeonDoors[location])
	{
		SDL_RenderTexture(m_resources->m_renderer, m_texDungeonDoors[location], NULL, NULL);
	}
}

void UltimaDungeon::DrawChest(short location) // $1A5A
{
	SDL_FRect FromRect;
	SDL_FRect ToRect;
	short shape, width;
	if (location > 20)
	{
		return;
	}
	shape = 33;
	m_resources->GenerateRect(&FromRect, (dngL[shape] * 2), (dngT[shape] * 2), (dngR[shape] * 2), (dngB[shape] * 2));
	width = (short)(FromRect.w / 2);
	if (chSide[location] == 1)
	{
		FromRect.w -= (width);
	}
	if (chSide[location] == 2)
	{
		FromRect.x += (width - 1);
		FromRect.w -= (width);
	}
	m_resources->GenerateRect(&ToRect, (chL[location] * 2), (chT[location] * 2), (chR[location] + 1) * 2, (chB[location] + 1) * 2);
	SDL_RenderTexture(m_resources->m_renderer, m_texDungeonShapes, &FromRect, &ToRect);
}

void UltimaDungeon::DrawLadder(short location, short direction) // $1AAB
{
	SDL_FRect FromRect{};
	SDL_FRect ToRect{};

	short lleft, ltop, lright, lbottom, swap, side;
	short width, height, base, rung, half, shape;
	if (location > 20)
	{
		return;
	}
	lleft = (chL[location] * 2);
	ltop = (chT[location] * 2);
	lright = (chR[location] * 2);
	lbottom = (chB[location] * 2);
	ltop = (short)(lbottom - ((lbottom - ltop) * 1.5f));
	base = lbottom;
	if (direction == 0)
	{
		swap = 512 - lbottom;
		lbottom = 512 - ltop;
		ltop = swap;
		base = ltop;
	}
	side = chSide[location];
	height = (short)(((lbottom - ltop) * 1.25f) - (lbottom - ltop)) / 2;
	width = (lbottom - ltop) / 12;
	rung = (((lbottom - ltop) / 2) + ltop) - (width / 2);
	half = lleft + ((lright - lleft) / 2);
	if (side == 1)
	{
		half = lright;
	}
	if (side == 2)
	{
		half = lleft;
	}
	FromRect.x = (float)(lleft - width - 2);
	FromRect.y = (float)(base - height);
	FromRect.w = (float)((lright + width + 2) - (lleft - width - 2));
	FromRect.h = (float)(2 * height);
	SDL_SetRenderDrawColor(m_resources->m_renderer, 0, 0, 0, 255);
	if (!m_isChunk0Wall0)
	{
		// In the case we're in a door, the floor and ceiling are going to be black, so no need for an overlapping rectangle which
		// overlaps the front door
		SDL_RenderFillRect(m_resources->m_renderer, &FromRect);
	}

	if (side == 0 || side == 1)
	{
		shape = 35;
		m_resources->GenerateRect(&FromRect, (dngL[shape] * 2), (dngT[shape] * 2), (dngR[shape] * 2), (dngB[shape] * 2));
		m_resources->GenerateRect(&ToRect, lleft, ltop, lleft + width, lbottom);
		SDL_RenderTexture(m_resources->m_renderer, m_texDungeonShapes, &FromRect, &ToRect);
		if (side == 1)
		{
			m_ladderRect.push_back(std::make_pair(FromRect, ToRect));
		}
		shape = 34;
		m_resources->GenerateRect(&FromRect, (dngL[shape] * 2), (dngT[shape] * 2), (dngR[shape] * 2), (dngB[shape] * 2));
		m_resources->GenerateRect(&ToRect, lleft, rung, half + 1, rung + width);
		SDL_RenderTexture(m_resources->m_renderer, m_texDungeonShapes, &FromRect, &ToRect);
	}
	if (side == 0 || side == 2)
	{
		shape = 35;
		m_resources->GenerateRect(&FromRect, (dngL[shape] * 2), (dngT[shape] * 2), (dngR[shape] * 2), (dngB[shape] * 2));
		m_resources->GenerateRect(&ToRect, lright - width, ltop, lright, lbottom);
		SDL_RenderTexture(m_resources->m_renderer, m_texDungeonShapes, &FromRect, &ToRect);
		if (side == 2)
		{
			m_ladderRect.push_back(std::make_pair(FromRect, ToRect));
		}
		shape = 34;
		m_resources->GenerateRect(&FromRect, (dngL[shape] * 2), (dngT[shape] * 2), (dngR[shape] * 2), (dngB[shape] * 2));
		m_resources->GenerateRect(&ToRect, half, rung, lright, rung + width);
		SDL_RenderTexture(m_resources->m_renderer, m_texDungeonShapes, &FromRect, &ToRect);
	}

	SDL_SetRenderDrawColor(m_resources->m_renderer, 0, 0, 0, 0);
}

void UltimaDungeon::DrawDungeonBackGround()
{
	SDL_FRect theRect{};
	theRect.x = 1800;
	theRect.y = 0;
	theRect.w = (float)600;
	theRect.h = (float)512;
	SDL_RenderTexture(m_resources->m_renderer, m_texDungeonShapes, &theRect, NULL);
}

void UltimaDungeon::DrawDungeon()
{
	SDL_FRect theRect{};

	if (m_forceRedraw)
	{
		RenderDungeon();
		m_forceRedraw = false;
	}

	if (!m_texDungeonPort)
	{
		return;
	}

	float ratio = m_resources->m_blockSize / 32.0f;

	theRect.x = 84.0f * ratio;
	theRect.y = 128.0f * ratio;
	theRect.w = 600.0f * ratio;
	theRect.h = 512.0f * ratio;

	m_resources->adjustRect(theRect);
	SDL_RenderTexture(m_resources->m_renderer, m_texDungeonPort, NULL, &theRect);
}

bool UltimaDungeon::CommandForward() const
{
	if (m_misc->m_callbackStack.size() > 0)
	{
		m_misc->m_callbackStack.pop();
	}

	m_scrollArea->UPrintMessage(165);
	m_misc->m_xs = m_HeadX[m_misc->m_heading] + m_misc->m_xpos & 0x0F;
	if (m_misc->m_xs < 0)
	{
		m_misc->m_xs += 16;
	}
	m_misc->m_ys = m_HeadY[m_misc->m_heading] + m_misc->m_ypos & 0x0F;
	if (m_misc->m_ys < 0)
	{
		m_misc->m_ys += 16;
	}
	if (GetXYDng(m_misc->m_xs, m_misc->m_ys) == 0x80)
	{
		m_misc->NoGo();
		return false;
	}
	m_misc->m_xpos = m_misc->m_xs;
	m_misc->m_ypos = m_misc->m_ys;

	return false;
}

bool UltimaDungeon::CommandRetreat() const
{
	if (m_misc->m_callbackStack.size() > 0)
	{
		m_misc->m_callbackStack.pop();
	}

	m_scrollArea->UPrintMessage(166);
	m_misc->m_xs = m_HeadX[(m_misc->m_heading + 2) & 3] + m_misc->m_xpos & 0x0F;
	if (m_misc->m_xs < 0)
	{
		m_misc->m_xs += 16;
	}
	m_misc->m_ys = m_HeadY[(m_misc->m_heading + 2) & 3] + m_misc->m_ypos & 0x0F;
	if (m_misc->m_ys < 0)
	{
		m_misc->m_ys += 16;
	}
	if (GetXYDng(m_misc->m_xs, m_misc->m_ys) == 0x80)
	{
		m_misc->NoGo();
		return false;
	}
	m_misc->m_xpos = m_misc->m_xs;
	m_misc->m_ypos = m_misc->m_ys;

	return false;
}

bool UltimaDungeon::CommandLeft() const
{
	if (m_misc->m_callbackStack.size() > 0)
	{
		m_misc->m_callbackStack.pop();
	}

	m_scrollArea->UPrintMessage(168);
	m_misc->m_xs = m_misc->m_xpos;
	m_misc->m_ys = m_misc->m_ypos;
	if (GetXYDng(m_misc->m_xs, m_misc->m_ys) >= 0xA0)
	{
		m_misc->NoGo();
		return false;
	}
	m_misc->m_heading--;
	if (m_misc->m_heading < 0)
	{
		m_misc->m_heading += 4;
	}

	return false;
}

bool UltimaDungeon::CommandRight() const
{
	if (m_misc->m_callbackStack.size() > 0)
	{
		m_misc->m_callbackStack.pop();
	}

	m_scrollArea->UPrintMessage(167);
	m_misc->m_xs = m_misc->m_xpos;
	m_misc->m_ys = m_misc->m_ypos;
	if (GetXYDng(m_misc->m_xs, m_misc->m_ys) >= 0xA0)
	{
		m_misc->NoGo();
		return false;
	}
	m_misc->m_heading++;
	if (m_misc->m_heading > 3)
	{
		m_misc->m_heading -= 4;
	}

	return false;
}

void UltimaDungeon::Forward()
{
	m_misc->AddFinishTurn();
	m_misc->m_callbackStack.push(std::bind(&UltimaDungeon::CommandForward, this));
}

void UltimaDungeon::Retreat()
{
	m_misc->AddFinishTurn();
	m_misc->m_callbackStack.push(std::bind(&UltimaDungeon::CommandRetreat, this));
}


void UltimaDungeon::Left()
{
	m_misc->AddFinishTurn();
	m_misc->m_callbackStack.push(std::bind(&UltimaDungeon::CommandLeft, this));
}

void UltimaDungeon::Right()
{
	m_misc->AddFinishTurn();
	m_misc->m_callbackStack.push(std::bind(&UltimaDungeon::CommandRight, this));
}

bool UltimaDungeon::CommandNotDngCmd()
{
	if (m_misc->m_callbackStack.size() > 0)
	{
		m_misc->m_callbackStack.pop();
	}

	m_scrollArea->UPrintMessage(172);

	return false;
}

void UltimaDungeon::NotDngCmd() // $8EF1
{
	m_audio->playSfx(SFX_ERROR1);
	m_misc->AddFinishTurn();
	m_misc->m_callbackStack.push(std::bind(&UltimaDungeon::CommandNotDngCmd, this));
}

bool UltimaDungeon::CommandKlimb() // $8EF1
{
	if (m_misc->m_callbackStack.size() > 0)
	{
		m_misc->m_callbackStack.pop();
	}

	m_scrollArea->UPrintMessage(170);
	if ((GetXYDng(m_misc->m_xpos, m_misc->m_ypos) & 0x10) == 0)
	{
		InvalCmd();
		return false;
	}
	m_dungeonLevel--;
	if (m_dungeonLevel >= 0 && m_dungeonLevel < 8)
	{
		return false;
	}
	m_dungeonLevel = 0;
	m_gExitDungeon = true;
	return false;
}

void UltimaDungeon::Klimb() // $8F37
{
	m_misc->AddFinishTurn();
	m_misc->m_callbackStack.push(std::bind(&UltimaDungeon::CommandKlimb, this));
}

bool UltimaDungeon::CommandDescend() // $8F0C
{
	if (m_misc->m_callbackStack.size() > 0)
	{
		m_misc->m_callbackStack.pop();
	}

	m_scrollArea->UPrintMessage(169);
	if (GetXYDng(m_misc->m_xpos, m_misc->m_ypos) > 127)
	{
		InvalCmd();
		return false;
	}
	if ((GetXYDng(m_misc->m_xpos, m_misc->m_ypos) & 0x20) == 0)
	{
		InvalCmd();
		return false;
	}
	m_dungeonLevel++;
	return false;
}


void UltimaDungeon::Descend() // $8F0C
{
	m_misc->AddFinishTurn();
	m_misc->m_callbackStack.push(std::bind(&UltimaDungeon::CommandDescend, this));
}

void UltimaDungeon::InvalCmd() // $8ED8
{
	m_audio->playSfx(SFX_ERROR1);
	m_scrollArea->UPrintMessage(171);
}

bool UltimaDungeon::CommandPeerGem()
{
	if (m_misc->m_callbackStack.size() > 0)
	{
		m_misc->m_callbackStack.pop();
	}

	m_scrollArea->UPrintMessage(75);
	m_misc->m_inputType = InputType::Transact;
	m_graphics->setFade(false);
	m_misc->m_callbackStack.push(std::bind(&UltimaDungeon::PeerGemCallback, this));
	m_misc->AddProcessEvent();
	return false;
}

void UltimaDungeon::PeerGem()
{
	m_misc->m_callbackStack.push(std::bind(&UltimaDungeon::CommandPeerGem, this));
}

bool UltimaDungeon::PeerGemCallback()
{
	if (m_misc->m_callbackStack.size() > 0)
	{
		m_misc->m_callbackStack.pop();
	}

	short rosnum;

	if (m_misc->m_input_num > 3 || m_misc->m_input_num < 0 || m_misc->m_Party[PARTY_ROSTERPOS1 + m_misc->m_input_num] == 0)
	{
		m_scrollArea->UPrintWin("\n\n");
		m_misc->AddFinishTurn();
		return false;
	}
	rosnum = m_misc->m_Party[PARTY_ROSTERPOS1 + m_misc->m_input_num];
	std::string strRosNum = std::to_string(rosnum) + std::string("\n\n");
	if (m_misc->m_Player[rosnum][37] < 1)
	{
		m_scrollArea->UPrintWin(strRosNum);
		m_scrollArea->UPrintMessage(67);
		m_misc->AddFinishTurn();
	}
	else
	{
		//m_scrollArea->blockPrompt(true);
		m_scrollArea->UPrintWin(strRosNum);
		m_misc->m_Player[rosnum][37]--;
		m_scrollArea->forceRedraw();
		m_graphics->m_miniMapInit = false;
		m_graphics->m_queuedMode = U3GraphicsMode::MiniMapDungeon;
		m_graphics->m_menuInit = false;
	}
	return false;
}

void UltimaDungeon::DrawSecretMessage()
{
	if (m_dungeonLevel == 6 && m_misc->m_xpos == 1 && m_misc->m_ypos == 1 && m_misc->m_heading == 3)
	{
		ShowSecret((m_misc->m_map_id - 12) * 3 + 0);
	}
	if (m_dungeonLevel == 6 && m_misc->m_xpos == 1 && m_misc->m_ypos == 15 && m_misc->m_heading == 2)
	{
		ShowSecret((m_misc->m_map_id - 12) * 3 + 1);
	}
	if (m_dungeonLevel == 7 && m_misc->m_xpos == 15 && m_misc->m_ypos == 15 && m_misc->m_heading == 1)
	{
		ShowSecret((m_misc->m_map_id - 12) * 3 + 2);
	}
}

bool UltimaDungeon::ShowSecret(short which) const
{
	if (which >= 0 && which < 21)
	{
		SDL_Texture* tex = m_texSecrets[which];
		if (tex != nullptr)
		{
			SDL_FRect frameRect{};
			SDL_FPoint size{};
			SDL_GetTextureSize(tex, &size.x, &size.y);
			frameRect.w = size.x;
			frameRect.h = size.y;
			frameRect.x = 300 - (frameRect.w / 2);
			frameRect.y = 282 - (frameRect.h / 2);
			SDL_RenderTexture(m_resources->m_renderer, tex, NULL, &frameRect);
		}
	}
	return true;
}

void UltimaDungeon::createOutlineText(std::string dispString, int texId)
{
	int endianbyte = std::endian::native == std::endian::big ? 3 : 0;

	SDL_Color sdl_text_color1 = { 255, 255, 255, 255 };
	SDL_Surface* surf = TTF_RenderText_Solid(m_resources->m_font, dispString.c_str(), dispString.size(), sdl_text_color1);
	m_texSecrets[texId] = SDL_CreateTexture(m_resources->m_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STATIC, surf->w, surf->h);
	SDL_SetTextureScaleMode(m_texSecrets[texId], SDL_SCALEMODE_NEAREST);
	std::vector<unsigned char> canvas;
	size_t canvasSize = static_cast<size_t>(surf->w) * static_cast<size_t>(surf->h) * 4;
	canvas.resize(canvasSize);
	std::fill(canvas.begin(), canvas.end(), 0);
	char* pixels = (char*)surf->pixels;
	// Worry about this later.  For right now, just assuming it's index8
	for (int indexY = 0; indexY < surf->h; indexY++)
	{
		char* start = pixels + (indexY * surf->pitch);
		for (int indexX = 0; indexX < surf->w; ++indexX)
		{
			if (*(start + indexX) != 0)
			{
				if (indexX == 0)
				{
				}
				else if (indexX - 1 == surf->w)
				{
				}
				else
				{
					if (*(start + (indexX - 1)) == 0)
					{
						int curByte = (indexY * (surf->w * 4) + ((indexX - 1) * 4) + endianbyte);
						canvas[curByte] = 0x80;
					}
					else if (*(start + (indexX + 1)) == 0)
					{
						int curByte = (indexY * (surf->w * 4) + ((indexX + 1) * 4) + 0);
						canvas[curByte] = 0x80;
						curByte++;
						canvas[curByte] = 0x80;
						curByte++;
						canvas[curByte] = 0x80;
						curByte++;
						canvas[curByte] = 0x80;
					}
					if (indexY < surf->h - 1)
					{
						if (*((start + surf->pitch) + indexX) == 0)
						{
							int curByte = (indexY + 1) * (surf->w * 4) + ((indexX - 1) * 4) + endianbyte;
							canvas[curByte] = 0x80;
						}
					}
					else if (indexY > 0)
					{
						if (*((start - surf->pitch) + indexX) == 0)
						{
							int curByte = (indexY - 1) * (surf->w * 4) + ((indexX - 1) * 4) + endianbyte;
							canvas[curByte] = 0x80;
						}
					}
				}
			}
		}
	}
	SDL_UpdateTexture(m_texSecrets[texId], NULL, canvas.data(), surf->w * 4);
	SDL_DestroySurface(surf);
}

void UltimaDungeon::createTextureSecrets()
{
	for (int index = 0; index < 21; ++index)
	{
		std::string dispString = m_resources->m_plistMap["Messages"][index];
		dispString.erase(std::remove(dispString.begin(), dispString.end(), '\n'), dispString.cend());
		createOutlineText(dispString, index);
	}
}

void UltimaDungeon::dungeonmech()
{
	m_misc->m_callbackStack.push(std::bind(&UltimaDungeon::FinishAge, this));
	m_misc->AgeChars();
}

bool UltimaDungeon::FinishAge()
{
	if (m_misc->m_callbackStack.size() > 0)
	{
		m_misc->m_callbackStack.pop();
	}
	
	m_resources->m_newMove = true;
	m_misc->m_wx = 0x18;
	m_misc->m_wy = 0x17;
	m_misc->m_xs = m_misc->m_xpos;
	m_misc->m_ys = m_misc->m_ypos;
	if (m_misc->m_gTorch > 0)
	{
		m_misc->m_gTorch--;
	}
	short value = GetXYDng(m_misc->m_xs, m_misc->m_ys);
	if (value != 0)
	{
		dngnotcombat(value);
		return false;
	}
	int rngVal = m_utilities->getRandom(0, 0x82 + m_dungeonLevel);
	if (rngVal < 128)
	{
		return false;
	}
	value = (short)m_utilities->getRandom(0, m_dungeonLevel + 2);
	if (value > 6)
	{
		value = 6;
	}
	value += 0x18;
	m_misc->m_gMonType = value * 2;
	m_spellCombat->Combat();

	return false;
}


void UltimaDungeon::dngnotcombat(short value)
{
	switch (value)
	{
	case 1: // $9076 time lord
		m_resources->m_overrideImage = 8;
		m_scrollArea->UPrintMessage(151);
		m_audio->m_currentSong = 9;
		m_audio->m_nextSong = 9;
		m_audio->musicUpdate();
		m_misc->m_inputType = InputType::AnyKey;
		m_misc->m_callbackStack.push(std::bind(&UltimaDungeon::TimeLordCallback, this));
		m_misc->AddProcessEvent();
		break;
	case 2: // $9174 fountain
		m_resources->m_overrideImage = 5;
		foundFountain();
		break;
	case 3: // $92C1 strange wind
		m_scrollArea->UPrintMessage(158);
		m_misc->m_gTorch = 0;
		break;
	case 4: // $9135 trap
		m_spellCombat->PutXYDng(0, m_misc->m_xs, m_misc->m_ys);
		m_scrollArea->UPrintMessage(159);
		m_audio->playSfx(SFX_STEP);
		if (m_misc->StealDisarmFail(m_misc->m_Party[PARTY_ROSTERPOS1]))
		{
			m_scrollArea->UPrintMessage(160);
			return;
		}
		m_misc->BombTrap();
		break;
	case 5: // $931C brand
		m_audio->m_currentSong = 0;
		m_audio->m_nextSong = 9;
		m_audio->musicUpdate();
		m_resources->m_overrideImage = 6;
		m_misc->m_wx = 0x18;
		m_misc->m_wy = 0x17;
		m_scrollArea->UPrintMessage(161);
		m_misc->m_inputType = InputType::Transact;
		//m_scrollArea->blockPrompt(true);
		m_misc->m_callbackStack.push(std::bind(&UltimaDungeon::MarkCallback, this));
		m_misc->AddProcessEvent();
		break;
	case 6: // $92DA gremlins
	{
		m_spellCombat->PutXYDng(0, m_misc->m_xs, m_misc->m_ys);
		short rngNum = (short)m_utilities->getRandom(0, m_misc->m_Party[PARTY_SIZE] - 1);
		if (m_misc->CheckAlive(rngNum))
		{
			short rosNum = m_misc->m_Party[PARTY_ROSTERPOS1 + rngNum];
			if (m_misc->m_Player[rosNum][32] < 1)
			{
				m_misc->m_Player[rosNum][32] = 0;
			}
			else
			{
				m_misc->m_Player[rosNum][32]--;
			}
			m_audio->playSfx(SFX_OUCH);
			m_scrollArea->UPrintMessage(163);
		}
		else
		{
			return;
		}
	}
		break;
	case 8: // $9053 writing
		m_scrollArea->UPrintMessage(164);
		m_misc->Speak(m_dungeonLevel + 1, 23);
		m_scrollArea->UPrintWin("\n");
		break;
	default:
		break;
	}
}

bool UltimaDungeon::TimeLordCallback()
{
	if (m_misc->m_callbackStack.size() > 0)
	{
		m_misc->m_callbackStack.pop();
	}

	m_audio->m_nextSong = 4;
	m_audio->musicUpdate();
	m_resources->m_overrideImage = -1;
	m_scrollArea->UPrintWin("\n");

	return false;
}

bool UltimaDungeon::foundFountain()
{
	if (m_misc->m_callbackStack.size() > 0)
	{
		m_misc->m_callbackStack.pop();
	}
	m_audio->m_currentSong = 9;
	m_audio->m_nextSong = 9;
	m_audio->musicUpdate();
	m_misc->m_wx = 0x18;
	m_misc->m_wy = 0x17;
	m_scrollArea->UPrintMessage(152);
	m_misc->m_inputType = InputType::Transact;
	m_misc->m_callbackStack.push(std::bind(&UltimaDungeon::FountainCallback, this));
	m_misc->AddProcessEvent();

	return false;
}

bool UltimaDungeon::FountainCallback()
{
	if (m_misc->m_callbackStack.size() > 0)
	{
		m_misc->m_callbackStack.pop();
	}

	short chNum = m_misc->m_input_num;
	std::string dispString;
	if (chNum >= 0)
	{
		dispString = std::to_string(chNum + 1) + std::string("\n");
	}
	else
	{
		dispString += '\n';
	}

	m_scrollArea->UPrintWin(dispString);
	if (chNum < 0 || chNum > 3)
	{
		m_audio->m_currentSong = 4;
		m_audio->m_nextSong = 4;
		m_audio->musicUpdate();
		m_resources->m_overrideImage = -1;
		return false;
	}
	if (m_misc->m_Party[PARTY_ROSTERPOS1 + chNum] == 0)
	{
		m_audio->m_currentSong = 4;
		m_audio->m_nextSong = 4;
		m_audio->musicUpdate();
		m_resources->m_overrideImage = -1;
		m_scrollArea->UPrintMessage(41);
		return false;
	}
	if (m_misc->CheckAlive(chNum) == false)
	{
		m_audio->playSfx(SFX_ERROR1);
		m_audio->playSfx(SFX_ERROR1);
		m_audio->m_currentSong = 4;
		m_audio->m_nextSong = 4;
		m_audio->musicUpdate();
		m_resources->m_overrideImage = -1;
		m_scrollArea->UPrintMessage(126);
		return false;
	}
	m_misc->m_rosNum = m_misc->m_Party[PARTY_ROSTERPOS1 + chNum];
	switch (m_misc->m_xpos & 0x03)
	{
	case 0: // Poison fountain
		m_scrollArea->UPrintMessage(154);
		m_misc->m_Player[m_misc->m_rosNum][17] = 'P';
		m_misc->InverseCharDetails(chNum, true);
		m_resources->m_inverses.elapsedTileTime = 0;
		m_resources->setInversed(true);
		m_resources->m_inverses.inverseTileTime = m_misc->damage_time;
		m_misc->m_callbackStack.push(std::bind(&UltimaDungeon::foundFountain, this));
		m_audio->playSfx(SFX_HIT);
		m_misc->AddInverse();
		break;
	case 1: // Heal fountain
		m_misc->m_Player[m_misc->m_rosNum][26] = m_misc->m_Player[m_misc->m_rosNum][28];
		m_misc->m_Player[m_misc->m_rosNum][27] = m_misc->m_Player[m_misc->m_rosNum][29];
		m_scrollArea->UPrintMessage(155);
		m_misc->m_callbackStack.push(std::bind(&UltimaDungeon::foundFountain, this));
		break;
	case 2: // Damage fountain
		m_scrollArea->UPrintMessage(156);
		m_misc->HPSubtract(m_misc->m_rosNum, 25);
		m_misc->InverseCharDetails(chNum, true);
		m_resources->m_inverses.tiles = true;
		m_resources->m_inverses.elapsedTileTime = 0;
		m_resources->setInversed(true);
		m_resources->m_inverses.inverseTileTime = m_misc->damage_time;
		m_misc->m_callbackStack.push(std::bind(&UltimaDungeon::foundFountain, this));
		m_audio->playSfx(SFX_HIT);
		m_misc->AddInverse();
		break;
	case 3:
		// Cure poison fountain
		m_scrollArea->UPrintMessage(157);
		if (m_misc->m_Player[m_misc->m_rosNum][17] == 'P')
		{
			m_misc->m_Player[m_misc->m_rosNum][17] = 'G';
		}
		m_misc->m_callbackStack.push(std::bind(&UltimaDungeon::foundFountain, this));
		break;
	default:
		m_resources->m_overrideImage = -1;
		break;
	}
	return false;
}

bool UltimaDungeon::MarkCallback()
{
	if (m_misc->m_callbackStack.size() > 0)
	{
		m_misc->m_callbackStack.pop();
	}

	unsigned char bits[8] = { 1, 2, 4, 8, 16, 32, 64, 128 };
	short chNum = m_misc->m_input_num;
	std::string dispString;
	if (chNum >= 0)
	{
		dispString = std::to_string(chNum + 1) + std::string("\n");
	}
	else
	{
		dispString += '\n';
	}
	
	m_scrollArea->UPrintWin(dispString);
	m_resources->m_overrideImage = -1;
	if (chNum < 0 || chNum > 3)
	{
		m_audio->m_currentSong = 4;
		m_audio->m_nextSong = 4;
		m_audio->musicUpdate();
		//m_scrollArea->blockPrompt(false);
		return false;
	}
	if (m_misc->m_Party[PARTY_ROSTERPOS1 + chNum] == 0)
	{
		m_audio->m_currentSong = 4;
		m_audio->m_nextSong = 4;
		m_audio->musicUpdate();
		//m_scrollArea->blockPrompt(false);
		m_scrollArea->UPrintMessage(41);
		return false;
	}
	if (m_misc->CheckAlive(chNum) == false)
	{
		m_audio->m_currentSong = 4;
		m_audio->m_nextSong = 4;
		m_audio->musicUpdate();
		//m_scrollArea->blockPrompt(false);
		m_scrollArea->UPrintMessage(126);
		return false;
	}
	m_misc->m_rosNum = m_misc->m_Party[PARTY_ROSTERPOS1 + chNum];
	m_misc->m_Player[m_misc->m_rosNum][14] = m_misc->m_Player[m_misc->m_rosNum][14] | bits[(m_misc->m_xpos & 3) + 4];

	m_misc->InverseCharDetails(chNum, true);
	m_resources->m_inverses.elapsedTileTime = 0;
	m_resources->setInversed(true);
	m_resources->m_inverses.inverseTileTime = m_misc->damage_time;
	m_misc->m_callbackStack.push(std::bind(&UltimaDungeon::MarkCallback2, this));
	m_misc->AddInverse();
	m_audio->playSfx(SFX_HIT);

	return false;
}

bool UltimaDungeon::MarkCallback2()
{
	if (m_misc->m_callbackStack.size() > 0)
	{
		m_misc->m_callbackStack.pop();
	}
	
	m_misc->HPSubtract(m_misc->m_rosNum, 50);
	m_scrollArea->UPrintMessage(162);
	//m_scrollArea->blockPrompt(false);
	return false;
}
