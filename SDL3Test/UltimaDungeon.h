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
	void loadGraphics();
	void DrawDungeon();
	void setForceRedraw()
	{
		m_forceRedraw = true;
	}
	unsigned char GetXYDng(short x, short y);

	unsigned char m_dungeonLevel;
private:
	void Routine6E6B();
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

	SDL_Texture* m_texDungeonShapes;
	SDL_Texture* m_texFountain;
	SDL_Texture* m_texTimeLord;
	SDL_Texture* m_texRod;
	SDL_Texture* m_texShrine;
	SDL_Texture* m_texDungeonPort;

	static constexpr std::string_view ResourceLoc = "Resources";
	static constexpr std::string_view GraphicsLoc = "Graphics";
	static constexpr std::string_view PListLoc = "PLists";
	static constexpr std::string_view ImagesLoc = "Images";
	static constexpr std::string_view Standard = "Standard";
	static constexpr std::string_view BinLoc = "Bin";
	static constexpr std::string_view SaveLoc = "Save";

	bool m_forceRedraw;
};

