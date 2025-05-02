#include <SDL3/SDL_rect.h>
#include <iostream>

#include "U3Resources.h"
#include "U3Misc.h"
#include "UltimaGraphics.h"
#include "UltimaIncludes.h"
#include "U3ScrollArea.h"
#include "UltimaSpellCombat.h"
#include "UltimaDungeon.h"
#include "UltimaMain.h"
#include "UltimaSound.h"

extern short blkSiz;

extern std::unique_ptr<U3Resources> m_resources;
extern std::unique_ptr<U3Misc> m_misc;
extern std::unique_ptr<U3ScrollArea> m_scrollArea;
extern std::unique_ptr<UltimaDungeon> m_dungeon;
extern std::unique_ptr<UltimaSpellCombat> m_spellCombat;
extern std::unique_ptr<U3Audio> m_audio;

extern GameMode newMode;

extern TTF_TextEngine* engine_surface;
extern short screenOffsetX;
extern short screenOffsetY;

U3Graphics::U3Graphics() :
    m_startTickCount(0),
    m_fadeTime(ExodusFadeTime_1),
    m_curMode(U3GraphicsMode::Map),
    m_texMap(nullptr),
    m_queuedMode(U3GraphicsMode::None),
    m_lastMode(U3GraphicsMode::None),
    m_blockSize(0),
    m_forceRedraw(true),
    m_fading(true),
    m_blinkElapsed(0),
    m_staydead(false),
    m_winElapsed(0),
    m_obsCurMode(OrganizeBottomScreen::None),
    m_mode_switch(false),
    m_allowRendering(false),
    m_hasLava(false),
    m_counter(0),
    m_menuInit(false),
    m_showMenu(false),
    m_playFade1(true),
    m_playFade2(true),
    m_playImmolate(true),
    m_fadeExodus(false),
    m_fadeUltima(false),
    m_writeLordBritish(false),
    m_curIntro(IntroEnum::PRE_FIGHT),
    m_startFightTick(0),
    m_miniMapInit(false)
{
    memset(m_storeIcons, 0, sizeof(unsigned char) * 19);
    memset(m_maskRestoreArray, 0, sizeof(unsigned char) * 128);
    memset(m_maskArray, 0, sizeof(unsigned char) * 128);
}

U3Graphics::~U3Graphics()
{
    m_buttons.clear();
    if (m_texMap)
    {
        SDL_DestroyTexture(m_texMap);
        m_texMap = nullptr;
    }
}

void U3Graphics::setForceRedraw()
{
    m_forceRedraw = true;
    m_dungeon->setForceRedraw();
}

void U3Graphics::setBlockSize(int blockSize)
{
    m_blockSize = blockSize;
    if (!m_texMap)
    {
        SDL_DestroyTexture(m_texMap);
        m_texMap = nullptr;
    }
    m_texMap = SDL_CreateTexture(m_resources->m_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 22 * blockSize, 22 * blockSize);
    SDL_SetTextureScaleMode(m_texMap, SDL_SCALEMODE_NEAREST);
    setForceRedraw();
}

void U3Graphics::InitializeStartTicks()
{
    m_startTickCount = SDL_GetTicks();
}

void U3Graphics::CreateIntroData()
{
    //m_resources->SetPreference(U3PreferencesType::Classic_Appearance, m_classic);
    m_resources->loadSignatureData();
    InitializeStartTicks();
}

void U3Graphics::CreateMenuData()
{
    m_resources->SetButtonVisibility(0, true);
    m_resources->SetButtonVisibility(1, true);
    m_resources->SetButtonVisibility(2, true);
    m_resources->SetButtonVisibility(8, true);
}

void U3Graphics::CreateOrganizeData()
{
    m_resources->m_selectedFormRect = -1;
    m_resources->SetButtonVisibility(3, true);
    m_resources->SetButtonVisibility(4, true);
    m_resources->SetButtonVisibility(5, !m_misc->m_partyFormed);
    m_resources->SetButtonVisibility(6, m_misc->m_partyFormed);
    m_resources->SetButtonVisibility(7, true);
    m_resources->CreatePartyNames();
}

void U3Graphics::DrawMoonGateStuff()
{
    m_resources->DrawMoongates();
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
            str = std::to_string(x + 1);
            m_resources->CenterMessage(str, 31, 32, x * 4);
        }
        if (m_misc->m_gameMode == GameStateMode::Map)
        {
            DrawMoonGateStuff();
        }
        if (m_misc->m_gameMode == GameStateMode::Dungeon)
        {
            m_dungeon->DngInfo();
        }
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

    m_resources->renderUI(which, x, y, false, offsetX, offsetY);
}

void U3Graphics::DrawFramePiece(short which, short x, short y, bool adjust)
{
	which--;

    m_resources->renderUI(which, x, y, adjust);
}

void U3Graphics::FightScene(Uint64 curTick)
{
    const short animorder[5] = { 0, 1, 2, 3, 2 };
    short anim = 1;

    if (m_curIntro == IntroEnum::PRE_FIGHT)
    {
        m_resources->drawIntro(2, -25);
    }
    else if (m_curIntro == IntroEnum::POST_FIGHT)
    {
        m_resources->drawIntro(4, 22);
        DrawFramePiece(12, 12, 23);
        DrawFramePiece(13, 27, 23);
        m_resources->CenterMessage(54, 23);
    }
    else
    {
        int x = (int)((curTick - m_startFightTick) / 100);
        if (x < 25)
        {
            int xPos = (x * 2) - 25;
            anim = (x % 3);
            m_resources->drawIntro(animorder[anim], xPos);
        }
        else if (x < 50)
        {
            int xPos = 25 - ((x - 25) * 2);
            anim = 2 - (x % 3);
            m_resources->drawIntro(animorder[anim], xPos);
        }
        else if (x < 67)
        {
            int xPos = ((x - 50) * 2) - 25;
            anim = (x % 3);
            m_resources->drawIntro(animorder[anim], xPos);
        }
        else if (x < 71)
        {
            m_resources->drawIntro(animorder[anim], 10);
        }
        else
        {
            if (m_playImmolate)
            {
                m_audio->playSfx(SFX_IMMOLATE);
                m_playImmolate = false;
            }

            m_resources->drawIntro(3, 4);
            if (x > 80)
            {
                m_curIntro = IntroEnum::POST_FIGHT;
            }
        }
    }
}

void U3Graphics::FadeOnExodusUltima(Uint64 curTick)
{
    Uint64 curPass = curTick - m_startTickCount;

    m_resources->renderStalagtites();

    if (curPass < 200 && !m_fadeExodus)
    {
        return;
    }
    
    Uint8 alpha = 0;
    if (curPass > m_fadeTime)
    {
        alpha = 255;
        if (!m_fadeExodus)
        {
            m_fadeExodus = true;
            InitializeStartTicks();
            alpha = 0;
            m_fadeTime = ExodusFadeTime_2;
            if (m_playFade2)
            {
                m_playFade2 = false;
                m_playImmolate = true;
                m_audio->playSfx(SFX_FADE2);
            }
        }
        else if (!m_fadeUltima)
        {
            m_fadeUltima = true;
            m_writeLordBritish = true;
            InitializeStartTicks();
        }
    }
    else
    {
        alpha = (Uint8)(255 * ((float)curPass / m_fadeTime));
    }
    
    
    if (!m_fadeExodus)
    {
        if (m_playFade1)
        {
            m_playFade1 = false;
            m_audio->playSfx(SFX_FADE1);
        }
        
        m_resources->drawExodus(alpha);
    }
    else
    {
        if (!m_fadeUltima)
        {
            m_resources->drawUltimaLogo(alpha);
        }
        else
        {
            Uint8 curAlpha = m_fadeExodus ? 255 : 0;
            m_resources->drawUltimaLogo(curAlpha);
        }

        m_resources->drawExodus(255);
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

    if (!m_writeLordBritish)
    {
        return;
    }
    m_resources->drawBy();
    m_resources->WriteLordBritish(curPass);

    if (showCredits)
    {
        m_resources->DrawCredits();
        if (m_curIntro == IntroEnum::PRE_FIGHT)
        {
            m_curIntro = IntroEnum::FIGHTING;
            m_startFightTick = curTick;
        }
    }
}

void U3Graphics::DrawDemoScreen(Uint64 curTick)
{
    m_resources->drawExodus(255);
    m_resources->CenterMessage(55, 10);
    DrawFramePiece(12, 14, 10);
    DrawFramePiece(13, 25, 10);
    m_resources->DrawDemo(curTick);
}

void U3Graphics::ChangeClassic()
{
    bool classic;
    m_resources->GetPreference(U3PreferencesType::Classic_Appearance, classic);
    classic = !classic;
    m_resources->SetPreference(U3PreferencesType::Classic_Appearance, classic);
}

void U3Graphics::DrawMenu()
{
    bool classic;
    m_resources->GetPreference(U3PreferencesType::Classic_Appearance, classic);

    m_resources->drawExodus(255);
    if (classic)
    {
        m_resources->CenterMessage(27, 11);    // From the depths of hell
        m_resources->CenterMessage(28, 12);    // he comes for vengeance!@#
    }
    else
    {
        m_resources->CenterMessage(97, 11);    // From the depths of hell
        m_resources->CenterMessage(98, 12);    // he comes for vengeance!@#
    }

    // The copyright message is displayed in "modern" appearance no matter what.
    m_resources->SetPreference(U3PreferencesType::Classic_Appearance, false);
    m_resources->CenterMessage(26, 22);
    m_resources->SetPreference(U3PreferencesType::Classic_Appearance, classic);

    m_resources->DrawButtons({0, 1, 2, 8});
}

void U3Graphics::DrawOrganizeMenu(SDL_Event event)
{
    std::string strMessage;
    std::string_view tempMessage;

    m_resources->drawExodus(255);
    
    switch (m_obsCurMode)
    {
    case OrganizeBottomScreen::FormParty:
    {
        m_resources->DrawButtons({ 5, 7 });
        tempMessage = SelectPartyStr;
        strMessage = std::string(tempMessage);
        size_t startPos = (40 - strMessage.size()) / 2;
        m_resources->CenterMessage(strMessage, (short)startPos, (short)strMessage.size(), 11);
        m_resources->DrawOrganizePartyRect();
    }
    break;
    case OrganizeBottomScreen::CreateCharacterAborted:
        m_resources->DrawOrganizeCreateCharacterAborted();
        break;
    case OrganizeBottomScreen::CreateCharacterDone:
        m_resources->DrawOrganizeCreateCharacterDone();
        break;
    case OrganizeBottomScreen::DispersedNoOne:
        m_resources->DrawOrganizePartyDispersed(true);
        break;
    case OrganizeBottomScreen::Dispersed:
        m_resources->DrawOrganizePartyDispersed(false);
        break;
    case OrganizeBottomScreen::PartyFormed:
        m_resources->DrawOrganizePartyFormed(false);
        break;
    case OrganizeBottomScreen::PartyFormedInUse:
        m_resources->DrawOrganizePartyFormed(true);
        break;
    case OrganizeBottomScreen::TerminateCharacter:
    {
        m_resources->DrawButtons({ 4, 7 });
        tempMessage = SelectRemoveCharacterStr;
        strMessage = std::string(tempMessage);
        size_t startPos = (40 - strMessage.size()) / 2;
        m_resources->CenterMessage(strMessage, (short)startPos, (short)strMessage.size(), 11);
        m_resources->DrawOrganizePartyRect();
    }
        break;
    case OrganizeBottomScreen::CreateCharacter:
    {
        m_resources->CenterMessage(33, 11);
        m_resources->DrawOrganizePartyRect();
        m_resources->m_CreateCharacterDlg->ProcessEvent(event);
        m_resources->m_CreateCharacterDlg->display();
    }
        break;
    case OrganizeBottomScreen::CreateCharacterChooseSlot:
    {
        std::string_view tempStr = SelectCharacterSlotStr;
        strMessage = std::string(tempStr);
        size_t startPos = (40 - strMessage.size()) / 2;
        m_resources->DrawButtons({ 7 });
        m_resources->CenterMessage(strMessage, (short)startPos, (short)strMessage.size(), 11);
        m_resources->DrawOrganizePartyRect();
    }
        break;
    default:
        m_resources->DrawButtons({ 3, 4, 5, 6, 7 });
        m_resources->CenterMessage(33, 11);
        m_resources->DrawOrganizePartyRect();
        break;
    }
}

short U3Graphics::MapConstrain(short value)
{
    if (m_misc->m_mapSize == 0)
    {
        return 0;
    }
    while (value < 0)
    {
        value += m_misc->m_mapSize;
    }
    while (value >= m_misc->m_mapSize)
    {
        value -= m_misc->m_mapSize;
    }
    return value;
}

void U3Graphics::renderMiniMap()
{
    SDL_FRect outRect{};
    unsigned char value;

    if (m_misc->m_mapSize <= 0)
    {
        return;
    }

    SDL_SetRenderDrawColor(m_resources->m_renderer, 0, 0, 0, 255);
    SDL_SetRenderTarget(m_resources->m_renderer, m_texMap);
    SDL_RenderClear(m_resources->m_renderer);

    float minSize = (m_blockSize * 22.0f) / m_misc->m_mapSize;

    outRect.w = minSize;
    outRect.h = minSize;
    for (short y = 0; y < m_misc->m_mapSize; y++)
    {
        outRect.y = (float)(y * minSize);
        for (short x = 0; x < m_misc->m_mapSize; x++)
        {
            value = m_misc->GetXYVal(x, y) / 4;
            int minival = value;
            int multval = value / 16;
            value += (unsigned char)(16 * multval);
            //value = m_resources->GetRealTile(value);
            outRect.x = (float)(x * minSize);

            if (value < 192)
            {
                if (m_resources->m_currentGraphics->mini_tiles.size() == 64)
                {
                    SDL_RenderTexture(m_resources->m_renderer, m_resources->m_currentGraphics->mini_tiles[minival], NULL, &outRect);
                }
                else
                {
                    // if it's an I and not up against the edge of the map,
                    if (value == 78 && x > 0 && x < m_misc->m_mapSize - 1 && y > 0 && y < m_misc->m_mapSize - 1) // letter I or Door
                    {
                        // and the item left of it is not a letter
                        unsigned char left = m_misc->GetXYVal(x - 1, y) / 4;
                        minival = left;
                        multval = left / 16;
                        left += (unsigned char)(16 * multval);

                        bool isLetter = false;
                        if (left < 69 || left > 106 || left == 78)
                        {
                            // and the item right of it is not a letter
                            unsigned char right = m_misc->GetXYVal(x + 1, y) / 4;
                            minival = right;
                            multval = right / 16;
                            right += (unsigned char)(16 * multval);
                            if (right < 69 || right > 106 || right == 78)
                            {
                                // and the item above it is not a letter, turn it into a door.
                                unsigned char above = m_misc->GetXYVal(x, y - 1) / 4;
                                minival = above;
                                multval = above / 16;
                                above += (unsigned char)(16 * multval);
                                if (above < 69 || above > 106)
                                {
                                    for (short index = x - 1; index > 0; --index)
                                    {
                                        unsigned char templet = m_misc->GetXYVal(index, y) / 4;
                                        minival = templet;
                                        multval = templet / 16;
                                        templet += (unsigned char)(16 * multval);
                                        if (templet != 78)
                                        {
                                            if (templet >= 69 && templet <= 106)
                                            {
                                                isLetter = true;
                                            }
                                            break;
                                        }
                                    }
                                    if (!isLetter)
                                    {
                                        value = 94;    // 0x5C normally for I
                                    }
                                }
                            }
                        }
                    }

                    SDL_RenderTexture(m_resources->m_renderer, m_resources->m_currentGraphics->tiles[value], NULL, &outRect);
                }
            }
        }
    }

    SDL_SetRenderTarget(m_resources->m_renderer, NULL);
    m_forceRedraw = false;
    SDL_SetRenderDrawColor(m_resources->m_renderer, 0, 0, 0, 0);
}

void U3Graphics::renderMiniMapDungeon()
{
    unsigned char chr;
    unsigned char under = 0;
    unsigned char value;

    SDL_SetRenderDrawColor(m_resources->m_renderer, 0, 0, 0, 255);
    SDL_SetRenderTarget(m_resources->m_renderer, m_texMap);
    SDL_RenderClear(m_resources->m_renderer);

    for (m_misc->m_ys = 0; m_misc->m_ys < 16; m_misc->m_ys++)
    {
        for (m_misc->m_xs = 0; m_misc->m_xs < 16; m_misc->m_xs++)
        {
            value = m_dungeon->GetXYDng(m_misc->m_xs, m_misc->m_ys);
            chr = 7;
            if (value == 0xC0)
            {
                chr = 0;
            }
            if (value == 0xA0)
            {
                chr = 1;
            }
            if (value == 0x80)
            {
                chr = 2;
            }
            if (value == 0x30)
            {
                chr = 3;
            }
            if (value == 0x20)
            {
                chr = 5;
            }
            if (value == 0x10)
            {
                chr = 4;
            }
            if (value == 0x00)
            {
                chr = 6;
            }

            DrawFramePiece(chr + 24, m_misc->m_xs + 3, m_misc->m_ys + 3, false);
            if (m_misc->m_xs == m_misc->m_xpos && m_misc->m_ys == m_misc->m_ypos)
            {
                under = chr;
            }
        }
    }

    SDL_SetRenderTarget(m_resources->m_renderer, NULL);
    m_forceRedraw = false;
    SDL_SetRenderDrawColor(m_resources->m_renderer, 0, 0, 0, 0);
}

void U3Graphics::DrawMiniMapDungeon()
{
    SDL_FRect theRect{};

    if (m_forceRedraw)
    {
        m_fading = true;
        renderMiniMapDungeon();
    }

    if (!m_texMap)
    {
        return;
    }

    theRect.x = (float)m_blockSize;
    theRect.y = (float)m_blockSize;
    theRect.w = (float)m_blockSize * 22;
    theRect.h = (float)m_blockSize * 22;
    m_resources->adjustRect(theRect);
    SDL_RenderTexture(m_resources->m_renderer, m_texMap, NULL, &theRect);

    if (m_fading)
    {
        DrawFramePiece(32, m_misc->m_xpos + 4, m_misc->m_ypos + 4);
    }
}

void U3Graphics::DrawMiniMap()
{
    SDL_FRect theRect{};

    if (m_forceRedraw)
    {
        m_fading = true;
        renderMiniMap();
    }

    if (!m_texMap)
    {
        return;
    } 

    theRect.x = (float)m_blockSize;
    theRect.y = (float)m_blockSize;
    theRect.w = (float)m_blockSize * 22;
    theRect.h = (float)m_blockSize * 22;
    m_resources->adjustRect(theRect);
    SDL_RenderTexture(m_resources->m_renderer, m_texMap, NULL, &theRect);

    float minSize = (m_blockSize * 22.0f) / m_misc->m_mapSize;

    theRect.x = (float)minSize * m_misc->m_xpos + m_blockSize;
    theRect.y = (float)minSize * m_misc->m_ypos + m_blockSize;
    theRect.w = (float)minSize;
    theRect.h = (float)minSize;

    float ratio = 0;
    if (m_fading)
    {
        ratio = (DelayScroll - m_blinkElapsed) / (float)DelayScroll;
    }
    else
    {
        ratio = (m_blinkElapsed) / (float)DelayScroll;
    }

    m_resources->adjustRect(theRect);

    SDL_SetRenderDrawColor(m_resources->m_renderer, (Uint8)(255 * ratio), (Uint8)(255 * ratio), (Uint8)(255 * ratio), 255);
    SDL_RenderFillRect(m_resources->m_renderer, &theRect);
    SDL_SetRenderDrawColor(m_resources->m_renderer, 0, 0, 0, 0);
}

void U3Graphics::DrawMap(unsigned char x, unsigned char y)
{
    const char xhide[11] = { 1, 1, 1, 1, 1, 0, -1, -1, -1, -1, -1 };
    const char yhide[11] = { 11, 11, 11, 11, 11, 0, -11, -11, -11, -11, -11 };

    unsigned char xt(0);
    unsigned char yt(0);
    unsigned char xm(0);
    unsigned char ym(0);
    unsigned char value(0);
    unsigned char offset2(0);
    unsigned char offset(0);
    unsigned char numy = 11;
    for (ym = y - 5; numy > 0; ym++)
    {
        unsigned char numx = 11;
        for (xm = x - 5; numx > 0; xm++)
        {
            unsigned char val = m_misc->GetXYVal(xm, ym);
            char variant = (val & 0x03);
            if (variant && val >= 92 && val <= 126)
            {
                m_resources->m_TileArray[offset++] = (((val / 4) - 23) * 2 + 79 + variant) * 2;
            }
            else
            {
                m_resources->m_TileArray[offset++] = (val & 0xFC) >> 1;
            }
            // if it's an I and not up against the edge of the map,
            if (val == 0xB8 && xm > 0 && xm < m_misc->m_mapSize - 1 && ym > 0) // letter I or Door
            {
                // and the item left of it is not a letter
                unsigned char left = m_misc->GetXYVal(xm - 1, ym);
                bool isLetter = false;
                if (left < 0x98 || left > 0xE4 || left == 0xB8)
                {
                    // and the item right of it is not a letter
                    unsigned char right = m_misc->GetXYVal(xm + 1, ym);
                    if (right < 0x98 || right > 0xE4 || right == 0xB8)
                    {
                        // and the item above it is not a letter, turn it into a door.
                        unsigned char above = m_misc->GetXYVal(xm, ym - 1);
                        if (above < 0x98 || above > 0xE4)
                        {
                            for (short index = xm - 1; index > 0; --index)
                            {
                                unsigned char templet = m_misc->GetXYVal(index, ym);
                                if (templet != 0xB8)
                                {
                                    if (templet >= 0x98 && templet <= 0xE4)
                                    {
                                        isLetter = true;
                                    }
                                    break;
                                }
                            }
                            if (!isLetter)
                            {
                                m_resources->m_TileArray[offset - 1] = 0x5D;    // 0x5C normally for I
                            }
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
            value = m_resources->m_TileArray[offset2];
            flag1 = false;
            if ((value == 6) || (value == 8) || (value == 0x46) || (value == 0x48))
            {
                m_resources->m_TileArray[offset] = 0x48; // Void
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
    m_misc->m_stx = x - 5;
    m_misc->m_sty = y - 5;
    m_resources->DrawTiles();
}

void U3Graphics::DrawGameMenu() const
{
    SDL_FRect theRect{};

    theRect.x = (float)m_blockSize;
    theRect.y = (float)m_blockSize;
    theRect.w = (float)m_blockSize * 22;
    theRect.h = (float)m_blockSize * 22;
    m_resources->adjustRect(theRect);
    SDL_RenderTexture(m_resources->m_renderer, m_resources->m_texSosariaMap, NULL, &theRect);
}

void U3Graphics::render(SDL_Event event, Uint64 deltaTime)
{
    switch (m_curMode)
    {
    case U3GraphicsMode::Combat:
        renderCombat(event, deltaTime);
        break;
    case U3GraphicsMode::Map:
        renderGameMap(event, deltaTime);
        break;
    case U3GraphicsMode::MiniMap:
        renderMiniMap(event, deltaTime);
        break;
    case U3GraphicsMode::Diorama:
        renderDiorama(event, deltaTime);
        break;
    case U3GraphicsMode::KreateMap:
        renderKreateMap(event, deltaTime);
        break;
    case U3GraphicsMode::MiniMapDungeon:
        renderMiniMapDungeon(event, deltaTime);
        break;
    case U3GraphicsMode::WinScreen:
        renderWinScreen(event, deltaTime, m_fading);
        break;
    case U3GraphicsMode::Dungeon:
        renderDungeon(event, deltaTime);
        break;
    case U3GraphicsMode::Menu:
        renderGameMenu(event, deltaTime);
        break;
    default:
        break;
    }
}

void U3Graphics::renderWinScreen() const
{
    SDL_SetRenderTarget(m_resources->m_renderer, m_texMap);
    SDL_RenderClear(m_resources->m_renderer);
    SDL_Color sdl_text_color = { 255, 255, 255 };

    float x = m_blockSize * 11.0f;
    
    
    for (int index = 0; index < 10; ++index)
    {
        float y = m_blockSize * 2.0f * (index + 1) - m_resources->m_font_y_offset;
        std::string strDisplay = std::string(WinText[index]);
        m_resources->renderDisplayString(m_resources->m_font, strDisplay, (int)x, (int)y, sdl_text_color, 2, false);
    }

    SDL_SetRenderTarget(m_resources->m_renderer, NULL);
}

void U3Graphics::DrawWinScreen(float ratio)
{
    SDL_FRect theRect{};

    if (m_forceRedraw)
    {
        renderWinScreen();
    }

    if (!m_texMap)
    {
        return;
    }

    theRect.x = (float)m_blockSize;
    theRect.y = (float)m_blockSize;
    theRect.w = (float)m_blockSize * 22;
    theRect.h = (float)m_blockSize * 22;
    m_resources->adjustRect(theRect);
    SDL_SetTextureAlphaMod(m_texMap, (Uint8)(255 * ratio));
    SDL_RenderTexture(m_resources->m_renderer, m_texMap, NULL, &theRect);
}


void U3Graphics::renderWinScreen(SDL_Event event, Uint64 deltaTime, [[maybe_unused]] bool fade)
{
    DrawFrame(1);
    m_resources->ShowChars(true);
    m_scrollArea->render(deltaTime);
    m_resources->DrawWind();
    m_blinkElapsed += deltaTime;
   
    if (m_blinkElapsed > WinFade)
    {
        m_blinkElapsed = WinFade;
    }

    bool returnToGame = m_misc->ProcessAnyEvent(event);


    if (returnToGame)
    {
        if (m_blinkElapsed >= WinFade)
        {
            if (!m_fading)
            {
                m_fading = true;
                m_blinkElapsed = 0;
            }
            else
            {
                m_forceRedraw = true;
                m_curMode = U3GraphicsMode::Map;
                m_mode_switch = true;
                m_resources->m_wasMove = true;
                //m_scrollArea->blockPrompt(false);
                m_scrollArea->UPrintWin("\n");
                m_misc->m_inputType = InputType::Default;
                m_misc->SafeExodus();
            }
        }
        else
        {
            m_blinkElapsed = WinFade;
        }
    }

    float ratio = (float)m_blinkElapsed / (float)WinFade;
    if (m_fading)
    {
        ratio = 1.0f - ratio;
    }
    DrawWinScreen(ratio);
}

void U3Graphics::renderMiniMap(SDL_Event event, Uint64 deltaTime)
{
    if (!m_miniMapInit)
    {
        m_miniMapInit = true;
        m_audio->m_currentSong = 9;
        m_audio->musicUpdate();
    }

    DrawFrame(1);
    DrawMiniMap();
    m_resources->ShowChars(true);
    m_scrollArea->render(deltaTime);
    m_resources->DrawWind();
    bool returnToGame = m_misc->ProcessAnyEvent(event);
    m_blinkElapsed += deltaTime;
    if (m_blinkElapsed > DelayScroll)
    {
        m_blinkElapsed %= DelayScroll;
        m_fading = !m_fading;
    }
    if (returnToGame)
    {
		m_misc->m_inputType = InputType::Default;
		m_forceRedraw = true;
		m_curMode = U3GraphicsMode::Map;
		m_mode_switch = true;
		m_resources->m_wasMove = true;
		//m_scrollArea->blockPrompt(false);
		m_scrollArea->UPrintWin("");
	}
}

void U3Graphics::renderDiorama(SDL_Event event, Uint64 deltaTime)
{
    DrawFrame(1);
    DrawDioramaMap();
    m_resources->ShowChars(true);
    m_scrollArea->render(deltaTime);
    m_resources->DrawWind();
    bool returnToGame = m_misc->ProcessAnyEvent(event);
    m_blinkElapsed += deltaTime;
    if (m_blinkElapsed > DelayScroll)
    {
        m_blinkElapsed %= DelayScroll;
        m_fading = !m_fading;
    }
    if (returnToGame)
    {
        m_forceRedraw = true;
        m_curMode = U3GraphicsMode::Map;
        m_mode_switch = true;
        m_resources->m_wasMove = true;
        //m_scrollArea->blockPrompt(false);
        m_scrollArea->UPrintWin("");
    }
}


void U3Graphics::renderKreateMap([[maybe_unused]]SDL_Event event, Uint64 deltaTime)
{
    if (m_mode_switch)
    {
        m_allowRendering = false;
        m_mode_switch = false;
        //Kreate1();
        m_misc->m_callbackStack.push(std::bind(&U3Graphics::Kreate1, this));
    }

    DrawFrame(1);
    DrawKreateMap();
    m_resources->ShowChars(true);
    m_scrollArea->render(deltaTime);
    m_resources->DrawWind();
    m_resources->DrawInverses(deltaTime);

    if (m_queuedMode != U3GraphicsMode::None)
    {
        m_curMode = m_queuedMode;
        m_mode_switch = true;
        m_queuedMode = U3GraphicsMode::None;
        return;
    }

    if (m_scrollArea->isUpdating() || !m_scrollArea->MessageQueueEmpty())
    {

    }
    else
    {
        while (m_misc->m_callbackStack.size() > 0)
        {
            std::function<bool()> curCallback = m_misc->m_callbackStack.top();
            bool resumeRendering = curCallback();
            if (resumeRendering)
            {
                // Resume rendering (i.e. we're waiting on a function)
                break;
            }
        }

        m_resources->updateGameTime(deltaTime);
    }
}

void U3Graphics::renderMiniMapDungeon(SDL_Event event, Uint64 deltaTime)
{
    if (!m_miniMapInit)
    {
        m_miniMapInit = true;
        m_audio->m_currentSong = 9;
        m_audio->m_nextSong = 4;
        m_audio->musicUpdate();
    }

    DrawFrame(1);
    DrawMiniMapDungeon();
    m_resources->ShowChars(true);
    m_scrollArea->render(deltaTime);
    m_dungeon->DngInfo();
    bool returnToGame = m_misc->ProcessAnyEvent(event);
    m_blinkElapsed += deltaTime;
    if (m_blinkElapsed > DungeonBlink)
    {
        m_blinkElapsed %= DungeonBlink;
        m_fading = !m_fading;
    }
    if (returnToGame)
    {
        m_misc->m_inputType = InputType::Default;
        m_forceRedraw = true;
        m_curMode = U3GraphicsMode::Dungeon;
        m_mode_switch = true;
        m_resources->m_wasMove = true;
        //m_scrollArea->blockPrompt(false);
        m_scrollArea->UPrintWin("");
    }
}

void U3Graphics::addButton(std::string strLabel, int x, int y, int width, std::function<void(int)> func)
{
    auto curButton = std::make_unique<U3Button>();
    m_buttons.push_back(std::move(curButton));
    m_buttons.back()->CreateTextButton(m_blockSize, m_resources->m_renderer, engine_surface, m_resources->m_font, strLabel, x, y, width);
    m_buttons.back()->setVisible(true);
    m_buttons.back()->SetButtonCallback(func);
}

void U3Graphics::returnToGame([[maybe_unused]]int button)
{
    if (m_menu_stack.empty())
    {
        // Something went seriously wrong here
        newMode = GameMode::Unknown;
    }
    m_curMode = m_menu_stack.top();
    m_menu_stack.pop();
}

void U3Graphics::backToMenu([[maybe_unused]] int button)
{
    newMode = GameMode::MainMenu;
}

void U3Graphics::quitGame([[maybe_unused]] int button)
{
    newMode = GameMode::Unknown;
}

bool U3Graphics::closeImage()
{
    if (m_misc->m_callbackStack.size() > 0)
    {
        m_misc->m_callbackStack.pop();
    }

    m_resources->m_overrideImage = -1;

    return false;
}

void U3Graphics::goCommandList([[maybe_unused]] int button)
{
    m_resources->m_overrideImage = 1;
    m_misc->m_inputType = InputType::AnyKey;
    m_misc->m_callbackStack.push(std::bind(&U3Graphics::closeImage, this));
    m_misc->AddProcessEvent();
}

void U3Graphics::goSpellList([[maybe_unused]] int button)
{
    m_resources->m_overrideImage = 2;
    m_misc->m_inputType = InputType::AnyKey;
    m_misc->m_callbackStack.push(std::bind(&U3Graphics::closeImage, this));
    m_misc->AddProcessEvent();
}

void U3Graphics::goTables([[maybe_unused]] int button)
{
    m_resources->m_overrideImage = 3;
    m_misc->m_inputType = InputType::AnyKey;
    m_misc->m_callbackStack.push(std::bind(&U3Graphics::closeImage, this));
    m_misc->AddProcessEvent();
}

void U3Graphics::goOptions([[maybe_unused]] int button)
{
    m_resources->CreateOptionsDlg();
    m_misc->AddOptions();
}

void U3Graphics::renderGameMenu(SDL_Event event, Uint64 deltaTime)
{
    if (!m_menuInit)
    {
        m_buttons.clear();
        addButton(std::string(OptionsStr), 114, 100, 120, std::bind(&U3Graphics::goOptions, this, std::placeholders::_1));
        addButton(std::string(CommandListStr), 114, 120, 120, std::bind(&U3Graphics::goCommandList, this, std::placeholders::_1));
        addButton(std::string(SpellListStr), 114, 140, 120, std::bind(&U3Graphics::goSpellList, this, std::placeholders::_1));
        addButton(std::string(TablesStr), 114, 160, 120, std::bind(&U3Graphics::goTables, this, std::placeholders::_1));
        addButton(std::string(MainMenuStr), 114, 180, 120, std::bind(&U3Graphics::backToMenu, this, std::placeholders::_1));
        addButton(std::string(QuitStr), 114, 200, 120, std::bind(&U3Graphics::quitGame, this, std::placeholders::_1));

        addButton(std::string(ReturnToGameStr), 114, 240, 120, std::bind(&U3Graphics::returnToGame, this, std::placeholders::_1));
        m_menuInit = true;
    }
    m_misc->m_currentEvent = event;
    DrawFrame(1);
    m_resources->ShowChars(true);
    if (m_resources->m_overrideImage >= 0)
    {
        m_resources->ImageDisplay();
    }
    else
    {
        float scaler = (float)m_blockSize / 16.0f;

        SDL_FRect theRect{};

        theRect.x = (float)m_blockSize;
        theRect.y = (float)m_blockSize;
        theRect.w = (float)m_blockSize * 22;
        theRect.h = (float)m_blockSize * 22;
        m_resources->adjustRect(theRect);

        DrawGameMenu();
        for (auto& curButton : m_buttons)
        {
            curButton->render(m_resources->m_renderer, m_blockSize, (int)(curButton->m_x * scaler + theRect.x),
                (int)(curButton->m_y * scaler + theRect.y));
        }
    }
    m_scrollArea->render(deltaTime);

    if (m_scrollArea->isPrompt())
    {
        m_resources->DrawPrompt();
    }

    /*if (m_resources->m_SetOptionsDlg)
    {
        m_resources->m_SetOptionsDlg->display();
    }*/

    bool updateGame = true;

    while (m_misc->m_callbackStack.size() > 0)
    {
        updateGame = false;
        std::function<bool()> curCallback = m_misc->m_callbackStack.top();
        bool resumeRendering = curCallback();
        if (resumeRendering)
        {
            // Resume rendering (i.e. we're waiting on a function)
            break;
        }
    }

    if (updateGame)
    {
        m_misc->ProcessMenuEvent(event);
    }
}

void U3Graphics::renderCombat(SDL_Event event, Uint64 deltaTime)
{
    if (event.type == SDL_EVENT_KEY_DOWN && event.key.key == SDLK_TAB)
    {
        m_showMenu = true;
    }

    if (m_queuedMode != U3GraphicsMode::None)
    {
        m_curMode = m_queuedMode;
        m_mode_switch = true;
        m_queuedMode = U3GraphicsMode::None;
        return;
    }

    m_misc->m_currentEvent = event;
    DrawFrame(1);
    m_resources->ShowChars(true);
    if (m_resources->m_overrideImage >= 0)
    {
        m_resources->ImageDisplay();
    }
    else
    {
        m_spellCombat->drawCombat();
        m_resources->DrawInverses(deltaTime);
    }
    m_scrollArea->render(deltaTime);
    bool updateGame = true;

    if (m_lastMode == U3GraphicsMode::Map)
    {
        DrawMoonGateStuff();
    }
    else if (m_lastMode == U3GraphicsMode::Dungeon)
    {
        m_dungeon->DngInfo();
    }
    if (m_spellCombat->m_combatStart)
    {
        m_spellCombat->combatstart();
        m_spellCombat->m_combatStart = false;
    }

    unsigned char chnum;
    std::string dispString;

    if (m_scrollArea->isUpdating() || !m_scrollArea->MessageQueueEmpty())
    {
        updateGame = false;
    }
    else
    {
        while (m_misc->m_callbackStack.size() > 0)
        {
            updateGame = false;
            m_spellCombat->m_updateBlink = false;
            m_spellCombat->m_cHide = false;
            std::function<bool()> curCallback = m_misc->m_callbackStack.top();
            bool resumeRendering = curCallback();
            if (resumeRendering)
            {
                // Resume rendering (i.e. we're waiting on a function)
                break;
            }
        }
    }
    if (m_misc->m_callbackStack.size() > 0)
    {
        updateGame = false;
    }

    if (m_staydead)
    {
        updateGame = false;
        if (m_showMenu)
        {
            m_menu_stack.push(m_curMode);
            m_curMode = U3GraphicsMode::Menu;
            m_menuInit = false;
            m_showMenu = false;
            return;
        }
    }
    if (updateGame)
    {
        if (m_spellCombat->m_newMove)
        {
            if (m_showMenu)
            {
                m_menu_stack.push(m_curMode);
                m_curMode = U3GraphicsMode::Menu;
                m_menuInit = false;
                m_showMenu = false;
                return;
            }

            m_spellCombat->m_newMove = false;
            if (m_spellCombat->m_monster_turn)
            {
                m_spellCombat->m_mon++;
            }
            else
            {
                m_spellCombat->m_updateBlink = true;
                m_spellCombat->m_cHide = false;
                m_spellCombat->m_elapsedFlashTime = 0;
                chnum = m_spellCombat->m_g835D;
                m_spellCombat->m_gChnum = chnum;
                m_spellCombat->m_activePlayer = chnum;

                if (m_misc->CheckAlive(chnum))
                {
                    m_misc->InverseChnum(chnum, true);
                    m_spellCombat->m_count = 0x2F;
                    m_scrollArea->UPrintMessage(134);
                    dispString = std::to_string(chnum + 1);
                    m_scrollArea->UPrintWin(dispString);
                    m_misc->m_wx++;
                    m_scrollArea->UPrintMessage(135);
                    m_misc->m_dx = m_misc->m_dy = 0;
                    m_misc->m_xs = m_misc->m_CharX[chnum];
                    m_misc->m_ys = m_misc->m_CharY[chnum];
                    m_spellCombat->m_count2 = 0xC0;
                }
                else
                {
                    m_misc->AddFinishTurn();
                }
            }
            updateGame = false;
        }
    }

    if (updateGame)
    {
        if (m_spellCombat->m_monster_turn)
        {
            m_scrollArea->blockPrompt(true);
            m_misc->AddFinishTurn();
            if (m_misc->m_MonsterHP[m_spellCombat->m_mon] != 0)
            {
                m_spellCombat->HandleMonsterMove();
            }
        }
        else
        {
            m_scrollArea->blockPrompt(false);
            if (m_resources->m_preferences.auto_combat)
            {
                // something went wrong here, switch to manual mode
                if (!m_misc->m_InputDeque.empty())
                {
                    event.type = SDL_EVENT_KEY_DOWN;
                    event.key.key = m_misc->m_InputDeque.back();
                    event.key.mod = 0;
                    m_misc->m_InputDeque.pop_back();
                }
                else
                {
                    chnum = m_spellCombat->m_g835D;
                    m_spellCombat->AutoCombat(chnum);
                    // something went wrong here, switch to manual mode
                    if (m_misc->m_InputDeque.empty())
                    {
                        m_resources->m_preferences.auto_combat = false;
                    }
                    else
                    {
                        event.type = SDL_EVENT_KEY_DOWN;
                        event.key.key = m_misc->m_InputDeque.back();
                        event.key.mod = 0;
                        m_misc->m_InputDeque.pop_back();
                    }
                }
            }

            m_misc->ProcessEvent(event);
            m_resources->updateGameTime(deltaTime);
        }
    }
    else
    {
        m_scrollArea->blockPrompt(true);
    }
    if (m_spellCombat->m_updateBlink)
    {
        m_spellCombat->updateGameTime(deltaTime);
    }

    if (m_scrollArea->isPrompt())
    {
        m_resources->DrawPrompt();
    }
}

void U3Graphics::renderGameMap(SDL_Event event, Uint64 deltaTime)
{
    m_misc->m_currentEvent = event;
    DrawFrame(1);
    if (m_resources->m_overrideImage >= 0)
    {
        m_resources->ImageDisplay();     
        m_resources->DrawZStatButtons();
    }
    else
    {
        //if (!m_misc->m_freezeAnimation)
        {
            DrawMap((unsigned char)m_misc->m_xpos, (unsigned char)m_misc->m_ypos);
        }
    }
    m_resources->ShowChars(true);
    m_resources->DrawInverses(deltaTime);
    m_resources->DrawWind();
    m_scrollArea->render(deltaTime);
    bool updateGame = true;

    if (m_scrollArea->isUpdating() || !m_scrollArea->MessageQueueEmpty())
    {
        updateGame = false;
    }
    else
    {
        while (m_misc->m_callbackStack.size() > 0)
        {
            updateGame = false;
            std::function<bool()> curCallback = m_misc->m_callbackStack.top();
            bool resumeRendering = curCallback();
            if (resumeRendering)
            {
                // Resume rendering (i.e. we're waiting on a function)
                break;
            }
        }
    }

    if (m_misc->m_callbackStack.size() > 0)
    {
        updateGame = false;
    }

    if (m_staydead)
    {
        updateGame = false;
    }

    if (updateGame)
    {
        if (m_resources->m_newMove)
        {
            //m_scrollArea->blockPrompt(false);
            m_resources->m_newMove = false;
            m_misc->CheckAllDead();
            m_misc->WhirlPool();
            updateGame = false;
        }
    }

    if (updateGame)
    {
        m_scrollArea->blockPrompt(false);
        m_misc->ProcessEvent(event);
        m_resources->updateGameTime(deltaTime);
    }
    else
    {
        m_scrollArea->blockPrompt(true);
    }

    if (m_scrollArea->isPrompt())
    {
        m_resources->DrawPrompt();
    }
}

void U3Graphics::renderDungeon(SDL_Event event, Uint64 deltaTime)
{
    if (m_dungeon->m_gExitDungeon)
    {
        m_dungeon->Routine6E6B();
        return;
    }

    m_misc->m_currentEvent = event;
    DrawFrame(1);
    if (m_resources->m_overrideImage >= 0)
    {
        m_resources->ImageDisplay();
    }
    else
    {
        
        //if (!m_misc->m_freezeAnimation)
        {
            m_dungeon->DrawDungeon();
        }
    }
    m_resources->ShowChars(true);
    m_resources->DrawInverses(deltaTime);
    m_scrollArea->render(deltaTime);
    bool updateGame = true;

    if (m_scrollArea->isUpdating() || !m_scrollArea->MessageQueueEmpty())
    {
        updateGame = false;
    }
    else
    {
        while (m_misc->m_callbackStack.size() > 0)
        {
            updateGame = false;
            std::function<bool()> curCallback = m_misc->m_callbackStack.top();
            bool resumeRendering = curCallback();
            if (resumeRendering)
            {
                // Resume rendering (i.e. we're waiting on a function)
                break;
            }
        }
    }

    if (m_misc->m_callbackStack.size() > 0)
    {
        updateGame = false;
    }

    if (m_staydead)
    {
        updateGame = false;
    }

    if (m_queuedMode != U3GraphicsMode::None && m_scrollArea->MessageQueueEmpty())
    {
        m_curMode = m_queuedMode;
        m_mode_switch = true;
        m_mode_switch = true;
        m_resources->m_wasMove = true;
        m_queuedMode = U3GraphicsMode::None;

        if (m_curMode == U3GraphicsMode::Combat)
        {
            // This was being placed before the battle started.  In the original version,
            // they control the graphic updates
            m_spellCombat->PutXYDng(0x40, m_misc->m_xpos, m_misc->m_ypos);
        }
        updateGame = false;
    }
    if (updateGame)
    {
        if (m_resources->m_newMove)
        {
            m_dungeon->setForceRedraw();
            //m_scrollArea->blockPrompt(false);
            m_resources->m_newMove = false;
            m_misc->CheckAllDead();

            /*if (!m_misc->m_checkDead && m_queuedMode == U3GraphicsMode::None)
            {
                m_scrollArea->blockPrompt(false);
            }*/

            if (m_misc->m_gTorch == 0 && !m_misc->m_checkDead)
            {
                m_scrollArea->UPrintMessage(150);

            }
            //m_scrollArea->blockPrompt(false);
            updateGame = false;
        }
    }

    if (updateGame)
    {
        m_scrollArea->blockPrompt(false);
        m_misc->ProcessEvent(event);
        m_resources->updateGameTime(deltaTime);
    }
    else
    {
        m_scrollArea->blockPrompt(true);
    }

    if (m_scrollArea->isPrompt())
    {
        m_resources->DrawPrompt();
    }
}