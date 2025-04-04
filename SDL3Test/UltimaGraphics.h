#pragma once

enum class IntroEnum
{
	PRE_FIGHT,
	FIGHTING,
	POST_FIGHT
};

class U3Graphics
{
public:
	explicit U3Graphics();
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
	short MapConstrain(short value);

	unsigned char m_maskRestoreArray[128];
	unsigned char m_maskArray[128];

private:
	bool m_classic;
	Uint64 m_startTickCount;
	Uint64 m_fadeTime;
};
