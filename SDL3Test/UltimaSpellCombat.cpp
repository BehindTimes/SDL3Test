#include "UltimaSpellCombat.h"
#include "U3ScrollArea.h"
#include "U3Misc.h"
#include "UltimaDungeon.h"

extern U3ScrollArea m_scrollArea;
extern U3Misc m_misc;
extern UltimaDungeon m_dungeon;

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