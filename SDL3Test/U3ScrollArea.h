#pragma once
#include <string>
#include <queue>
#include <deque>
#include <utility>
#include <SDL3/SDL.h>
#include <SDL3/SDL_rect.h>
#include <SDL3_ttf/SDL_ttf.h>

class U3ScrollArea
{
public:
	explicit U3ScrollArea();
	~U3ScrollArea();
	const bool isUpdating()
	{
		return m_updating;
	}
	void UPrintMessage(short msgNum);
	void UPrintMessageRewrapped(short msgNum);
	void render(Uint64 currentTickCount);
	void setRenderer(SDL_Renderer* m_renderer);
	void setBlockSize(int blockSize);
	void DrawPrompt();
	void UPrintWin(std::string gString);
	std::string RewrapString(std::string str);
	void forceRedraw()
	{
		m_forceRedraw = true;
	}
	bool isPrompt();

	float m_top;
	float m_maxTop;
private:
	bool updateQueue();
	void redraw();
	void cleanupMessages();
	bool m_updating;
	SDL_Renderer* m_renderer;
	std::deque<std::pair<bool, std::string>> m_messages;
	std::queue<std::string> m_messageQueue;
	int m_blockSize;
	Uint64 m_currentTicks;
	Uint64 m_elapsedTime;
	const Uint64 SCROLL_DELAY = 100;
	SDL_Texture* m_texDisplay;
	bool m_updateRatio;
	bool m_forceRedraw;
	bool m_updateQueue;
	bool m_update;
	bool m_scrolling;
	bool m_queueBegin;
};

