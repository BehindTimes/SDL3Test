#include "U3ScrollArea.h"
#include "U3Resources.h"
#include "U3Utilities.h"
#include "UltimaGraphics.h"

extern U3Resources m_resources;
extern U3Graphics m_graphics;
extern U3Utilities m_utilities;

U3ScrollArea::U3ScrollArea() :
	m_updating(false),
	m_renderer(nullptr),
	m_blockSize(0),
	m_texDisplay(nullptr),
	m_currentTicks(0),
	m_elapsedTime(0),
	m_totalDelay(0),
	m_top(0),
	m_maxTop(0),
	m_appendLine(false),
	m_ratio(0),
	m_totalElapsedTime(0),
	m_append(false),
	m_first_update(false),
	m_updateRatio(false),
	m_forceRedraw(true),
	m_updateQueue(false),
	m_update(false)
{
	m_messages.emplace_back(1, "");
}

U3ScrollArea::~U3ScrollArea()
{
	if (m_texDisplay)
	{
		SDL_DestroyTexture(m_texDisplay);
	}
}

void U3ScrollArea::UPrintWin(std::string gString)
{
	auto vecString = m_utilities.splitString(gString, '\n', true);
	
	for (size_t index = 0; index < vecString.size(); ++index)
	{
		if (vecString[index] == std::string("\n"))
		{
			m_messageQueue.emplace(std::string(""));
		}
		else
		{
			if (index == 0 && m_messageQueue.size() > 0)
			{
				if (m_messageQueue.back() == std::string("\n"))
				{
					m_messageQueue.emplace(vecString[index]);
				}
				else
				{
					m_messageQueue.back() += vecString[index];
				}
			}
			else
			{
				m_messageQueue.emplace(vecString[index]);
			}
		}
		if (index != vecString.size() - 1)
		{
			m_messageQueue.emplace("\n");
		}
	}
	if (gString.ends_with('\n'))
	{
		m_messageQueue.emplace("\n");
	}

	if (m_messageQueue.size() > 0)
	{
		m_totalDelay = (m_messageQueue.size() - 1) * SCROLL_DELAY;
		m_updating = true;
		m_elapsedTime = 0;
		m_ratio = 0;
		m_totalElapsedTime = 0;
		m_first_update = true;

		
		m_updateQueue = true;

	}
}

void U3ScrollArea::UPrintMessage(short msgNum)
{
	msgNum--;
	if (m_resources.m_plistMap.contains("Messages"))
	{
		if (m_resources.m_plistMap["Messages"].size() > msgNum)
		{
			std::string message = m_resources.m_plistMap["Messages"][msgNum];
			UPrintWin(message);
		}
	}
}

void U3ScrollArea::redraw()
{
	SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 0);
	SDL_SetRenderTarget(m_renderer, m_texDisplay);
	SDL_RenderClear(m_renderer);

	auto tempMessages = m_messages;

	float startPos = 23.0f * m_blockSize;
	int tempIndex = 6;

	while(!tempMessages.empty())
	{
		auto curPair = tempMessages.back();
		tempMessages.pop_back();
		m_graphics.DrawFramePieceScroll(12, 0, tempIndex);
		if (curPair.second.size() > 0)
		{
			m_resources.renderString(curPair.second, curPair.first ? 1 : 0, tempIndex, false);
		}
		tempIndex--;
	}

	SDL_SetRenderTarget(m_renderer, NULL);
	SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 0);
}

bool U3ScrollArea::updateQueue()
{
	bool retval = false;
	m_updateQueue = false;

	if (m_messageQueue.size() == 0)
	{
		return true;
	}
	auto curStr = m_messageQueue.front();
	m_messageQueue.pop();
	m_forceRedraw = true;
	m_update = true;

	if (curStr != std::string("\n"))
	{
		m_messages.back().second += curStr;
		if (m_messageQueue.size() == 0)
		{
			m_update = false;
		}
		/*if (m_update)
		{
			m_messages.push_back(std::make_pair(0, ""));
		}*/
	}
	else
	{
		m_messages.push_back(std::make_pair(0, ""));
		if (m_messageQueue.size() == 0)
		{
			m_update = false;
			m_messages.back().first = true;
			retval = true;
		}
		else
		{
			// append the next message
			curStr = m_messageQueue.front();
			m_messageQueue.pop();
			bool isNewLine = false;
			if (curStr != std::string("\n"))
			{
				m_messages.back().second += curStr;
			}
			else
			{
				isNewLine = true;
			}
			if (m_messageQueue.size() == 0)
			{
				m_update = false;
				if (isNewLine)
				{
					m_messages.back().first = true;
					retval = true;
				}
			}
		}
	}
	while (m_messages.size() > 8)
	{
		m_messages.pop_front();
	}
	return retval;
}

void U3ScrollArea::render(Uint64 currentTickCount)
{
	float yOffset = 0;
	bool bFinished = false;

	if (m_updating)
	{
		if (m_updateQueue)
		{
			bFinished = updateQueue();
			m_forceRedraw = true;
		}
		if (SCROLL_DELAY == 0)
		{
			m_updateQueue = true;
			m_updating = !bFinished;
		}
		else
		{
			bFinished = false;
			while (m_elapsedTime > SCROLL_DELAY)
			{
				m_elapsedTime -= SCROLL_DELAY;
				bFinished = updateQueue();
				if (bFinished)
				{
					m_elapsedTime = 0;
					m_updating = false;
					break;
				}
				m_forceRedraw = true;
			}
			if (!bFinished)
			{
				if (m_update)
				{
					yOffset = ((float)m_elapsedTime / (float)SCROLL_DELAY) * m_blockSize;
				}
				m_elapsedTime += currentTickCount;
			}
		}
	}

	if (m_forceRedraw)
	{
		redraw();
		m_forceRedraw = false;
	}

	bool classic = false;
	m_resources.GetPreference(U3PreferencesType::Classic_Appearance, classic);
	if(classic)
	{
		///yOffset = 0;
	}

	SDL_FRect fromRect;
	fromRect.x = (float)(0.0f * m_blockSize);
	fromRect.y = (float)(0.0f * m_blockSize) + yOffset;
	fromRect.w = (float)17.0f * m_blockSize;
	fromRect.h = (float)7.0f * m_blockSize;
	SDL_FRect toRect;
	toRect.x = (float)(24.0f * m_blockSize);
	toRect.y = (float)(17.0f * m_blockSize);
	toRect.w = (float)17.0f * m_blockSize;
	toRect.h = (float)7.0f * m_blockSize;
	
	m_resources.adjustRect(toRect);
	SDL_RenderTexture(m_renderer, m_texDisplay, &fromRect, &toRect);

#if 0
	SDL_FRect myRect;

	myRect.x = (float)0;
	myRect.y = (float)0;
	myRect.w = (float)17.0f * m_blockSize;
	myRect.h = (float)8.0f * m_blockSize;
	SDL_SetRenderTarget(m_renderer, m_texDisplay);
	SDL_RenderClear(m_renderer);
	//SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 0);
	//SDL_RenderFillRect(m_renderer, &myRect);
	//SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 0);

	if (!m_updating)
	{
		while (m_messages.size() > 7)
		{
			m_messages.pop_front();
		}
		auto tempMessages = m_messages;
		for (size_t index = 0; index < m_messages.size(); ++index)
		{
			auto curPair = tempMessages.front();
			tempMessages.pop_front();
			if (curPair.first)
			{
				m_graphics.DrawFramePieceScroll(12, 0, (short)(7 - (m_messages.size() - index)));
			}
			if (curPair.second.size() > 0)
			{
				m_resources.renderString(curPair.second, curPair.first ? 1 : 0, (int)(7 - (m_messages.size() - index)), false);
			}
		}
	}
	else
	{
		if (m_first_update)
		{
			m_first_update = false;
			auto curPair = m_messageQueue.front();
			m_messageQueue.pop();
			if (curPair.second != std::string("\n"))
			{
				m_messages.back().second += curPair.second;
				m_updateRatio = false;
			}
			else
			{
				m_updateRatio = true;
			}
			if (!m_messageQueue.empty())
			{
				m_updateRatio = true;
			}
		}
		else
		{
			if (SCROLL_DELAY > 0)
			{
				m_ratio = (float)m_elapsedTime / (float)SCROLL_DELAY;
				while (m_ratio >= 1.0f)
				{
					bool isNewLine = false;
					if (m_messageQueue.size() == 0) // we've overshot, so end now
					{
						if (m_messages.back().second == std::string("\n"))
						{
							m_messages.back().first = true;
							m_messages.back().second.clear();
						}
						m_updating = false;
						m_ratio = 0;
						break;
					}
					m_ratio -= 1;
					m_messages.push_back(std::make_pair(false, ""));
					if (m_messageQueue.front().second != std::string("\n"))
					{
						m_messages.back().second = m_messageQueue.front().second;
					}
					else
					{
						isNewLine = true;
					}
					
					m_messageQueue.pop();
					if (m_messageQueue.size() == 0 && isNewLine)
					{
						m_messages.back().first = true;
					}
				}
			}
			else
			{
				if (m_messageQueue.size() == 0) // we've overshot, so end now
				{
					if (m_messages.back().second == std::string("\n"))
					{
						m_messages.back().first = true;
						m_messages.back().second.clear();
					}
					m_updating = false;
					m_ratio = 0;
				}
				else
				{
					m_ratio -= 1;
					bool isNewLine = false;
					m_messages.push_back(std::make_pair(false, ""));
					if (m_messageQueue.front().second != std::string("\n"))
					{
						m_messages.back().second = m_messageQueue.front().second;
					}
					else
					{
						isNewLine = true;
					}
					m_messageQueue.pop();
					if (m_messageQueue.size() == 0 && isNewLine)
					{
						m_messages.back().first = true;
					}
				}
			}
			if (!m_updating)
			{
				while (m_messages.size() > 7)
				{
					m_messages.pop_front();
				}
			}
			auto tempMessages = m_messages;
			for (size_t index = 0; index < m_messages.size(); ++index)
			{
				int offsetY = 0;
				if (m_updateRatio)
				{
					offsetY = (int)((m_ratio * m_blockSize) * -1);
				}
				else
				{
					//offsetY = (int)((m_blockSize) * -1);
				}
				
				auto curPair = tempMessages.front();
				tempMessages.pop_front();
				if (curPair.first)
				{
					m_graphics.DrawFramePieceScroll(12, 0, (short)(7 - (m_messages.size() - index)), 0, offsetY);
				}
				if (curPair.second.size() > 0)
				{
					m_resources.renderString(curPair.second, curPair.first ? 1 : 0, (int)(7 - (m_messages.size() - index)), false, 0, offsetY);
				}
			}
		}

		m_elapsedTime += currentTickCount;
	}
	SDL_SetRenderTarget(m_renderer, NULL);
	myRect.x += (24.0f * m_blockSize);
	myRect.y += (17.0f * m_blockSize);
	m_resources.adjustRect(myRect);
	SDL_RenderTexture(m_renderer, m_texDisplay, NULL, &myRect);
#endif
}

void U3ScrollArea::setRenderer(SDL_Renderer* renderer)
{
	m_renderer = renderer;
}

void U3ScrollArea::setBlockSize(int blockSize)
{
	m_blockSize = blockSize;
	if (m_texDisplay)
	{
		SDL_DestroyTexture(m_texDisplay);
	}
	m_texDisplay = SDL_CreateTexture(m_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, m_blockSize * 17, m_blockSize * 8);
	m_forceRedraw = true;
}

void U3ScrollArea::DrawPrompt()
{
	m_graphics.DrawFramePieceScroll(12, 0, 6);
}

