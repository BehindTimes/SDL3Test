#pragma once

#include <SDL3/SDL.h>

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
	MiniMapDungeon,
	PauseScreen,
	BlackScreen,
	WinScreen,
	Dungeon
};

enum class OrganizeBottomScreen
{
	None,
	Dispersed,
	DispersedNoOne,
	FormParty,
	PartyFormed,
	PartyFormedInUse,
	CreateCharacterChooseSlot,
	CreateCharacter,
	CreateCharacterAborted,
	CreateCharacterDone,
	TerminateCharacter
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
	void DrawFramePiece(short which, short x, short y, bool adjust = true);
	void DrawFramePieceScroll(short which, short x, short y, int offsetX = 0, int offsetY = 0);
	void FadeOnExodusUltima(Uint64 curTick);
	void WriteLordBritish(Uint64 curTick);
	void FightScene(Uint64 curTick);
	void DrawDemoScreen(Uint64 curTick);
	void DrawMenu();
	void DrawOrganizeMenu(SDL_Event event);
	void DrawMap(unsigned char x, unsigned char y);
	void DrawMiniMap();
	short MapConstrain(short value);
	void ChangeClassic();
	void setBlockSize(int blockSize);
	void setForceRedraw();
	void setFade(bool fade)
	{
		m_blinkElapsed = 0;
		m_fading = fade;
	}
	
	void render(SDL_Event event, Uint64 deltaTime);

	unsigned char m_maskRestoreArray[128];
	unsigned char m_maskArray[128];
	U3GraphicsMode m_curMode;
	U3GraphicsMode m_queuedMode;
	Uint64 m_winElapsed;

	bool m_staydead;
	OrganizeBottomScreen m_obsCurMode;

private:
	void renderMiniMapDungeon();
	void DrawMiniMapDungeon();
	void renderMiniMap();
	void renderMiniMapDungeon(SDL_Event event, Uint64 deltaTime);
	void DrawMoonGateStuff();
	void renderGameMap(SDL_Event event, Uint64 deltaTime);
	void renderMiniMap(SDL_Event event, Uint64 deltaTime);
	void renderWinScreen(SDL_Event event, Uint64 deltaTime, bool fade);
	void DrawWinScreen(float ratio);
	void renderWinScreen();
	void renderDungeon(SDL_Event event, Uint64 deltaTime);

	static constexpr Uint64 DelayScroll = 700;
	static constexpr Uint64 WinFade = 300;
	static constexpr Uint64 DungeonBlink = 300;
	bool m_fading;

	bool m_classic;
	Uint64 m_startTickCount;
	Uint64 m_fadeTime;
	
	int m_blockSize;
	bool m_forceRedraw;
	Uint64 m_blinkElapsed;
	SDL_Texture* m_texMap;
};
