#pragma once
#include <SDL3/SDL.h>
#include <functional>
#include <string>
#include <SDL3_ttf/SDL_ttf.h>

class U3Button
{
public:
	explicit U3Button();
	~U3Button();

	void resizeButton(SDL_Renderer* renderer, TTF_TextEngine* engine_surface, TTF_Font* font);
	void CreateTextButton(SDL_Renderer* renderer, TTF_TextEngine* engine_surface, TTF_Font* font, std::string strText);
	void forceCapture();
	void click();
	void render(SDL_Renderer* renderer, int blockSize, int x, int y, short adjustX, short adjustY);
	void setRect(SDL_Renderer* renderer, SDL_Texture* buttonImage, int x, int y, int width, int height, bool has_clicked, bool has_disabled);
	void setMouseCapture(int blockSize, int capture, float mouse_x, float mouse_y, float x, float y, short adjustX, short adjustY);
	void setMouseCapture(int capture, float mouse_x, float mouse_y);
	void SetButtonCallback(std::function<void()> func);
	void setVisible(bool visible);
	void renderCentered(SDL_Renderer* renderer);
	void updateLocationCentered(SDL_FRect fillRect);
	SDL_Texture* getDefaultTexture() { return m_texDefault; }
	SDL_Texture* getPushedTexture() { return m_texPushed; }
	float getWidth() { return m_width; }
	float getHeight() { return m_height; }
private:
	SDL_Texture* m_texDefault;
	SDL_Texture* m_texPushed;
	SDL_Texture* m_texDisabled;
	SDL_FRect m_renderRect;
	std::function<void()> m_callbackFunction;

	float m_width;
	float m_height;
	bool m_captureMouse;
	bool m_showPushed;
	bool m_visible;
	bool m_forcecapture;
	std::string m_text;
};

