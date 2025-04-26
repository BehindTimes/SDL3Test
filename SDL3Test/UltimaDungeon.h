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
	void DngInfo();
	bool loadGraphics();
	void DrawDungeon();
	void setForceRedraw()
	{
		m_forceRedraw = true;
	}
	unsigned char GetXYDng(short x, short y);
	bool HandleDefaultKeyPress(SDL_Keycode key);

	unsigned char m_dungeonLevel;
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
	void DrawDoor(short location);
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
	short getLeftMost(unsigned char* canvas, short startX, short endX);
	short getRightMost(unsigned char* canvas, short startX, short endX);
	void DrawSecretMessage();
	bool ShowSecret(short which);
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
	bool CommandForward();
	bool CommandRetreat();
	bool CommandLeft();
	bool CommandRight();
	bool FinishAge();

	SDL_Texture* m_texDungeonDoors[32];
	SDL_Texture* m_texSecrets[21];
	SDL_Texture* m_texDungeonShapes;
	SDL_Texture* m_texDungeonPort;

	std::vector < std::pair<SDL_FRect, SDL_FRect>> m_ladderRect; // A hack to render ladders

	static constexpr std::string_view ResourceLoc = "Resources";
	static constexpr std::string_view GraphicsLoc = "Graphics";
	static constexpr std::string_view PListLoc = "PLists";
	static constexpr std::string_view ImagesLoc = "Images";
	static constexpr std::string_view Standard = "Standard";
	static constexpr std::string_view BinLoc = "Bin";
	static constexpr std::string_view SaveLoc = "Save";

	bool m_forceRedraw;
	char m_HeadX[4];
	char m_HeadY[4];
	bool m_isChunk0Wall0;
};

