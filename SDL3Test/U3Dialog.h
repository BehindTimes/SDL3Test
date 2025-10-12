#pragma once
#include <vector>
#include <array>
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <string>
#include "U3Theme.h"
#include "U3Button.h"


enum class DialogType
{
	Alert,
	DITL,
	Custom
};

struct U3CheckBox
{
public:
	U3CheckBox(int blockSize, int x, int y);
	~U3CheckBox();

	void render(SDL_Renderer* renderer, int x, int y) const;

	void setChecked(bool checked) { m_checked = checked; }

	bool m_checked;
	int m_x;
	int m_y;
	int m_blockSize;
};

struct U3DlgLabel
{
public:
	U3DlgLabel(TTF_TextEngine* engine_surface, TTF_Font* font, std::string strLabel, int x, int y);
	~U3DlgLabel();

	void updateLabelFont(TTF_TextEngine* engine_surface, TTF_Font* font);
	void setText(TTF_TextEngine* engine_surface, TTF_Font* font, std::string text);

	std::string m_strLabel;
	TTF_Text* m_ttfLabel;
	int m_x;
	int m_y;
};

struct U3TextBox
{
public:
	U3TextBox(int blockSize, int x, int y, int width);
	~U3TextBox();

	void U3TextBoxFont(TTF_TextEngine* engine_surface, TTF_Font* font, int blockSize);
	void render(SDL_Renderer* renderer, int x, int y);
	void setText(TTF_TextEngine* engine_surface, TTF_Font* font, std::string text);
	std::string getText() const { return m_strText; }

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

struct codData
{
	codData() :
		theme(0),
		classic(false),
		include_wind(false),
		auto_heal(false),
		auto_save(false),
		auto_combat(false),
		play_music(false),
		play_sfx(false),
		is_full_screen(false),
		allow_diagonals(false),
		auto_heal_amount(750),
		volume_music(100),
		volume_sfx(100)
	{
	}
	int theme;
	bool classic;
	bool include_wind;
	bool auto_heal;
	bool auto_save;
	bool auto_combat;
	bool play_music;
	bool play_sfx;
	bool is_full_screen;
	bool allow_diagonals;
	int auto_heal_amount;
	int volume_music;
	int volume_sfx;
};

class ChooseOptionsDialog
{
public:
	explicit ChooseOptionsDialog(SDL_Renderer* renderer, TTF_TextEngine* engine_surface);
	~ChooseOptionsDialog();

	void init();
	bool display();
	void changeBlockSize(int blockSize);
	void addLabel(std::string strLabel, int x, int y);
	void addButton(std::string strLabel, int x, int y, std::function<void(int)> func, ButtonType type = ButtonType::Text);
	void addTextBox(int x, int y, int width);
	void addCheckBox(int x, int y);
	void cancelPushed(int id);
	void okPushed(int id);
	bool updateDialog(float xPos, float yPos, int mouseState);
	void SetDialogFinishedCallback(std::function<void(int)> func);
	void HandleEvent(SDL_Event& event);

	SDL_FRect m_Rect;

	codData m_codData;

private:
	bool createFont();
	void renderDisplayString(TTF_Text* text_obj, int x, int y, SDL_Color color);
	void themeUp(int id);
	void themeDown(int id);
	void sfxVolumeUp(int id);
	void sfxVolumeDown(int id);
	void musicVolumeUp(int id);
	void musicVolumeDown(int id);
	void healLimitUp(int id);
	void healLimitDown(int id);

	int m_blockSize;
	SDL_Renderer* m_renderer;
	TTF_TextEngine* m_engine_surface;
	std::vector<std::unique_ptr<U3DlgLabel>> m_labels;
	std::vector<std::unique_ptr<U3Button>> m_buttons;
	std::vector<std::unique_ptr<U3TextBox>> m_textBoxes;
	std::vector<std::unique_ptr<U3CheckBox>> m_checkBoxes;
	TTF_Font* m_font; // block size font

	std::string m_upArrow;
	std::string m_downArrow;
	std::function<void(int)> m_callBack;
	int m_closeValue;
	int m_curTheme;
	int m_healLimit;
	int m_sfxVolume;
	int m_musicVolume;
	
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
	void addButton(std::string strLabel, int x, int y, std::function<void(int)> func, ButtonType type = ButtonType::Text);
	void addTextBox(int x, int y, int width);
	void updateDialog(float xPos, float yPos, int mouseState);
	void loadPresets();
	void ProcessEvent(SDL_Event event);
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
	void HandleInputText(SDL_KeyboardEvent key);

	bool createFont();
	void renderDisplayString(TTF_Text* text_obj, int x, int y, SDL_Color color);
	int getClassIndex(char value);

	int m_blockSize;
	SDL_Renderer* m_renderer;
	TTF_TextEngine* m_engine_surface;
	std::vector<std::unique_ptr<U3DlgLabel>> m_labels;
	std::vector<std::unique_ptr<U3Button>> m_buttons;
	std::vector<std::unique_ptr<U3TextBox>> m_textBoxes;
	TTF_Font* m_font; // block size font

	std::string m_upArrow;
	std::string m_downArrow;
	std::vector<std::array<char, 5>> m_Presets;
	ccdData m_ccdData;
};

struct DitlInfo
{
	DitlInfo() :
		x1(0),
		y1(0),
		x2(0),
		y2(0),
		type(0),
		id(0)
	{
	}
	int x1;
	int y1;
	int x2;
	int y2;
	int type;
	std::string text;
	int id;
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
	int ReturnValue() const { return m_retVal; }

private:
	void RewrapMessage(std::string& strMesssage);
	void DrawFramePiece(int part, int x, int y);
	void adjustRect(SDL_FRect& myRect);
	void loadString();
	//void loadDitlString(int blockSize, std::function<void(int)> callback);
	bool processDitlData(std::vector<unsigned char> data, std::vector<DitlInfo>& ditl_vec);
	void loadDitl(int blockSize, std::function<void(int)> callback);
	bool createFont();
	void createButton(std::function<void(int)> callback);
	void createBorder(int x, int y, int numBlocksW, int numBlocksH);
	TTF_Text* calcDisplayString(TTF_Font* font, std::string curString, int& outWidth, int& outHeight, SDL_Color color);
	void renderDisplayString(TTF_Text* text_obj, int x, int y, SDL_Color color);
	void calculateRects();
	void finishedCallback(int button);
	short getDitlShort(std::vector<unsigned char> data, int& address);
	

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

