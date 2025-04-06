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
	m_top(0),
	m_maxTop(0),
	m_updateRatio(false),
	m_forceRedraw(true),
	m_updateQueue(false),
	m_update(false),
	m_scrolling(false),
	m_queueBegin(false)
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
	if (m_messageQueue.size() == 0)
	{
		m_queueBegin = true;
	}
	for (size_t index = 0; index < vecString.size(); ++index)
	{
		if (index == 0 && m_messageQueue.size() > 0)
		{
			if (m_messageQueue.back() == std::string("\n"))
			{
				m_messageQueue.emplace(vecString[index]);
			}
			else if(vecString[index] == std::string("\n"))
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

	if (m_messageQueue.size() > 0)
	{
		m_updating = true;
		m_elapsedTime = 0;
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

void U3ScrollArea::UPrintMessageRewrapped(short msgNum)
{
	bool classic;
	m_resources.GetPreference(U3PreferencesType::Classic_Appearance, classic);
	msgNum--;
	if (m_resources.m_plistMap.contains("Messages"))
	{
		if (m_resources.m_plistMap["Messages"].size() > msgNum)
		{
			std::string message = m_resources.m_plistMap["Messages"][msgNum];
			if (classic)
			{
				UPrintWin(message);
			}
			else
			{
				message = RewrapString(message);
				UPrintWin(message);
			}
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
		if (curPair.first)
		{
			m_graphics.DrawFramePieceScroll(12, 0, tempIndex);
		}
		if (curPair.second.size() > 0)
		{
			m_resources.renderString(curPair.second, curPair.first ? 1 : 0, tempIndex, false);
		}
		tempIndex--;
	}

	SDL_SetRenderTarget(m_renderer, NULL);
	SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 0);
}

void U3ScrollArea::cleanupMessages()
{
	while (m_messages.size() > 8)
	{
		m_messages.pop_front();
	}
}

bool U3ScrollArea::updateQueue()
{
	bool retval = false;
	m_updateQueue = false;

	if (m_messageQueue.size() == 0)
	{
		cleanupMessages();
		return true;
	}
	if (m_queueBegin)
	{
		m_queueBegin = false;
		auto curStr = m_messageQueue.front();
		if (curStr == std::string("\n"))
		{
			m_forceRedraw = true;
			m_update = true;
			cleanupMessages();
			return false;
		}
		else
		{
			while (curStr != std::string("\n"))
			{
				m_messageQueue.pop();
				m_messages.back().second += curStr;
				if (m_messageQueue.size() == 0)
				{
					cleanupMessages();
					return false;
				}
				m_forceRedraw = true;
				m_update = true;
				curStr = m_messageQueue.front();	
			}
			if (m_messageQueue.size() == 0)
			{
				cleanupMessages();
				return true;
			}
			else
			{
				cleanupMessages();
				return false;
			}
		}
	}
	m_forceRedraw = true;
	m_update = true;
	bool updated = false;
	auto curStr = m_messageQueue.front();
	if (curStr == std::string("\n"))
	{
		m_messages.push_back(std::make_pair(0, ""));
		m_messageQueue.pop();

		if (m_messageQueue.size() == 0)
		{
			m_update = false;
			m_messages.back().first = true;
			cleanupMessages();
			return true;
		}
		curStr = m_messageQueue.front();
	}
	while (curStr != std::string("\n"))
	{
		m_messageQueue.pop();
		m_messages.back().second += curStr;
		if (m_messageQueue.size() == 0)
		{
			m_update = false;
			cleanupMessages();
			return true;
		}
		curStr = m_messageQueue.front();
		updated = true;
	}
	cleanupMessages();
	return retval;
}

void U3ScrollArea::render(Uint64 currentTickCount)
{
	float yOffset = 0;
	m_scrolling = false;
	bool bFinished = false;

	if (m_updating)
	{
		if (m_updateQueue && m_queueBegin)
		{
			bFinished = updateQueue();
			m_forceRedraw = true;
		}
		else if(SCROLL_DELAY == 0)
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
	if (yOffset > 0)
	{
		m_scrolling = true;
	}
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

bool U3ScrollArea::isPrompt()
{
	if (m_scrolling)
	{
		return false;
	}
	if (m_messages.size() > 0)
	{
		return m_messages.back().first;
	}
	return true;
}

std::string U3ScrollArea::RewrapString(std::string str)
{
	std::string strMessage;
	int textLen;
	TTF_Text* text_obj = NULL;
	int maxSize = 15 * m_blockSize;
	int totalMaxSize = 15 * m_blockSize;
	bool emptyBackString = true;
	std::vector<std::string> tokens;
	auto vecString = m_utilities.splitString(str, '\n', true);

	for (std::string& tempString : vecString)
	{
		auto vecTemp = m_utilities.splitString(tempString, ' ', true);
		tokens.insert(tokens.end(), vecTemp.begin(), vecTemp.end());
	}
	if (m_messages.size() > 0)
	{
		if (m_messages.back().first)
		{
			maxSize -= m_blockSize;
		}
		if (m_messages.back().second.size() > 0)
		{
			textLen = m_resources.getTextWidth(m_messages.back().second);
			maxSize -= textLen;
			emptyBackString = false;
		}
	}
	size_t start_index = 0;
	size_t end_index = tokens.size() - 1;
	for (start_index = 0; start_index < tokens.size(); ++start_index)
	{
		std::string tempString = tokens[start_index];
		if (tempString == std::string("\n"))
		{
			tempString = std::string("\n");
			strMessage += tempString;
			maxSize = totalMaxSize;
		}
		else
		{
			break;
		}
	}
	int endNewLines = 0;
	for (size_t temp_index = tokens.size() - 1; temp_index > start_index; --temp_index)
	{
		std::string tempString = tokens[temp_index];
		if (tempString == std::string("\n"))
		{
			endNewLines++;
		}
		else
		{
			break;
		}
	}


	for (size_t index = start_index; index < tokens.size() - endNewLines; ++index)
	{
		std::string tempString = tokens[index];
		if (tempString == std::string("\n"))
		{
			tempString = std::string(" ");
		}
		textLen = m_resources.getTextWidth(tempString);
		if (index == 0) // Special append case
		{
			if (emptyBackString) // append here no matter what
			{
				strMessage += tempString;
				maxSize -= m_blockSize;
				continue;
			}
		}
		if (maxSize < textLen)
		{
			strMessage += std::string("\n");
			maxSize = totalMaxSize;
		}
		strMessage += tempString;
		maxSize = maxSize - textLen;
	}
	for (size_t temp_index = 0; temp_index < endNewLines; ++temp_index)
	{
		strMessage += std::string("\n");
	}
	return strMessage;
}
