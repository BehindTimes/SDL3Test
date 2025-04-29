#pragma once
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <filesystem>
#include <functional>
#include <map>
#include <random>
#include <string>
#include <vector>
#include <libxml/xmlreader.h>
#include "U3Button.h"
#include "U3Dialog.h"

enum class U3PreferencesType
{
	Auto_Save,
	Allow_Diagonal,
	Classic_Appearance,
	Include_Wind,
	Sound_Inactive
};

struct U3Preferences
{
	U3Preferences() :
		auto_save(false),
		include_wind(true),
		classic_appearance(false),
		full_screen(false),
		theme("Standard"),
		allow_diagonal(false),
		sound_inactive(false)
	{
	}

	bool auto_save;
	bool include_wind;
	bool classic_appearance;
	bool full_screen;
	std::string theme;
	bool allow_diagonal;
	bool sound_inactive;
};

struct InverseStruct
{
	InverseStruct()
	{
		character_num[0] = false;
		character_num[1] = false;
		character_num[2] = false;
		character_num[3] = false;
		tiles = false;
		inverseTileTime = 0;
		elapsedTileTime = 0;
		char_details[0] = false;
		char_details[1] = false;
		char_details[2] = false;
		char_details[3] = false;
		fill = false;
		additive = false;
		color.a = 255;
		color.r = 255;
		color.g = 255;
		color.b = 255;
		stayInversed = false;
	}

	bool character_num[4];
	bool char_details[4];
	bool tiles;
	bool fill;
	bool additive;
	bool stayInversed;
	Uint64 inverseTileTime;
	Uint64 elapsedTileTime;
	SDL_Color color;
};

struct PartyDisplay
{
	PartyDisplay() :
		ShortName(nullptr),
		Number(nullptr),
		Name(nullptr),
		Level(nullptr),
		Desc(nullptr)
	{
	}
	TTF_Text* ShortName;
	TTF_Text* Number;
	TTF_Text* Name;
	TTF_Text* Level;
	TTF_Text* Desc;
};

class U3Resources
{
public:
	explicit U3Resources();
	~U3Resources();

	bool init(SDL_Renderer* renderer);
	void CalculateBlockSize();

	void displayFPS(int fps) const;

	bool loadPreferences();
	void savePreferences();

	void renderUI(int part, int x, int y, bool adjust = true, int offsetX = 0, int offsetY = 0);
	void DrawFramePieceReal(int part, int x, int y, bool adjust = false);
	int renderString(std::string curString, int x, int y, bool autoadjust = true, int offsetX = 0, int offsetY = 0, bool pretty_print = false);
	void renderDisplayString(TTF_Font* font, std::string curString, int x, int y, SDL_Color color, int align = 0, bool autoadjust = true) const;
	void renderDisplayString(TTF_Text* text_obj, int x, int y, SDL_Color color, int align = 0, bool autoadjust = true);
	void renderStalagtites();
	void drawIntro(int shape, int offset);
	void drawExodus(Uint8 alpha);
	void drawUltimaLogo(Uint8 alpha);
	void drawBy();
	void changeTheme(int theme);
	void changeTheme(std::string theme);
	void loadSignatureData();
	void PlotSig(int x, int y) const;
	void WriteLordBritish(Uint64 curPass);
	void DrawCredits();
	void CenterMessage(short which, short y);
	void CenterMessage(std::string message, short xStart, short xEnd, short y);
	void SetPreference(U3PreferencesType type, bool value);
	void GetPreference(U3PreferencesType type, bool& value) const;
	void UpdateButtons(float xPos, float yPos, int mouseState);
	void UpdateFormParty(float xPos, float yPos, int mouseState);
	void UpdateTerminateCharacter(float xPos, float yPos, int mouseState);
	void UpdateCreateCharacterChooseSlot(float xPos, float yPos, int mouseState);
	void UpdateCreateCharacter(float xPos, float yPos, int mouseState) const;
	void DrawButtons(std::vector<short> buttons);
	void SetButtonCallback(short button, std::function<void(int)> func);
	void SetButtonVisibility(short button, bool visible);
	void DrawOrganizePartyRect();
	void DrawOrganizePartyDispersed(bool wasDispersed);
	void DrawOrganizeCreateCharacterAborted();
	void DrawOrganizeCreateCharacterDone();
	void DrawOrganizePartyFormed(bool inUse);
	void DrawDemo(Uint64 curTick);
	void setTickCount(Uint64 curTick, bool initializeTimer = true);
	void DemoUpdate(Uint64 curTick);
	bool CheckJourneyOnward();
	bool CheckRosterSpace() const;
	void CreateAlertMessage(int message, DialogType type = DialogType::Alert);
	bool HandleAlert(SDL_Event& event);
	bool HasAlert();
	void DrawTiles();
	void DrawMasked(unsigned short shape, unsigned short x, unsigned short y);
	void ShowChars(bool force);
	void DrawWind();
	void DoWind();
	void DrawInverses(Uint64 delta_time);
	void DrawMoongates();
	void updateGameTime(Uint64 deltaTime);

	void ScrollThings();
	void AnimateTiles();
	void TwiddleFlags();
	void ExodusLights();

	void updateTime(Uint64 curTick);
	void RenderCharStats(short ch, SDL_FRect rect);
	void DrawPrompt();
	void adjustRect(SDL_FRect& myRect);
	int getTextWidth(std::string str) const;
	bool isInversed() const { return m_isInversed; }
	void setInversed(bool isInversed);
	int GetRealTile(int tilenum);
	int AlertReturn() const
	{
		return m_alertReturn;
	}
	void GenerateRect(SDL_FRect* FromRect, int left, int top, int right, int bottom);
	void ImageDisplay();
	void SwapShape(short shape);
	void CreatePartyNames();
	void CleanupPartyNames();
	void GenerateZStatImage(int rosNum);
	void createZStatButtons();

	void DrawZStatButtons();

	unsigned char m_TileArray[128];
	SDL_Texture* m_texDisplay;
	std::map<std::string, std::vector<std::string>> m_plistMap;
	InverseStruct m_inverses;
	SDL_Renderer* m_renderer;
	ModeGraphics* m_currentGraphics;
	ModeGraphics* m_standardGraphics; // Fills in missing graphics

	TTF_Font* m_font; // block size font
	TTF_Font* m_font_9; // 9 point font
	TTF_Font* m_font_10; // 10 point font
	TTF_Font* m_font_11; // 11 point font
	TTF_Font* m_font_12; // 12 point font
	TTF_Font* m_font_18; // 12 point font

	Uint64 m_delta_time;
	float m_font_y_offset;
	PartyDisplay m_partyDisplay[20];

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

	int m_blockSize;
	int m_overrideImage;
	bool m_newMove;
	Uint64 m_elapsedMoveTime;
	bool m_wasMove;
	int m_selectedFormRect;
	std::vector<int> m_selectedCharacters;
	std::unique_ptr<CreateCharacterDialog> m_CreateCharacterDlg;
	float m_characterRecordWidth;
	float m_characterRecordHeight;
	SDL_Texture* m_texRaceClass;
	SDL_Texture* m_texCharacterRecord;
	bool m_fullUpdate;
	std::vector< U3Button> m_zstatbuttons;
	U3Preferences m_preferences;
	std::vector<std::array<unsigned char, 200>> m_cons_data;

private:
	void LoadResource(std::string strFile);
	void loadTiles(ModeGraphics& curGraphics, std::string strFile);
	void loadMiniTiles(ModeGraphics& curGraphics, std::string strFile);
	void DrawButton(short butNum);
	bool loadFont();
	bool createFont();
	void loadGraphics();
	bool loadImages();
	void loadButtons();
	bool loadDemo();
	bool loadPLists();
	bool loadCons();
	bool checkFiles();

	void HideMonsters();
	void ShowMonsters();

	void UPrint(std::string gString, char x, char y, bool autoadjust = false);

	void drawImage(SDL_Texture* texture, float x, float y, float width, float height);

	void processDoc(xmlDocPtr docPtr, std::vector<std::string >& curVec);
	std::map<std::string, std::string> processSettingsDoc(xmlDocPtr docPtr);
	xmlNodePtr findNodeByName(xmlNodePtr rootnode, const xmlChar* nodename);
	void GetTileRectForIndex(int tileNum, SDL_FRect& myRect) const;
	void GetTileRectForIndex(int tileNum, SDL_FRect& myRect, float tileXSize, float tileYSize, int num_tiles_y);
	void ScrollShape(int tilenum, float offset);
	
	void AlertCallback();
	SDL_FRect GetTileRectForIndex(short index) const;
	void DrawPortrait(char charNum);
	void zStatDistributeCallback(int button);
	void zStatJoinGold(int button);

	static constexpr std::string_view FontLoc = "Fonts";
	static constexpr std::string_view ResourceLoc = "Resources";
	static constexpr std::string_view GraphicsLoc = "Graphics";
	static constexpr std::string_view PListLoc = "PLists";
	static constexpr std::string_view ImagesLoc = "Images";
	static constexpr std::string_view Standard = "Standard";
	static constexpr std::string_view BinLoc = "Bin";
	static constexpr std::string_view SaveLoc = "Save";

	static constexpr std::string_view LevelStr = "Level ";
	static constexpr std::string_view FoodStr = "Food: ";
	static constexpr std::string_view UnknownStr = "Unknown";

	static constexpr Uint64 MoveTime = 6000;
	static constexpr Uint64 DelayScroll = 2000;
	static constexpr Uint64 DelayDemo = 250;
	static constexpr Uint64 DelayFlags = 80;
	static constexpr Uint64 DelayAnimate = 80;
	static constexpr Uint64 DelayWind = 2560;

	std::map<std::string, ModeGraphics> m_allGraphics;
	std::vector<std::string> m_themes;
	
	std::unique_ptr<U3Dialog> m_AlertDlg;

	size_t m_defaultMode;

	SDL_Texture* m_texStalagtites;
	SDL_Texture* m_texIntro[5];
	SDL_Texture* m_texExodus;
	SDL_Texture* m_texExodusFade;
	SDL_Texture* m_texUltimaLogo;
	SDL_Texture* m_texUltimaLogoFade;
	SDL_Texture* m_texBy;
	SDL_Texture* m_texCredits;
	SDL_Texture* m_texStats;
	SDL_Texture* m_texPortraits;

	SDL_Texture* m_texSosariaMap;
	SDL_Texture* m_texFountain;
	SDL_Texture* m_texRod;
	SDL_Texture* m_texShrine;
	SDL_Texture* m_texTimeLord;

	SDL_Texture* m_texDistributeFood;
	SDL_Texture* m_texGatherGold;
	SDL_Texture* m_texDistributeFoodPushed;
	SDL_Texture* m_texGatherGoldPushed;

	std::vector<unsigned char> m_vecSigData;
	std::vector< U3Button> m_buttons;
	std::vector< short> m_currentButtons;
	

	int m_exodusWidth;
	int m_exodusHeight;
	int m_ultimaLogoWidth;
	int m_ultimaLogoHeight;
	int m_portraitHeight;
	int m_portraitWidth;

	std::vector<unsigned char> m_demoData;

	unsigned char m_demoBgndTiles[114];
	int m_demoptr;
	int m_demoDelay;
	Uint64 m_curTickScroll;
	Uint64 m_elapsedTimeScroll;
	Uint64 m_curTickDemo;
	Uint64 m_elapsedTimeDemo;
	Uint64 m_elapsedTimeFlag;
	Uint64 m_elapsedTimeAnimate;
	Uint64 m_elapsedWindTime;

	short m_animFlag[4];
	short m_twiddleFlag[4];
	int m_numUpdateFlag;
	int m_numUpdateAnimate;
	bool m_shapeSwap[256];
	bool m_cleanupAlert;

	int m_xPos;
	int m_yPos;
	bool m_isInversed;
	
	bool m_updateWind;
	int m_alertReturn;
	unsigned char m_exoduslitez;
};

