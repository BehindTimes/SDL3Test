#include "U3Misc.h"
#include "UltimaGraphics.h"
#include "U3ScrollArea.h"
#include "U3Resources.h"
#include "UltimaSpellCombat.h"
#include "UltimaIncludes.h"
#include "U3Utilities.h"
#include "UltimaDungeon.h"
#include "UltimaSound.h"
#include <SDL3/SDL.h>
#include <iostream>

extern std::unique_ptr<U3Resources> m_resources;
extern std::unique_ptr<U3ScrollArea> m_scrollArea;
extern std::unique_ptr<U3Utilities> m_utilities;
extern std::unique_ptr<UltimaSpellCombat> m_spellCombat;
extern std::unique_ptr<UltimaDungeon> m_dungeon;
extern std::unique_ptr<U3Misc> m_misc;
extern std::unique_ptr<U3Audio> m_audio;

void U3Graphics::AllWater()
{
    for (short y = 0; y < m_misc->m_mapSize; y++)
    {
        for (short x = 0; x < m_misc->m_mapSize; x++)
        {
            m_misc->PutXYVal(0, (unsigned char)x, (unsigned char)y);
        }
    }
}

bool U3Graphics::CheckIfHasLava()
{
    short x, y;
    for (y = 1; y < m_misc->m_mapSize - 1; y++)
    {
        for (x = 1; x < m_misc->m_mapSize - 1; x++)
        {
            if ((m_misc->GetXYVal(x, y) == 0x84) && (m_misc->GetXYVal(x, y + 1) == 0x04))
            {
                return true;
            }
            if ((m_misc->GetXYVal(x, y) == 0x84) && (m_misc->GetXYVal(x + 1, y) == 0x04))
            {
                return true;
            }
        }
    }
    return false;
}

void U3Graphics::CleanSurround()
{
    short x, y, value;

    for (y = 1; y < m_misc->m_mapSize - 1; y++)
    {
        for (x = 1; x < m_misc->m_mapSize - 1; x++)
        {
            value = m_misc->GetXYVal(x, y);
            if (value != m_misc->GetXYVal(x - 1, y - 1) && value != m_misc->GetXYVal(x + 1, y - 1))
            {
                if (value != m_misc->GetXYVal(x + 1, y - 1) && value != m_misc->GetXYVal(x - 1, y + 1))
                {
                    m_misc->PutXYVal(m_misc->GetXYVal(x - 1, y), (unsigned char)x, (unsigned char)y);
                }
            }
        }
    }
}

void U3Graphics::CleanUpSingle()
{
    short x, y, value;

    for (y = 1; y < m_misc->m_mapSize - 1; y++)
    {
        for (x = 1; x < m_misc->m_mapSize - 1; x++)
        {
            value = m_misc->GetXYVal(x, y);
            if (value != m_misc->GetXYVal(x - 1, y) && value != m_misc->GetXYVal(x + 1, y))
            {
                if (value != m_misc->GetXYVal(x, y - 1) && value != m_misc->GetXYVal(x, y + 1))
                {
                    m_misc->PutXYVal(m_misc->GetXYVal(x - 1, y), (unsigned char)x, (unsigned char)y);
                }
            }
        }
    }
}

void U3Graphics::CleanUpDiags(short what)
{
    short x, y;
    int rngNum;
    for (y = 1; y < m_misc->m_mapSize - 2; y++)
    {
        for (x = 1; x < m_misc->m_mapSize - 2; x++)
        {
            if (m_misc->GetXYVal(x, y) == what)
            {
                if (m_misc->GetXYVal(x - 1, y - 1) == what && m_misc->GetXYVal(x, y - 1) != what && m_misc->GetXYVal(x - 1, y) != what)
                {
                    rngNum = m_utilities->getRandom(0, 255);
                    if (rngNum > 127)
                    {
                        m_misc->PutXYVal((unsigned char)what, (unsigned char)x - 1, (unsigned char)y);
                    }
                    else
                    {
                        m_misc->PutXYVal((unsigned char)what, (unsigned char)x, (unsigned char)y - 1);
                    }
                }
                if (m_misc->GetXYVal(x - 1, y + 1) == what && m_misc->GetXYVal(x - 1, y) != what && m_misc->GetXYVal(x, y + 1) != what)
                {
                    rngNum = m_utilities->getRandom(0, 255);
                    if (rngNum > 127)
                    {
                        m_misc->PutXYVal((unsigned char)what, (unsigned char)x - 1, (unsigned char)y);
                    }
                    else
                    {
                        m_misc->PutXYVal((unsigned char)what, (unsigned char)x, (unsigned char)y + 1);
                    }
                }
            }
        }
    }
}

void U3Graphics::MapSplat(unsigned char startX, unsigned char startY, unsigned char size, unsigned char onlyOn, unsigned char type)
{
    short leftLimit, rightLimit, topLimit, bottomLimit;
    short x, y, oldx, oldy, counter, amount;
    short dir[4] = { 0, -1, 0, 1 };

    x = startX;
    y = startY;
    oldx = x;
    oldy = y;
    amount = size * size;
    leftLimit = startX - (size / 2);
    if (leftLimit < 1)
    {
        leftLimit = 1;
    }
    rightLimit = startX + (size / 2);
    if (rightLimit > m_misc->m_mapSize - 2)
    {
        rightLimit = m_misc->m_mapSize - 2;
    }
    topLimit = startY - (size / 2);
    if (topLimit < 1)
    {
        topLimit = 1;
    }
    bottomLimit = startY + (size / 2);
    if (bottomLimit > m_misc->m_mapSize - 2)
    {
        bottomLimit = m_misc->m_mapSize - 2;
    }
    for (counter = 0; counter < amount; counter++)
    {
        if (m_misc->GetXYVal(x, y) == onlyOn)
        {
            m_misc->PutXYVal(type, (unsigned char)x, (unsigned char)y);
            //ShowDot(x, y);
        }
        while (oldx == x && oldy == y)
        {
            int rngNum = m_utilities->getRandom(0, 3);
            x += (dir[rngNum]);
            rngNum = m_utilities->getRandom(0, 3);
            y += (dir[rngNum]);
            x = MapConstrain(x);
            y = MapConstrain(y);
            if (x != oldx && y != oldy)
            {
                x = oldx;
                y = oldy;
            }
        }
        oldx = x;
        oldy = y;
        if (x < leftLimit || x > rightLimit || y < topLimit || y > bottomLimit)
        {
            x = startX;
            y = startY;
        }
    }
}

bool U3Graphics::Kreate13()
{
    if (m_misc->m_callbackStack.size() > 0)
    {
        m_misc->m_callbackStack.pop();
    }
    m_misc->m_Party[2] = 0;
    m_forceRedraw = true;

    unsigned short value = 0xFF;
    unsigned short target = 0x04;
    if (m_misc->m_Party[1] == 0x16)
    {
        target = 0;
    }
    short x = 0;
    short y = 0;
    while (value != target)
    {
        x = (short)m_utilities->getRandom(0, m_misc->m_mapSize - 1);
        y = (short)m_utilities->getRandom(0, m_misc->m_mapSize - 1);
        value = m_misc->GetXYVal((unsigned char)x, (unsigned char)y);
    }

    m_misc->m_xpos = x;
    m_misc->m_ypos = y;
    m_misc->m_WhirlX = 0;
    m_misc->m_WhirlY = 0;

    m_misc->PutMiscStuff();
    m_misc->AddQuitSave();
    m_audio->playSfx(SFX_LBLEVELRISE);
    m_misc->m_callbackStack.push(std::bind(&U3Graphics::Kreate14, this));
    

    return false;
}

bool U3Graphics::Kreate14()
{
    if (m_misc->m_callbackStack.size() > 0)
    {
        m_misc->m_callbackStack.pop();
    }
    m_queuedMode = U3GraphicsMode::MiniMap;
    m_menuInit = false;
    return false;
}

bool U3Graphics::Kreate12()
{
    if (m_misc->m_callbackStack.size() > 0)
    {
        m_misc->m_callbackStack.pop();
    }
    m_misc->DelayGame(DelaySplat, std::bind(&U3Graphics::Kreate11, this));
    m_forceRedraw = true;
    return false;
}

bool U3Graphics::Kreate11()
{
    if (m_misc->m_callbackStack.size() > 0)
    {
        m_misc->m_callbackStack.pop();
    }
    short byte = m_misc->m_opnum;
    unsigned short proximityLimit = 20;
    unsigned short value = 0;

    short x = 0;
    short y = 0;

    while (value != 0x04)
    {
        x = (short)m_utilities->getRandom(0, m_misc->m_mapSize - 1);
        y = (short)m_utilities->getRandom(0, m_misc->m_mapSize - 1);
        value = m_misc->GetXYVal(x, y);
        if ((byte % 2) && (m_misc->GetXYVal(x, y - 1) != 0x10) && (m_misc->GetXYVal(x, y + 1) != 0x10))
        {
            value = 0;
        }
        if (byte > 0 && value == 0x04)
        {
            for (short byte2 = 0; byte2 < byte; byte2++)
            {
                if ((m_utilities->Absolute(x - m_misc->m_MoonXTable[byte2]) + m_utilities->Absolute(y - m_misc->m_MoonYTable[byte2]) < proximityLimit))
                {
                    value = 0xFF;
                    m_counter++;
                    if (m_counter > 20)
                    {
                        m_counter = 0;
                        proximityLimit--;
                    }
                }
            }
        }
    }

    m_misc->m_MoonXTable[byte] = (unsigned char)x;
    m_misc->m_MoonYTable[byte] = (unsigned char)y;
    //->PutXYVal(0x88, (unsigned char)x, (unsigned char)y);

    m_misc->m_opnum++;

    return false;
}

bool U3Graphics::Kreate10()
{
    if (m_misc->m_callbackStack.size() > 0)
    {
        m_misc->m_callbackStack.pop();
    }
    m_forceRedraw = true;

    m_counter = 0;

    m_misc->m_callbackStack.push(std::bind(&U3Graphics::Kreate13, this));
    m_misc->m_opnum = 0;
    for (int byte = 0; byte < 8; byte++)
    {
        m_misc->m_callbackStack.push(std::bind(&U3Graphics::Kreate12, this));
    }
    m_misc->m_opnum = 0;

    return false;
}

bool U3Graphics::Kreate9()
{
    if (m_misc->m_callbackStack.size() > 0)
    {
        m_misc->m_callbackStack.pop();
    }
    short byte = m_misc->m_opnum;

    unsigned short target = 0x04;
    if (m_storeIcons[byte] == 0x14)
    {
        target = 0x10; // mountain for dungeon
    }
    if (byte == 8 || byte == 4)
    {
        target = 0x0C; // forest for dawn & yew
    }
    if (byte == 11)
    {
        target = 0x10; // mountain for death gulch
    }

    unsigned short value = 0xFF;
    unsigned short counter = 0;
    unsigned short proximityLimit = 18;

    short x = 0;
    short y = 0;

    while (value != target)
    {
        x = (short)m_utilities->getRandom(0, m_misc->m_mapSize - 1);
        y = (short)m_utilities->getRandom(0, m_misc->m_mapSize - 1);
        value = m_misc->GetXYVal(x, y);

        if (target == 0x10 && value == target)
        {
            if (m_misc->GetXYVal(x, y + 1) > 0x0C && m_misc->GetXYVal(x, y - 1) > 0x0C)
            {
                if (m_misc->GetXYVal(x + 1, y) > 0x0C && m_misc->GetXYVal(x - 1, y) > 0x0C)
                {
                    value = 0xFF;
                }
            }
            if (m_misc->GetXYVal(x, y - 1) != 0x10)
            {
                value = 0xFF;
            }
            if (m_misc->GetXYVal(x - 1, y) != 0x10 && m_misc->GetXYVal(x + 1, y) != 0x10)
            {
                value = 0xFF;
            }
        }
        if (target != 0x10 && value == target && byte != 1)
        {
            if (m_misc->GetXYVal(x, y + 1) != target || m_misc->GetXYVal(x, y - 1) != target || m_misc->GetXYVal(x - 1, y) != target)
            {
                counter++;
                if (counter < 100)
                {
                    value = 0xFF;
                }
            }
        }
        if (value == target && (byte == 9 || byte == 10)) // devil guard & fawn by water
        {
            if (m_misc->GetXYVal(x + 1, y) != 0)
            {
                counter++;
                if (counter < 100)
                {
                    value = 0xFF;
                }
            }
        }
        if (value == target && byte == 11) // death gulch by mountains
        {
            if (m_misc->GetXYVal(x + 1, y) != 0x10)
            {
                counter++;
                if (counter < 100)
                {
                    value = 0xFF;
                }
            }
        }
        if (value == target && byte == 1 && m_hasLava) // Exodus by lava
        {
            if (m_misc->GetXYVal(x, y - 1) != 0x84 && m_misc->GetXYVal(x - 1, y) != 0x84)
            {
                counter++;
                if (counter < 2000)
                {
                    value = 0xFF;
                }
            }
        }
        if (value == target) // after above rigorous testing!
        {
            if (byte > 0)
            {
                for (short byte2 = 0; byte2 < byte; byte2++)
                {
                    if (m_utilities->Absolute(x - m_misc->m_LocationX[byte2]) + m_utilities->Absolute(y - m_misc->m_LocationY[byte2]) < proximityLimit)
                    {
                        value = 0xFF;
                        counter++;
                        if (counter > 20)
                        {
                            counter = 0;
                            proximityLimit--;
                        }
                    }
                }
            }
        }
    }

    m_misc->m_LocationX[byte] = (unsigned char)x;
    m_misc->m_LocationY[byte] = (unsigned char)y;
    m_misc->PutXYVal(m_storeIcons[byte], (unsigned char)x, (unsigned char)y);

    m_misc->m_opnum++;
    return false;
}

bool U3Graphics::Kreate8()
{
    if (m_misc->m_callbackStack.size() > 0)
    {
        m_misc->m_callbackStack.pop();
    }
    m_misc->DelayGame(DelaySplat, std::bind(&U3Graphics::Kreate9, this));
    m_forceRedraw = true;
    return false;
}

bool U3Graphics::Kreate7()
{
    if (m_misc->m_callbackStack.size() > 0)
    {
        m_misc->m_callbackStack.pop();
    }
    m_forceRedraw = true;
    CleanUpSingle();
    CleanSurround();
    CleanUpSingle();
    CleanSurround();
    CleanUpDiags(0x10);
    CleanUpDiags(0x04);

    for (short byte = 0; byte < 3; byte++)
    {
        unsigned char rng1 = (unsigned char)m_utilities->getRandom(2, m_misc->m_mapSize - 2);
        unsigned char rng2 = (unsigned char)m_utilities->getRandom(2, m_misc->m_mapSize - 2);
        unsigned char rng3 = (unsigned char)m_utilities->getRandom(2, 12);
        MapSplat(rng1, rng2, rng3, 0x10, 0x84);
    }
    CleanUpDiags(0x84);
    CleanUpDiags(0x10);
    CleanUpSingle();
    m_hasLava = CheckIfHasLava();

    m_misc->m_callbackStack.push(std::bind(&U3Graphics::Kreate10, this));
    m_misc->m_opnum = 0;
    for (int byte = 0; byte < 19; byte++)
    {
        m_misc->m_callbackStack.push(std::bind(&U3Graphics::Kreate8, this));
    }

    return false;
}

bool U3Graphics::Kreate6()
{
    if (m_misc->m_callbackStack.size() > 0)
    {
        m_misc->m_callbackStack.pop();
    }
    m_forceRedraw = true;
    for (short byte = 0; byte < 20; byte++)
    {
        unsigned char rng1 = (unsigned char)m_utilities->getRandom(2, m_misc->m_mapSize - 2);
        unsigned char rng2 = (unsigned char)m_utilities->getRandom(2, m_misc->m_mapSize - 2);
        unsigned char rng3 = (unsigned char)m_utilities->getRandom(4, 16);
        MapSplat(rng1, rng2, rng3, 0x04, 0x08);
    }

    m_misc->DelayGame(DelayKreate, std::bind(&U3Graphics::Kreate7, this));

    return false;
}

bool U3Graphics::Kreate5()
{
    if (m_misc->m_callbackStack.size() > 0)
    {
        m_misc->m_callbackStack.pop();
    }
    m_forceRedraw = true;
    for (short byte = 0; byte < 16; byte++)
    {
        unsigned char rng1 = (unsigned char)m_utilities->getRandom(2, m_misc->m_mapSize - 2);
        unsigned char rng2 = (unsigned char)m_utilities->getRandom(2, m_misc->m_mapSize - 2);
        unsigned char rng3 = (unsigned char)m_utilities->getRandom(4, 16);
        MapSplat(rng1, rng2, rng3, 0x04, 0x0C);
    }

    m_misc->DelayGame(DelayKreate, std::bind(&U3Graphics::Kreate6, this));

    return false;
}

bool U3Graphics::Kreate4()
{
    if (m_misc->m_callbackStack.size() > 0)
    {
        m_misc->m_callbackStack.pop();
    }
    m_forceRedraw = true;
    CleanUpSingle();
    CleanSurround();
    CleanUpDiags(0x04);

    for (short byte = 0; byte < 64; byte++)
    {
        unsigned char rng1 = (unsigned char)m_utilities->getRandom(2, m_misc->m_mapSize - 2);
        unsigned char rng2 = (unsigned char)m_utilities->getRandom(2, m_misc->m_mapSize - 2);
        unsigned char rng3 = (unsigned char)m_utilities->getRandom(2, 8);
        MapSplat(rng1, rng2, rng3, 0x04, 0x10);
    }

    m_misc->DelayGame(DelayKreate, std::bind(&U3Graphics::Kreate5, this));

    return false;
}

bool U3Graphics::Kreate3()
{
    if (m_misc->m_callbackStack.size() > 0)
    {
        m_misc->m_callbackStack.pop();
    }
    m_forceRedraw = true;
    unsigned char range = (unsigned char)m_utilities->getRandom(4, (m_misc->m_mapSize / 2));
    unsigned char halfrange = range / 2;
    m_misc->DelayGame(DelaySplat);
    unsigned char rng1 = (unsigned char)m_utilities->getRandom(halfrange, m_misc->m_mapSize - halfrange);
    unsigned char rng2 = (unsigned char)m_utilities->getRandom(halfrange, m_misc->m_mapSize - halfrange);
    MapSplat(rng1, rng2, range, 0, 4);
    return false;
}

bool U3Graphics::Kreate2()
{
    if (m_misc->m_callbackStack.size() > 0)
    {
        m_misc->m_callbackStack.pop();
    }
    m_allowRendering = true;

    AllWater();
    m_audio->playSfx(SFX_BIGDEATH);

    m_misc->m_callbackStack.push(std::bind(&U3Graphics::Kreate4, this));
    int rngNum = m_utilities->getRandom(8, 72);
    for (int byte = 0; byte < rngNum; byte++)
    {
        m_misc->m_callbackStack.push(std::bind(&U3Graphics::Kreate3, this));
    }

    return false;
}

bool U3Graphics::Kreate1()
{
    if (m_misc->m_callbackStack.size() > 0)
    {
        m_misc->m_callbackStack.pop();
    }
    
    m_misc->m_Party[2] = 255; // Will prevent the moongates from being shown

    m_misc->DelayGame(DelayKreate, std::bind(&U3Graphics::Kreate2, this));

    for (short byte = 0; byte < 32; byte++)
    {
        m_misc->m_Monsters[byte] = 0;
    }
    for (short byte = 0; byte < 19; byte++)
    {
        m_storeIcons[byte] = m_misc->GetXYVal(m_misc->m_LocationX[byte], m_misc->m_LocationY[byte]);
        if (m_storeIcons[byte] == 0x0C)
        {
            m_storeIcons[byte] = 0x18; // dawn
        }
    }

    return false;
}

void U3Graphics::renderKreateMap()
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

void U3Graphics::DrawKreateMap()
{
    if (!m_allowRendering)
    {
        return;
    }

    if (m_forceRedraw)
    {
        renderKreateMap();
    }

    if (!m_texMap)
    {
        return;
    }

    SDL_FRect theRect{};

    theRect.x = (float)m_blockSize;
    theRect.y = (float)m_blockSize;
    theRect.w = (float)m_blockSize * 22;
    theRect.h = (float)m_blockSize * 22;
    m_resources->adjustRect(theRect);
    SDL_RenderTexture(m_resources->m_renderer, m_texMap, NULL, &theRect);
}

void U3Graphics::renderDiorama()
{
    SDL_FRect fromRect{};
    SDL_FRect fromRect1{};
    SDL_FRect outRect{};
    SDL_FRect toRect{};
    SDL_FRect toRect1{};

    char ht[64] = { 0, 1, 2, 4, 6, 4, 4, 4, 1, 4, 4, 4, 1, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
                   6, 0, 4, 6, 0, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 4, 4, 6, 6, 4, 4 };

    short minSize = (short)((m_blockSize * 22) / m_misc->m_mapSize);

    short value;

    SDL_SetRenderDrawColor(m_resources->m_renderer, 0, 0, 0, 255);
    SDL_SetRenderTarget(m_resources->m_renderer, m_texMap);
    SDL_RenderClear(m_resources->m_renderer);
    ////////////////////////////////////////////////////////////////
    
    outRect.w = minSize;
    outRect.h = minSize;

    SDL_SetRenderDrawBlendMode(m_resources->m_renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(m_resources->m_renderer, 0, 0, 0, 128);

    for (short y = 0; y < m_misc->m_mapSize; y++)
    {
        outRect.y = (float)(y * minSize);
        for (short x = 0; x < m_misc->m_mapSize; x++)
        {
            value = m_misc->GetXYVal(x, y) / 4;
            short top = ht[value];
            int minival = value;
            int multval = value / 16;
            value += (unsigned char)(16 * multval);
            //value = m_resources->GetRealTile(value);
            outRect.x = (float)(x * minSize);

			if (value < 192)
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

                outRect.y -= top;
                outRect.x -= top;
                bool shade = false;

                if (x > 0 && y > 0 && top < 3)
                {
                    if ((ht[m_misc->GetXYVal(x - 1, y) / 4] > 3) || (ht[m_misc->GetXYVal(x, y - 1) / 4] > 3) ||
                        (ht[m_misc->GetXYVal(x - 1, y - 1) / 4] > 3))
                    {
                        shade = true;
                    }
                }

                fromRect.x = 0;
                fromRect.y = 0;
                fromRect.w = (float)TEXTURE_SIZE_X;
                fromRect.h = ((float)TEXTURE_SIZE_Y / (float)minSize);
                fromRect1.x = 0;
                fromRect1.y = 0;
                fromRect1.w = ((float)TEXTURE_SIZE_X / (float)minSize);;
                fromRect1.h = (float)TEXTURE_SIZE_Y;

                toRect.h = 1.0f;
                toRect.w = minSize;

                toRect1.w = 1;
                toRect1.h = minSize;

                if (top > 0)
                {
                    for (int tempVal = 0; tempVal <= top; tempVal++)
                    {
                        toRect.x = outRect.x + tempVal;
                        toRect.y = (outRect.y + outRect.h) + tempVal;

                        toRect1.x = outRect.x + tempVal + outRect.w;
                        toRect1.y = (outRect.y + tempVal);
                        //SDL_RenderFillRect(m_resources->m_renderer, &toRect);
                        //SDL_RenderFillRect(m_resources->m_renderer, &toRect1);
                        SDL_RenderTexture(m_resources->m_renderer, m_resources->m_currentGraphics->tiles[value], &fromRect, &toRect);
                        SDL_RenderFillRect(m_resources->m_renderer, &toRect);
                        SDL_RenderTexture(m_resources->m_renderer, m_resources->m_currentGraphics->tiles[value], &fromRect1, &toRect1);
                        SDL_RenderFillRect(m_resources->m_renderer, &toRect1);
                    }
                }

				SDL_RenderTexture(m_resources->m_renderer, m_resources->m_currentGraphics->tiles[value], NULL, &outRect);

                if (shade)
                {
                    
                    SDL_RenderFillRect(m_resources->m_renderer, &outRect);      
                }

                outRect.y += top;
                outRect.x += top;
			}
		}
	}
    SDL_SetRenderDrawBlendMode(m_resources->m_renderer, SDL_BLENDMODE_NONE);
    
    /////////////////////////////////////////////////////////////

    SDL_SetRenderTarget(m_resources->m_renderer, NULL);
    m_forceRedraw = false;
    SDL_SetRenderDrawColor(m_resources->m_renderer, 0, 0, 0, 0);
}

void U3Graphics::DrawDioramaMap()
{
    SDL_FRect theRect{};

    if (m_forceRedraw)
    {
        m_fading = true;
        renderDiorama();
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
