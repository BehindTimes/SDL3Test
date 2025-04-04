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
	void render(Uint64 currentTickCount);
	void setRenderer(SDL_Renderer* m_renderer);
	void setBlockSize(int blockSize);
	void DrawPrompt();
	void UPrintWin(std::string gString);
	void forceRedraw()
	{
		m_forceRedraw = true;
	}

	float m_top;
	float m_maxTop;
private:
	bool updateQueue();
	void redraw();
	bool m_updating;
	bool m_appendLine;
	SDL_Renderer* m_renderer;
	std::deque<std::pair<bool, std::string>> m_messages;
	std::queue<std::string> m_messageQueue;
	int m_blockSize;
	Uint64 m_currentTicks;
	Uint64 m_elapsedTime;
	Uint64 m_totalElapsedTime;
	bool append;

	const Uint64 SCROLL_DELAY = 100;
	SDL_Texture* m_texDisplay;
	Uint64 m_totalDelay;
	float m_ratio;
	bool m_append;
	bool m_first_update;
	bool m_updateRatio;

	bool m_forceRedraw;
	bool m_updateQueue;
	bool m_update;
};

