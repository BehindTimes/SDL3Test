#include "UltimaDungeon.h"
#include "UltimaGraphics.h"
#include "U3Misc.h"
#include "U3Resources.h"
#include <SDL_image.h>

extern U3Graphics m_graphics;
extern U3Misc m_misc;
extern U3Resources m_resources;

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
	m_texFountain(nullptr),
	m_texTimeLord(nullptr),
	m_texRod(nullptr),
	m_texShrine(nullptr),
	m_forceRedraw(true),
	m_texDungeonPort(nullptr)
{
}

UltimaDungeon::~UltimaDungeon()
{
	if (m_texDungeonPort)
	{
		SDL_DestroyTexture(m_texDungeonPort);
	}
	if (m_texDungeonShapes)
	{
		SDL_DestroyTexture(m_texDungeonShapes);
	}
	if (m_texFountain)
	{
		SDL_DestroyTexture(m_texFountain);
	}
	if (m_texTimeLord)
	{
		SDL_DestroyTexture(m_texTimeLord);
	}
	if (m_texRod)
	{
		SDL_DestroyTexture(m_texRod);
	}
	if (m_texShrine)
	{
		SDL_DestroyTexture(m_texShrine);
	}
}

void UltimaDungeon::loadGraphics()
{
	std::filesystem::path currentPath = std::filesystem::current_path();
	currentPath /= ResourceLoc;
	currentPath /= ImagesLoc;
	currentPath /= "DungeonShapes.png";

	m_texDungeonShapes = IMG_LoadTexture(m_resources.m_renderer, currentPath.string().c_str());
	SDL_SetTextureScaleMode(m_texDungeonShapes, SDL_SCALEMODE_NEAREST);

	currentPath = std::filesystem::current_path();
	currentPath /= ResourceLoc;
	currentPath /= ImagesLoc;
	currentPath /= "Fountain.jpg";

	m_texFountain = IMG_LoadTexture(m_resources.m_renderer, currentPath.string().c_str());
	SDL_SetTextureScaleMode(m_texFountain, SDL_SCALEMODE_NEAREST);

	currentPath = std::filesystem::current_path();
	currentPath /= ResourceLoc;
	currentPath /= ImagesLoc;
	currentPath /= "Shrine.jpg";

	m_texShrine = IMG_LoadTexture(m_resources.m_renderer, currentPath.string().c_str());
	SDL_SetTextureScaleMode(m_texShrine, SDL_SCALEMODE_NEAREST);

	currentPath = std::filesystem::current_path();
	currentPath /= ResourceLoc;
	currentPath /= ImagesLoc;
	currentPath /= "TimeLord.jpg";

	m_texTimeLord = IMG_LoadTexture(m_resources.m_renderer, currentPath.string().c_str());
	SDL_SetTextureScaleMode(m_texTimeLord, SDL_SCALEMODE_NEAREST);

	currentPath = std::filesystem::current_path();
	currentPath /= ResourceLoc;
	currentPath /= ImagesLoc;
	currentPath /= "Rod.jpg";

	m_texRod = IMG_LoadTexture(m_resources.m_renderer, currentPath.string().c_str());
	SDL_SetTextureScaleMode(m_texRod, SDL_SCALEMODE_NEAREST);

	m_texDungeonPort = SDL_CreateTexture(m_resources.m_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 600, 512);
}

void UltimaDungeon::DungeonStart(short mode)
{
	m_graphics.m_curMode = U3GraphicsMode::Dungeon;
	m_misc.m_gameMode = GameStateMode::Dungeon;

	m_misc.m_wx = 0x18;
	m_misc.m_wy = 0x17;

	bool autosave;
	m_resources.GetPreference(U3PreferencesType::Auto_Save, autosave);
	if (autosave)
	{
		m_misc.GetSosaria();
		m_misc.PutRoster();
		m_misc.PutParty();
		m_misc.PutSosaria();
	}
	else
	{
		m_misc.PullSosaria();
	}
	m_forceRedraw = true;
}

void UltimaDungeon::Routine6E6B()
{
	m_misc.m_xpos = m_misc.m_zp[0xE3];
	m_misc.m_ypos = m_misc.m_zp[0xE4];
	m_graphics.m_curMode = U3GraphicsMode::Map;
	m_misc.m_gameMode = GameStateMode::Map;
	m_misc.m_Party[2] = 0;    // back to surface
}

void UltimaDungeon::DngInfo()
{
    m_graphics.DrawFramePiece(12, 8, 0);
    m_graphics.DrawFramePiece(13, 15, 0);
    m_graphics.DrawFramePiece(12, 6, 23);
    m_graphics.DrawFramePiece(13, 17, 23);
    m_graphics.DrawFramePiece(10, 5, 23);
    m_graphics.DrawFramePiece(10, 7, 0);
    m_graphics.DrawFramePiece(10, 16, 0);

	SDL_FRect myRect;

	myRect.x = (float)(9 * m_resources.m_blockSize);
	myRect.y = 0;
	myRect.w = (float)(6 * m_resources.m_blockSize);
	myRect.h = (float)(m_resources.m_blockSize);

	m_resources.adjustRect(myRect);

	SDL_SetRenderDrawColor(m_resources.m_renderer, 0, 0, 0, 255);
	SDL_RenderFillRect(m_resources.m_renderer, &myRect);

	myRect.x = (float)(7 * m_resources.m_blockSize);
	myRect.y = (float)(23 * m_resources.m_blockSize);
	myRect.w = (float)(10 * m_resources.m_blockSize);
	m_resources.adjustRect(myRect);
	SDL_RenderFillRect(m_resources.m_renderer, &myRect);

	SDL_SetRenderDrawColor(m_resources.m_renderer, 0, 0, 0, 0);

	std::string dispString = m_resources.m_plistMap["MoreMessages"][47];
	dispString += std::to_string(m_dungeonLevel + 1);

	m_resources.renderString(dispString, 9, 0);
	dispString = m_resources.m_plistMap["MoreMessages"][48];
	m_resources.renderString(dispString, 7, 23);
	std::string strHeading;
	switch (m_misc.m_heading)
	{
	case 0:
		strHeading = m_resources.m_plistMap["MoreMessages"][49];
		break;
	case 1:
		strHeading = m_resources.m_plistMap["MoreMessages"][50];
		break;
	case 2:
		strHeading = m_resources.m_plistMap["MoreMessages"][51];
		break;
	case 3:
		strHeading = m_resources.m_plistMap["MoreMessages"][52];
		break;
	default:
		break;
	}
	m_resources.renderString(strHeading, 12, 23);
}

void UltimaDungeon::Chunk2() // $183A
{
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

	value = DungeonBlock(0);

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
	SDL_SetRenderDrawColor(m_resources.m_renderer, 255, 0, 0, 255);
	SDL_SetRenderTarget(m_resources.m_renderer, m_texDungeonPort);
	SDL_RenderClear(m_resources.m_renderer);

	DrawDungeonBackGround();
	Chunk1();

	SDL_SetRenderTarget(m_resources.m_renderer, NULL);
	SDL_SetRenderDrawColor(m_resources.m_renderer, 0, 0, 0, 0);
}

unsigned char UltimaDungeon::GetXYDng(short x, short y) // $93DE
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
	return m_misc.m_Dungeon[(m_misc.m_dungeonLevel * 256) + (y * 16) + x];
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
	misc = m_misc.m_heading;
	while (misc > 0)
	{
		misc--;
		swap = dx;
		dx = (-dy);
		dy = swap;
	}
	m_misc.m_xs = (m_misc.m_xpos + dx) & 0x0F;
	m_misc.m_ys = (m_misc.m_ypos + dy) & 0x0F;
	if (m_misc.m_xs < 0)
	{
		m_misc.m_xs += 16;
	}
	if (m_misc.m_ys < 0)
	{
		m_misc.m_ys += 16;
	}
	tile = GetXYDng(m_misc.m_xs, m_misc.m_ys);
	if (tile < 128)
	{
		if ((tile & 0x10) != 0)
		{
			//DrawLadder(location, 0);
		}
		if ((tile & 0x20) != 0)
		{
			//DrawLadder(location, 1);
		}
		if ((tile & 0x40) != 0)
		{
			//DrawChest(location);
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
	SDL_FRect FromRect;
	SDL_FRect ToRect;
	if (location > 31)
	{
		//DrawDoor(location - 32);
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

	SDL_RenderTexture(m_resources.m_renderer, m_texDungeonShapes, &FromRect, &ToRect);
	//CopyBits(LWPortCopyBits(nDngShapes), LWPortCopyBits(dungPort), &FromRect, &ToRect, srcCopy, nil);
	return location;
}

void UltimaDungeon::DrawDungeonBackGround()
{
	SDL_FRect theRect;
	theRect.x = 1800;
	theRect.y = 0;
	theRect.w = (float)600;
	theRect.h = (float)512;
	SDL_RenderTexture(m_resources.m_renderer, m_texDungeonShapes, &theRect, NULL);
}

void UltimaDungeon::DrawDungeon()
{
	SDL_FRect theRect;

	if (m_forceRedraw)
	{
		RenderDungeon();
		m_forceRedraw = false;
	}

	if (!m_texDungeonPort)
	{
		return;
	}

	float ratio = m_resources.m_blockSize / 32.0f;

	theRect.x = 84.0f * ratio;
	theRect.y = 128.0f * ratio;
	theRect.w = 600.0f * ratio;
	theRect.h = 512.0f * ratio;

	m_resources.adjustRect(theRect);
	SDL_RenderTexture(m_resources.m_renderer, m_texDungeonPort, NULL, &theRect);
}
