#include "UltimaSpellCombat.h"
#include "U3ScrollArea.h"
#include "U3Misc.h"
#include "UltimaDungeon.h"
#include "U3Resources.h"
#include "U3Utilities.h"
#include "UltimaGraphics.h"
#include "UltimaIncludes.h"

extern std::unique_ptr<U3Resources> m_resources;
extern std::unique_ptr<U3Misc> m_misc;
extern std::unique_ptr<U3ScrollArea> m_scrollArea;
extern std::unique_ptr<U3Utilities> m_utilities;
extern std::unique_ptr<UltimaDungeon> m_dungeon;
extern std::unique_ptr<U3Graphics> m_graphics;

void UltimaSpellCombat::SetupFuture()
{
    short mon, chnum, distance, closestChar, closestVal, newx, newy, deltaX, deltaY;

    SetupNow();
    for (mon = 0; mon < 8; mon++)
    {
        if (m_misc->m_MonsterHP[mon] != 0)
        {
            closestChar = 8;     // non-player
            closestVal = 128;    // way big
            for (chnum = 0; chnum < 4; chnum++)
            {
                distance = m_utilities->Absolute(m_futureMonX[mon] - m_misc->m_CharX[chnum]) + m_utilities->Absolute(m_futureMonY[mon] - m_misc->m_CharY[chnum]);
                if (distance < closestVal)
                {
                    closestVal = distance;
                    closestChar = chnum;
                }
            }
            deltaX = m_misc->GetHeading(m_misc->m_CharX[closestChar] - m_futureMonX[mon]);
            deltaY = m_misc->GetHeading(m_misc->m_CharY[closestChar] - m_futureMonY[mon]);
            newx = m_futureMonX[mon] + deltaX;
            newy = m_futureMonY[mon] + deltaY;
            if (FutureMonsterHere(newx, newy))
            {
                newx = m_futureMonX[mon];
                newy = m_futureMonY[mon] + deltaY;
                if (FutureMonsterHere(newx, newy))
                {
                    newx = m_futureMonX[mon] + deltaX;
                    newy = m_futureMonY[mon];
                    if (FutureMonsterHere(newx, newy))
                    {
                        newx = m_futureMonX[mon];
                        newy = m_futureMonY[mon];
                    }
                }
            }
            m_futureMonX[mon] = (unsigned char)newx;
            m_futureMonY[mon] = (unsigned char)newy;
        }
    }
}

bool UltimaSpellCombat::FutureMonsterHere(short x, short y) const  // monster or character
{
    short mon;
    bool result;
    result = false;
    for (mon = 7; mon >= 0; mon--)
    {
        if (m_misc->m_MonsterHP[mon] != 0)
        {
            if (m_futureMonX[mon] == x && m_futureMonY[mon] == y)
            {
                result = true;
            }
        }
    }
    for (mon = 0; mon < 4; mon++)
    {
        if (m_misc->CheckAlive(mon))
        {
            if (m_misc->m_CharX[mon] == x && m_misc->m_CharY[mon] == y)
            {
                result = true;
            }
        }
    }
    return result;
}

bool UltimaSpellCombat::NearlyDead(short who)   // 0=anybody, otherwise 1-4
{
    short chnum;
    long hp;
    bool nearlyDead;

    nearlyDead = false;
    if (who > 0)   // a specific character
    {
        chnum = who - 1;
        hp = m_misc->m_Player[m_misc->m_Party[6 + chnum]][26] * 256 + m_misc->m_Player[m_misc->m_Party[6 + chnum]][27];
        if (hp < 50)
        {
            nearlyDead = true;
        }
    }
    else   // anybody
    {
        for (chnum = 0; chnum < 4; chnum++)
        {
            hp = m_misc->CheckAlive(chnum);
            if (hp == false)
            {
                nearlyDead = true;
            }
            else
            {
                hp = m_misc->m_Player[m_misc->m_Party[6 + chnum]][26] * 256 + m_misc->m_Player[m_misc->m_Party[6 + chnum]][27];
                if (hp < 50)
                {
                    nearlyDead = true;
                }
            }
        }
    }
    return nearlyDead;
}

bool UltimaSpellCombat::MonsterCanAttack(short x, short y)
{
    bool result = false;
    short mon;

    /*  // first check for ones you can't run from
    if (gMonType==0x1C) return TRUE; // Man-O-War
    if (gMonType==0x1A) return TRUE; // Serpent
    if (gMonType==0x2C) return TRUE; // Wizard
    if (gMonType==0x36) return TRUE; // Daemon
    if (gMonType==0x3C) return TRUE; // Balron */    // don't bother, it's ok to run from these.
    // now check around this spot
    result |= (CombatMonsterHere(x - 1, y - 1) != 255);
    result |= (CombatMonsterHere(x, y - 1) != 255);
    result |= (CombatMonsterHere(x + 1, y - 1) != 255);
    result |= (CombatMonsterHere(x - 1, y) != 255);
    result |= (CombatMonsterHere(x + 1, y) != 255);
    result |= (CombatMonsterHere(x - 1, y + 1) != 255);
    result |= (CombatMonsterHere(x, y + 1) != 255);
    result |= (CombatMonsterHere(x + 1, y + 1) != 255);
    // now check for things that can shoot (dragons)
    if (m_misc->m_gMonType == 0x3A)
    {
        for (mon = 0; mon < 8; mon++)
        {
            if (m_misc->m_MonsterHP[mon])
            {
                if (m_utilities->Absolute(m_misc->m_MonsterX[mon] - x) == m_utilities->Absolute(m_misc->m_MonsterY[mon] - y))
                {
                    result = true;
                }
            }
        }
    }
    return result;
}

short UltimaSpellCombat::ThreatValue()
{   // total experience value of monsters
    char mon;
    short total, expval;

    expval = m_misc->m_Experience[(m_misc->m_gMonType / 2) & 0x0F];
    total = 0;
    for (mon = 0; mon < 8; mon++)
    {
        if (m_misc->m_MonsterHP[mon] != 0)
        {
            total += expval;
        }
    }
    // Things that are poisonous are twice the threat
    if (m_misc->m_gMonType == 0x1C || m_misc->m_gMonType == 0x3C || m_misc->m_gMonType == 0x38)
    {
        total *= 2;
    }
    return total;
}

bool UltimaSpellCombat::CombatCharHere(short x, short y)
{
    short value, chnum;
    bool isOneHere;

    isOneHere = false;
    value = m_misc->GetXYTile(x, y);
    if (value != 2 && value != 4 && value != 6 && value != 0x10)
    {
        isOneHere = true;
    }
    for (chnum = 0; chnum < 4; chnum++)
    {
        if (m_misc->m_CharX[chnum] == x && m_misc->m_CharY[chnum] == y)
        {
            isOneHere = true;
        }
    }
    return isOneHere;
}

void UltimaSpellCombat::SetupNow()
{
    short mon;

    for (mon = 0; mon < 8; mon++)
    {
        m_futureMonX[mon] = m_misc->m_MonsterX[mon];
        m_futureMonY[mon] = m_misc->m_MonsterY[mon];
    }
}

unsigned int UltimaSpellCombat::DirToNearestMonster(short chnum)  // returns key to 'press' to head towards nearest baddie
{
    short mon, distance, closestMonster, closestVal;
    short deltaX, deltaY;

    closestMonster = 8;    // non-monster
    closestVal = 128;      // way big
    for (mon = 0; mon < 8; mon++)
    {
        if (m_misc->m_MonsterHP[mon] != 0)
        {
            distance = m_utilities->Absolute(m_futureMonX[mon] - m_misc->m_CharX[chnum]) + m_utilities->Absolute(m_futureMonY[mon] - m_misc->m_CharY[chnum]);
            if (distance < closestVal)
            {
                closestVal = distance;
                closestMonster = mon;
            }
        }
    }
    deltaX = m_misc->GetHeading(m_futureMonX[closestMonster] - m_misc->m_CharX[chnum]);
    deltaY = m_misc->GetHeading(m_futureMonY[closestMonster] - m_misc->m_CharY[chnum]);
    return AutoMoveChar(chnum, deltaX, deltaY);
}


unsigned int UltimaSpellCombat::LineUpToMonster(short chnum)  // returns key to 'press' to end up lined up to a monster next turn.
{
    unsigned int dir;
    bool allowDigonals;
    m_resources->GetPreference(U3PreferencesType::Allow_Diagonal, allowDigonals);

    if (m_misc->m_CharX[chnum] < 6)   // left half of screen
    {
        dir = MonsterLinedUp(chnum, m_misc->m_CharX[chnum] + 1, m_misc->m_CharY[chnum]);
        if (dir)
        {
            return AutoMoveChar(chnum, 1, 0);
        }
        dir = MonsterLinedUp(chnum, m_misc->m_CharX[chnum] - 1, m_misc->m_CharY[chnum]);
        if (dir)
        {
            return AutoMoveChar(chnum, -1, 0);
        }
        dir = MonsterLinedUp(chnum, m_misc->m_CharX[chnum], m_misc->m_CharY[chnum] - 1);
        if (dir)
        {
            return AutoMoveChar(chnum, 0, -1);
        }
        dir = MonsterLinedUp(chnum, m_misc->m_CharX[chnum], m_misc->m_CharY[chnum] + 1);
        if (dir)
        {
            return AutoMoveChar(chnum, 0, 1);
        }
        if (!allowDigonals)
        {
            return DirToNearestMonster(chnum);
        }
        dir = MonsterLinedUp(chnum, m_misc->m_CharX[chnum] + 1, m_misc->m_CharY[chnum] - 1);
        if (dir)
        {
            return dir;
        }
        dir = MonsterLinedUp(chnum, m_misc->m_CharX[chnum] + 1, m_misc->m_CharY[chnum] + 1);
        if (dir)
        {
            return dir;
        }
        dir = MonsterLinedUp(chnum, m_misc->m_CharX[chnum] - 1, m_misc->m_CharY[chnum] - 1);
        if (dir)
        {
            return dir;
        }
        dir = MonsterLinedUp(chnum, m_misc->m_CharX[chnum] - 1, m_misc->m_CharY[chnum] + 1);
        if (dir)
        {
            return dir;
        }
        return DirToNearestMonster(chnum);
    }
    else   // right half of screen
    {
        dir = MonsterLinedUp(chnum, m_misc->m_CharX[chnum] - 1, m_misc->m_CharY[chnum]);
        if (dir)
        {
            return AutoMoveChar(chnum, -1, 0);
        }
        dir = MonsterLinedUp(chnum, m_misc->m_CharX[chnum] + 1, m_misc->m_CharY[chnum]);
        if (dir)
        {
            return AutoMoveChar(chnum, 1, 0);
        }
        dir = MonsterLinedUp(chnum, m_misc->m_CharX[chnum], m_misc->m_CharY[chnum] - 1);
        if (dir)
        {
            return AutoMoveChar(chnum, 0, -1);
        }
        dir = MonsterLinedUp(chnum, m_misc->m_CharX[chnum], m_misc->m_CharY[chnum] + 1);
        if (dir)
        {
            return AutoMoveChar(chnum, 0, 1);
        }
        if (!allowDigonals)
        {
            return DirToNearestMonster(chnum);
        }
        dir = MonsterLinedUp(chnum, m_misc->m_CharX[chnum] - 1, m_misc->m_CharY[chnum] - 1);
        if (dir)
        {
            return dir;
        }
        dir = MonsterLinedUp(chnum, m_misc->m_CharX[chnum] - 1, m_misc->m_CharY[chnum] + 1);
        if (dir)
        {
            return dir;
        }
        dir = MonsterLinedUp(chnum, m_misc->m_CharX[chnum] + 1, m_misc->m_CharY[chnum] - 1);
        if (dir)
        {
            return dir;
        }
        dir = MonsterLinedUp(chnum, m_misc->m_CharX[chnum] + 1, m_misc->m_CharY[chnum] + 1);
        if (dir)
        {
            return dir;
        }
        return DirToNearestMonster(chnum);
    }
}

unsigned int UltimaSpellCombat::doKeyNow(short deltaX, short deltaY)
{
    unsigned int keyToPress = SDLK_SPACE;    // Pass
    if (deltaX == -1 && deltaY == 1)
    {
        keyToPress = SDLK_KP_1;    // Southwest
    }
    if (deltaX == 0 && deltaY == 1)
    {
        keyToPress = SDLK_DOWN;    // South
    }
    if (deltaX == 1 && deltaY == 1)
    {
        keyToPress = SDLK_KP_3;    // Southeast
    }
    if (deltaX == -1 && deltaY == 0)
    {
        keyToPress = SDLK_LEFT;    // West
    }
    if (deltaX == 1 && deltaY == 0)
    {
        keyToPress = SDLK_RIGHT;    // East
    }
    if (deltaX == -1 && deltaY == -1)
    {
        keyToPress = SDLK_KP_7;    // Northwest
    }
    if (deltaX == 0 && deltaY == -1)
    {
        keyToPress = SDLK_UP;    // North
    }
    if (deltaX == 1 && deltaY == -1)
    {
        keyToPress = SDLK_KP_9;    // Northeast
    }
    return keyToPress;
}

unsigned int UltimaSpellCombat::AutoMoveChar(short chnum, short deltaX, short deltaY)
{
    short saveDeltaX;

    // if not allowed to move diagonally, and the nearest monster is diagonally
    // away, choose vertical over horizontal.
    bool allowDiagonal;
    m_resources->GetPreference(U3PreferencesType::Allow_Diagonal, allowDiagonal);
    if (allowDiagonal == 0 && deltaX != 0 && deltaY != 0)
    {
        deltaX = 0;
    }
    if (!CombatCharHere(m_misc->m_CharX[chnum] + deltaX, m_misc->m_CharY[chnum] + deltaY))
    {
        return doKeyNow(deltaX, deltaY);
    }
    if (deltaX == 0)
    {
        if (allowDiagonal)
        {
            deltaX = 1;
            if (!CombatCharHere(m_misc->m_CharX[chnum] + deltaX, m_misc->m_CharY[chnum] + deltaY))
            {
                return doKeyNow(deltaX, deltaY);
            }
            deltaX = -1;
            if (!CombatCharHere(m_misc->m_CharX[chnum] + deltaX, m_misc->m_CharY[chnum] + deltaY))
            {
                return doKeyNow(deltaX, deltaY);
            }
        }
        deltaY = 0;
        deltaX = 1;
        if (!CombatCharHere(m_misc->m_CharX[chnum] + deltaX, m_misc->m_CharY[chnum] + deltaY))
        {
            return doKeyNow(deltaX, deltaY);
        }
        deltaX = -1;
        if (!CombatCharHere(m_misc->m_CharX[chnum] + deltaX, m_misc->m_CharY[chnum] + deltaY))
        {
            return doKeyNow(deltaX, deltaY);
        }
        deltaX = 2;
        return doKeyNow(deltaX, deltaY);    // PASS!@1
    }
    if (deltaY == 0)
    {
        if (allowDiagonal)
        {
            deltaY = -1;
            if (!CombatCharHere(m_misc->m_CharX[chnum] + deltaX, m_misc->m_CharY[chnum] + deltaY))
            {
                return doKeyNow(deltaX, deltaY);
            }
            deltaY = 1;
            if (!CombatCharHere(m_misc->m_CharX[chnum] + deltaX, m_misc->m_CharY[chnum] + deltaY))
            {
                return doKeyNow(deltaX, deltaY);
            }
        }
        deltaX = 0;
        deltaY = -1;
        if (!CombatCharHere(m_misc->m_CharX[chnum] + deltaX, m_misc->m_CharY[chnum] + deltaY))
        {
            return doKeyNow(deltaX, deltaY);
        }
        deltaY = 1;
        if (!CombatCharHere(m_misc->m_CharX[chnum] + deltaX, m_misc->m_CharY[chnum] + deltaY))
        {
            return doKeyNow(deltaX, deltaY);
        }
        deltaY = 2;
        return doKeyNow(deltaX, deltaY);    // I give up, I'm stuck, pass
    }
    // must be an attempted diagonal move
    saveDeltaX = deltaX;
    deltaX = 0;
    if (!CombatCharHere(m_misc->m_CharX[chnum] + deltaX, m_misc->m_CharY[chnum] + deltaY))
    {
        return doKeyNow(deltaX, deltaY);
    }
    deltaX = saveDeltaX;
    deltaY = 0;
    if (!CombatCharHere(m_misc->m_CharX[chnum] + deltaX, m_misc->m_CharY[chnum] + deltaY))
    {
        return doKeyNow(deltaX, deltaY);
    }
    deltaX = 2;    // pass!@

    return doKeyNow(deltaX, deltaY);
}

unsigned int UltimaSpellCombat::MonsterLinedUp(short chnum, short x, short y) // returns key to 'press' to shoot nearest
{
    // baddie.  0 = Nothing lined up
    short mon, distance, closestMonster, closestVal;
    short deltaX, deltaY;
    unsigned int keyToPress;
    bool thisOne;

    closestMonster = 8;    // non-monster
    closestVal = 128;      // way big
    for (mon = 0; mon < 8; mon++)
    {
        thisOne = false;
        if (m_misc->m_MonsterHP[mon] != 0)
        {
            if (m_futureMonX[mon] == x || m_futureMonY[mon] == y)
            {
                thisOne = true;
            }
            bool allowDiagonals;
            m_resources->GetPreference(U3PreferencesType::Allow_Diagonal, allowDiagonals);
            if (allowDiagonals)
            {
                thisOne |= (m_utilities->Absolute(x - m_futureMonX[mon]) == m_utilities->Absolute(y - m_futureMonY[mon]));
            }
        }
        if (thisOne)
        {
            distance = m_utilities->Absolute(m_futureMonX[mon] - x) + m_utilities->Absolute(m_futureMonY[mon] - y);
            if (distance < closestVal)
            {
                closestVal = distance;
                closestMonster = mon;
            }
        }
    }
    if (closestMonster == 8)
    {
        return 0;
    }
    deltaX = m_misc->GetHeading(m_futureMonX[closestMonster] - x);
    deltaY = m_misc->GetHeading(m_futureMonY[closestMonster] - y);
    keyToPress = 0;
    if (deltaX == -1 && deltaY == 1)
    {
        keyToPress = SDLK_KP_1;    // Southwest
    }
    if (deltaX == 0 && deltaY == 1)
    {
        keyToPress = SDLK_DOWN;    // South
    }
    if (deltaX == 1 && deltaY == 1)
    {
        keyToPress = SDLK_KP_3;    // Southeast
    }
    if (deltaX == -1 && deltaY == 0)
    {
        keyToPress = SDLK_LEFT;    // West
    }
    if (deltaX == 1 && deltaY == 0)
    {
        keyToPress = SDLK_RIGHT;    // East
    }
    if (deltaX == -1 && deltaY == -1)
    {
        keyToPress = SDLK_KP_7;    // Northwest
    }
    if (deltaX == 0 && deltaY == -1)
    {
        keyToPress = SDLK_UP;    // North
    }
    if (deltaX == 1 && deltaY == -1)
    {
        keyToPress = SDLK_KP_9;    // Northeast
    }
    if (chnum < 4)
    {
        return AutoMoveChar(chnum, deltaX, deltaY);
    }
    return keyToPress;
}

unsigned int UltimaSpellCombat::MonsterNearby(short chnum)
{
    short x, y;
    bool allowDiagonal;
    m_resources->GetPreference(U3PreferencesType::Allow_Diagonal, allowDiagonal);

    x = m_misc->m_CharX[chnum];
    y = m_misc->m_CharY[chnum];
    if (CombatMonsterHere(x, y - 1) != 255)
    {
        return SDLK_UP;    // North
    }
    if (CombatMonsterHere(x - 1, y) != 255)
    {
        return SDLK_LEFT;    // West
    }
    if (CombatMonsterHere(x + 1, y) != 255)
    {
        return SDLK_RIGHT;    // East
    }
    if (CombatMonsterHere(x, y + 1) != 255)
    {
        return SDLK_DOWN;    // South
    }
    if (!allowDiagonal)
    {
        return 0;
    }
    if (CombatMonsterHere(x - 1, y - 1) != 255)
    {
        return SDLK_KP_7;    // Northwest
    }
    if (CombatMonsterHere(x - 1, y + 1) != 255)
    {
        return SDLK_KP_1;    // Southwest
    }
    if (CombatMonsterHere(x + 1, y - 1) != 255)
    {
        return SDLK_KP_9;    // Northeast
    }
    if (CombatMonsterHere(x + 1, y + 1) != 255)
    {
        return SDLK_KP_3;    // Northwest
    }
    return 0;
}

void UltimaSpellCombat::AutoCombat(short chnum)
{
    char clss;
    short lowestHP, lowChar;
    short x, y, rosNum, chnum2, wpn, magic;
    unsigned int dir;
    short hp;
    bool isWiz, isCler, isMulti, castMittar;

    rosNum = m_misc->m_Party[6 + chnum];
    magic = m_misc->m_Player[rosNum][25];
    clss = m_misc->m_Player[rosNum][23];
    isMulti = (clss == m_misc->m_careerTable[8] || clss == m_misc->m_careerTable[10]);
    isWiz = (clss == m_misc->m_careerTable[2] || clss == m_misc->m_careerTable[6] || clss == m_misc->m_careerTable[9] || isMulti);
    isCler = (clss == m_misc->m_careerTable[1] || clss == m_misc->m_careerTable[4] || clss == m_misc->m_careerTable[7] || isMulti);
    x = m_misc->m_CharX[chnum];
    y = m_misc->m_CharY[chnum];
    // Nearly dead, run away!
    if (NearlyDead(chnum + 1) && MonsterCanAttack(x, y))
    {
        bool allowDiagonal;
        m_resources->GetPreference(U3PreferencesType::Allow_Diagonal, allowDiagonal);
        if (!MonsterCanAttack(x, y + 1) && !CombatCharHere(x, y + 1))
        {
            m_misc->m_InputDeque.push_back(SDLK_DOWN);
            return;
        }
        if (allowDiagonal) {
            if (!MonsterCanAttack(x - 1, y + 1) && !CombatCharHere(x - 1, y + 1))
            {
                m_misc->m_InputDeque.push_back(SDLK_KP_1);
                return;
            }
            if (!MonsterCanAttack(x + 1, y + 1) && !CombatCharHere(x + 1, y + 1))
            {
                m_misc->m_InputDeque.push_back(SDLK_KP_3);
                return;
            }
        }
        if (!MonsterCanAttack(x - 1, y) && !CombatCharHere(x - 1, y))
        {
            m_misc->m_InputDeque.push_back(SDLK_LEFT);
            return;
        }
        if (!MonsterCanAttack(x + 1, y) && !CombatCharHere(x + 1, y))
        {
            m_misc->m_InputDeque.push_back(SDLK_RIGHT);
            return;
        }
        if (!MonsterCanAttack(x, y - 1) && !CombatCharHere(x, y - 1))
        {
            m_misc->m_InputDeque.push_back(SDLK_UP);
            return;
        }
        if (allowDiagonal)
        {
            if (!MonsterCanAttack(x - 1, y - 1) && !CombatCharHere(x - 1, y - 1))
            {
                m_misc->m_InputDeque.push_back(SDLK_KP_7);
                return;
            }
            if (!MonsterCanAttack(x + 1, y - 1) && !CombatCharHere(x + 1, y - 1))
            {
                m_misc->m_InputDeque.push_back(SDLK_KP_9);
                return;
            }
        }
    }
    // Can cast Repond, haven't casted it yet, and these are orcs?
    if ((m_misc->m_gMonType == 0x30) && (m_g5521 == 0) && isWiz)
    {
        m_misc->m_InputDeque.push_back(SDLK_A);
        if (isMulti)
        {
            m_misc->m_InputDeque.push_back(SDLK_W);
        }
        m_misc->m_InputDeque.push_back(SDLK_C);
        return;
    }
    // Can cast Pontori, haven't casted it yet, and these are skellyz?
    if ((m_misc->m_gMonType == 0x32) && (m_g56E7 == 0) && isCler)
    {
        m_misc->m_InputDeque.push_back(SDLK_A);
        if (isMulti)
        {
            m_misc->m_InputDeque.push_back(SDLK_C);
        }
        m_misc->m_InputDeque.push_back(SDLK_C);
        return;
    }
    // Can cast nameless spell and there is a big threat
    if (magic >= 75 && isWiz && (ThreatValue() > 60))
    {
        m_misc->m_InputDeque.push_back(SDLK_P);
        if (isMulti)
        {
            m_misc->m_InputDeque.push_back(SDLK_W);
        }
        m_misc->m_InputDeque.push_back(SDLK_C);
        return;
    }
    // Can cast ZXKUQYB and there is a bigger threat
    if (magic >= 70 && isCler && (ThreatValue() > 80))
    {
        m_misc->m_InputDeque.push_back(SDLK_O);
        if (isMulti)
        {
            m_misc->m_InputDeque.push_back(SDLK_C);
        }
        m_misc->m_InputDeque.push_back(SDLK_C);
        return;
    }
    // Can cast Sanctu, and a character needs it?
    if (magic >= 10 && isCler)
    {
        lowestHP = 9999;
        lowChar = -1;
        for (chnum2 = 0; chnum2 < 4; chnum2++)
        {
            hp = m_misc->m_Player[m_misc->m_Party[6 + chnum2]][26] * 256 + m_misc->m_Player[m_misc->m_Party[6 + chnum2]][27];
            if (m_misc->CheckAlive(chnum2) && hp < lowestHP)
            {
                lowestHP = hp;
                lowChar = chnum2;
            }
        }
        if (lowestHP < 75)
        {
            m_misc->m_InputDeque.push_back(SDLK_1 + lowChar);
            m_misc->m_InputDeque.push_back(SDLK_C);
            if (isMulti)
            {
                m_misc->m_InputDeque.push_back(SDLK_C);
            }
            m_misc->m_InputDeque.push_back(SDLK_C);
            return;
        }
    }

    // Can cast Mittar and not already weilding a magic bow?
    castMittar = false;
    wpn = m_misc->m_Player[rosNum][48];
    if ((magic >= 5) && isWiz && (wpn != 9) && (wpn != 13))
    {
        castMittar = true;
    }
    if (wpn == 3 || wpn == 5 || wpn == 9 || wpn == 13 || castMittar)    // projectile weapon
    {
        SetupNow();
        dir = MonsterLinedUp(5, m_misc->m_CharX[chnum], m_misc->m_CharY[chnum]);
        if (dir != 0 && dir != SDLK_SPACE)
        {
            if (castMittar)
            {
                m_misc->m_InputDeque.push_back(dir);
                m_misc->m_InputDeque.push_back(SDLK_B);
                if (isMulti)
                {
                    m_misc->m_InputDeque.push_back(SDLK_W);
                }
                m_misc->m_InputDeque.push_back(SDLK_C);
            }
            else
            {
                m_misc->m_InputDeque.push_back(dir);
                m_misc->m_InputDeque.push_back(SDLK_A);
            }
            return;
        }
        else
        {
            SetupFuture();
            dir = LineUpToMonster(chnum);
            m_misc->m_InputDeque.push_back(dir);
        }
    }
    else   // hand to hand weapon only
    {
        if (NearlyDead(chnum + 1) && m_misc->m_CharY[chnum] < 10)
        {
            m_misc->m_InputDeque.push_back(SDLK_SPACE);
            return;
        }    // don't advance!
        dir = MonsterNearby(chnum);
        if (dir != 0)
        {
            //if (NearlyDead(chnum+1)) { AddMacro(' '); return; } // don't advance!
            m_misc->m_InputDeque.push_back(dir);
            m_misc->m_InputDeque.push_back(SDLK_A);
            return;
        }
        else
        {
            SetupFuture();
            dir = DirToNearestMonster(chnum);
            m_misc->m_InputDeque.push_back(dir);
            return;
        }
    }
}
