#include "UltimaSpellCombat.h"
#include "U3ScrollArea.h"
#include "U3Misc.h"
#include "UltimaDungeon.h"
#include "U3Resources.h"
#include "U3Utilities.h"

extern U3ScrollArea m_scrollArea;
extern U3Misc m_misc;
extern UltimaDungeon m_dungeon;
extern U3Resources m_resources;
extern U3Utilities m_utilities;

UltimaSpellCombat::UltimaSpellCombat() :
    m_hitType(0),
    m_origValue(0),
    m_x(0),
    m_y(0),
    m_g835E(0)
{
}

void UltimaSpellCombat::Incap()
{
	m_scrollArea.UPrintMessage(126);
}

unsigned char UltimaSpellCombat::GetXYDng(short x, short y)
{
    if (y < 0)
    {
        y += 16;
    }
    if (x < 0)
    {
        x += 16;
    }
    if (y > 15)
    {
        y -= 16;
    }
    if (x > 15)
    {
        x -= 16;
    }
    return m_misc.m_Dungeon[(m_dungeon.m_dungeonLevel * 256) + (y * 16) + x];
}

void UltimaSpellCombat::PutXYDng(unsigned char value, short x, short y) // ??
{
    m_misc.m_Dungeon[(m_dungeon.m_dungeonLevel * 256) + (y * 16) + x] = value;
}

void UltimaSpellCombat::ShowHit(short x, short y, unsigned char hitType, unsigned char tileUnder)
{
    bool sound_inactive;
    m_resources.GetPreference(U3PreferencesType::Sound_Inactive, sound_inactive);
    if (sound_inactive)
    {
        return;
    }
    //if (prefs.soundActive) return;
    m_origValue = m_misc.GetXYTile(m_x, m_y);
    m_misc.m_gBallTileBackground = tileUnder;
    m_misc.PutXYTile(hitType, m_x, m_y);
    m_resources.SwapShape(hitType);
    m_misc.DelayGame(200, std::bind(&UltimaSpellCombat::ShowHitCallback, this));
    m_hitType = hitType;
    m_x = x;
    m_y = y;
}

bool UltimaSpellCombat::ShowHitCallback()
{
    m_resources.SwapShape(m_hitType);
    m_misc.PutXYTile(m_origValue, m_x, m_y);

    return false;
}

void UltimaSpellCombat::Poison(short chnum) // $8881
{
    short rosNum;
    rosNum = m_misc.m_Party[6 + chnum];
    int rngNum = m_utilities.getRandom(0, 255);
    if ((rngNum & 0x03) != 0)
    {
        return;
    }
    if (m_misc.m_Player[rosNum][17] != 'G')
    {
        return;
    }
    m_misc.m_Player[rosNum][17] = 'P';
    m_scrollArea.UPrintMessage(141);
    std::string dispString = std::to_string(chnum + 1);
    m_scrollArea.UPrintWin(dispString);
    m_misc.m_wx++;
    m_scrollArea.UPrintMessage(149);
}

void UltimaSpellCombat::Missed() // $8414
{
    m_scrollArea.UPrintMessage(147);
}

void UltimaSpellCombat::Pilfer(short chnum) // $881F
{
    short rosNum, item;
    rosNum = m_misc.m_Party[6 + chnum];
    int rngNum = m_utilities.getRandom(0, 255);
    if (rngNum < 128)
    {
        rngNum = m_utilities.getRandom(0, 15);
        item = rngNum;
        if (item == 0)
        {
            return;
        }
        if (m_misc.m_Player[rosNum][48] == item)
            return;
        if (m_misc.m_Player[rosNum][48 + item] == 0)
            return;
        m_misc.m_Player[rosNum][48 + item] = 0;
    }
    else
    {
        rngNum = m_utilities.getRandom(0, 7);
        item = rngNum;
        if (item == 0)
        {
            return;
        }
        if (m_misc.m_Player[rosNum][40] == item)
        {
            return;
        }
        if (m_misc.m_Player[rosNum][40 + item] == 0)
        {
            return;
        }
        m_misc.m_Player[rosNum][40 + item] = 0;
    }
    m_scrollArea.UPrintMessage(141);
    std::string dispString = std::to_string(chnum + 1);
    m_scrollArea.UPrintWin(dispString);
    m_misc.m_wx++;
    m_scrollArea.UPrintMessage(148);
}

unsigned char UltimaSpellCombat::DetermineShape(short type) // $7F5D
{
    if (type == m_misc.m_careerTable[0])
    {
        return 0x80;
    }
    if (type == m_misc.m_careerTable[1])
    {
        return 0x82;
    }
    if (type == m_misc.m_careerTable[2])
    {
        return 0x84;
    }
    if (type == m_misc.m_careerTable[3])
    {
        return 0x86;
    }
    if (type == m_misc.m_careerTable[4])
    {
        return 0x80;
    }
    if (type == m_misc.m_careerTable[5])
    {
        return 0x80;
    }
    if (type == m_misc.m_careerTable[6])
    {
        return 0x22;
    }
    if (type == m_misc.m_careerTable[7])
    {
        return 0x84;
    }
    if (type == m_misc.m_careerTable[8])
    {
        return 0x82;
    }
    if (type == m_misc.m_careerTable[9])
    {
        return 0x84;
    }
    return 0x7E;
}

unsigned char UltimaSpellCombat::CombatCharacterHere(short x, short y)
{
    char i;
    for (i = 3; i >= 0; i--)
    {
        if (m_misc.m_CharX[i] == x && m_misc.m_CharY[i] == y)
        {
            return i;
        }
    }
    return 255;
}

unsigned char UltimaSpellCombat::CombatMonsterHere(short x, short y) // $7D24
{
    short mon;
    short result;
    result = 255;
    for (mon = 7; mon >= 0; mon--)
    {
        if (m_misc.m_MonsterHP[mon] != 0)
        {
            if (m_misc.m_MonsterX[mon] == x && m_misc.m_MonsterY[mon] == y)
            {
                result = mon;
            }
        }
    }
    return (unsigned char)result;
}

unsigned char UltimaSpellCombat::ExodusCastle() // $6F43
{
    short value;
    value = m_misc.m_Party[2];
    if (m_misc.m_Party[15] == 1)
    {
        return 0xFF;
    }
    if (value == 0x80)
    {
        value = m_g835E;
    }
    if (value != 3)
    {
        return 0xFF;
    }
    if (m_misc.m_Party[3] != m_misc.m_LocationX[1])
    {
        return 0xFF;
    }
    return 0;    // In Exodus Castle!
}
