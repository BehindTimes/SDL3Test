#pragma once

#include <string>

#define NIL_PTR         0L
#define BASERES         400
#define BASECURSORID    500
#define GAMESCREENID    BASERES
#define FRAMETILE       BASERES+1
#define DEPTH           8
#define TILEON          32
#define XMON            64
#define YMON            96
#define HPMON           128
#define VARMON          160
#define APPLEMENU       128
#define ABOUTID         1
#define FILEMENU        129
#define SAVEID         1
#define PAUSEID        2
#define ABORTID        4
#define QUITID         5
#define EDITMENU        130
#define SPECIALMENU     131
#define SOUNDID        1
#define MUSICID        2
#define SPEECHID       3
#define CONSTRAINID    4
#define AUTOCOMBATID   5
#define DOUBLESIZEID   7
#define FULLSCREENID   8
#define REFERENCEMENU   132
#define COMMANDLIST    1
#define SPELLLIST      2

#define DRAGTHRESH      16

// New propertylist-based prefs keys.  Mostly inverse meanings to 1.x prefs so that nil is default.
extern std::string U3PrefCheckedResourcesDate;
extern std::string U3PrefSoundInactive;
extern std::string U3PrefMusicInactive;
extern std::string U3PrefSoundVolume;
extern std::string U3PrefMusicVolume;
extern std::string U3PrefSpeechInactive;
extern std::string U3PrefSpeedUnconstrain;
extern std::string U3PrefOriginalSize;
extern std::string U3PrefFullScreen;
extern std::string U3PrefDontAskDisplayMode;
extern std::string U3PrefNoEducateAboutFullScreen;
extern std::string U3PrefFullScreenResChange;
extern std::string U3PrefClassicAppearance;
extern std::string U3PrefIncludeWind;
extern std::string U3PrefNoDiagonals;
extern std::string U3PrefAutoSave;
extern std::string U3PrefManualCombat;
extern std::string U3PrefNoAutoHeal;
extern std::string U3PrefAsyncSound;
extern std::string U3PrefHealThreshold;
extern std::string U3PrefCurWindowX;
extern std::string U3PrefCurWindowY;
extern std::string U3PrefSaveWindowX;
extern std::string U3PrefSaveWindowY;
extern std::string U3PrefGameFont;
extern std::string U3PrefTileSet;
extern std::string U3PrefUserName;
extern std::string U3PrefRegCode;
extern std::string U3PrefLatestReleaseNumber;
extern std::string U3PrefLatestReleaseNote;
extern std::string U3PrefInformedNewVersionDate;
extern std::string U3PrefInformDayInterval;

static constexpr std::string_view CursorsLoc = "Cursors";
static constexpr std::string_view ResourceLoc = "Resources";
static constexpr std::string_view MusicLoc = "Music";
static constexpr std::string_view SfxLoc = "Sounds";
static constexpr std::string_view Standard = "Standard";
static constexpr std::string_view FontLoc = "Fonts";
static constexpr std::string_view ImagesLoc = "Images";
static constexpr std::string_view BinLoc = "Bin";
static constexpr std::string_view GraphicsLoc = "Graphics";
static constexpr std::string_view PListLoc = "PLists";
static constexpr std::string_view SaveLoc = "Save";

static constexpr std::string_view NameString = "Name:";
static constexpr std::string_view StrengthString = "Strength:";
static constexpr std::string_view DexterityString = "Dexterity:";
static constexpr std::string_view IntelligenceString = "Intelligence:";
static constexpr std::string_view WisdomString = "Wisdom:";
static constexpr std::string_view PointsString = "Points Remaining:";
static constexpr std::string_view SexString = "Sex:";
static constexpr std::string_view RaceString = "Race:";
static constexpr std::string_view TypeString = "Type:";
static constexpr std::string_view OKString = "  OK  ";
static constexpr std::string_view CancelString = " Cancel ";
static constexpr std::string_view RandomNameString = " Random Name ";

static constexpr std::string_view DoorString = "Door";
static constexpr std::string_view DioramaString = "Diorama\n";
static constexpr std::string_view StrString = "\nSTR...";
static constexpr std::string_view DexString = "\nDEX...";
static constexpr std::string_view IntString = "\nINT...";
static constexpr std::string_view WisString = "\nWIS...";
static constexpr std::string_view HpString = "\nH.P...";
static constexpr std::string_view HmString = "\nH.M...";
static constexpr std::string_view GoldString = "\nGOLD: ";
static constexpr std::string_view ExpString = "\nEXP...";
static constexpr std::string_view GemString = "\nGEMS..";
static constexpr std::string_view KeyString = "\nKEYS..";
static constexpr std::string_view PowdString = "\nPOWD..";
static constexpr std::string_view TrchString = "\nTRCH..";
static constexpr std::string_view CoDString = "\nCARD OF DEATH";
static constexpr std::string_view CoSString = "\nCARD OF SOL";
static constexpr std::string_view CoLString = "\nCARD OF LOVE";
static constexpr std::string_view CoMString = "\nCARD OF MOONS";
static constexpr std::string_view MoForceString = "\nMARK OF FORCE";
static constexpr std::string_view MoFireString = "\nMARK OF FIRE";
static constexpr std::string_view MoSString = "\nMARK OF SNAKE";
static constexpr std::string_view MoKString = "\nMARK OF KINGS";
static constexpr std::string_view WeaponString = "\nWEAPON:";
static constexpr std::string_view ArmorString = "\nARMOUR:";
static constexpr std::string_view WeaponsString = "\n**WEAPONS**";
static constexpr std::string_view NoWeaponsString = "\n02-Hands-(A)\n**ARMOUR**";
static constexpr std::string_view NoArmorString = "\n01-Skin-(A)";

static constexpr std::string_view TerraformString = "TERRAFORM";
static constexpr std::string_view ArmageddonString = "ARMAGEDDON";
static constexpr std::string_view FlotellumString = "FLOTELLUM";
static constexpr std::string_view ScreamString = "\nAIEEEEE!\n\n";
static constexpr std::string_view WinText[10] = {
		"   And so it came to",
		"pass  that  on  this",
		"day EXODUS,hell-born",
		"incarnate  of  evil",
		"was vanquished  from",
		"SOSARIA.   What  now",
		"lies  ahead  in  the",
		"ULTIMA saga can only",
		"be pure speculation!",
		"Onward to ULTIMA IV!"
};
static constexpr std::string_view SelectPartyStr = "SELECT PARTY MEMBERS";
static constexpr std::string_view SelectCharacterSlotStr = "SELECT CHARACTER SLOT";
static constexpr std::string_view SelectRemoveCharacterStr = "SELECT CHARACTER TO REMOVE";
static constexpr std::string_view TypeNumStr = "TypeNum-";
static constexpr std::string_view CreateStr = "Kreate (Y/N):";
static constexpr std::string_view LevelStr = "Level ";
static constexpr std::string_view FoodStr = "Food: ";
static constexpr std::string_view UnknownStr = "Unknown";

static constexpr std::string_view FemaleStr = "Female";
static constexpr std::string_view MaleStr = "Male";
static constexpr std::string_view OtherStr = "Other";
static constexpr std::string_view HumanStr = "Human";
static constexpr std::string_view FighterStr = "Fighter";

static constexpr std::string_view GameOptionsStr = "Game Options";
static constexpr std::string_view AppearanceStr = "Appearance:";
static constexpr std::string_view ClassicStr = "Classic";
static constexpr std::string_view ThemeStr = "Theme";
static constexpr std::string_view OtherOptionsStr = "Other Options:";
static constexpr std::string_view WindStr = "Wind";
static constexpr std::string_view DiagonalMovementStr = "Diagonal Movement";
static constexpr std::string_view AutoCombatStr = "Auto Combat";
static constexpr std::string_view AutoHealStr = "Auto Heal";
static constexpr std::string_view AutoSaveStr = "Auto Save";
static constexpr std::string_view AudioStr = "Audio:";
static constexpr std::string_view MusicStr = "Music";
static constexpr std::string_view SFXStr = "SFX";
static constexpr std::string_view FullScreenStr = "Full Screen";

static constexpr std::string_view OptionsStr = "Options";
static constexpr std::string_view CommandListStr = "Command List";
static constexpr std::string_view SpellListStr = "Spell List";
static constexpr std::string_view TablesStr = "Tables";
static constexpr std::string_view MainMenuStr = "Main Menu";
static constexpr std::string_view QuitStr = "Quit";
static constexpr std::string_view ReturnToGameStr = "Return to Game";

static const int ExodusFadeTime_1 = 4000;
static const int ExodusFadeTime_2 = 3000;

enum {
	initCmd = 1,
	freqDurationCmd = 40
};

const int TEXTURE_SIZE_X = 128;
const int TEXTURE_SIZE_Y = 128;

enum SFX_LIST
{
	SFX_ALARM = 0,
	SFX_ATTACK,
	SFX_BIGDEATH,
	SFX_BUMP,
	SFX_COMBATSTART,
	SFX_COMBATVICTORY,
	SFX_CREAK,
	SFX_DEATHFEMALE,
	SFX_DEATHMALE,
	SFX_DOWNWARDS,
	SFX_ERROR1,
	SFX_ERROR2,
	SFX_EXPLEVELUP,
	SFX_FAILEDSPELL,
	SFX_FORCEFIELD,
	SFX_HEAL,
	SFX_HIT,
	SFX_HORSEWALK,
	SFX_IMMOLATE,
	SFX_INVOCATION,
	SFX_LBLEVELRISE,
	SFX_MISCSPELL,
	SFX_MONSTERSPELL,
	SFX_MOONGATE,
	SFX_MOUNTHORSE,
	SFX_OUCH,
	SFX_SHOOT,
	SFX_SHRINE,
	SFX_SINK,
	SFX_STEP,
	SFX_SWISH1,
	SFX_SWISH2,
	SFX_SWISH3,
	SFX_SWISH4,
	SFX_TORCHIGNITE,
	SFX_UPWARDS,
	SFX_FADE1,
	SFX_FADE2
};

enum CURSOR_LIST
{
	CL_ATTACK,
	CL_BACKWARDS,
	CL_BOARD,
	CL_CANNON,
	CL_CHEST,
	CL_DEAD,
	CL_DOWN,
	CL_EAST,
	CL_ENTER,
	CL_EXIT,
	CL_FORWARD,
	CL_LEFT,
	CL_LOOK,
	CL_NORTH,
	CL_NORTHEAST,
	CL_NORTHWEST,
	CL_PASS,
	CL_RIGHT,
	CL_SOUTH,
	CL_SOUTHEAST,
	CL_SOUTHWEST,
	CL_TALK,
	CL_TORCH,
	CL_UNLOCK,
	CL_UP,
	CL_USE,
	CL_WEST
};
