#include <SDL3/SDL.h>
#include <SDL3/SDL_rect.h>
#include <iostream>

#include "U3Resources.h"
#include "U3Misc.h"
#include "UltimaGraphics.h"
#include "UltimaIncludes.h"

extern short blkSiz;

extern SDL_Renderer* renderer;
extern U3Resources m_resources;
extern U3Misc m_misc;

U3Graphics::U3Graphics() :
    m_classic(false),
    m_startTickCount(0),
    m_fadeTime(2400)
{
    memset(m_maskRestoreArray, 0, sizeof(unsigned char) * 128);
    memset(m_maskArray, 0, sizeof(unsigned char) * 128);
}

void U3Graphics::InitializeStartTicks()
{
    m_startTickCount = SDL_GetTicks();
}

void U3Graphics::CreateIntroData()
{
    m_resources.SetPreference(U3PreferencesType::Classic_Appearance, m_classic);
    m_resources.loadSignatureData();
    InitializeStartTicks();
}

void U3Graphics::CreateMenuData()
{
    m_resources.SetButtonVisibility(0, true);
    m_resources.SetButtonVisibility(1, true);
    m_resources.SetButtonVisibility(2, true);
    m_resources.SetButtonVisibility(8, true);
}

void U3Graphics::CreateOrganizeData()
{
    m_resources.SetButtonVisibility(3, true);
    m_resources.SetButtonVisibility(4, true);
    m_resources.SetButtonVisibility(5, true);
    m_resources.SetButtonVisibility(6, false);
    m_resources.SetButtonVisibility(7, true);
}

void U3Graphics::DrawMoonGateStuff()
{
    m_resources.DrawMoongates();
}

void U3Graphics::DrawFrame(short which)
{
    short x;
    std::string str;

    if (which == 1)
    {
        for (x = 1; x < 39; x++)
        {
            DrawFramePiece(10, x, 0);
        }
        for (x = 1; x < 23; x++)
        {
            DrawFramePiece(10, x, 23);
        }
        for (x = 24; x < 39; x++)
        {
            DrawFramePiece(10, x, 4);
            DrawFramePiece(10, x, 8);
            DrawFramePiece(10, x, 12);
            DrawFramePiece(10, x, 16);
        }
        for (x = 1; x < 23; x++)
        {
            DrawFramePiece(11, 0, x);
            DrawFramePiece(11, 23, x);
        }
        for (x = 1; x < 16; x++)
        {
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
        for (x = 0; x < 4; x++)
        {
            DrawFramePiece(12, 30, x * 4);
            DrawFramePiece(13, 32, x * 4);
            str = std::to_string(x);
            m_resources.CenterMessage(str, 31, 32, x * 4);
        }
        bool hasWind;
        m_resources.GetPreference(U3PreferencesType::Include_Wind, hasWind);
        if(hasWind)
        {
            m_resources.DrawWind();
        }
        DrawMoonGateStuff();
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

void U3Graphics::DrawFramePieceScroll(short which, short x, short y, int offsetX, int offsetY)
{
    which--;

    m_resources.renderUI(which, x, y, false, offsetX, offsetY);
}

void U3Graphics::DrawFramePiece(short which, short x, short y)
{
	which--;

    m_resources.renderUI(which, x, y);
}

bool fadeExodus = false;
bool fadeUltima = false;
bool writeLordBritish = false;
IntroEnum curIntro = IntroEnum::PRE_FIGHT;
Uint64 startFightTick = 0;

void U3Graphics::FightScene(Uint64 curTick)
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

void U3Graphics::FadeOnExodusUltima(Uint64 curTick)
{
    Uint64 curPass = curTick - m_startTickCount;

    m_resources.renderStalagtites();

    if (curPass < 200 && !fadeExodus)
    {
        return;
    }
    
    Uint8 alpha = 0;
    if (curPass > m_fadeTime)
    {
        alpha = 255;
        if (!fadeExodus)
        {
            fadeExodus = true;
            InitializeStartTicks();
            alpha = 0;
            m_fadeTime = 2200;
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
        alpha = (Uint8)(255 * ((float)curPass / m_fadeTime));
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

void U3Graphics::WriteLordBritish(Uint64 curTick)
{
    const int RENDERTIME = 3850;
    bool showCredits = false;
    Uint64 curPass = curTick - m_startTickCount;

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



void U3Graphics::DrawDemoScreen(Uint64 curTick)
{
    m_resources.drawExodus(255);
    m_resources.CenterMessage(55, 10);
    DrawFramePiece(12, 14, 10);
    DrawFramePiece(13, 25, 10);
    m_resources.DrawDemo(curTick);
}

void U3Graphics::DrawMenu()
{
    m_resources.drawExodus(255);
    if (m_classic)
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
    m_resources.SetPreference(U3PreferencesType::Classic_Appearance, m_classic);

    m_resources.DrawButtons({0, 1, 2, 8});
}

void U3Graphics::DrawOrganizeMenu()
{
    m_resources.drawExodus(255);
    m_resources.DrawButtons({ 3, 4, 5, 6, 7 });

    m_resources.CenterMessage(33, 11);
    m_resources.DrawOrganizePartyRect();
}

short U3Graphics::MapConstrain(short value)
{
    if (m_misc.m_mapSize == 0)
    {
        return 0;
    }
    while (value < 0)
    {
        value += m_misc.m_mapSize;
    }
    while (value >= m_misc.m_mapSize)
    {
        value -= m_misc.m_mapSize;
    }
    return value;
}

void U3Graphics::DrawMap(unsigned char x, unsigned char y)
{
    const char xhide[11] = { 1, 1, 1, 1, 1, 0, -1, -1, -1, -1, -1 };
    const char yhide[11] = { 11, 11, 11, 11, 11, 0, -11, -11, -11, -11, -11 };

    unsigned char xt, yt, xm, ym, offset2, value;

    unsigned char offset = 0;
    unsigned char numy = 11;
    for (ym = y - 5; numy > 0; ym++)
    {
        unsigned char numx = 11;
        for (xm = x - 5; numx > 0; xm++)
        {
            unsigned char val = m_misc.GetXYVal(xm, ym);
            char variant = (val & 0x03);
            if (variant && val >= 92 && val <= 126)
            {
                m_resources.m_TileArray[offset++] = (((val / 4) - 23) * 2 + 79 + variant) * 2;
            }
            else
            {
                m_resources.m_TileArray[offset++] = (val & 0xFC) >> 1;
            }
            // if it's an I and not up against the edge of the map,
            if (val == 0xB8 && xm > 0 && xm < m_misc.m_mapSize - 1 && ym > 0) // letter I or Door
            {
                // and the item left of it is not a letter
                unsigned char left = m_misc.GetXYVal(xm - 1, ym);
                if (left < 0x98 || left > 0xE4 || left == 0xB8)
                {
                    // and the item right of it is not a letter
                    unsigned char right = m_misc.GetXYVal(xm + 1, ym);
                    if (right < 0x98 || right > 0xE4 || right == 0xB8)
                    {
                        // and the item above it is not a letter, turn it into a door.
                        unsigned char above = m_misc.GetXYVal(xm, ym - 1);
                        if (above < 0x98 || above > 0xE4)
                        {
                            m_resources.m_TileArray[offset - 1] = 0x5D;    // 0x5C normally for I
                        }
                    }
                }
            }
            numx--;
        }
        numy--;
    }
    
    bool flag1 = false;
    offset = 120;
    xm = 10;
    ym = 10;
    while (ym < 11)
    {
        if (!flag1)
        {
            xt = xm;
            yt = ym;
            offset2 = offset;
        }
        value = offset2 + xhide[xt] + yhide[yt];
        if (value != 0x3C)
        {
            offset2 = value;
            value = m_resources.m_TileArray[offset2];
            flag1 = false;
            if ((value == 6) || (value == 8) || (value == 0x46) || (value == 0x48))
            {
                m_resources.m_TileArray[offset] = 0x48; // Void
            }
            else
            {
                xt = xt + xhide[xt];
                yt = yt + xhide[yt];
                flag1 = true;
            }
        }
        else
        {
            flag1 = false;
        }
        if (!flag1)
        {
            offset--;
            xm--;
            if (xm > 10)
            {
                xm = 10;
                ym--;
            }
        }
    }
    m_misc.m_stx = x - 5;
    m_misc.m_sty = y - 5;
    m_resources.DrawTiles();
}
