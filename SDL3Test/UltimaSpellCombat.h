#pragma once

#include <SDL3/SDL.h>

class UltimaSpellCombat
{
public:
	UltimaSpellCombat();
	void Incap();
	unsigned char GetXYDng(short x, short y);
	void PutXYDng(unsigned char value, short x, short y);
	void ShowHit(short x, short y, unsigned char hitType, unsigned char tileUnder);
	void Poison(short chnum);
	void Missed();
	void Pilfer(short chnum);
	unsigned char DetermineShape(short type);
	unsigned char CombatCharacterHere(short x, short y);
	unsigned char CombatMonsterHere(short x, short y);
	unsigned char ExodusCastle() const;
	void Spell(short chnum, short spellnum);
	void Combat();
	void drawCombat();
	void combatstart();
	bool HandleDefaultKeyPress(SDL_Keycode key);

	unsigned char m_g5521;
	unsigned char m_g835D;
	bool m_newMove;
	bool m_combatStart;
	unsigned char m_count;
	unsigned char m_count2;
private:
	void Failed();
	bool ShowHitCallback() const;
	void BigDeath(short damage, short chnum);
	void Flashriek();
	bool BigDeathCallback();
	bool BigDeathCallback1();
	bool BigDeathCallback2();
	void DamageMonster(short which, short damage, short chnum);
	void AddExp(short chnum, short amount);
	short BackGround(short montype);
	unsigned char HowMany() const;
	void PrintMonster(short which, bool plural, char variant);
	void GetScreen(short resid);

	unsigned char m_hitType;
	unsigned char m_origValue;
	short m_x;
	short m_y;
	unsigned char m_g835E;
	unsigned char m_g56E7;

	short m_chNum;
	short m_damage;
};

