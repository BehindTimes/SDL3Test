#pragma once

#include <stack>
#include <SDL3/SDL.h>
#include "U3Button.h"

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
	Diorama,
	KreateMap,
	PauseScreen,
	BlackScreen,
	WinScreen,
	Dungeon,
	Combat,
	Menu
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
	void WriteLordBritish(Uint64 curTick) const;
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
	U3GraphicsMode m_lastMode;
	U3GraphicsMode m_curMode;
	U3GraphicsMode m_queuedMode;
	Uint64 m_winElapsed;

	bool m_staydead;
	OrganizeBottomScreen m_obsCurMode;
	SDL_Texture* m_texMap;
	bool m_mode_switch;
	std::stack<U3GraphicsMode> m_menu_stack;
	bool m_menuInit;

	std::vector<std::unique_ptr<U3Button>> m_buttons;

private:
	void renderMiniMapDungeon();
	void DrawMiniMapDungeon();
	void renderMiniMap();
	void renderMiniMapDungeon(SDL_Event event, Uint64 deltaTime);
	void DrawMoonGateStuff();
	void renderGameMenu(SDL_Event event, Uint64 deltaTime);
	void renderCombat(SDL_Event event, Uint64 deltaTime);
	void renderGameMap(SDL_Event event, Uint64 deltaTime);
	void renderMiniMap(SDL_Event event, Uint64 deltaTime);
	void renderDiorama(SDL_Event event, Uint64 deltaTime);
	void renderKreateMap(SDL_Event event, Uint64 deltaTime);
	void renderWinScreen(SDL_Event event, Uint64 deltaTime, bool fade);
	void DrawWinScreen(float ratio);
	void renderWinScreen() const;
	void renderDungeon(SDL_Event event, Uint64 deltaTime);
	void DrawDioramaMap();
	void renderDiorama();
	void renderKreateMap();
	void DrawKreateMap();
	bool Kreate1();
	bool Kreate2();
	bool Kreate3();
	bool Kreate4();
	bool Kreate5();
	bool Kreate6();
	bool Kreate7();
	bool Kreate8();
	bool Kreate9();
	bool Kreate10();
	bool Kreate11();
	bool Kreate12();
	bool Kreate13();
	void AllWater();
	void MapSplat(unsigned char startX, unsigned char startY, unsigned char size, unsigned char onlyOn, unsigned char type);
	void CleanSurround();
	void CleanUpSingle();
	void CleanUpDiags(short what);
	bool CheckIfHasLava();
	void DrawGameMenu();
	void addButton(std::string strLabel, int x, int y, int width, std::function<void(int)> func);
	

	static constexpr Uint64 DelayScroll = 700;
	static constexpr Uint64 WinFade = 300;
	static constexpr Uint64 DungeonBlink = 300;
	static constexpr Uint64 DelayKreate = 1000;
	static constexpr Uint64 DelaySplat = 20;
	bool m_fading;
	bool m_allowRendering;

	Uint64 m_startTickCount;
	Uint64 m_fadeTime;
	
	int m_blockSize;
	bool m_forceRedraw;
	Uint64 m_blinkElapsed;
	unsigned char m_storeIcons[19];
	bool m_hasLava;
	unsigned short m_counter;
};
