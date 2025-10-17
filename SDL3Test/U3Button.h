#pragma once
#include <SDL3/SDL.h>
#include <functional>
#include <string>
#include <SDL3_ttf/SDL_ttf.h>

enum class ButtonType
{
	Text,
	UpArrow,
	DownArrow
};


class U3Button
{
public:
	explicit U3Button();
	~U3Button();

	void resizeButton(int m_blockSize, SDL_Renderer* renderer, TTF_TextEngine* engine_surface, TTF_Font* font);
	void CreateTextButton(int blockSize, SDL_Renderer* renderer, TTF_TextEngine* engine_surface, TTF_Font* font, std::string strText, int x = 0, int y = 0, int override_width = 0);
	void CreateImageButton(int blockSize, SDL_Renderer* renderer, SDL_Texture* buttonImage, SDL_Texture* buttonPushedImage, int width, int height);
	void forceCapture();
	void click();
	void render(SDL_Renderer* renderer, int blockSize, int x, int y, short adjustX, short adjustY);
	void render(SDL_Renderer* renderer, int blockSize, int x, int y, bool adjust = false);
	void setRect(SDL_Renderer* renderer, SDL_Texture* buttonImage, int blockSize, int x, int y, int width, int height, bool has_clicked, bool has_disabled);
	void setMouseCapture(int blockSize, int capture, float mouse_x, float mouse_y, float x, float y, short adjustX, short adjustY);
	void setMouseCapture(int capture, float mouse_x, float mouse_y);
	void SetButtonCallback(std::function<void(int)> func, int id = 0);
	void setVisible(bool visible);
	void renderCentered(SDL_Renderer* renderer);
	void updateLocationCentered(SDL_FRect fillRect);
	SDL_Texture* getDefaultTexture() { return m_texDefault; }
	SDL_Texture* getPushedTexture() { return m_texPushed; }
	float getWidth() const { return m_width; }
	float getHeight() const { return m_height; }
	bool isCaptured() const { return m_captureMouse; }
	void setType(ButtonType type) { m_type = type; }
	SDL_FRect m_renderRect;

	int m_x;
	int m_y;
private:
	SDL_Texture* m_texDefault;
	SDL_Texture* m_texPushed;
	SDL_Texture* m_texDisabled;
	std::function<void(int)> m_callbackFunction;

	float m_override_width;
	float m_width;
	float m_height;
	bool m_captureMouse;
	bool m_showPushed;
	bool m_visible;
	bool m_forcecapture;
	int m_id;
	std::string m_text;
	bool m_hasFocus;
	ButtonType m_type;
};

