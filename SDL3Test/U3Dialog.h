#pragma once
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <string>
#include <vector>
#include "U3Theme.h"
#include "U3Button.h"


enum class DialogType
{
	Alert,
	DITL
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
private:
	void RewrapMessage(std::string& strMesssage);
	void DrawFramePiece(int part, int x, int y);
	void adjustRect(SDL_FRect& myRect);
	void loadString();
	void loadDitl();
	bool createFont();
	void createButton(std::function<void()> callback);
	void createBorder(int x, int y, int numBlocksW, int numBlocksH);
	TTF_Text* calcDisplayString(TTF_Font* font, std::string curString, int& outWidth, int& outHeight, SDL_Color color);
	void renderDisplayString(TTF_Text* text_obj, int x, int y, SDL_Color color);
	void calculateRects();

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
};

