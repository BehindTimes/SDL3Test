#pragma once
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
	unsigned char ExodusCastle();
private:
	void ShowHitCallback();
	unsigned char m_hitType;
	unsigned char m_origValue;
	short m_x;
	short m_y;
	unsigned char m_g835E;
};

