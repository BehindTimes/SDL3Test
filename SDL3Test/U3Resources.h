#pragma once
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <string>
#include <vector>
#include <map>
#include <functional>
#include <libxml/xmlreader.h>
#include "U3Button.h"

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

struct ModeGraphics
{
	ModeGraphics() :
		font(nullptr),
		mini(nullptr),
		//tiles(nullptr),
		ui(nullptr),
		//tile_target(nullptr),
		ui_width(0),
		ui_height(0),
		font_width(0),
		font_height(0),
		tiles_width(0),
		tiles_height(0)
	{
	}

	SDL_Texture* font;
	SDL_Texture* mini;
	SDL_Texture* ui;

	std::vector<SDL_Texture*> tiles;
	std::vector < SDL_Texture*> tile_target;

	float ui_width;
	float ui_height;
	float font_width;
	float font_height;
	float tiles_width;
	float tiles_height;
};

class U3Resources
{
public:
	explicit U3Resources();
	~U3Resources();

	void init(SDL_Renderer* renderer);
	void CaclulateBlockSize();

	void renderUI(int part, int x, int y);
	void renderString(std::string curString, int x, int y);
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
	void DrawOrganizePartyRect();
	void DrawDemo(Uint64 curTick);
	void setTickCount(Uint64 curTick, bool initializeTimer = true);
private:
	void loadTiles(ModeGraphics& curGraphics, std::string strFile);
	void DrawButton(short butNum);
	bool loadFont();
	bool createFont();
	void loadGraphics();
	void loadImages();
	void loadButtons();
	void loadDemo();
	bool loadPLists();
	void ScrollThings();
	void drawImage(SDL_Texture* texture, float x, float y, float width, float height);
	void adjustRect(SDL_FRect& myRect);
	void processDoc(xmlDocPtr docPtr, std::vector<std::string >& curVec);
	xmlNodePtr findNodeByName(xmlNodePtr rootnode, const xmlChar* nodename);
	void GetTileRectForIndex(int tileNum, SDL_FRect& myRect);
	void GetTileRectForIndex(SDL_Texture* curTexture, int tileNum, SDL_FRect& myRect, float tileXSize, float tileYSize);
	void ScrollShape(int tilenum, float offset);

	static constexpr std::string_view FontLoc = "Fonts";
	static constexpr std::string_view ResourceLoc = "Resources";
	static constexpr std::string_view GraphicsLoc = "Graphics";
	static constexpr std::string_view PListLoc = "PLists";
	static constexpr std::string_view ImagesLoc = "Images";
	static constexpr std::string_view Standard = "Standard";
	static constexpr std::string_view BinLoc = "Bin";

	static constexpr Uint64 Delay = 2500;

	SDL_Renderer* m_renderer;

	std::map<std::string, ModeGraphics> m_allGraphics;
	std::vector<std::string> m_modes;
	ModeGraphics* m_currentGraphics;
	ModeGraphics* m_standardGraphics; // Fills in missing graphics

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
	TTF_Font* m_font_12; // 12 point font

	std::vector<unsigned char> m_demoData;
	unsigned char m_TileArray[128];
	unsigned char m_demoBgndTiles[114];
	int m_demoptr;
	Uint64 m_curTick;
	Uint64 m_elapsedTime;
};

