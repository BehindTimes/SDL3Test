#include <filesystem>
#include <SDL_image.h>
#include "U3Dialog.h"
#include "U3Button.h"
#include "U3Utilities.h"

extern SDL_Window* window;
extern short screenOffsetX;
extern short screenOffsetY;
extern U3Utilities m_utilities;

U3Dialog::U3Dialog(SDL_Renderer* renderer, TTF_TextEngine* engine_surface,
	ModeGraphics** currentGraphics, ModeGraphics** standardGraphics,
	int blockSize, int message, std::function<void()> callback) :
	m_renderer(renderer),
	m_engine_surface(engine_surface),
	m_currentGraphics(currentGraphics),
	m_standardGraphics(standardGraphics),
	m_dialogLoaded(false),
	m_font(nullptr),
	m_blockSize(blockSize),
	m_message(message - 1),
	m_titleRect(0),
	m_messageRect(0),
	m_numblocksW(0),
	m_numblocksH(0),
	m_text_obj_title(nullptr),
	m_text_obj_message(nullptr),
	m_titleX(0),
	m_titleY(0),
	m_messageX(0),
	m_messageY(0)
{
	createFont();
	loadString();
	createButton(callback);
	calculateRects();
}

U3Dialog::~U3Dialog()
{
	if (m_text_obj_title)
	{
		TTF_DestroyText(m_text_obj_title);
		m_text_obj_title = nullptr;
	}

	if (m_text_obj_message)
	{
		TTF_DestroyText(m_text_obj_message);
		m_text_obj_message = nullptr;
	}

	if (m_font)
	{
		TTF_CloseFont(m_font);
	}
}

void U3Dialog::calculateRects()
{
	int title_width = 0;
	int title_height = 0;
	int message_width = 0;
	int message_height = 0;
	int windowWidth;
	int windowHeight;

	SDL_Color sdl_text_color = { 255, 255, 255 };

	SDL_GetWindowSize(window, &windowWidth, &windowHeight);

	m_text_obj_title = calcDisplayString(m_font, m_strTitle, title_width, title_height, sdl_text_color);
	m_text_obj_message = calcDisplayString(m_font, m_strMessage, message_width, message_height, sdl_text_color);

	int maxWidth = std::max<int>(message_width, title_width);
	int totalHeight = title_height + message_height;

	m_numblocksW = (int)(std::ceil((float)maxWidth / (float)m_blockSize));
	m_numblocksH = (int)(std::ceil((float)totalHeight / (float)m_blockSize));
	m_numblocksW += 2;
	m_numblocksH += 4;

	int xStart = (windowWidth - (m_numblocksW * m_blockSize)) / 2;
	int yStart = (windowHeight - (m_numblocksH * m_blockSize)) / 2;
	m_dialogRect.x = (float)xStart - screenOffsetX;
	m_dialogRect.y = (float)yStart - screenOffsetY;
	m_dialogRect.w = (float)m_numblocksW * m_blockSize;
	m_dialogRect.h = (float)m_numblocksH * m_blockSize;

	m_titleRect = m_dialogRect;

	m_titleRect.x += (float)m_blockSize;
	m_titleRect.y += (float)m_blockSize;
	m_titleRect.w -= (float)m_blockSize * 2.0f;
	m_titleRect.h = (float)m_blockSize;

	adjustRect(m_titleRect);

	m_messageRect = m_titleRect;

	m_messageRect.y += (float)(m_blockSize * 2);
	m_messageRect.h = (float)(m_numblocksH - 4) * m_blockSize;

	xStart = (int)((windowWidth - title_width) / 2.0f);

	m_titleX = xStart - screenOffsetX;
	m_titleY = (int)(m_dialogRect.y + m_blockSize);
	m_messageX = (int)(m_dialogRect.x + m_blockSize);
	m_messageY = (int)(m_dialogRect.y + m_blockSize * 3);

	SDL_FRect fillRect = m_messageRect;

	fillRect.y = m_dialogRect.y + (float)(m_numblocksH - 2) * m_blockSize;
	fillRect.h = (float)m_blockSize;

	m_backButton.updateLocationCentered(fillRect);
}

void U3Dialog::changeBlockSize(int blockSize)
{
	m_blockSize = blockSize;
	if (m_font)
	{
		TTF_CloseFont(m_font);
		m_font = nullptr;
	}
	createFont();
	calculateRects();
}

void U3Dialog::createButton(std::function<void()> callback)
{
	short butLeft = 100;
	short butRight = 153;
	short butTop = 300;
	short butBottom = 325;

	std::filesystem::path currentPath = std::filesystem::current_path();
	currentPath /= ResourceLoc;
	currentPath /= ImagesLoc;
	currentPath /= "Buttons.png";

	SDL_Texture* buttonImage = IMG_LoadTexture(m_renderer, currentPath.string().c_str());

	m_backButton.setRect(m_renderer, buttonImage, butLeft, butTop,
		butRight - butLeft, butBottom - butTop,
		true, false);

	if (buttonImage)
	{
		SDL_DestroyTexture(buttonImage);
	}
	m_backButton.setVisible(true);

	m_backButton.SetButtonCallback(callback);
}

bool U3Dialog::createFont()
{
	bool retVal = false;
	std::filesystem::path currentPath = std::filesystem::current_path();
	currentPath /= ResourceLoc;
	currentPath /= FontLoc;
	currentPath /= "FreeSerif.ttf";

	if (m_blockSize > 8)
	{
		m_font = TTF_OpenFont(currentPath.string().c_str(), (float)m_blockSize - 4);
		if (m_font)
		{
			retVal = true;
		}
	}

	return retVal;
}

void U3Dialog::adjustRect(SDL_FRect& myRect)
{
	myRect.x += screenOffsetX;
	myRect.y += screenOffsetY;
}

void U3Dialog::loadString()
{
	std::filesystem::path currentPathTitle = std::filesystem::current_path();
	currentPathTitle /= ResourceLoc;
	currentPathTitle /= TextLoc;
	currentPathTitle /= std::string("MainResources.rsrc_STR#_418_Alert Messages_") + std::to_string(m_message * 2) + std::string(".txt");

	std::filesystem::path currentPathMessage = std::filesystem::current_path();
	currentPathMessage /= ResourceLoc;
	currentPathMessage /= TextLoc;
	currentPathMessage /= std::string("MainResources.rsrc_STR#_418_Alert Messages_") + std::to_string(m_message * 2 + 1) + std::string(".txt");

	std::uintmax_t file_size = std::filesystem::file_size(currentPathTitle);
	SDL_IOStream* file = SDL_IOFromFile(currentPathTitle.string().c_str(), "rb");

	if (!file)
	{
		return;
	}
	std::vector<char> fileTitleData;
	fileTitleData.resize(file_size);

	SDL_ReadIO(file, fileTitleData.data(), file_size);
	SDL_CloseIO(file);
	fileTitleData.emplace_back(0);

	file_size = std::filesystem::file_size(currentPathMessage);
	file = SDL_IOFromFile(currentPathMessage.string().c_str(), "rb");

	if (!file)
	{
		return;
	}
	std::vector<char> fileData;
	fileData.resize(file_size);

	SDL_ReadIO(file, fileData.data(), file_size);
	SDL_CloseIO(file);
	fileData.emplace_back(0);

	m_strTitle = std::string(fileTitleData.data());
	m_strMessage = std::string(fileData.data());
	auto newLineTokens = m_utilities.splitString(m_strMessage, '\n', true);
	m_strMessage.clear();
	bool maxLineCountHit = false;
	const int MAX_LEN_TOKEN = 40;
	const int MAX_LINE_COUNT = 12;
	int line_count = 0;
	for (auto& curLineToken : newLineTokens)
	{
		line_count++;
		if (line_count > MAX_LINE_COUNT)
		{
			maxLineCountHit = true;
			break;
		}

		if (maxLineCountHit)
		{
			break;
		}
		auto tokens = m_utilities.splitString(curLineToken, ' ');
		size_t nCurLen = 0;
		
		for (auto& curToken : tokens)
		{
			if (curToken == std::string("\n"))
			{
				m_strMessage += std::string("\n");
				nCurLen = 0;
				line_count++;
				if (line_count > MAX_LINE_COUNT)
				{
					maxLineCountHit = true;
					break;
				}
			}
			else
			{
				if (nCurLen + curToken.size() > MAX_LEN_TOKEN)
				{
					m_strMessage += std::string("\n");
					nCurLen = 0;
					line_count++;
					if (line_count > MAX_LINE_COUNT)
					{
						maxLineCountHit = true;
						break;
					}
				}
				if (nCurLen > 0)
				{
					m_strMessage += std::string(" ");
					nCurLen++;
				}
			}
			m_strMessage += curToken;
			nCurLen += curToken.size();
		}
	}
}

TTF_Text* U3Dialog::calcDisplayString(TTF_Font* font, std::string curString, int& outWidth, int& outHeight, SDL_Color color)
{
	TTF_Text* text_obj = NULL;
	text_obj = TTF_CreateText(m_engine_surface, font, curString.c_str(), 0);

	TTF_SetTextColor(text_obj, color.r, color.g, color.b, 255);

	TTF_GetTextSize(text_obj, &outWidth, &outHeight);

	return text_obj;
}

void U3Dialog::renderDisplayString(TTF_Text* text_obj, int x, int y, SDL_Color color)
{
	TTF_DrawRendererText(text_obj, (float)x + screenOffsetX, (float)y + screenOffsetY);
}

void U3Dialog::DrawFramePiece(int part, int x, int y)
{
	SDL_FRect frameRect, myRect;

	myRect.x = (float)(x);
	myRect.y = (float)(y);
	myRect.w = (float)(m_blockSize);
	myRect.h = (float)(m_blockSize);

	adjustRect(myRect);

	int xPart = part % UI_NUM_X;
	int yPart = part / UI_NUM_X;

	if ((*m_currentGraphics)->ui)
	{
		frameRect.x = (float)((*m_currentGraphics)->ui_width * xPart);
		frameRect.y = (float)((*m_currentGraphics)->ui_height * yPart);
		frameRect.w = (*m_currentGraphics)->ui_width;
		frameRect.h = (*m_currentGraphics)->ui_height;

		SDL_RenderTexture(m_renderer, (*m_currentGraphics)->ui, &frameRect, &myRect);
	}
	else // fallback for missing graphics
	{
		frameRect.x = (float)((*m_standardGraphics)->ui_width * xPart);
		frameRect.y = (float)((*m_standardGraphics)->ui_height * yPart);
		frameRect.w = (*m_standardGraphics)->ui_width;
		frameRect.h = (*m_standardGraphics)->ui_height;

		SDL_RenderTexture(m_renderer, (*m_standardGraphics)->ui, &frameRect, &myRect);
	}
}

void U3Dialog::createBorder(int x, int y, int numBlocksW, int numBlocksH)
{
	DrawFramePiece(0, x, y);
	DrawFramePiece(6, x, y + ((numBlocksH - 1) * m_blockSize));
	DrawFramePiece(2, x + ((numBlocksW - 1) * m_blockSize), y);
	DrawFramePiece(8, x + ((numBlocksW - 1) * m_blockSize), y + ((numBlocksH - 1) * m_blockSize));

	for (int index = 1; index < numBlocksW - 1; index++)
	{
		DrawFramePiece(9, x + (index * m_blockSize), y);
		DrawFramePiece(9, x + (index * m_blockSize), y + ((numBlocksH - 1) * m_blockSize));
		DrawFramePiece(9, x + (index * m_blockSize), y + (2 * m_blockSize));
	}
	for (int index = 1; index < numBlocksH - 1; index++)
	{
		if (index != 2)
		{
			DrawFramePiece(10, x, y + (index * m_blockSize));
			DrawFramePiece(10, x + ((numBlocksW - 1) * m_blockSize), y + (index * m_blockSize));
		}
		else
		{
			DrawFramePiece(3, x, y + (index * m_blockSize));
			DrawFramePiece(5, x + ((numBlocksW - 1) * m_blockSize), y + (index * m_blockSize));
		}
	}
}

bool U3Dialog::display()
{
	if (!m_font || m_blockSize <= 0)
	{
		return false;
	}
	SDL_Color sdl_text_color = { 255, 255, 255 };

	SDL_SetRenderDrawColor(m_renderer, 128, 128, 128, 255);
	SDL_RenderFillRect(m_renderer, &m_titleRect);

	SDL_SetRenderDrawColor(m_renderer, 160, 160, 160, 160);
	SDL_RenderFillRect(m_renderer, &m_messageRect);

	SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 0);

	createBorder((int)m_dialogRect.x, (int)m_dialogRect.y, m_numblocksW, m_numblocksH);

	renderDisplayString(m_text_obj_title, m_titleX, m_titleY, sdl_text_color);
	renderDisplayString(m_text_obj_message, m_messageX, m_messageY, sdl_text_color);

	m_backButton.renderCentered(m_renderer);

	SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 0);

	return true;
}

void U3Dialog::HandleEvent(SDL_Event& event)
{
	bool updateMouse = false;
	int mouseState = 0;

	switch (event.type)
	{
	case SDL_EVENT_KEY_DOWN:
		break;
	case SDL_EVENT_MOUSE_BUTTON_DOWN:
		mouseState = 1;
		updateMouse = true;
		break;
	case SDL_EVENT_MOUSE_BUTTON_UP:
		mouseState = 2;
		updateMouse = true;
		break;
	case SDL_EVENT_MOUSE_MOTION:
		mouseState = 0;
		updateMouse = true;
		break;
	default:
		break;
	}

	if (updateMouse)
	{
		m_backButton.setMouseCapture(mouseState, event.motion.x, event.motion.y);
	}
}
