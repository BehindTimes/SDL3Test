#pragma once
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <filesystem>
#include <functional>
#include <map>
#include <string>
#include <vector>
#include <libxml/xmlreader.h>
#include "U3Button.h"
#include "U3Dialog.h"

enum class U3PreferencesType
{
	Classic_Appearance
};

struct U3Preferences
{
	U3Preferences()
	{
		classic_appearance = false;
		full_screen = false;
		mode = 0;
	}

	bool classic_appearance;
	bool full_screen;
	int mode;
};

class U3Resources
{
public:
	explicit U3Resources();
	~U3Resources();

	bool init(SDL_Renderer* renderer);
	void CalculateBlockSize();

	void renderUI(int part, int x, int y);
	void renderString(std::string curString, int x, int y);
	void renderDisplayString(TTF_Font* font, std::string curString, int x, int y, SDL_Color color, int align = 0);
	void renderStalagtites();
	void drawIntro(int shape, int offset);
	void drawExodus(Uint8 alpha);
	void drawUltimaLogo(Uint8 alpha);
	void drawBy();
	void changeMode(int mode);
	void loadSignatureData();
	void PlotSig(int x, int y);
	void WriteLordBritish(Uint64 curPass);
	void DrawCredits();
	void CenterMessage(short which, short y);
	void SetPreference(U3PreferencesType type, bool value);
	void UpdateButtons(float xPos, float yPos, int mouseState);
	void DrawButtons(std::vector<short> buttons);
	void SetButtonCallback(short button, std::function<void()> func);
	void SetButtonVisibility(short button, bool visible);
	void DrawOrganizePartyRect();
	void DrawDemo(Uint64 curTick);
	void setTickCount(Uint64 curTick, bool initializeTimer = true);
	void DemoUpdate(Uint64 curTick);
	bool CheckJourneyOnward();
	void CreateAlertMessage(int message);
	bool HasAlert(SDL_Event& event);
private:
	void loadTiles(ModeGraphics& curGraphics, std::string strFile);
	void DrawButton(short butNum);
	bool loadFont();
	bool createFont();
	void loadGraphics();
	void loadImages();
	bool loadSaveGame();
	void loadButtons();
	void loadDemo();
	bool loadPLists();
	void ScrollThings();
	void AnimateTiles();
	void TwiddleFlags();
	void drawImage(SDL_Texture* texture, float x, float y, float width, float height);
	void adjustRect(SDL_FRect& myRect);
	void processDoc(xmlDocPtr docPtr, std::vector<std::string >& curVec);
	xmlNodePtr findNodeByName(xmlNodePtr rootnode, const xmlChar* nodename);
	void GetTileRectForIndex(int tileNum, SDL_FRect& myRect);
	void GetTileRectForIndex(SDL_Texture* curTexture, int tileNum, SDL_FRect& myRect, float tileXSize, float tileYSize);
	void ScrollShape(int tilenum, float offset);
	void SwapShape(short shape);
	bool loadRoster(std::filesystem::path rosterPath);
	bool loadParty(std::filesystem::path partyPath);
	void m_AlertCallback();

	static constexpr std::string_view FontLoc = "Fonts";
	static constexpr std::string_view ResourceLoc = "Resources";
	static constexpr std::string_view GraphicsLoc = "Graphics";
	static constexpr std::string_view PListLoc = "PLists";
	static constexpr std::string_view ImagesLoc = "Images";
	static constexpr std::string_view Standard = "Standard";
	static constexpr std::string_view BinLoc = "Bin";
	static constexpr std::string_view SaveLoc = "Save";
	static constexpr std::string_view TextLoc = "Text";

	static constexpr Uint64 DelayScroll = 2000;
	static constexpr Uint64 DelayDemo = 250;
	static constexpr Uint64 DelayFlags = 80;
	static constexpr Uint64 DelayAnimate = 80;

	SDL_Renderer* m_renderer;

	std::map<std::string, ModeGraphics> m_allGraphics;
	std::vector<std::string> m_modes;
	ModeGraphics* m_currentGraphics;
	ModeGraphics* m_standardGraphics; // Fills in missing graphics
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

	U3Preferences m_preferences;
	std::vector<unsigned char> m_vecSigData;
	std::vector< U3Button> m_buttons;
	std::vector< short> m_currentButtons;
	std::map<std::string, std::vector<std::string>> m_plistMap;
	int m_exodusWidth;
	int m_exodusHeight;
	int m_ultimaLogoWidth;
	int m_ultimaLogoHeight;
	int m_blockSize;
	TTF_Font* m_font; // block size font
	TTF_Font* m_font_11; // 11 point font

	std::vector<unsigned char> m_demoData;
	unsigned char m_TileArray[128];
	unsigned char m_demoBgndTiles[114];
	int m_demoptr;
	int m_demoDelay;
	Uint64 m_curTickScroll;
	Uint64 m_elapsedTimeScroll;
	Uint64 m_curTickDemo;
	Uint64 m_elapsedTimeDemo;
	Uint64 m_elapsedTimeFlag;
	Uint64 m_elapsedTimeAnimate;

	short m_animFlag[4];
	short m_twiddleFlag[4];
	int m_numUpdateFlag;
	int m_numUpdateAnimate;
	bool m_shapeSwap[256];
	unsigned char m_Player[21][65];
	unsigned char m_Party[64];
	bool m_cleanupAlert;

	int m_xPos;
	int m_yPos;
};

