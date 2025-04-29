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

enum {
	initCmd = 1,
	freqDurationCmd = 40
};

const int TEXTURE_SIZE_X = 128;
const int TEXTURE_SIZE_Y = 128;