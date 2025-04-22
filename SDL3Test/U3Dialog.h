#pragma once
#include <vector>
#include <array>
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <string>
#include <vector>
#include "U3Theme.h"
#include "U3Button.h"


enum class DialogType
{
	Alert,
	DITL,
	Custom
};

struct U3DlgLabel
{
public:
	U3DlgLabel(TTF_TextEngine* engine_surface, TTF_Font* font, std::string strLabel, int x, int y);
	~U3DlgLabel();

	void updateLabelFont(TTF_TextEngine* engine_surface, TTF_Font* font);

	std::string m_strLabel;
	TTF_Text* m_ttfLabel;
	int m_x;
	int m_y;
};

struct U3TextBox
{
public:
	U3TextBox(TTF_TextEngine* engine_surface, TTF_Font* font, int blockSize, int x, int y, int width);
	~U3TextBox();

	void U3TextBoxFont(TTF_TextEngine* engine_surface, TTF_Font* font, int blockSize);
	void render(SDL_Renderer* renderer, int x, int y);
	void setText(TTF_TextEngine* engine_surface, TTF_Font* font, std::string text);

	int m_x;
	int m_y;
	int m_width;
	int m_blockSize;
	bool m_readOnly;

private:
	std::string m_strText;
	TTF_Text* m_ttfText;
	void renderDisplayString(TTF_Text* text_obj, int x, int y, SDL_Color color);
};

struct ccdData
{
	ccdData() :
		strength(5),
		wisdom(5),
		dexterity(5),
		intelligence(5),
		sex(0),
		race(0),
		type(0),
		remaining(0)
	{
	}
	std::string name;
	unsigned char strength;
	unsigned char wisdom;
	unsigned char dexterity;
	unsigned char intelligence;
	unsigned char remaining;

	int sex;
	int type;
	int race;
};

class CreateCharacterDialog
{
public:
	explicit CreateCharacterDialog(SDL_Renderer* renderer, TTF_TextEngine* engine_surface);
	~CreateCharacterDialog();

	void init();
	bool display();
	void changeBlockSize(int blockSize);
	void addLabel(std::string strLabel, int x, int y);
	void addButton(std::string strLabel, int x, int y, std::function<void(int)> func);
	void addTextBox(int x, int y, int width);
	void updateDialog(float xPos, float yPos, int mouseState);
	void loadPresets();
	SDL_FRect m_Rect;

	unsigned char* m_curPlayer;

private:
	void strengthUp(int id);
	void strengthDown(int id);
	void dexterityUp(int id);
	void dexterityDown(int id);
	void intelligenceUp(int id);
	void intelligenceDown(int id);
	void wisdomUp(int id);
	void wisdomDown(int id);
	void sexUp(int id);
	void sexDown(int id);
	void raceUp(int id);
	void raceDown(int id);
	void typeUp(int id);
	void typeDown(int id);
	void cancelPushed(int id);
	void okPushed(int id);
	void randomNamePushed(int id);

	bool createFont();
	void renderDisplayString(TTF_Text* text_obj, int x, int y, SDL_Color color);
	int getClassIndex(char value);

	static constexpr std::string_view ResourceLoc = "Resources";
	static constexpr std::string_view TextLoc = "Text";
	static constexpr std::string_view FontLoc = "Fonts";
	static constexpr std::string_view ImagesLoc = "Images";
	static constexpr std::string_view BinLoc = "Bin";

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

	int m_blockSize;
	SDL_Renderer* m_renderer;
	TTF_TextEngine* m_engine_surface;
	std::vector<std::unique_ptr<U3DlgLabel>> m_labels;
	std::vector<std::unique_ptr<U3Button>> m_buttons;
	std::vector<std::unique_ptr<U3TextBox>> m_textBoxes;
	TTF_Font* m_font; // block size font

	std::string m_upArrow;
	std::string m_downArrow;
	std::vector<std::array<char, 4>> m_Presets;
	ccdData m_ccdData;
};

class U3Dialog
{
public:
	explicit U3Dialog(SDL_Renderer* renderer, TTF_TextEngine* engine_surface,
		ModeGraphics** currentGraphics, ModeGraphics** standardGraphics,
		int blockSize, int message, DialogType type, std::function<void()> func);
	~U3Dialog();

	bool display();
	void changeBlockSize(int blockSize);
	void HandleEvent(SDL_Event& event);
	const int ReturnValue() { return m_retVal; }

private:
	void RewrapMessage(std::string& strMesssage);
	void DrawFramePiece(int part, int x, int y);
	void adjustRect(SDL_FRect& myRect);
	void loadString();
	void loadDitl(int blockSize, std::function<void(int)> callback);
	bool createFont();
	void createButton(std::function<void(int)> callback);
	void createBorder(int x, int y, int numBlocksW, int numBlocksH);
	TTF_Text* calcDisplayString(TTF_Font* font, std::string curString, int& outWidth, int& outHeight, SDL_Color color);
	void renderDisplayString(TTF_Text* text_obj, int x, int y, SDL_Color color);
	void calculateRects();
	void finishedCallback(int button);

	static constexpr std::string_view ResourceLoc = "Resources";
	static constexpr std::string_view TextLoc = "Text";
	static constexpr std::string_view FontLoc = "Fonts";
	static constexpr std::string_view ImagesLoc = "Images";
	static constexpr std::string_view BinLoc = "Bin";

	U3Button m_backButton;

	SDL_Renderer* m_renderer;
	TTF_TextEngine* m_engine_surface;

	TTF_Text* m_text_obj_title;
	TTF_Text* m_text_obj_message;

	ModeGraphics** m_currentGraphics;
	ModeGraphics** m_standardGraphics;
	bool m_dialogLoaded;
	int m_message;
	std::string m_strTitle;
	std::string m_strMessage;
	TTF_Font* m_font; // block size font
	int m_blockSize;
	SDL_FRect m_titleRect;
	SDL_FRect m_messageRect;
	SDL_FRect m_dialogRect;
	int m_numblocksW;
	int m_numblocksH;
	int m_titleX;
	int m_titleY;
	int m_messageX;
	int m_messageY;
	std::vector<std::unique_ptr<U3Button>> m_vecButtons;
	SDL_Texture* m_icon;
	//std::vector<TTF_Text*> m_textButtons;
	std::function<void()> m_callback;
	int m_retVal;
};

