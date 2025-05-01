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
	Dungeon,
	Combat
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
	AnyKeyEscape,
	BuySell,
	Restricted,
	SleepCallback,
	GuildVendor,
	NumImmediate,
	LetterImmediate,
	ChooseWizardCleric,
	ZStats
};

class U3Misc
{
public:
	explicit U3Misc();
	~U3Misc();

	void CheckAllDead();
	bool OpenRstr();
	void GetMiscStuff(bool defaultData);
	bool GetRoster();
	bool GetParty();
	bool GetSosaria();
	bool PutRoster() const;
	bool PutParty() const;
	bool PutSosaria();

	unsigned char GetXYTile(short x, short y);
	unsigned char GetXYVal(short x, short y);
	void PutXYVal(unsigned char value, unsigned char x, unsigned char y);
	void PutXYTile(short value, short x, short y);
	bool CheckAlive(short member) const;
	void LoadUltimaMap(short map);
	void BlockExodus();
	short MonsterHere(short x, short y) const;
	short MaxMana(char rosNum) const;
	bool ProcessEvent(SDL_Event event);
	bool ProcessAnyEvent(SDL_Event event);
	void InverseTiles(bool value, Uint64 tileTime = 1000);
	void InverseCharDetails(short num, bool value);
	void HandleCallback(bool sleeping);
	void OtherCommand(short yell);
	void Yell(short mode);
	void SafeExodus();
	void PullSosaria();
	void PushSosaria();

	void DelayGame(Uint64 delay_time);
	void DelayGame(Uint64 delay_time, std::function<bool()> callback);

	void North();
	void South();
	void East();
	void West();
	void SouthEast();
	void SouthWest();
	void NorthEast();
	void NorthWest();

	void Cast(short chNum);
	void ModifyOrder();
	void NegateTime();
	void ReadyWeapon(short chnum);
	void WearArmor();
	void ZStats(short mode, short chnum);

	void GetChest(short spell, short chnum);
	void HandEquip();
	void Ignite();
	void JoinGold();
	void JoinGold(short chnum);
	void Speak(short perNum, short shnum) const;
	bool StealDisarmFail(short rosNum) const;

	void NoGo();
	void BombTrap();
	bool BombTrapCallback();
	void PutMiscStuff() const;
	void ResetSosaria();
	bool HPSubtract(short rosNum, short amount);
	void Pass();
	void Routine6E35();
	void AgeChars();
	bool FinishTurnCallback();
	bool AgeCallback();
	bool FinishAge();
	void AddProcessEvent();
	void AddInverse();
	void GetChest();
	void Yell();
	void AddFinishTurn();
	void WhirlPool();
	void PrintTile(short tile, bool plural);
	void InverseChnum(char which, bool value);
	char GetHeading(short value);
	void What2();
	void NegateTime(short chnum);
	void callClerickChoose();
	void HPAdd(short member, short amount);
	void setInputTypeNum(std::function<bool()> func, int inputLength = 2);
	bool ProcessMenuEvent(SDL_Event event);
	bool ShowOptions();
	void AddOptions();
	void AddQuitSave();

	unsigned char m_Player[21][65];
	unsigned char m_Party[64];

	short m_mapSize;
	int m_mapOffset;
	std::vector<unsigned char> m_map;
	short m_xpos;
	short m_ypos;

	short m_oy;
	short m_ox;

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
	short m_wx;
	short m_wy;
	bool m_checkDead;
	bool m_m5BDC;
	GameStateMode m_gameMode;
	GameStateMode m_lastMode;
	short m_heading;
	short m_xs;
	short m_ys;
	short m_dx;
	short m_dy;

	bool m_numOnly;
	short m_opnum;
	short m_opnum2;
	short m_restrictedStart;
	short m_gTorch;
	short m_lastCard;

	short m_storedir;
	short m_rosNum;
	short m_chNum;

	std::stack<std::function<bool()>> m_callbackStack;
	std::string m_input;
	short m_input_num;
	short m_maxInputLength;
	std::queue<int> m_weaponsList;
	bool m_YellStat;
	Uint64 m_elapsedSleepTime;
	Uint64 m_sleepCheckTime;
	short m_map_id;

	static const Uint64 damage_time = 50;
	bool m_freezeAnimation;
	SDL_Event m_currentEvent;
	bool m_partyFormed;
	bool m_surpressTextDisplay;
	short m_gTimeNegate;
	std::deque<SDL_Keycode> m_InputDeque;
	int m_lastSaveNumberOfMoves;

private:
	void Attack();
	void Board();
	void Descend();
	void Enter();
	void Exit();
	void Fire();
	void Look();
	void Klimb();
	void OtherCommand();
	void PeerGem();
	void QuitSave();
	void Steal();
	void Transact();
	void Unlock();

	bool HandleKeyPress(SDL_KeyboardEvent key);
	void LetterCommand(SDL_Keycode key);
	bool ValidTrans(char value) const;
	//bool ValidDir(unsigned char value);
	bool ValidDir();

	bool LookCallback();
	
	bool TransactCallback();
	bool TransactCallback2();
	void What();
	
	void NotHere();
	void NoEffect();
	void Routine6E6B();
	
	void IncMoves();
	
	void HandleAnyKey();
	void HandleAnyKeyEscape(SDL_Keycode key);
	bool FinalizeHealingCallback();
	void ClearTiles();
	
	void HandleLetterImmediate(SDL_Keycode key);
	void HandleNumImmediate(SDL_Keycode key);
	void HandleInputGuild(SDL_Keycode key);
	void HandleInputRestricted(SDL_Keycode key);
	void HandleInputYesNo(SDL_Keycode key);
	void HandleInputChooseWizardCleric(SDL_Keycode key);
	void HandleInputBuySell(SDL_Keycode key);
	void HandleInputText(SDL_KeyboardEvent key);
	bool HandleDefaultKeyPress(SDL_Keycode key);
	void HandleDircetionKeyPress(SDL_Keycode key);
	void HandleTransactPress(SDL_Keycode key);
	void PrintMonster(short which, bool plural, char variant);
	
	void Shop(short shopNum, short chnum);
	bool tavernCallback();
	bool grocerCallback();
	bool moreFoodCallback();
	bool InputNumCallback();
	bool InputTextCallback();
	
	void setInputTypeYesNo(std::function<bool()> func);
	void setInputTypeBuySell(std::function<bool()> func);
	void setInputTypeRestricted(std::function<bool()> func, short start);
	void setInputTypeNumImmediate(std::function<bool()> func);
	bool anotherDrinkCallback();
	bool weaponsListCallback();
	bool armorsListCallback();
	void weaponsBuyOrSell();
	void armorsBuyOrSell();
	bool listWeaponsCallback();
	bool listArmorsCallback();
	void WeaponList();
	void ArmorsList();
	void PrintWeaponList(short weapon);
	void PrintArmorList(short armor);
	bool weaponsBuySellCallback();
	bool armorsBuySellCallback();
	bool weaponsBuyCallback();
	bool armorsBuyCallback();
	bool weaponsSellCallback();
	bool armorsSellCallback();
	bool weaponsBuySellFinishedCallback();
	bool armorsBuySellFinishedCallback();
	void weaponsSellMissing();
	void armorsSellMissing();
	void weaponsBuyPoor();
	void armorsBuyPoor();
	bool AddGold(short rosNum, short gold, bool overflow);
	bool healingCallback();
	bool healingCallback2();
	bool healingPayCallback();
	void SpellNoize(short opnum, short opnum2);
	bool guildCallback();
	bool GuildPay(short rosNum, short cost);
	void GuildGive(short rosNum, short item, short amount);
	bool guildCompleteCallback();
	bool oracleCallback();
	bool oracleFinishCallback();
	bool horseVendorCallback();
	bool MoveMonsters();
	bool ExodusCastleCallback();
	void GetMonsterDir(short monNum);
	
	//bool moveshoot(short offset);
	bool move7AAA();
	bool moveshoot();
	bool moveshoot2();
	//void move7AAA(short offset);
	//bool moveoutside(short offset);
	bool AttackCallback();
	bool IgniteCallback();
	bool JoinGoldCallback();
	bool UnlockCallback();
	bool UnlockKeyCallback();
	bool PeerGemCallback();
	bool CheckAllDeadPause();
	bool HandleDeadResponse();
	bool HandleDeadResponse1();
	bool ResurrectCallback();
	bool OtherCallback();
	bool OtherCallback1();
	bool BribeCallback();
	bool InsertCallback();
	bool InsertCallback1();
	bool InsertCallback2();
	
	bool ExodusDieCallback();
	bool ExodusDieCallback1();
	bool ExodusDieCallback2();
	bool ExodusDieCallback3();
	bool ExodusDieCallback4();
	bool SleepCallback();
	
	bool StealCallback();
	bool StealCallback1();
	
	void StealFail();
	bool GetChestCallback();
	void GetChest1(short chnum);
	void GetChestBooty();
	bool ChestAcidCallback();
	bool ChestPoisonCallback();
	bool ChestGasCallback();
	void AddItem(short rosNum, short item, short amount);
	bool FinishEatFood();
	void EatFood(short member, short amount, std::function<bool()> callback);
	bool EatFoodCallback();
	
	bool EndTurnCallback();
	bool InverseCallback();

	bool CommandNorth();
	bool CommandSouth();
	bool CommandEast();
	bool CommandWest();
	bool CommandSouthEast();
	bool CommandSouthWest();
	bool CommandNorthEast();
	bool CommandNorthWest();
	bool CommandNorthCallback();
	bool CommandSouthCallback();
	bool CommandEastCallback();
	bool CommandWestCallback();
	bool CommandSouthEastCallback();
	bool CommandSouthWestCallback();
	bool CommandNorthEastCallback();
	bool CommandNorthWestCallback();
	bool CommandAttack();
	bool CommandBoard();
	bool CommandCast();
	bool CommandDescend();
	bool CommandEnter();
	bool CommandExit();
	bool CommandFire();
	bool CommandFinishTurn();
	bool CommandGetChest();
	bool CommandHandEquip();
	bool CommandIgnite();
	bool CommandKlimb();
	bool CommandLook();
	bool CommandModifyOrder();
	bool CommandNegateTime();
	bool CommandPass();
	bool CommandPeerGem();
	bool CommandQuitSave();
	bool CommandTransact();
	bool CommandSteal();
	bool CommandUnlock();
	bool CommandOther();
	bool CommandReadyWeapon();
	bool CommandWearArmor();
	bool CommandYell();
	bool CommandZStats();
	
	bool ShowAlert();

	bool ProcessEventCallback();
	void Whirl1();
	void Whirl2(short newx, short newy);
	void Whirl3();
	void GoWhirlPool();
	void MoonGateUpdate();
	void HandleMoonStep();
	bool HandleMoonStepCallback();
	bool HandleMoonStepCallback1();
	bool GoWhirlPoolCallback();
	bool GoWhirlPoolCallback1();
	bool GoWhirlPoolCallback2();
	bool FinishAll1();
	bool FinishAll2();
	bool EnterShrineCallback();
	void Shrine(short chnum);
	short ShrineRace(short race);
	bool shrineCallback();
	bool shrineCallback1();
	void Stats(short mode, short chnum);
	bool StatsCallback();
	bool StatsCallback1();
	bool StatsCallback2();
	void WearArmour(short chnum, char armour, bool preset);
	bool WearArmourCallback();
	bool WearArmourCallback1();
	void ReadyWeapon(short chnum, char weapon, bool preset);
	bool ReadyWeaponCallback();
	bool ReadyWeaponCallback1();
	bool NegateTimeCallback();
	
	bool ModifyOrderCallback();
	bool ModifyOrderCallback1();
	bool HandEquipCallback();
	bool HandEquipCallback1();
	bool HandEquipCallback2();
	bool handWeaponCallback();
	bool handArmorCallback();
	bool handFoodCallback();
	bool handGoldCallback();
	bool handItemCallback();
	bool handItemCallback1();
	bool FireCallback();
	bool fireloopCallback();
	void firehit();
	bool showHitCallback();
	bool CastCallback();
	bool ProcessMagic();
	bool ClericChoose();
	bool ClericChooseCallback();
	bool WizardChoose();
	bool WizardChooseCallback();
	bool EitherChoose();
	bool EitherChooseCallback();

	void SpawnMonster();
	void AttackCode(short whichMon);
	bool fireloop();
	void HandleForceField();
	void HandleLava();
	bool HandleLavaCallback();
	bool HandleLavaCallback1();
	bool movemon();
	bool moveoutside();
	short GetXY(short x, short y);
	void PutXY(short a, short x, short y);
	bool moveshoot2Callback();
	bool movemonCallback();
	bool movemonCallback1();
	bool KreateCallback();

	static const Uint64 exodus_death_time = 50;
	static const Uint64 screen_flicker_time = 200;
	static const Uint64 whirlpool_time = 3000;
	static const int WhirlPoolMax = 4;
	const char      WhirlXtable[8] = { 0,1,1,1,0,-1,-1,-1 };
	const char      WhirlYtable[8] = { 1,1,0,-1,-1,-1,0,1 };

	int m_transactNum;
	long m_gTime[2];
	std::unique_ptr <SosariaHandle> m_saved_map;
	int m_gWhirlCtr;
	short m_gMoon[2];
	unsigned char m_validDirValue;
	bool m_GoodPlace;

	short m_moveMonsterOffset;
	short m_value;
	bool m_storeBool;
};

