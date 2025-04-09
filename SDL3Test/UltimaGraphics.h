#pragma once

enum class IntroEnum
{
	PRE_FIGHT,
	FIGHTING,
	POST_FIGHT
};

enum class U3GraphicsMode
{
	None,
	Map,
	MiniMap,
	PauseScreen,
	BlackScreen
};

class U3Graphics
{
public:
	explicit U3Graphics();
	~U3Graphics();
	void InitializeStartTicks();
	void CreateIntroData();
	void CreateMenuData();
	void CreateOrganizeData();
	void DrawFrame(short which);
	void DrawFramePiece(short which, short x, short y);
	void DrawFramePieceScroll(short which, short x, short y, int offsetX = 0, int offsetY = 0);
	void FadeOnExodusUltima(Uint64 curTick);
	void WriteLordBritish(Uint64 curTick);
	void FightScene(Uint64 curTick);
	void DrawDemoScreen(Uint64 curTick);
	void DrawMenu();
	void DrawOrganizeMenu();
	void DrawMap(unsigned char x, unsigned char y);
	void DrawMiniMap();
	short MapConstrain(short value);
	void ChangeClassic();
	void setBlockSize(int blockSize);
	void setForceRedraw()
	{
		m_forceRedraw = true;
	}
	void resetBlink()
	{
		m_blinkElapsed = 0;
		m_fading = true;
	}
	
	void render(SDL_Event event, Uint64 deltaTime, bool& wasMove);

	unsigned char m_maskRestoreArray[128];
	unsigned char m_maskArray[128];
	U3GraphicsMode m_curMode;
	U3GraphicsMode m_queuedMode;

private:
	void renderMiniMap();
	void DrawMoonGateStuff();
	void renderGameMap(SDL_Event event, Uint64 deltaTime, bool& wasMove);
	void renderMiniMap(SDL_Event event, Uint64 deltaTime, bool& wasMove);

	static constexpr Uint64 DelayScroll = 700;
	bool m_fading;

	bool m_classic;
	Uint64 m_startTickCount;
	Uint64 m_fadeTime;
	SDL_Texture* m_texMap;
	int m_blockSize;
	bool m_forceRedraw;
	Uint64 m_blinkElapsed;
};
