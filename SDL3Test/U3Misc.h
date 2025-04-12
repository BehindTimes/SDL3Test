#pragma once

#include <SDL3/SDL.h>
#include <filesystem>
#include <functional>
#include <stack>
#include <queue>

struct SosariaHandle
{
	SosariaHandle() :
		WhirlX(0),
		WhirlY(0),
		WhirlDX(0),
		WhirlDY(0)
	{
		memset(Monsters, 0, sizeof(unsigned char) * 256);
	}
	std::vector<unsigned char> Map;
	unsigned char Monsters[256];
	unsigned char WhirlX;
	unsigned char WhirlY;
	unsigned char WhirlDX;
	unsigned char WhirlDY;
};

enum class GameStateMode
{
	Map,
	Dungeon
};

enum class InputType
{
	Default,
	None,
	GetDirection,
	Transact,
	InputText,
	YesNo,
	AnyKey,
	BuySell,
	Restricted,
	Callback,
	SleepCallback,
	GuildVendor,
	NumImmediate,
	LetterImmediate,
};

class U3Misc
{
public:
	explicit U3Misc();
	~U3Misc();

	void CheckAllDead();
	void OpenRstr();
	void GetMiscStuff(bool defaultData);
	bool GetRoster();
	bool GetParty();
	bool GetSosaria();
	bool PutRoster();
	bool PutParty();
	bool PutSosaria();

	unsigned char GetXYTile(short x, short y);
	unsigned char GetXYVal(int x, int y);
	void PutXYVal(unsigned char value, unsigned char x, unsigned char y);
	void PutXYTile(short value, short x, short y);
	bool CheckAlive(short member);
	void LoadUltimaMap(int map);
	void BlockExodus();
	short MonsterHere(short x, short y);
	short MaxMana(char rosNum);
	bool ProcessEvent(SDL_Event event);
	bool ProcessAnyEvent(SDL_Event event);
	void InverseTiles(bool value);
	void InverseCharDetails(short num, bool value);
	void HandleCallback(bool sleeping);
	void OtherCommand(short yell);
	void Yell(short mode);
	void SafeExodus();
	void PullSosaria();
	void PushSosaria();

	void North();
	void South();
	void East();
	void West();
	void SouthEast();
	void SouthWest();
	void NorthEast();
	void NorthWest();

	void GetChest(short spell, short chnum);
	void Ignite();
	void JoinGold();
	void JoinGold(short chnum);

	void NoGo();

	unsigned char m_Player[21][65];
	unsigned char m_Party[64];

	short m_mapSize;
	int m_mapOffset;
	std::vector<unsigned char> m_map;
	int m_xpos;
	int m_ypos;

	short m_stx;
	short m_sty;
	unsigned char m_MoonXTable[8];
	unsigned char m_MoonYTable[8];
	unsigned char m_careerTable[12];
	unsigned char m_wpnUseTable[12];
	unsigned char m_armUseTable[12];
	unsigned char m_LocationX[20];
	unsigned char m_LocationY[20];
	unsigned char m_Experience[17];
	unsigned char m_Monsters[256];
	unsigned char m_Talk[256];
	unsigned char m_Dungeon[2048];
	short m_WhirlX;
	short m_WhirlY;
	char m_WhirlDX;
	char m_WhirlDY;
	unsigned char m_gBallTileBackground;

	unsigned char m_MonsterX[8];
	unsigned char m_MonsterY[8];
	unsigned char m_MonsterTile[8];
	unsigned char m_MonsterHP[8];

	unsigned char m_CharX[4];
	unsigned char m_CharY[4];
	unsigned char m_CharTile[4];
	unsigned char m_CharShape[4];
	unsigned char m_cHide;
	bool m_gShapeSwapped[256];
	bool m_gHorseFacingEast;
	char m_gMonVarType;
	short m_gMonType;
	bool m_gResurrect;
	char m_WindDir;
	short m_zp[255];
	short m_gMoonDisp[2];
	InputType m_inputType;
	int m_wx;
	int m_wy;
	bool m_checkDead;
	bool m5BDC;
	GameStateMode m_gameMode;
	short m_heading;
	int m_xs;
	int m_ys;
	int m_dx;
	int m_dy;

	bool m_numOnly;
	short m_opnum;
	short m_opnum2;
	short m_restrictedStart;
	short m_gTorch;
	short m_lastCard;

	short m_storedir;
	short m_rosNum;

	std::stack<std::function<void()>> m_callbackStack;
	std::string m_input;
	int m_input_num;
	int m_maxInputLength;
	std::queue<int> m_weaponsList;
	bool m_YellStat;
	Uint64 m_elapsedSleepTime;
	Uint64 m_sleepCheckTime;

private:
	void Attack();
	void Board();
	void Descend();
	void Enter();
	void Exit();
	void Fire();
	void Look();
	void Klimb();
	void PeerGem();
	void Steal();
	void Transact();
	void Unlock();

	bool HandleKeyPress(SDL_KeyboardEvent key);
	void LetterCommand(SDL_Keycode key);
	bool ValidTrans(char value);
	bool ValidDir(unsigned char value);
	
	void Pass();
	
	void LookCallback();
	
	void TransactCallback();
	void TransactCallback2();
	void What();
	void What2();
	void NotHere();
	void NoEffect();
	void Routine6E6B();
	void Routine6E35();
	void IncMoves();
	void FinishAll();
	void HandleAnyKey();
	void FinalizeHealingCallback();
	void ClearTiles();
	
	void HandleLetterImmediate(SDL_Keycode key);
	void HandleNumImmediate(SDL_Keycode key);
	void HandleInputGuild(SDL_Keycode key);
	void HandleInputRestricted(SDL_Keycode key);
	void HandleInputYesNo(SDL_Keycode key);
	void HandleInputBuySell(SDL_Keycode key);
	void HandleInputText(SDL_KeyboardEvent key);
	bool HandleDefaultKeyPress(SDL_Keycode key);
	void HandleDircetionKeyPress(SDL_Keycode key);
	void HandleTransactPress(SDL_Keycode key);
	void PrintMonster(short which, bool plural, char variant);
	void PrintTile(short tile, bool plural);
	void InverseChnum(char which, bool value);
	void Speak(short perNum, short shnum);
	void Shop(short shopNum, short chnum);
	void tavernCallback();
	void grocerCallback();
	void moreFoodCallback();
	void InputNumCallback();
	void InputTextCallback();
	void setInputTypeNum(std::function<void()> func, int inputLength = 2);
	void setInputTypeYesNo(std::function<void()> func);
	void setInputTypeBuySell(std::function<void()> func);
	void setInputTypeRestricted(std::function<void()> func, short start);
	void setInputTypeNumImmediate(std::function<void()> func);
	void anotherDrinkCallback();
	void weaponsListCallback();
	void armorsListCallback();
	void weaponsBuyOrSell();
	void armorsBuyOrSell();
	void listWeaponsCallback();
	void listArmorsCallback();
	void WeaponList();
	void ArmorsList();
	void PrintWeaponList(short weapon);
	void PrintArmorList(short armor);
	void weaponsBuySellCallback();
	void armorsBuySellCallback();
	void weaponsBuyCallback();
	void armorsBuyCallback();
	void weaponsSellCallback();
	void armorsSellCallback();
	void weaponsBuySellFinishedCallback();
	void armorsBuySellFinishedCallback();
	void weaponsSellMissing();
	void armorsSellMissing();
	void weaponsBuyPoor();
	void armorsBuyPoor();
	bool AddGold(short rosNum, short gold, bool overflow);
	void healingCallback();
	void healingCallback2();
	void healingPayCallback();
	void SpellNoize(short opnum, short opnum2);
	void guildCallback();
	bool GuildPay(short rosNum, short cost);
	void GuildGive(short rosNum, short item, short amount);
	void guildCompleteCallback();
	void oracleCallback();
	void oracleFinishCallback();
	void horseVendorCallback();
	void MoveMonsters();
	void GetMonsterDir(short monNum);
	char GetHeading(short value);
	bool moveshoot(int offset);
	void move7AAA(int offset);
	bool moveoutside(int offset);
	void AttackCallback();
	void IgniteCallback();
	void JoinGoldCallback();
	void UnlockCallback();
	void UnlockKeyCallback();
	void PeerGemCallback();
	void CheckAllDeadPause();
	void HandleDeadResponse();
	void HandleDeadResponse1();
	void ResurrectCallback();
	void OtherCallback();
	void OtherCallback1();
	void EmptyCallback();
	void BribeCallback();
	void InsertCallback();
	void InsertCallback1();
	void InsertCallback2();
	bool HPSubtract(short rosNum, short amount);
	void ExodusDieCallback();
	void ExodusDieCallback1();
	void ExodusDieCallback2();
	void ExodusDieCallback3();
	void ExodusDieCallback4();
	void SleepCallback();
	void BombTrap();
	void BombTrapCallback();
	void StealCallback();
	void StealCallback1();
	bool StealDisarmFail(short rosNum);
	void StealFail();
	void GetChestCallback();
	void GetChest1(short chnum);
	void GetChestBooty();
	void ChestAcidCallback();
	void ChestPoisonCallback();
	void ChestGasCallback();
	void AddItem(short rosNum, short item, short amount);

	static constexpr std::string_view SaveLoc = "Save";
	static constexpr std::string_view ResourceLoc = "Resources";
	static constexpr std::string_view BinLoc = "Bin";
	static constexpr std::string_view DoorString = "Door";
	static constexpr std::string_view DioramaString = "Diorama\n";

	static const Uint64 damage_time = 50;
	static const Uint64 exodus_death_time = 50;
	static const Uint64 screen_flicker_time = 200;

	int m_transactNum;
	std::unique_ptr <SosariaHandle> m_saved_map;
};

