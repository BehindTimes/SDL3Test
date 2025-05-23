#pragma once

#include <SDL3/SDL.h>
#include <filesystem>
#include <functional>
#include <string>

class UltimaDungeon
{
public:
	explicit UltimaDungeon();
	~UltimaDungeon();

	void DungeonStart(short mode);
	void DngInfo() const;
	bool loadGraphics();
	void DrawDungeon();
	void setForceRedraw()
	{
		m_forceRedraw = true;
	}
	unsigned char GetXYDng(short x, short y) const;
	bool HandleDefaultKeyPress(SDL_Keycode key);

	char m_dungeonLevel;
	bool m_gExitDungeon;
	void Routine6E6B();
	void dungeonmech();
	
private:
	
	void createDoorPolygons();
	void RenderDungeon();
	void DrawDungeonBackGround();
	short DrawWall(short location);
	short DungeonBlock(short location);
	void Chunk1();
	void Chunk2();
	void Chunk3();
	void Chunk4();
	void Chunk5();
	void Chunk6();
	void Chunk7();
	void DrawLadder(short location, short direction);
	void DrawDoor(short location) const;
	void DrawChest(short location);

	void LetterCommand(SDL_Keycode key);
	void Forward();
	void Retreat();
	void Left();
	void Right();
	void NotDngCmd();
	void Klimb();
	void Descend();
	void InvalCmd();
	void PeerGem();
	bool PeerGemCallback();
	void fillDoorPoly(unsigned char* canvas, short minX, short maxX, short minY, short maxY);
	void createDoorPoly(unsigned char* canvas, short dx1, short dy1, short dx2, short dy2, short dy3, short dy4);
	void renderLine(unsigned char* canvas, short x1, short y1, short x2, short y2);
	int getLeftMost(unsigned char* canvas, int startX, int endX);
	int getRightMost(unsigned char* canvas, int startX, int endX);
	void DrawSecretMessage();
	bool ShowSecret(short which) const;
	void createTextureSecrets();
	void createOutlineText(std::string dispString, int texId);
	void dngnotcombat(short value);
	bool MarkCallback();
	bool MarkCallback2();
	bool foundFountain();
	bool FountainCallback();
	bool TimeLordCallback();
	bool CommandNotDngCmd();
	bool CommandDescend();
	bool CommandKlimb();
	bool CommandPeerGem();
	bool CommandForward() const;
	bool CommandRetreat() const;
	bool CommandLeft() const;
	bool CommandRight() const;
	bool FinishAge();

	SDL_Texture* m_texDungeonDoors[32];
	SDL_Texture* m_texSecrets[21];
	SDL_Texture* m_texDungeonShapes;
	SDL_Texture* m_texDungeonPort;

	std::vector < std::pair<SDL_FRect, SDL_FRect>> m_ladderRect; // A hack to render ladders

	bool m_forceRedraw;
	char m_HeadX[4];
	char m_HeadY[4];
	bool m_isChunk0Wall0;
};

