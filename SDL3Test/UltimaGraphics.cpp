#include <SDL3/SDL.h>
#include <SDL3/SDL_rect.h>
#include <iostream>

#include "U3Resources.h"
#include "UltimaGraphics.h"
#include "UltimaIncludes.h"

extern short blkSiz;

extern SDL_Renderer* renderer;
extern U3Resources m_resources;

Uint64 startTickCount;
Uint64 fadeTime = 2400;
bool classic = true;

void InitializeStartTicks()
{
    startTickCount = SDL_GetTicks();
}

void CreateIntroData()
{
    m_resources.SetPreference(U3PreferencesType::Classic_Appearance, classic);
    m_resources.loadSignatureData();
    InitializeStartTicks();
}

void DrawFrame(short which)
{
    short x;

    if (which == 1) {
        for (x = 1; x < 39; x++) {
            DrawFramePiece(10, x, 0);
        }
        for (x = 1; x < 23; x++) {
            DrawFramePiece(10, x, 23);
        }
        for (x = 24; x < 39; x++) {
            DrawFramePiece(10, x, 4);
            DrawFramePiece(10, x, 8);
            DrawFramePiece(10, x, 12);
            DrawFramePiece(10, x, 16);
        }
        for (x = 1; x < 23; x++) {
            DrawFramePiece(11, 0, x);
            DrawFramePiece(11, 23, x);
        }
        for (x = 1; x < 16; x++) {
            DrawFramePiece(11, 39, x);
        }
        DrawFramePiece(1, 0, 0);
        DrawFramePiece(3, 39, 0);
        DrawFramePiece(7, 0, 23);
        DrawFramePiece(2, 23, 0);
        DrawFramePiece(9, 39, 16);
        DrawFramePiece(9, 23, 23);
        DrawFramePiece(4, 23, 4);
        DrawFramePiece(6, 39, 4);
        DrawFramePiece(4, 23, 8);
        DrawFramePiece(6, 39, 8);
        DrawFramePiece(4, 23, 12);
        DrawFramePiece(6, 39, 12);
        DrawFramePiece(4, 23, 16);
        /*for (x = 0; x < 4; x++) {
            DrawFramePiece(12, 30, x * 4);
            DrawFramePiece(13, 32, x * 4);
            str[0] = 1;
            str[1] = '1' + x;
            UCenterAt(str, 31, x * 4);
        }
        if (CFPreferencesGetAppBooleanValue(U3PrefIncludeWind, kCFPreferencesCurrentApplication, NULL)) {
            UCenterAt("\p          ", 7, 23);
            DrawFramePiece(12, 6, 23);
            DrawFramePiece(13, 17, 23);
        }
        DrawMoonGateStuff();*/
    }
	if ((which == 2) || (which == 3))
	{
        DrawFramePiece(1, 0, 0);
        DrawFramePiece(7, 0, 23);
        DrawFramePiece(3, 39, 0);
        DrawFramePiece(9, 39, 23);
        for (x = 1; x < 39; x++)
        {
            DrawFramePiece(10, x, 0);
        }
        for (x = 1; x < 39; x++)
        {
            DrawFramePiece(10, x, 23);
        }
        if (which == 2)
        {
            for (x = 1; x < 39; x++)
            {
                DrawFramePiece(10, x, 10);
            }
        }
        for (x = 1; x < 23; x++)
        {
            DrawFramePiece(11, 0, x);
        }
        for (x = 1; x < 23; x++)
        {
            DrawFramePiece(11, 39, x);
        }
        if (which == 2)
        {
            DrawFramePiece(4, 0, 10);
            DrawFramePiece(6, 39, 10);
        }
	}
}

void DrawFramePiece(short which, short x, short y)
{
	//SDL_FRect frameRect, myRect;

	which--;

    m_resources.renderUI(which, x, y);
}

bool fadeExodus = false;
bool fadeUltima = false;
bool writeLordBritish = false;
IntroEnum curIntro = IntroEnum::PRE_FIGHT;
Uint64 startFightTick = 0;

void FightScene(Uint64 curTick)
{
    const short animorder[5] = { 0, 1, 2, 3, 2 };
    short anim = 1;

    if (curIntro == IntroEnum::PRE_FIGHT)
    {
        m_resources.drawIntro(2, -25);
    }
    else if (curIntro == IntroEnum::POST_FIGHT)
    {
        m_resources.drawIntro(4, 22);
        DrawFramePiece(12, 12, 23);
        DrawFramePiece(13, 27, 23);
        m_resources.CenterMessage(54, 23);
    }
    else
    {
        int x = (int)((curTick - startFightTick) / 100);
        if (x < 25)
        {
            int xPos = (x * 2) - 25;
            anim = (x % 3);
            m_resources.drawIntro(animorder[anim], xPos);
        }
        else if (x < 50)
        {
            int xPos = 25 - ((x - 25) * 2);
            anim = 2 - (x % 3);
            m_resources.drawIntro(animorder[anim], xPos);
        }
        else if (x < 67)
        {
            int xPos = ((x - 50) * 2) - 25;
            anim = (x % 3);
            m_resources.drawIntro(animorder[anim], xPos);
        }
        else if (x < 71)
        {
            m_resources.drawIntro(animorder[anim], 10);
        }
        else if (x < 100)
        {
            m_resources.drawIntro(3, 4);
            if (x > 98)
            {
                curIntro = IntroEnum::POST_FIGHT;
            }
        }
    }
}

void FadeOnExodusUltima(Uint64 curTick)
{
    Uint64 curPass = curTick - startTickCount;

    m_resources.renderStalagtites();

    if (curPass < 200 && !fadeExodus)
    {
        return;
    }
    
    Uint8 alpha = 0;
    if (curPass > fadeTime)
    {
        alpha = 255;
        if (!fadeExodus)
        {
            fadeExodus = true;
            InitializeStartTicks();
            alpha = 0;
            fadeTime = 2200;
        }
        else if (!fadeUltima)
        {
            fadeUltima = true;
            writeLordBritish = true;
            InitializeStartTicks();
        }
    }
    else
    {
        alpha = (Uint8)(255 * ((float)curPass / fadeTime));
    }
    
    
    if (!fadeExodus)
    {
        m_resources.drawExodus(alpha);
    }
    else
    {
        if (!fadeUltima)
        {
            m_resources.drawUltimaLogo(alpha);
        }
        else
        {
            Uint8 curAlpha = fadeExodus ? 255 : 0;
            m_resources.drawUltimaLogo(curAlpha);
        }

        m_resources.drawExodus(255);
    }
}

void WriteLordBritish(Uint64 curTick)
{
    const int RENDERTIME = 3850;
    bool showCredits = false;
    Uint64 curPass = curTick - startTickCount;

    if (curPass > RENDERTIME)
    {
        curPass = RENDERTIME;
        showCredits = true;
    }

    curPass /= 10;

    if (!writeLordBritish)
    {
        return;
    }
    m_resources.drawBy();
    m_resources.WriteLordBritish(curPass);

    if (showCredits)
    {
        m_resources.DrawCredits();
        if (curIntro == IntroEnum::PRE_FIGHT)
        {
            curIntro = IntroEnum::FIGHTING;
            startFightTick = curTick;
        }
    }
}



void DrawDemoScreen(Uint64 curTick)
{
    m_resources.drawExodus(255);
    m_resources.CenterMessage(55, 10);
    DrawFramePiece(12, 14, 10);
    DrawFramePiece(13, 25, 10);
    m_resources.DrawDemo(curTick);
}

void DrawMenu()
{
    m_resources.drawExodus(255);
    if (classic)
    {
        m_resources.CenterMessage(27, 11);    // From the depths of hell
        m_resources.CenterMessage(28, 12);    // he comes for vengeance!@#
    }
    else
    {
        m_resources.CenterMessage(97, 11);    // From the depths of hell
        m_resources.CenterMessage(98, 12);    // he comes for vengeance!@#
    }

    // The copyright message is displayed in "modern" appearance no matter what.
    m_resources.SetPreference(U3PreferencesType::Classic_Appearance, false);
    m_resources.CenterMessage(26, 22);
    m_resources.SetPreference(U3PreferencesType::Classic_Appearance, classic);

    m_resources.DrawButtons({0, 1, 2, 8});
    /*m_resources.DrawButton(0); // Return to View
    m_resources.DrawButton(1); // Organize Party
    m_resources.DrawButton(2); // Journey Onward
    m_resources.DrawButton(8); // Adjust Options*/
}

void DrawOrganizeMenu()
{
    m_resources.drawExodus(255);
    m_resources.DrawButtons({ 3, 4, 5, 7 });

    m_resources.CenterMessage(33, 11);
    m_resources.DrawOrganizePartyRect();
}
