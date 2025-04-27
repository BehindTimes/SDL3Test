#pragma once
#include <string>
#include <queue>
#include <deque>
#include <utility>
#include <SDL3/SDL.h>
#include <SDL3/SDL_rect.h>
#include <SDL3_ttf/SDL_ttf.h>

struct sTextFlags
{
	sTextFlags()
	{
		pretty_print = false;
		prompt = false;
	}
	sTextFlags(bool has_pretty, bool has_prompt)
	{
		pretty_print = has_pretty;
		prompt = has_prompt;
	}
	bool pretty_print;
	bool prompt;
};

class U3ScrollArea
{
public:
	explicit U3ScrollArea();
	~U3ScrollArea();
	bool isUpdating() const
	{
		return m_updating;
	}
	void UPrintMessage(short msgNum);
	void UPrintMessageRewrapped(short msgNum);
	void render(Uint64 currentTickCount);
	void setRenderer(SDL_Renderer* m_renderer);
	void setBlockSize(int blockSize);
	void DrawPrompt();
	void UPrintWin(std::string gString, bool prettyPrint = false);
	std::string RewrapString(std::string str);
	void forceRedraw()
	{
		m_forceRedraw = true;
	}
	bool MessageQueueEmpty()
	{
		return m_messageQueue.empty();
	}
	void setInput(bool hasInput);
	bool isPrompt();
	void setInputString(std::string strValue);
	void blockPrompt(bool block);
	float m_top;
	float m_maxTop;
private:
	bool updateQueue();
	void redraw();
	void cleanupMessages();
	bool m_updating;
	SDL_Renderer* m_renderer;
	std::deque<std::pair<sTextFlags, std::string>> m_messages;
	std::queue<std::pair<sTextFlags, std::string>> m_messageQueue;
	int m_blockSize;
	Uint64 m_currentTicks;
	Uint64 m_elapsedTime;
	Uint64 m_elapsedTimeCursor;
	const Uint64 SCROLL_DELAY = 100;
	const Uint64 CURSOR_DELAY = 100;
	SDL_Texture* m_texDisplay;
	bool m_updateRatio;
	bool m_forceRedraw;
	bool m_updateQueue;
	bool m_update;
	bool m_scrolling;
	bool m_queueBegin;
	bool m_hasInput;
	std::string m_input;
	int m_cursorPos;
	int m_curCursor;
	bool m_block;
};

