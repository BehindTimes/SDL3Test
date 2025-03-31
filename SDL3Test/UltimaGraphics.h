#pragma once

enum class IntroEnum
{
	PRE_FIGHT,
	FIGHTING,
	POST_FIGHT
};

void CreateIntroData();
void CreateMenuData();
void CreateOrganizeData();
void DrawFrame(short which);
void DrawFramePiece(short which, short x, short y);
void FadeOnExodusUltima(Uint64 curTick);
void WriteLordBritish(Uint64 curTick);
void FightScene(Uint64 curTick);
void DrawDemoScreen(Uint64 curTick);
void DrawMenu();
void DrawOrganizeMenu();