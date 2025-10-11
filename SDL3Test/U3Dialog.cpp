#include <SDL3_image/SDL_image.h>
#include <filesystem>

#include "UltimaGraphics.h"
#include "UltimaIncludes.h"
#include "U3Button.h"
#include "U3Dialog.h"
#include "U3Misc.h"
#include "U3Resources.h"
#include "U3Utilities.h"

extern SDL_Window* window;
extern short screenOffsetX;
extern short screenOffsetY;
extern std::unique_ptr<U3Resources> m_resources;
extern std::unique_ptr<U3Utilities> m_utilities;
extern std::unique_ptr<U3Graphics> m_graphics;
extern std::unique_ptr<U3Misc> m_misc;

U3Dialog::U3Dialog(SDL_Renderer* renderer, TTF_TextEngine* engine_surface,
	ModeGraphics** currentGraphics, ModeGraphics** standardGraphics,
	int blockSize, int message, DialogType type, std::function<void()> callback) :
	m_renderer(renderer),
	m_engine_surface(engine_surface),
	m_currentGraphics(currentGraphics),
	m_standardGraphics(standardGraphics),
	m_dialogLoaded(false),
	m_font(nullptr),
	m_blockSize(blockSize),
	m_message(message - 1),
	m_titleRect(),
	m_messageRect(),
	m_numblocksW(0),
	m_numblocksH(0),
	m_text_obj_title(nullptr),
	m_text_obj_message(nullptr),
	m_titleX(0),
	m_titleY(0),
	m_messageX(0),
	m_messageY(0),
	m_icon(nullptr),
	m_callback(callback),
	m_retVal(0)
{
	createFont();
	if (type == DialogType::Alert)
	{
		loadString();
		createButton(std::bind(&U3Dialog::finishedCallback, this, std::placeholders::_1));
	}
	else if (type == DialogType::DITL)
	{
		loadDitl(m_blockSize, std::bind(&U3Dialog::finishedCallback, this, std::placeholders::_1));
		//createButton(callback);
	}
	calculateRects();
}

U3Dialog::~U3Dialog()
{
	/*if (m_icon)
	{
		SDL_DestroyTexture(m_icon);
	}*/
	m_vecButtons.clear();
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
		m_font = nullptr;
	}
}

void U3Dialog::finishedCallback(int button)
{
	m_retVal = button;
	if (m_callback)
	{
		m_callback();
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

	int offsetTitle = 0;
	if (!m_strTitle.empty())
	{
		offsetTitle = 2;
	}
	int maxWidth = std::max<int>(message_width, title_width);
	int totalHeight = title_height + message_height;

	m_numblocksW = (int)(std::ceil((float)maxWidth / (float)m_blockSize));
	m_numblocksH = (int)(std::ceil((float)totalHeight / (float)m_blockSize));

	m_numblocksH += 4;
	m_numblocksW += 2;

	if (m_strTitle.empty())
	{
		m_numblocksH--;
	}

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

	m_messageRect.y += (float)(m_blockSize * offsetTitle);
	m_messageRect.h = (float)(m_numblocksH - (2 + offsetTitle)) * m_blockSize;

	xStart = (int)((windowWidth - title_width) / 2.0f);

	m_titleX = xStart - screenOffsetX;
	m_titleY = (int)(m_dialogRect.y + m_blockSize);
	m_messageX = (int)(m_dialogRect.x + m_blockSize);
	m_messageY = (int)(m_dialogRect.y + m_blockSize * (3 - (2 - offsetTitle)));

	SDL_FRect fillRect = m_messageRect;

	fillRect.y = m_dialogRect.y + (float)(m_numblocksH - 2) * m_blockSize;
	fillRect.h = (float)m_blockSize;

	if (m_vecButtons.size() > 0)
	{
		float totalsize = 0;
		for (auto& curButton : m_vecButtons)
		{
			totalsize += curButton->getWidth();
		}

		float hRatio = fillRect.h / m_vecButtons[0]->getHeight();
		totalsize *= hRatio;
		
		float tempStart = (m_messageRect.w - totalsize) / 2.0f;
		fillRect.x += tempStart;
		const int BUTTON_SPACE = 2;
		//fillRect.x -= (BUTTON_SPACE * (m_vecButtons.size() - 1));
		
		for (size_t index = 0; index < m_vecButtons.size(); ++index)
		{
			fillRect.w = m_vecButtons[index]->getWidth() * hRatio;
			m_vecButtons[index]->updateLocationCentered(fillRect);
			fillRect.x += (m_vecButtons[index]->getWidth() * hRatio);
			fillRect.x += BUTTON_SPACE;
			//fillRect.x += BUTTON_SPACE;
		}
	}
	else
	{
		m_backButton.updateLocationCentered(fillRect);
	}
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
	for (auto& curButton : m_vecButtons)
	{
		curButton->resizeButton(blockSize, m_renderer, m_engine_surface, m_font);
	}
	calculateRects();
}

void U3Dialog::createButton(std::function<void(int)> callback)
{
	short butLeft = 100;
	short butTop = 300;
	short butWidth = 212;
	short butHeight = 100;

	std::filesystem::path currentPath = m_resources->m_exePath;
	currentPath /= ResourceLoc;
	currentPath /= ImagesLoc;
	currentPath /= "Buttons.png";

	SDL_Texture* buttonImage = IMG_LoadTexture(m_renderer, currentPath.string().c_str());

	m_backButton.setRect(m_renderer, buttonImage, m_blockSize, butLeft, butTop,
		butWidth, butHeight,
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
	std::filesystem::path currentPath = m_resources->m_exePath;
	currentPath /= ResourceLoc;
	currentPath /= FontLoc;
	currentPath /= m_resources->m_preferences.font;

	if (m_font)
	{
		TTF_CloseFont(m_font);
		m_font = nullptr;
	}

	if (m_blockSize > 8)
	{
		m_font = TTF_OpenFont(currentPath.string().c_str(), (float)m_blockSize * 0.875f);
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

short U3Dialog::getDitlShort(std::vector<unsigned char> data, int& address)
{
	short value = 0;
	for (int byte = 0; byte < 2; ++byte)
	{
		value <<= 8;
		value += data[address];
		address++;
	}
	return value;
}

bool U3Dialog::processDitlData(std::vector<unsigned char> data, std::vector<DitlInfo>& ditl_vec)
{
	int numitems = 0;
	int address = 0;
	numitems = getDitlShort(data, address);
	numitems++;

	std::string strFromApostrophe;
	std::string strToApostrophe;
	std::string strFromLeftQuote;
	std::string strToLeftQuote;
	std::string strFromRightQuote;
	std::string strToRightQuote;

	strFromApostrophe += static_cast<char>(0xd5);

	strToApostrophe += static_cast<char>(0xe2);
	strToApostrophe += static_cast<char>(0x80);
	strToApostrophe += static_cast<char>(0x99);

	strFromLeftQuote += static_cast<char>(0xd2);

	strToLeftQuote += static_cast<char>(0xe2);
	strToLeftQuote += static_cast<char>(0x80);
	strToLeftQuote += static_cast<char>(0x9c);

	strFromRightQuote += static_cast<char>(0xd3);

	strToRightQuote += static_cast<char>(0xe2);
	strToRightQuote += static_cast<char>(0x80);
	strToRightQuote += static_cast<char>(0x9d);

	for (int byte = 0; byte < numitems; byte++)
	{
		DitlInfo curInfo;

		address += 4; // ?  What's this?
		curInfo.y1 = getDitlShort(data, address);
		curInfo.x1 = getDitlShort(data, address);
		curInfo.y2 = getDitlShort(data, address);
		curInfo.x2 = getDitlShort(data, address);
		curInfo.type = data[address];
		address++;
		int len = data[address];
		address++;
		std::vector<char> vec_text;
		switch (curInfo.type)
		{
		case 4: // button
			vec_text.resize(len);
			std::copy(data.begin() + address,
				data.begin() + address + len, vec_text.begin());
			vec_text.push_back(0);
			address += len;
			if (len % 2 != 0)
			{
				address++; // Seems to be 16 bit aligned
			}
			curInfo.text = std::string(vec_text.data());
			break;
		case 0x88: // text
			vec_text.resize(len);
			std::copy(data.begin() + address,
				data.begin() + address + len, vec_text.begin());
			vec_text.push_back(0);
			address += len;
			if (len % 2 != 0)
			{
				address++; // Seems to be 16 bit aligned
			}
			curInfo.text = std::string(vec_text.data());
			break;
		case 0xA0: // icon
			if (len != 2)
			{
				return false; // Guess I'm wrong
			}
			curInfo.id = getDitlShort(data, address);
			break;
		}

		curInfo.text = m_utilities->replaceAll(curInfo.text, std::string("\r"), std::string("\n"));
		curInfo.text = m_utilities->replaceAll(curInfo.text, strFromApostrophe, strToApostrophe);
		curInfo.text = m_utilities->replaceAll(curInfo.text, strFromLeftQuote, strToLeftQuote);
		curInfo.text = m_utilities->replaceAll(curInfo.text, strFromRightQuote, strToRightQuote);
		
		ditl_vec.push_back(curInfo);

	}
	
	return true;
}

// I'm completely guessing on this stuff
void U3Dialog::loadDitl(int blockSize, std::function<void(int)> callback)
{
	Uint32 file_size = 0;
	size_t start_address = 0x253b; // This I know is 1

	for (int index = 1; index < (m_message - BASERES) - 1; index++)
	{
		file_size = 0;
		for (int byte = 0; byte < 4; ++byte)
		{
			file_size <<= 8;
			file_size += m_resources->m_vecResourceData[start_address + byte];
		}
		start_address += file_size + 4;
	}
	file_size <<= 8;
	for (int byte = 0; byte < 4; ++byte)
	{
		file_size <<= 8;
		file_size += m_resources->m_vecResourceData[start_address + byte];
	}
	std::vector<unsigned char> data;
	data.resize(file_size);
	std::copy(m_resources->m_vecResourceData.begin() + (start_address + 4),
		m_resources->m_vecResourceData.begin() + (start_address + 4) + file_size, data.begin());
	
	std::vector<DitlInfo> ditl_data;
	if (!processDitlData(data, ditl_data))
	{
		return;
	}

	int curId = 0;

	m_strMessage.clear();

	for (auto& temp_data : ditl_data)
	{
		if (temp_data.type == 0xA0) // Icon
		{
			int tempid = temp_data.id - 400;
			if (tempid >= 0 && tempid < 2)
			{
				m_icon = m_resources->m_texIcons[tempid];
			}
			//std::string icon_str = std::to_string(temp_data.id);
			//m_utilities->trim(icon_str);
			/*std::filesystem::path IconPathMessage = m_resources->m_exePath;
			IconPathMessage /= ResourceLoc;
			IconPathMessage /= BinLoc;
			IconPathMessage /= IconPathMessage;
			IconPathMessage /= std::string("MainResources.rsrc_cicn_");
			IconPathMessage += icon_str;
			IconPathMessage += std::string(".bmp");

			if (std::filesystem::exists(IconPathMessage) && !m_icon)
			{
				m_icon = IMG_LoadTexture(m_renderer, IconPathMessage.string().c_str());
			}*/
		}
		else if (ditl_data[curId].type == 0x04) // Button
		{
			std::string strText = std::string(" ") + temp_data.text + std::string(" "); // just to give some extra padding
			//m_vecButtons.emplace_back(m_renderer, m_engine_surface, m_font, strText);

			curId++;
			auto curButton = std::make_unique<U3Button>();
			m_vecButtons.push_back(std::move(curButton));
			m_vecButtons.back()->CreateTextButton(blockSize, m_renderer, m_engine_surface, m_font, strText);
			m_vecButtons.back()->SetButtonCallback(callback, curId);
		}
		else if (ditl_data[curId].type == 0x88) // Text
		{
			m_strMessage = temp_data.text;
			RewrapMessage(m_strMessage);
		}
	}
}
/*
void U3Dialog::loadDitlString(int blockSize, std::function<void(int)> callback)
{
	std::filesystem::path currentPathMessage = m_resources->m_exePath;
	currentPathMessage /= ResourceLoc;
	currentPathMessage /= TextLoc;
	currentPathMessage /= std::string("MainResources.rsrc_DITL_") + std::to_string(m_message + 1) + std::string(".txt");

	if (!std::filesystem::exists(currentPathMessage))
	{
		return;
	}

	std::uintmax_t file_size = std::filesystem::file_size(currentPathMessage);
	std::string strTemp = m_utilities->PathToSDLString(currentPathMessage);
	if (strTemp.empty())
	{
		return;
	}
	SDL_IOStream* file = SDL_IOFromFile(strTemp.c_str(), "rb");

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
	strTemp = m_utilities->PathToSDLString(currentPathMessage);
	if (strTemp.empty())
	{
		return;
	}
	file = SDL_IOFromFile(strTemp.c_str(), "rb");

	if (!file)
	{
		return;
	}
	std::vector<char> fileData;
	fileData.resize(file_size);

	SDL_ReadIO(file, fileData.data(), file_size);
	SDL_CloseIO(file);
	fileData.emplace_back(0);
	std::string strData = std::string(fileData.data());

	auto tokens = m_utilities->splitString(strData, '\n');

	int mode = 0;

	m_strMessage.clear();

	int curId = 0;

	for (auto& curToken : tokens)
	{
		if (curToken.size() == 0)
		{
			continue;
		}
		curToken.erase(0, 1);
		m_utilities->trim(curToken);
		auto linetokens = m_utilities->splitString(curToken, ':');
		if (linetokens.size() < 2)
		{
			continue;
		}
		if (linetokens[0].find("item", 0) != std::string::npos)
		{
			if (linetokens[1].find("BUTTON", 0) != std::string::npos)
			{
				mode = 1;
			}
			else if (linetokens[1].find("TEXT", 0) != std::string::npos)
			{
				mode = 2;
			}
			else if (linetokens[1].find("ICON", 0) != std::string::npos)
			{
				mode = 3;
			}
			else
			{
				mode = 0;
			}
		}
		else if (linetokens[0].rfind("text", 0) != std::string::npos)
		{
			// m_textButtons
			if (mode == 1)
			{
				std::string strText = linetokens[1];
				m_utilities->trim(strText);
				size_t nStart = strText.find('\"');
				size_t nEnd = strText.rfind('\"');
				if (nStart != std::string::npos && nStart < nEnd)
				{
					strText = strText.substr(nStart + 1, (nEnd - nStart) - 1);
					strText = std::string(" ") + strText + std::string(" "); // just to give some extra padding
					//m_vecButtons.emplace_back(m_renderer, m_engine_surface, m_font, strText);

					curId++;
					auto curButton = std::make_unique<U3Button>();
					m_vecButtons.push_back(std::move(curButton));
					m_vecButtons.back()->CreateTextButton(blockSize, m_renderer, m_engine_surface, m_font, strText);
					m_vecButtons.back()->SetButtonCallback(callback, curId);
				}
			}
			else if (mode == 2)
			{
				std::string strText = linetokens[1];
				m_utilities->trim(strText);
				size_t nStart = strText.find('\"');
				size_t nEnd = strText.rfind('\"');
				if (nStart != std::string::npos && nStart < nEnd)
				{
					strText = strText.substr(nStart + 1, (nEnd - nStart) - 1);

					m_strMessage += strText;
				}
			}
		}
		else if (linetokens[0].rfind("ICON", 0) != std::string::npos)
		{
			if (mode == 3)
			{
				std::string icon_str = linetokens[1];
				m_utilities->trim(icon_str);
				std::filesystem::path IconPathMessage = m_resources->m_exePath;
				IconPathMessage /= ResourceLoc;
				IconPathMessage /= BinLoc;
				IconPathMessage /= IconPathMessage;
				IconPathMessage /= std::string("MainResources.rsrc_cicn_");
				IconPathMessage += icon_str;
				IconPathMessage += std::string(".bmp");

				if (std::filesystem::exists(IconPathMessage) && !m_icon)
				{
					m_icon = IMG_LoadTexture(m_renderer, IconPathMessage.string().c_str());
				}
			}
		}
	}

	m_strTitle.clear();
	RewrapMessage(m_strMessage);
}*/

void U3Dialog::RewrapMessage([[maybe_unused]]std::string& strMesssage)
{
	auto newLineTokens = m_utilities->splitString(m_strMessage, '\n', true);
	m_strMessage.clear();
	bool maxLineCountHit = false;
	const int MAX_LEN_TOKEN = 40;
	const int MAX_LINE_COUNT = 16;
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
		auto tokens = m_utilities->splitString(curLineToken, ' ');
		size_t nCurLen = 0;

		for (auto& curToken : tokens)
		{
			if (curToken == std::string("\n"))
			{
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

void U3Dialog::loadString()
{
	std::string strFromApostrophe;
	std::string strToApostrophe;
	std::string strFromLeftQuote;
	std::string strToLeftQuote;
	std::string strFromRightQuote;
	std::string strToRightQuote;

	// Probably a better way to handle all of this

	strFromApostrophe += static_cast<char>(0xd5);

	strToApostrophe += static_cast<char>(0xe2);
	strToApostrophe += static_cast<char>(0x80);
	strToApostrophe += static_cast<char>(0x99);

	strFromLeftQuote += static_cast<char>(0xd2);

	strToLeftQuote += static_cast<char>(0xe2);
	strToLeftQuote += static_cast<char>(0x80);
	strToLeftQuote += static_cast<char>(0x9c);

	strFromRightQuote += static_cast<char>(0xd3);

	strToRightQuote += static_cast<char>(0xe2);
	strToRightQuote += static_cast<char>(0x80);
	strToRightQuote += static_cast<char>(0x9d);

	std::vector<char> fileTitleData;
	std::vector<char> fileData;
	size_t start_address = 0x29c01;
	unsigned char dispStrSize;
	char str255[256];

	for (int index = 0; index < m_message * 2; ++index)
	{
		dispStrSize = m_resources->m_vecResourceData[start_address];
		start_address += static_cast<size_t>(dispStrSize + 1);
	}
	dispStrSize = m_resources->m_vecResourceData[start_address];
	memset(str255, 0, sizeof(char) * 256);
	start_address++;
	memcpy(str255, m_resources->m_vecResourceData.data() + start_address, sizeof(char) * dispStrSize);
	str255[255] = 0; // Get rid of the pesky warning that was already taken care of
	std::string strTemp = std::string(str255);
	m_strTitle = m_utilities->replaceAll(strTemp, strFromApostrophe, strToApostrophe);
	m_strTitle = m_utilities->replaceAll(m_strTitle, strFromLeftQuote, strToLeftQuote);
	m_strTitle = m_utilities->replaceAll(m_strTitle, strFromRightQuote, strToRightQuote);
	start_address += dispStrSize;
	dispStrSize = m_resources->m_vecResourceData[start_address];
	memset(str255, 0, sizeof(char) * 256);
	start_address++;
	memcpy(str255, m_resources->m_vecResourceData.data() + start_address, sizeof(char) * dispStrSize);
	strTemp = std::string(str255);
	m_strMessage = m_utilities->replaceAll(strTemp, std::string("\r"), std::string("\n"));
	m_strMessage = m_utilities->replaceAll(m_strMessage, strFromApostrophe, strToApostrophe);
	m_strMessage = m_utilities->replaceAll(m_strMessage, strFromLeftQuote, strToLeftQuote);
	m_strMessage = m_utilities->replaceAll(m_strMessage, strFromRightQuote, strToRightQuote);
	
	RewrapMessage(m_strMessage);
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
	TTF_SetTextColor(text_obj, color.r, color.g, color.b, 255);
	TTF_DrawRendererText(text_obj, (float)x + screenOffsetX, (float)y + screenOffsetY);
}

void U3Dialog::DrawFramePiece(int part, int x, int y)
{
	SDL_FRect frameRect{};
	SDL_FRect myRect{};

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
		if (!m_strTitle.empty())
		{
			DrawFramePiece(9, x + (index * m_blockSize), y + (2 * m_blockSize));
		}
		DrawFramePiece(9, x + (index * m_blockSize), y + ((numBlocksH - 1) * m_blockSize));
		
	}
	for (int index = 1; index < numBlocksH - 1; index++)
	{
		if (index != 2 || m_strTitle.empty())
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

	if (!m_strTitle.empty())
	{
		SDL_SetRenderDrawColor(m_renderer, 128, 128, 128, 255);
		SDL_RenderFillRect(m_renderer, &m_titleRect);
		renderDisplayString(m_text_obj_title, m_titleX, m_titleY, sdl_text_color);
	}

	SDL_SetRenderDrawColor(m_renderer, 160, 160, 160, 160);
	SDL_RenderFillRect(m_renderer, &m_messageRect);

	SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 0);

	createBorder((int)m_dialogRect.x, (int)m_dialogRect.y, m_numblocksW, m_numblocksH);
	
	renderDisplayString(m_text_obj_message, m_messageX, m_messageY, sdl_text_color);

	if (m_vecButtons.size() > 0)
	{
		for (auto& curButton : m_vecButtons)
		{
			curButton->renderCentered(m_renderer);
		}
	}
	else
	{
		m_backButton.renderCentered(m_renderer);
	}

	if (m_icon)
	{
		SDL_FRect frameRect{};
		frameRect.x = m_dialogRect.x + (((m_numblocksW - 1) / 2.0f) * m_blockSize);
		frameRect.y = m_dialogRect.y;
		frameRect.w = (float)m_blockSize;
		frameRect.h = (float)m_blockSize;
		m_resources->adjustRect(frameRect);
		SDL_RenderTexture(m_renderer, m_icon, NULL, &frameRect);
	}

	SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 0);

	return true;
}

void U3Dialog::HandleEvent(SDL_Event& event)
{
	bool updateMouse = false;
	int mouseState = 0;
	int mouse_buttons;

	switch (event.type)
	{
	case SDL_EVENT_KEY_UP:
		if (event.key.key == SDLK_RETURN)
		{
			m_backButton.click();
		}
		break;
	case SDL_EVENT_KEY_DOWN:
		if (event.key.key == SDLK_RETURN)
		{
			m_backButton.forceCapture();
		}
		break;
	case SDL_EVENT_MOUSE_BUTTON_DOWN:
		mouse_buttons = SDL_GetMouseState(nullptr, nullptr);
		if (mouse_buttons & SDL_BUTTON_MASK(SDL_BUTTON_LEFT))
		{
			mouseState = 1;
			updateMouse = true;
		}
		break;
	case SDL_EVENT_MOUSE_BUTTON_UP:
		mouse_buttons = SDL_GetMouseState(nullptr, nullptr);
		if (!(mouse_buttons & SDL_BUTTON_MASK(SDL_BUTTON_LEFT)))
		{
			mouseState = 2;
			updateMouse = true;
		}
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
		if (m_vecButtons.size() > 0)
		{
			for (auto& curButton : m_vecButtons)
			{
				curButton->setMouseCapture(mouseState, event.motion.x, event.motion.y);
			}
		}
		else
		{
			m_backButton.setMouseCapture(mouseState, event.motion.x, event.motion.y);
		}
	}
}

U3CheckBox::U3CheckBox(int blockSize, int x, int y) :
	m_x(x),
	m_y(y),
	m_blockSize(blockSize),
	m_checked(false)
{
}

U3CheckBox::~U3CheckBox()
{
}

void U3CheckBox::render(SDL_Renderer* renderer, int x, int y) const
{
	SDL_FRect myRect{};

	myRect.x = (float)x;
	myRect.y = (float)y;
	myRect.w = (float)m_blockSize;
	myRect.h = (float)m_blockSize;

	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderFillRect(renderer, &myRect);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderRect(renderer, &myRect);

	if (m_checked)
	{
		float scaler = (float)m_blockSize / 16.0f;

		myRect.x = (float)x + (3 * scaler);
		myRect.y = (float)y + (3 * scaler);
		myRect.w = (float)(10) * scaler;
		myRect.h = (float)(10) * scaler;

		SDL_RenderFillRect(renderer, &myRect);
	}
}

U3TextBox::U3TextBox(int blockSize, int x, int y, int width) :
	m_ttfText(nullptr),
	m_x(x),
	m_y(y),
	m_width(width),
	m_blockSize(blockSize),
	m_readOnly(true)
{
}

U3TextBox::~U3TextBox()
{
	if (m_ttfText)
	{
		TTF_DestroyText(m_ttfText);
		m_ttfText = nullptr;
	}
}

void U3TextBox::U3TextBoxFont(TTF_TextEngine* engine_surface, TTF_Font* font, int blockSize)
{
	if (m_ttfText)
	{
		TTF_DestroyText(m_ttfText);
		m_ttfText = nullptr;
	}
	m_ttfText = TTF_CreateText(engine_surface, font, m_strText.c_str(), 0);
	m_blockSize = blockSize;
}

void U3TextBox::render(SDL_Renderer* renderer, int x, int y)
{
	SDL_FRect myRect{};
	float scaler = (float)m_blockSize / 16.0f;

	myRect.x = (float)x;
	myRect.y = (float)y;
	myRect.w = (float)m_width * scaler;
	myRect.h = (float)m_blockSize;

	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderFillRect(renderer, &myRect);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderRect(renderer, &myRect);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);

	SDL_Color sdl_text_color = { 0, 0, 0 };

	renderDisplayString(m_ttfText, (int)(x + (4 * scaler)), (int)(y - (1 * scaler)), sdl_text_color);
}

void U3TextBox::renderDisplayString(TTF_Text* text_obj, int x, int y, SDL_Color color)
{
	if (!text_obj)
	{
		return;
	}
	TTF_SetTextColor(text_obj, color.r, color.g, color.b, 255);
	TTF_DrawRendererText(text_obj, (float)x, (float)y);
}

void U3TextBox::setText(TTF_TextEngine* engine_surface, TTF_Font* font, std::string text)
{
	if (m_ttfText)
	{
		TTF_DestroyText(m_ttfText);
		m_ttfText = nullptr;
	}
	m_strText = text;
	m_ttfText = TTF_CreateText(engine_surface, font, text.c_str(), 0);
}

U3DlgLabel::U3DlgLabel(TTF_TextEngine* engine_surface, TTF_Font* font, std::string strLabel, int x, int y) :
	m_strLabel(strLabel),
	m_ttfLabel(nullptr),
	m_x(x),
	m_y(y)
{
	m_ttfLabel = TTF_CreateText(engine_surface, font, m_strLabel.c_str(), 0);
}

U3DlgLabel::~U3DlgLabel()
{
	if (m_ttfLabel)
	{
		TTF_DestroyText(m_ttfLabel);
		m_ttfLabel = nullptr;
	}
}

void U3DlgLabel::setText(TTF_TextEngine* engine_surface, TTF_Font* font, std::string text)
{
	if (m_ttfLabel)
	{
		TTF_DestroyText(m_ttfLabel);
		m_ttfLabel = nullptr;
	}
	m_strLabel = text;
	m_ttfLabel = TTF_CreateText(engine_surface, font, text.c_str(), 0);
}

void U3DlgLabel::updateLabelFont(TTF_TextEngine* engine_surface, TTF_Font* font)
{
	if (m_ttfLabel)
	{
		TTF_DestroyText(m_ttfLabel);
		m_ttfLabel = nullptr;
	}
	m_ttfLabel = TTF_CreateText(engine_surface, font, m_strLabel.c_str(), 0);
}

void ChooseOptionsDialog::HandleEvent(SDL_Event& event)
{
	bool updateMouse = false;
	int mouseState = 0;
	int mouse_buttons;

	switch (event.type)
	{
	case SDL_EVENT_KEY_UP:
		break;
	case SDL_EVENT_KEY_DOWN:
		break;
	case SDL_EVENT_MOUSE_BUTTON_DOWN:
		mouse_buttons = SDL_GetMouseState(nullptr, nullptr);
		if ((mouse_buttons & SDL_BUTTON_MASK(SDL_BUTTON_LEFT)))
		{
			mouseState = 1;
			updateMouse = true;
		}
		break;
	case SDL_EVENT_MOUSE_BUTTON_UP:
		mouse_buttons = SDL_GetMouseState(nullptr, nullptr);
		if (!(mouse_buttons & SDL_BUTTON_MASK(SDL_BUTTON_LEFT)))
		{
			mouseState = 2;
			updateMouse = true;
		}
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
		m_resources->m_SetOptionsDlg->updateDialog(event.motion.x, event.motion.y, mouseState);
	}
}

ChooseOptionsDialog::ChooseOptionsDialog(SDL_Renderer* renderer, TTF_TextEngine* engine_surface) :
	m_renderer(renderer),
	m_engine_surface(engine_surface),
	m_blockSize(32),
	m_Rect(),
	m_font(nullptr),
	m_closeValue(-1),
	m_curTheme(-1),
	m_healLimit(750),
	m_sfxVolume(100),
	m_musicVolume(100)
{
	m_upArrow += static_cast<char>(0xE2);
	m_upArrow += static_cast<char>(0x96);
	m_upArrow += static_cast<char>(0xB4);

	m_downArrow += static_cast<char>(0xE2);
	m_downArrow += static_cast<char>(0x96);
	m_downArrow += static_cast<char>(0xBE);

	m_curTheme = m_resources->m_currentTheme;
}

ChooseOptionsDialog::~ChooseOptionsDialog()
{
	m_checkBoxes.clear();
	m_textBoxes.clear();
	m_labels.clear();
	m_buttons.clear();

	if (m_font)
	{
		TTF_CloseFont(m_font);
		m_font = nullptr;
	}
}

void ChooseOptionsDialog::themeUp([[maybe_unused]]int id)
{
	if (m_resources->m_themes.size() < 1)
	{
		return;
	}
	m_curTheme++;
	m_curTheme %= m_resources->m_themes.size();

	std::string strTheme = m_resources->m_themes[m_curTheme];
	m_textBoxes[0]->setText(m_engine_surface, m_font, strTheme);
}

void ChooseOptionsDialog::themeDown([[maybe_unused]] int id)
{
	if (m_resources->m_themes.size() < 1)
	{
		return;
	}
	m_curTheme--;
	if (m_curTheme < 0)
	{
		m_curTheme = (int)m_resources->m_themes.size() - 1;
	}
	std::string strTheme = m_resources->m_themes[m_curTheme];
	m_textBoxes[0]->setText(m_engine_surface, m_font, strTheme);
}

void ChooseOptionsDialog::sfxVolumeUp([[maybe_unused]] int id)
{
	m_sfxVolume += 10;
	if (m_sfxVolume > 100)
	{
		m_sfxVolume = 100;
	}

	std::string strVal = std::to_string(m_sfxVolume);

	m_textBoxes[2]->setText(m_engine_surface, m_font, strVal);
}

void ChooseOptionsDialog::sfxVolumeDown([[maybe_unused]] int id)
{
	m_sfxVolume -= 10;
	if (m_sfxVolume < 0)
	{
		m_sfxVolume = 0;
	}

	std::string strVal = std::to_string(m_sfxVolume);

	m_textBoxes[2]->setText(m_engine_surface, m_font, strVal);
}

void ChooseOptionsDialog::musicVolumeUp([[maybe_unused]] int id)
{
	m_musicVolume += 10;
	if (m_musicVolume > 100)
	{
		m_musicVolume = 100;
	}

	std::string strVal = std::to_string(m_musicVolume);

	m_textBoxes[1]->setText(m_engine_surface, m_font, strVal);
}

void ChooseOptionsDialog::musicVolumeDown([[maybe_unused]] int id)
{
	m_musicVolume -= 10;
	if (m_musicVolume < 0)
	{
		m_musicVolume = 0;
	}

	std::string strVal = std::to_string(m_musicVolume);

	m_textBoxes[1]->setText(m_engine_surface, m_font, strVal);
}

void ChooseOptionsDialog::healLimitUp([[maybe_unused]] int id)
{
	m_healLimit += 250;
	if (m_healLimit > 10000)
	{
		m_healLimit = 10000;
	}

	std::string strVal = std::to_string(m_healLimit);

	m_textBoxes[3]->setText(m_engine_surface, m_font, strVal);
}

void ChooseOptionsDialog::healLimitDown([[maybe_unused]] int id)
{
	m_healLimit -= 250;
	if (m_healLimit < 0)
	{
		m_healLimit = 0;
	}

	std::string strVal = std::to_string(m_healLimit);

	m_textBoxes[3]->setText(m_engine_surface, m_font, strVal);
}

void ChooseOptionsDialog::init()
{
	// Initial size is 640x384 with a block size of 16
	float scaler = (float)m_blockSize / 16.0f;
	if (scaler < 1.0f)
	{
		return;
	}
	int w;
	int h;
	createFont();
	addLabel(std::string(GameOptionsStr), 0, 0);
	TTF_GetTextSize(m_labels.back()->m_ttfLabel, &w, &h);
	int text_width_scaled = (int)(w / scaler);
	
	m_labels.back()->m_x = (int)((m_Rect.w - text_width_scaled) / 2);

	addLabel(std::string(AppearanceStr), 4, 20);
	addLabel(std::string(ClassicStr), 8, 40);
	addLabel(std::string(ThemeStr), 8, 68);
	addLabel(std::string(OtherOptionsStr), 190, 20);
	addLabel(std::string(WindStr), 194, 40);
	addLabel(std::string(DiagonalMovementStr), 194, 60);
	addLabel(std::string(AutoCombatStr), 194, 80);
	addLabel(std::string(AutoHealStr), 194, 100);
	addLabel(std::string(LimitStr), 198, 130);
	addLabel(std::string(AutoSaveStr), 194, 160);
	addLabel(std::string(FullScreenStr), 8, 100);
	addLabel(std::string(AudioStr), 4, 120);
	addLabel(std::string(MusicStr), 8, 140);
	addLabel(std::string(VolumeStr), 75, 140);
	addLabel(std::string(SFXStr), 8, 180);
	addLabel(std::string(VolumeStr), 75, 180);

	addCheckBox((int)(m_Rect.w - 20), 40);
	addCheckBox((int)(m_Rect.w - 20), 60);
	addCheckBox((int)(m_Rect.w - 20), 80);
	addCheckBox((int)(m_Rect.w - 20), 100);
	addCheckBox((int)(m_Rect.w - 20), 160);

	addCheckBox(80, 40);
	addCheckBox(80, 100);
	addCheckBox(50, 140);
	addCheckBox(50, 180);

	m_checkBoxes[0]->setChecked(m_resources->m_preferences.include_wind);
	m_checkBoxes[1]->setChecked(m_resources->m_preferences.allow_diagonal);
	m_checkBoxes[2]->setChecked(m_resources->m_preferences.auto_combat);
	m_checkBoxes[3]->setChecked(m_resources->m_preferences.auto_heal);
	m_checkBoxes[4]->setChecked(m_resources->m_preferences.auto_save);

	m_checkBoxes[5]->setChecked(m_resources->m_preferences.classic_appearance);
	m_checkBoxes[6]->setChecked(m_resources->m_preferences.full_screen);
	m_checkBoxes[7]->setChecked(m_resources->m_preferences.play_music);
	m_checkBoxes[8]->setChecked(m_resources->m_preferences.play_sfx);

	addTextBox(50, 68, 110);
	addTextBox(124, 140, 36);
	addTextBox(124, 180, 36);
	addTextBox((int)m_Rect.w - 72, 130, 48);
	std::string strTheme = m_resources->m_themes[m_curTheme];
	m_textBoxes[0]->setText(m_engine_surface, m_font, strTheme);
	std::string strMusVol = std::to_string(m_resources->m_preferences.volume_music);
	m_textBoxes[1]->setText(m_engine_surface, m_font, strMusVol);
	std::string strSfxVol = std::to_string(m_resources->m_preferences.volume_sfx);
	m_textBoxes[2]->setText(m_engine_surface, m_font, strSfxVol);
	std::string strHealAmount = std::to_string(m_resources->m_preferences.auto_heal_amount);
	m_textBoxes[3]->setText(m_engine_surface, m_font, strHealAmount);

	m_musicVolume = m_resources->m_preferences.volume_music;
	m_sfxVolume = m_resources->m_preferences.volume_sfx;
	m_healLimit = m_resources->m_preferences.auto_heal_amount;

	addButton(m_upArrow, 164, 60, std::bind(&ChooseOptionsDialog::themeUp, this, std::placeholders::_1));
	addButton(m_downArrow, 164, 76, std::bind(&ChooseOptionsDialog::themeDown, this, std::placeholders::_1));
	addButton(std::string(CancelString), 194, 216, std::bind(&ChooseOptionsDialog::cancelPushed, this, std::placeholders::_1));
	addButton(std::string(OKString), 266, 216, std::bind(&ChooseOptionsDialog::okPushed, this, std::placeholders::_1));

	addButton(m_upArrow, 164, 132, std::bind(&ChooseOptionsDialog::musicVolumeUp, this, std::placeholders::_1));
	addButton(m_downArrow, 164, 148, std::bind(&ChooseOptionsDialog::musicVolumeDown, this, std::placeholders::_1));

	addButton(m_upArrow, 164, 172, std::bind(&ChooseOptionsDialog::sfxVolumeUp, this, std::placeholders::_1));
	addButton(m_downArrow, 164, 188, std::bind(&ChooseOptionsDialog::sfxVolumeDown, this, std::placeholders::_1));

	addButton(m_upArrow, (int)(m_Rect.w - 19), 122, std::bind(&ChooseOptionsDialog::healLimitUp, this, std::placeholders::_1));
	addButton(m_downArrow, (int)(m_Rect.w - 19), 138, std::bind(&ChooseOptionsDialog::healLimitDown, this, std::placeholders::_1));
}

void ChooseOptionsDialog::changeBlockSize(int blockSize)
{
	m_blockSize = blockSize;
	if (m_font)
	{
		TTF_CloseFont(m_font);
		m_font = nullptr;
	}
	createFont();

	m_labels.clear();
	m_buttons.clear();
	m_textBoxes.clear();
	m_checkBoxes.clear();
	init();
}

bool ChooseOptionsDialog::display()
{
	SDL_FRect myRect{};
	SDL_FRect fromRect{};
	float scaler = (float)m_blockSize / 16.0f;

	myRect.x = (float)m_Rect.x * scaler;
	myRect.y = (float)m_Rect.y * scaler;
	myRect.w = (float)m_Rect.w * scaler;
	myRect.h = (float)m_Rect.h * scaler;

	m_resources->adjustRect(myRect);

	SDL_SetRenderDrawColor(m_renderer, 224, 224, 224, 255);
	SDL_RenderFillRect(m_renderer, &myRect);

	myRect.x = (float)m_Rect.x * scaler;
	myRect.y = (float)m_Rect.y * scaler;
	myRect.w = (float)m_Rect.w * scaler;
	myRect.h = 20.0f * scaler;

	m_resources->adjustRect(myRect);

	SDL_SetRenderDrawColor(m_renderer, 192, 192, 192, 255);
	SDL_RenderFillRect(m_renderer, &myRect);


	SDL_Color sdl_text_color = { 0, 0, 0 };

	for (auto& curLabel : m_labels)
	{
		renderDisplayString(curLabel->m_ttfLabel, (int)(curLabel->m_x * scaler + m_Rect.x * scaler),
			(int)(curLabel->m_y * scaler + m_Rect.y * scaler), sdl_text_color);
	}

	for (auto& curButton : m_buttons)
	{
		curButton->render(m_renderer, m_blockSize, (int)(curButton->m_x * scaler + m_Rect.x * scaler) + screenOffsetX,
			(int)(curButton->m_y * scaler + m_Rect.y * scaler) + screenOffsetY);
	}

	for (auto& curTextBox : m_textBoxes)
	{
		curTextBox->render(m_renderer, (int)(curTextBox->m_x * scaler + m_Rect.x * scaler) + screenOffsetX,
			(int)(curTextBox->m_y * scaler + m_Rect.y * scaler) + screenOffsetY);
	}

	for (auto& curCheckBox : m_checkBoxes)
	{
		curCheckBox->render(m_renderer, (int)(curCheckBox->m_x * scaler + m_Rect.x * scaler) + screenOffsetX,
			(int)(curCheckBox->m_y * scaler + m_Rect.y * scaler) + screenOffsetY);
	}

	myRect.x = (float)m_Rect.x * scaler;
	myRect.y = (float)m_Rect.y * scaler;
	myRect.w = (float)m_Rect.w * scaler;
	myRect.h = (float)m_Rect.h * scaler;

	m_resources->adjustRect(myRect);

	SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
	SDL_RenderRect(m_renderer, &myRect);

	SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 0);
	return false;
}

bool ChooseOptionsDialog::createFont()
{
	bool retVal = false;
	std::filesystem::path currentPath = m_resources->m_exePath;
	currentPath /= ResourceLoc;
	currentPath /= FontLoc;
	currentPath /= m_resources->m_preferences.font;

	if (m_font)
	{
		TTF_CloseFont(m_font);
		m_font = nullptr;
	}

	if (m_blockSize > 8)
	{
		m_font = TTF_OpenFont(currentPath.string().c_str(), (float)m_blockSize * 0.875f);
		if (m_font)
		{
			retVal = true;
		}
	}

	return retVal;
}

void ChooseOptionsDialog::renderDisplayString(TTF_Text* text_obj, int x, int y, SDL_Color color)
{
	TTF_SetTextColor(text_obj, color.r, color.g, color.b, 255);
	TTF_DrawRendererText(text_obj, (float)x + screenOffsetX, (float)y + screenOffsetY);
}

void ChooseOptionsDialog::addLabel(std::string strLabel, int x, int y)
{
	auto curLabel = std::make_unique<U3DlgLabel>(m_engine_surface, m_font, strLabel, x, y);
	m_labels.push_back(std::move(curLabel));
}

void ChooseOptionsDialog::addButton(std::string strLabel, int x, int y, std::function<void(int)> func)
{
	auto curButton = std::make_unique<U3Button>();
	m_buttons.push_back(std::move(curButton));
	m_buttons.back()->CreateTextButton(m_blockSize, m_renderer, m_engine_surface, m_font, strLabel, x, y);
	m_buttons.back()->setVisible(true);
	m_buttons.back()->SetButtonCallback(func);
}

void ChooseOptionsDialog::addTextBox(int x, int y, int width)
{
	auto curTextBox = std::make_unique<U3TextBox>(m_blockSize, x, y, width);
	m_textBoxes.push_back(std::move(curTextBox));
}

void ChooseOptionsDialog::addCheckBox(int x, int y)
{
	auto curCheckBox = std::make_unique<U3CheckBox>(m_blockSize, x, y);
	m_checkBoxes.push_back(std::move(curCheckBox));
}


void ChooseOptionsDialog::cancelPushed([[maybe_unused]] int id)
{
	m_closeValue = 0;
	m_callBack(0);
}

void ChooseOptionsDialog::okPushed([[maybe_unused]] int id)
{
	m_codData.theme = m_curTheme;
	m_codData.include_wind = m_checkBoxes[0]->m_checked;
	m_codData.allow_diagonals = m_checkBoxes[1]->m_checked;
	m_codData.auto_combat = m_checkBoxes[2]->m_checked;
	m_codData.auto_heal = m_checkBoxes[3]->m_checked;
	m_codData.auto_save = m_checkBoxes[4]->m_checked;
	m_codData.classic = m_checkBoxes[5]->m_checked;
	m_codData.is_full_screen = m_checkBoxes[6]->m_checked;
	m_codData.play_music = m_checkBoxes[7]->m_checked;
	m_codData.play_sfx = m_checkBoxes[8]->m_checked;

	int musicvol = std::stoi(m_textBoxes[1]->getText());
	int sfxvol = std::stoi(m_textBoxes[2]->getText());
	int heallimit = std::stoi(m_textBoxes[3]->getText());

	m_codData.volume_music = musicvol;
	m_codData.volume_sfx = sfxvol;
	m_codData.auto_heal_amount = heallimit;

	m_closeValue = 1;
	m_callBack(1);
}

bool ChooseOptionsDialog::updateDialog(float xPos, float yPos, int mouseState)
{
	float scaler = (float)m_blockSize / 16.0f;

	for (auto& curCheck : m_checkBoxes)
	{
		float butOffsetX = (float)(curCheck->m_x * scaler + m_Rect.x * scaler) + screenOffsetX;
		float butOffsetY = (float)(curCheck->m_y * scaler + m_Rect.y * scaler) + screenOffsetY;

		if (mouseState == 2) // mouse up
		{
			if (butOffsetX < xPos && butOffsetX + m_blockSize > xPos &&
				butOffsetY < yPos && butOffsetY + m_blockSize > yPos)
			{
				curCheck->m_checked = !curCheck->m_checked;
			}
		}
	}

	for (auto& curButton : m_buttons)
	{
		float butOffsetX = (float)(curButton->m_x * scaler + m_Rect.x * scaler);
		float butOffsetY = (float)(curButton->m_y * scaler + m_Rect.y * scaler);
		if (mouseState == 2) // mouse up
		{
			curButton->setMouseCapture(m_blockSize, 2, xPos, yPos,
				butOffsetX, butOffsetY, screenOffsetX, screenOffsetY);
		}
		else if (mouseState == 1) // mouse down
		{
			curButton->setMouseCapture(m_blockSize, 1, xPos, yPos,
				butOffsetX, butOffsetY, screenOffsetX, screenOffsetY);
		}
		else // mouse move
		{
			curButton->setMouseCapture(m_blockSize, 0, xPos, yPos,
				butOffsetX, butOffsetY, screenOffsetX, screenOffsetY);
		}
	}

	return (m_closeValue >= 0);
}

void ChooseOptionsDialog::SetDialogFinishedCallback(std::function<void(int)> func)
{
	m_callBack = func;

	/*
	* m_checkBoxes[0]->setChecked(m_resources->m_preferences.include_wind);
	m_checkBoxes[1]->setChecked(m_resources->m_preferences.allow_diagonal);
	m_checkBoxes[2]->setChecked(m_resources->m_preferences.auto_combat);
	m_checkBoxes[3]->setChecked(m_resources->m_preferences.auto_heal);
	m_checkBoxes[4]->setChecked(m_resources->m_preferences.auto_save);

	m_checkBoxes[5]->setChecked(m_resources->m_preferences.classic_appearance);
	m_checkBoxes[6]->setChecked(m_resources->m_preferences.full_screen);
	m_checkBoxes[7]->setChecked(m_resources->m_preferences.play_music);
	m_checkBoxes[8]->setChecked(m_resources->m_preferences.play_sfx);
	*/
}


CreateCharacterDialog::CreateCharacterDialog(SDL_Renderer* renderer, TTF_TextEngine* engine_surface) :
	m_renderer(renderer),
	m_engine_surface(engine_surface),
	m_blockSize(32),
	m_Rect(),
	m_font(nullptr),
	m_curPlayer(nullptr)
{
	m_upArrow += static_cast<char>(0xE2);
	m_upArrow += static_cast<char>(0x96);
	m_upArrow += static_cast<char>(0xB4);

	m_downArrow += static_cast<char>(0xE2);
	m_downArrow += static_cast<char>(0x96);
	m_downArrow += static_cast<char>(0xBE);
}

CreateCharacterDialog::~CreateCharacterDialog()
{
	m_textBoxes.clear();
	m_labels.clear();
	m_buttons.clear();

	if (m_font)
	{
		TTF_CloseFont(m_font);
		m_font = nullptr;
	}
}

void CreateCharacterDialog::loadPresets()
{
	/*std::filesystem::path currentPath = m_resources->m_exePath;
	currentPath /= ResourceLoc;
	currentPath /= TextLoc;
	currentPath /= "MainResources.rsrc_STR#_415_Class Preset Values_";*/

	m_Presets.resize((int)m_resources->m_plistMap["Classes"].size());

	size_t startAddress = 0x29b56;
	char curData[16];
	memset(curData, 0, sizeof(char) * 16);

	for (int index = 0; index < (int)m_resources->m_plistMap["Classes"].size(); ++index)
	{
		m_Presets[index][4] = 0;
		unsigned char dispStringLen = m_resources->m_vecResourceData[startAddress];
		startAddress++;
		if (dispStringLen != 14)
		{
			m_Presets[index][0] = 5;
			m_Presets[index][1] = 5;
			m_Presets[index][2] = 5;
			m_Presets[index][3] = 5;
		}
		else
		{
			memcpy(curData, m_resources->m_vecResourceData.data() + startAddress, sizeof(char) * 14);
			startAddress += 14;
		}

		curData[15] = 0; // Get rid of that pesky warning
		std::string strData = std::string(curData);
		auto vals = m_utilities->splitString(strData, ' ');

		if (vals.size() != 5)
		{
			m_Presets[index][0] = 5;
			m_Presets[index][1] = 5;
			m_Presets[index][2] = 5;
			m_Presets[index][3] = 5;
		}
		else
		{
			int totalCount = 0;
			try
			{
				for (int tempIndex = 1; tempIndex < 5; ++tempIndex)
				{
					int tempcount = std::stoi(vals[tempIndex]);
					m_Presets[index][static_cast<size_t>(tempIndex) - 1] = (char)tempcount;
					totalCount += tempcount;
				}
				if (vals[0].size() >= 2)
				{
					m_Presets[index][4] = vals[0][1] - '0';
				}
				if (totalCount < 20 || totalCount > 50)
				{
					m_Presets[index][0] = 5;
					m_Presets[index][1] = 5;
					m_Presets[index][2] = 5;
					m_Presets[index][3] = 5;
				}
			}
			catch ([[maybe_unused]]const std::exception& e)
			{
				m_Presets[index][0] = 5;
				m_Presets[index][1] = 5;
				m_Presets[index][2] = 5;
				m_Presets[index][3] = 5;
			}
		}
	}
	int rngVal = m_utilities->getRandom(0, 1);
	m_ccdData.sex = rngVal;
	if (rngVal == 0)
	{
		int maxVal = (int)m_resources->m_plistMap["Male"].size();
		rngVal = m_utilities->getRandom(0, maxVal - 1);
		m_ccdData.name = m_resources->m_plistMap["Male"][rngVal];
	}
	else
	{
		int maxVal = (int)m_resources->m_plistMap["Female"].size();
		rngVal = m_utilities->getRandom(0, maxVal - 1);
		m_ccdData.name = m_resources->m_plistMap["Female"][rngVal];
	}
	if (m_ccdData.name.size() > 12)
	{
		m_ccdData.name.clear();
	}

	m_ccdData.type = m_utilities->getRandom(0, (int)m_resources->m_plistMap["Classes"].size() - 1);

	if (m_ccdData.type < m_Presets.size())
	{
		m_ccdData.race = m_Presets[m_ccdData.type][4];
		m_ccdData.strength = m_Presets[m_ccdData.type][0];
		m_ccdData.dexterity = m_Presets[m_ccdData.type][1];
		m_ccdData.intelligence = m_Presets[m_ccdData.type][2];
		m_ccdData.wisdom = m_Presets[m_ccdData.type][3];
		m_ccdData.remaining = 50 - (m_ccdData.strength + m_ccdData.dexterity + m_ccdData.intelligence + m_ccdData.wisdom);
		if (m_ccdData.remaining < 0)
		{
			m_ccdData.remaining = 0;
		}
	}
}

int CreateCharacterDialog::getClassIndex([[maybe_unused]]char value)
{
	return 0;
}

void CreateCharacterDialog::init()
{
	float scaler = 2.0f;
	float ratio = m_resources->m_characterRecordWidth / (m_Rect.w * scaler);
	int offsetY = (int)(m_resources->m_characterRecordHeight * ratio) / 2;

	createFont();
	addLabel(std::to_string((int)m_ccdData.strength), 112, 40 + offsetY);
	addLabel(std::to_string((int)m_ccdData.dexterity), 112, 74 + offsetY);
	addLabel(std::to_string((int)m_ccdData.intelligence), 112, 108 + offsetY);
	addLabel(std::to_string((int)m_ccdData.wisdom), 112, 142 + offsetY);
	addLabel(std::to_string((int)m_ccdData.remaining), 112, 176 + offsetY);

	addLabel(std::string(NameString), 8, 8 + offsetY);
	addLabel(std::string(StrengthString), 8, 40 + offsetY);
	addLabel(std::string(DexterityString), 8, 74 + offsetY);
	addLabel(std::string(IntelligenceString), 8, 108 + offsetY);
	addLabel(std::string(WisdomString), 8, 142 + offsetY);
	addLabel(std::string(PointsString), 8, 176 + offsetY);

	addLabel(std::string(SexString), 176, 56 + offsetY);
	addLabel(std::string(RaceString), 176, 90 + offsetY);
	addLabel(std::string(TypeString), 176, 124 + offsetY);

	addButton(m_upArrow, 144, 32 + offsetY, std::bind(&CreateCharacterDialog::strengthUp, this, std::placeholders::_1));
	addButton(m_downArrow, 144, 48 + offsetY, std::bind(&CreateCharacterDialog::strengthDown, this, std::placeholders::_1));
	addButton(m_upArrow, 144, 66 + offsetY, std::bind(&CreateCharacterDialog::dexterityUp, this, std::placeholders::_1));
	addButton(m_downArrow, 144, 82 + offsetY, std::bind(&CreateCharacterDialog::dexterityDown, this, std::placeholders::_1));
	addButton(m_upArrow, 144, 100 + offsetY, std::bind(&CreateCharacterDialog::intelligenceUp, this, std::placeholders::_1));
	addButton(m_downArrow, 144, 116 + offsetY, std::bind(&CreateCharacterDialog::intelligenceDown, this, std::placeholders::_1));
	addButton(m_upArrow, 144, 134 + offsetY, std::bind(&CreateCharacterDialog::wisdomUp, this, std::placeholders::_1));
	addButton(m_downArrow, 144, 150 + offsetY, std::bind(&CreateCharacterDialog::wisdomDown, this, std::placeholders::_1));

	addButton(m_upArrow, 312, 48 + offsetY, std::bind(&CreateCharacterDialog::sexUp, this, std::placeholders::_1));
	addButton(m_downArrow, 312, 64 + offsetY, std::bind(&CreateCharacterDialog::sexDown, this, std::placeholders::_1));
	addButton(m_upArrow, 312, 82 + offsetY, std::bind(&CreateCharacterDialog::raceUp, this, std::placeholders::_1));
	addButton(m_downArrow, 312, 98 + offsetY, std::bind(&CreateCharacterDialog::raceDown, this, std::placeholders::_1));
	addButton(m_upArrow, 312, 116 + offsetY, std::bind(&CreateCharacterDialog::typeUp, this, std::placeholders::_1));
	addButton(m_downArrow, 312, 132 + offsetY, std::bind(&CreateCharacterDialog::typeDown, this, std::placeholders::_1));

	addButton(std::string(CancelString), 176, 176 + offsetY, std::bind(&CreateCharacterDialog::cancelPushed, this, std::placeholders::_1));
	addButton(std::string(OKString), 256, 176 + offsetY, std::bind(&CreateCharacterDialog::okPushed, this, std::placeholders::_1));
	addButton(std::string(RandomNameString), 237, 8 + offsetY, std::bind(&CreateCharacterDialog::randomNamePushed, this, std::placeholders::_1));

	addTextBox(50, 8 + offsetY, 182);
	addTextBox(210, 56 + offsetY, 100);
	addTextBox(210, 90 + offsetY, 100);
	addTextBox(210, 124 + offsetY, 100);

	m_textBoxes[0]->setText(m_engine_surface, m_font, m_ccdData.name);
	std::string strSex;
	switch (m_ccdData.sex)
	{
	case 1:
		strSex = std::string(FemaleStr);
		break;
	case 2:
		strSex = std::string(OtherStr);
		break;
	default:
		strSex = std::string(MaleStr);
		break;
	}
	m_textBoxes[1]->setText(m_engine_surface, m_font, strSex);

	std::string strRace;
	if (m_resources->m_plistMap["Races"].size() < m_ccdData.race)
	{
		strRace = std::string(HumanStr);
	}
	else
	{
		strRace = m_resources->m_plistMap["Races"][m_ccdData.race];
	}
	m_textBoxes[2]->setText(m_engine_surface, m_font, strRace);
	std::string strClass;
	if (m_resources->m_plistMap["Classes"].size() < m_ccdData.type)
	{
		strClass = std::string(FighterStr);
	}
	else
	{
		strClass = m_resources->m_plistMap["Classes"][m_ccdData.type];
	}
	m_textBoxes[3]->setText(m_engine_surface, m_font, strClass);
}

void CreateCharacterDialog::strengthUp([[maybe_unused]]int id)
{
	if (m_ccdData.remaining > 0 && m_ccdData.strength < 25)
	{
		m_ccdData.remaining--;
		m_ccdData.strength++;
		m_labels[0]->setText(m_engine_surface, m_font, std::to_string(m_ccdData.strength));
		m_labels[4]->setText(m_engine_surface, m_font, std::to_string(m_ccdData.remaining));
	}
}

void CreateCharacterDialog::strengthDown([[maybe_unused]] int id)
{
	if (m_ccdData.remaining < 35 && m_ccdData.strength > 5)
	{
		m_ccdData.remaining++;
		m_ccdData.strength--;
		m_labels[0]->setText(m_engine_surface, m_font, std::to_string(m_ccdData.strength));
		m_labels[4]->setText(m_engine_surface, m_font, std::to_string(m_ccdData.remaining));
	}
}

void CreateCharacterDialog::dexterityUp([[maybe_unused]] int id)
{
	if (m_ccdData.remaining > 0 && m_ccdData.dexterity < 25)
	{
		m_ccdData.remaining--;
		m_ccdData.dexterity++;
		m_labels[1]->setText(m_engine_surface, m_font, std::to_string(m_ccdData.dexterity));
		m_labels[4]->setText(m_engine_surface, m_font, std::to_string(m_ccdData.remaining));
	}
}

void CreateCharacterDialog::dexterityDown([[maybe_unused]] int id)
{
	if (m_ccdData.remaining < 35 && m_ccdData.dexterity > 5)
	{
		m_ccdData.remaining++;
		m_ccdData.dexterity--;
		m_labels[1]->setText(m_engine_surface, m_font, std::to_string(m_ccdData.dexterity));
		m_labels[4]->setText(m_engine_surface, m_font, std::to_string(m_ccdData.remaining));
	}
}

void CreateCharacterDialog::intelligenceUp([[maybe_unused]] int id)
{
	if (m_ccdData.remaining > 0 && m_ccdData.intelligence < 25)
	{
		m_ccdData.remaining--;
		m_ccdData.intelligence++;
		m_labels[2]->setText(m_engine_surface, m_font, std::to_string(m_ccdData.intelligence));
		m_labels[4]->setText(m_engine_surface, m_font, std::to_string(m_ccdData.remaining));
	}
}

void CreateCharacterDialog::intelligenceDown([[maybe_unused]] int id)
{
	if (m_ccdData.remaining < 35 && m_ccdData.intelligence > 5)
	{
		m_ccdData.remaining++;
		m_ccdData.intelligence--;
		m_labels[2]->setText(m_engine_surface, m_font, std::to_string(m_ccdData.intelligence));
		m_labels[4]->setText(m_engine_surface, m_font, std::to_string(m_ccdData.remaining));
	}
}

void CreateCharacterDialog::wisdomUp([[maybe_unused]] int id)
{
	if (m_ccdData.remaining > 0 && m_ccdData.wisdom < 25)
	{
		m_ccdData.remaining--;
		m_ccdData.wisdom++;
		m_labels[3]->setText(m_engine_surface, m_font, std::to_string(m_ccdData.wisdom));
		m_labels[4]->setText(m_engine_surface, m_font, std::to_string(m_ccdData.remaining));
	}
}

void CreateCharacterDialog::wisdomDown([[maybe_unused]] int id)
{
	if (m_ccdData.remaining < 35 && m_ccdData.wisdom > 5)
	{
		m_ccdData.remaining++;
		m_ccdData.wisdom--;
		m_labels[3]->setText(m_engine_surface, m_font, std::to_string(m_ccdData.wisdom));
		m_labels[4]->setText(m_engine_surface, m_font, std::to_string(m_ccdData.remaining));
	}
}

void CreateCharacterDialog::sexUp([[maybe_unused]] int id)
{
	m_ccdData.sex++;
	if (m_ccdData.sex > 2)
	{
		m_ccdData.sex = 0;
	}
	std::string strSex;
	switch (m_ccdData.sex)
	{
	case 1:
		strSex = std::string("Female");
		break;
	case 2:
		strSex = std::string("Other");
		break;
	default:
		strSex = std::string("Male");
		break;
	}
	m_textBoxes[1]->setText(m_engine_surface, m_font, strSex);
}

void CreateCharacterDialog::sexDown([[maybe_unused]] int id)
{
	m_ccdData.sex--;
	if (m_ccdData.sex < 0)
	{
		m_ccdData.sex = 2;
	}
	std::string strSex;
	switch (m_ccdData.sex)
	{
	case 1:
		strSex = std::string("Female");
		break;
	case 2:
		strSex = std::string("Other");
		break;
	default:
		strSex = std::string("Male");
		break;
	}
	m_textBoxes[1]->setText(m_engine_surface, m_font, strSex);
}

void CreateCharacterDialog::raceUp([[maybe_unused]] int id)
{
	m_ccdData.race++;
	if (m_ccdData.race >= m_resources->m_plistMap["Races"].size())
	{
		m_ccdData.race = 0;
	}
	if (m_resources->m_plistMap["Races"].size() > 0)
	{
		std::string strValue = m_resources->m_plistMap["Races"][m_ccdData.race];
		m_textBoxes[2]->setText(m_engine_surface, m_font, strValue);
	}
}

void CreateCharacterDialog::raceDown([[maybe_unused]] int id)
{
	m_ccdData.race--;
	if (m_ccdData.race < 0)
	{
		m_ccdData.race = (int)m_resources->m_plistMap["Races"].size() - 1;
	}
	if (m_resources->m_plistMap["Races"].size() > 0)
	{
		std::string strValue = m_resources->m_plistMap["Races"][m_ccdData.race];
		m_textBoxes[2]->setText(m_engine_surface, m_font, strValue);
	}
}

void CreateCharacterDialog::typeUp([[maybe_unused]] int id)
{
	m_ccdData.type++;
	if (m_ccdData.type >= m_resources->m_plistMap["Classes"].size())
	{
		m_ccdData.type = 0;
	}
	if (m_resources->m_plistMap["Classes"].size() > 0)
	{
		std::string strValue = m_resources->m_plistMap["Classes"][m_ccdData.type];
		m_textBoxes[3]->setText(m_engine_surface, m_font, strValue);
	}
}

void CreateCharacterDialog::typeDown([[maybe_unused]] int id)
{
	m_ccdData.type--;
	if (m_ccdData.type < 0)
	{
		m_ccdData.type = (int)m_resources->m_plistMap["Classes"].size() - 1;
	}
	if (m_resources->m_plistMap["Classes"].size() > 0)
	{
		std::string strValue = m_resources->m_plistMap["Classes"][m_ccdData.type];
		m_textBoxes[3]->setText(m_engine_surface, m_font, strValue);
	}
}

void CreateCharacterDialog::cancelPushed([[maybe_unused]] int id)
{
	m_graphics->m_obsCurMode = OrganizeBottomScreen::CreateCharacterAborted;
}

void CreateCharacterDialog::okPushed([[maybe_unused]] int id)
{
	m_graphics->m_obsCurMode = OrganizeBottomScreen::CreateCharacterDone;
	memset(m_curPlayer, 0, sizeof(unsigned char) * 65);
	size_t copySize = std::min<size_t>(13, m_ccdData.name.size());
	memcpy(m_curPlayer, m_ccdData.name.c_str(), copySize);
	if (m_ccdData.sex == 0)
	{
		m_curPlayer[PLAYER_SEX] = 'M';
	}
	else if (m_ccdData.sex == 1)
	{
		m_curPlayer[PLAYER_SEX] = 'F';
	}
	else
	{
		m_curPlayer[PLAYER_SEX] = 'O';
	}
	if(m_resources->m_plistMap["Races"].size() > m_ccdData.race)
	{
		if (m_resources->m_plistMap["Races"][m_ccdData.race].size() > 0)
		{
			m_curPlayer[22] = m_resources->m_plistMap["Races"][m_ccdData.race].c_str()[0];
		}
		else
		{
			m_curPlayer[22] = 'H';
		}
	}
	else
	{
		m_curPlayer[22] = 'H';
	}
	if (m_resources->m_plistMap["Classes"].size() > m_ccdData.type)
	{
		if (m_resources->m_plistMap["Classes"][m_ccdData.type].size() > 0)
		{
			m_curPlayer[23] = m_resources->m_plistMap["Classes"][m_ccdData.type].c_str()[0];
		}
		else
		{
			m_curPlayer[23] = 'F';
		}
	}
	else
	{
		m_curPlayer[23] = 'F';
	}
	m_curPlayer[PLAYER_STR] = m_ccdData.strength;		// Strength
	m_curPlayer[PLAYER_DEX] = m_ccdData.dexterity;		// Dexterity
	m_curPlayer[PLAYER_INT] = m_ccdData.intelligence;   // Intelligence
	m_curPlayer[PLAYER_WIS] = m_ccdData.wisdom;			// Wisdom

	m_curPlayer[PLAYER_STATUS] = 'G';    // Good Health
	m_curPlayer[27] = 100;    // Current Hit Points
	m_curPlayer[PLAYER_MAX_HP] = 100;    // Max Hit Points
	m_curPlayer[PLAYER_FOOD_1] = 1;
	m_curPlayer[PLAYER_FOOD_2] = 50;     // Food
	m_curPlayer[36] = 150;    // Gold Pieces
	m_curPlayer[41] = 1;      // Cloth
	m_curPlayer[40] = 1;      // pre-readied
	m_curPlayer[49] = 1;      // Dagger
	m_curPlayer[48] = 1;      // pre-readied
	m_misc->PutRoster();

	m_resources->CreatePartyNames();
}

void CreateCharacterDialog::randomNamePushed([[maybe_unused]] int id)
{
	int rngVal;
	if (m_ccdData.sex == 0)
	{
		int maxVal = (int)m_resources->m_plistMap["Male"].size();
		rngVal = m_utilities->getRandom(0, maxVal - 1);
		m_ccdData.name = m_resources->m_plistMap["Male"][rngVal];
	}
	else if(m_ccdData.sex == 1)
	{
		int maxVal = (int)m_resources->m_plistMap["Female"].size();
		rngVal = m_utilities->getRandom(0, maxVal - 1);
		m_ccdData.name = m_resources->m_plistMap["Female"][rngVal];
	}
	else
	{
		int maxVal = (int)m_resources->m_plistMap["Intersex"].size();
		rngVal = m_utilities->getRandom(0, maxVal - 1);
		m_ccdData.name = m_resources->m_plistMap["Intersex"][rngVal];
	}
	if (m_ccdData.name.size() > 12)
	{
		m_ccdData.name.clear();
	}
	m_textBoxes[0]->setText(m_engine_surface, m_font, m_ccdData.name);
}


void CreateCharacterDialog::changeBlockSize(int blockSize)
{
	m_blockSize = blockSize;
	if (m_font)
	{
		TTF_CloseFont(m_font);
		m_font = nullptr;
	}
	createFont();

	m_labels.clear();
	m_buttons.clear();
	m_textBoxes.clear();
	init();
}

void CreateCharacterDialog::renderDisplayString(TTF_Text* text_obj, int x, int y, SDL_Color color)
{
	TTF_SetTextColor(text_obj, color.r, color.g, color.b, 255);
	TTF_DrawRendererText(text_obj, (float)x + screenOffsetX, (float)y + screenOffsetY);
}

void CreateCharacterDialog::HandleInputText(SDL_KeyboardEvent key)
{
	const int maxInputLength = 12;
	SDL_Keycode display_key = SDL_GetKeyFromScancode(key.scancode, key.mod, false);
	if (key.key >= SDLK_A && key.key <= SDLK_Z)
	{
			if (maxInputLength > (int)m_ccdData.name.size())
			{
				m_ccdData.name += (char)display_key;
				m_textBoxes[0]->setText(m_engine_surface, m_font, m_ccdData.name);
			}
	}
	else if (key.key == SDLK_BACKSPACE)
	{
		if (!m_ccdData.name.empty())
		{
			m_ccdData.name.pop_back();
			m_textBoxes[0]->setText(m_engine_surface, m_font, m_ccdData.name);
		}
	}
}

void CreateCharacterDialog::ProcessEvent(SDL_Event event)
{
	switch (event.type)
	{
	case SDL_EVENT_KEY_DOWN:
		if (!(event.key.mod & SDL_KMOD_ALT) && !(event.key.mod & SDL_KMOD_CTRL) && !(event.key.mod & SDL_KMOD_GUI))
		{
			HandleInputText(event.key);
		}
		break;
	default:
		break;
	}
}

bool CreateCharacterDialog::display()
{
	if (m_resources->m_characterRecordWidth == 0 || m_resources->m_characterRecordHeight == 0)
	{
		return false;
	}
	SDL_FRect myRect{};
	SDL_FRect fromRect{};
	float scaler = (float)m_blockSize / 16.0f;
	float ratio = m_resources->m_characterRecordHeight / m_resources->m_characterRecordWidth;

	myRect.x = (float)m_Rect.x * scaler;
	myRect.y = (float)m_Rect.y * scaler;
	myRect.w = (float)m_Rect.w * scaler;
	myRect.h = (float)m_Rect.h * scaler;

	m_resources->adjustRect(myRect);

	SDL_SetRenderDrawColor(m_renderer, 224, 224, 224, 255);
	SDL_RenderFillRect(m_renderer, &myRect);

	myRect.x = (float)m_Rect.x * scaler;
	myRect.y = (float)m_Rect.y * scaler;
	myRect.w = (float)m_Rect.w * scaler;
	myRect.h = myRect.w * ratio;

	m_resources->adjustRect(myRect);

	SDL_RenderTexture(m_renderer, m_resources->m_texCharacterRecord, NULL, &myRect);

	fromRect.x = 0;
	fromRect.y = 34.0f * m_ccdData.race;
	fromRect.w = 400;
	fromRect.h = 34;

	myRect.x = (float)m_Rect.x * scaler;
	myRect.y = ((float)m_Rect.y * scaler) + ((float)m_Rect.h * scaler) - ((myRect.w * (34.0f / 400.0f)) + (myRect.w * (53.0f / 400.0f)));
	myRect.w = (float)m_Rect.w * scaler;
	myRect.h = myRect.w * (34.0f / 400.0f);

	m_resources->adjustRect(myRect);

	SDL_RenderTexture(m_renderer, m_resources->m_texRaceClass, &fromRect, &myRect);

	fromRect.x = 0;
	fromRect.y = 170 + (53.0f * m_ccdData.type);
	fromRect.w = 400;
	fromRect.h = 53;

	myRect.x = (float)m_Rect.x * scaler;
	myRect.y = ((float)m_Rect.y * scaler) + ((float)m_Rect.h * scaler) - ((myRect.w * (53.0f / 400.0f)));
	myRect.w = (float)m_Rect.w * scaler;
	myRect.h = myRect.w * (53.0f / 400.0f);

	m_resources->adjustRect(myRect);

	SDL_RenderTexture(m_renderer, m_resources->m_texRaceClass, &fromRect, &myRect);

	SDL_Color sdl_text_color = { 0, 0, 0 };

	for (auto& curLabel : m_labels)
	{
		renderDisplayString(curLabel->m_ttfLabel, (int)(curLabel->m_x * scaler + m_Rect.x * scaler),
			(int)(curLabel->m_y * scaler + m_Rect.y * scaler), sdl_text_color);
	}

	for (auto& curButton : m_buttons)
	{
		curButton->render(m_renderer, m_blockSize, (int)(curButton->m_x * scaler + m_Rect.x * scaler) + screenOffsetX,
			(int)(curButton->m_y * scaler + m_Rect.y * scaler) + screenOffsetY);
	}

	for (auto& curTextBox : m_textBoxes)
	{
		curTextBox->render(m_renderer, (int)(curTextBox->m_x * scaler + m_Rect.x * scaler) + screenOffsetX,
			(int)(curTextBox->m_y * scaler + m_Rect.y * scaler) + screenOffsetY);
	}

	myRect.x = (float)m_Rect.x * scaler;
	myRect.y = (float)m_Rect.y * scaler;
	myRect.w = (float)m_Rect.w * scaler;
	myRect.h = (float)m_Rect.h * scaler;

	m_resources->adjustRect(myRect);

	SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
	SDL_RenderRect(m_renderer, &myRect);

	SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 0);
	return false;
}

void CreateCharacterDialog::addLabel(std::string strLabel, int x, int y)
{
	auto curLabel = std::make_unique<U3DlgLabel>(m_engine_surface, m_font, strLabel, x, y);
	m_labels.push_back(std::move(curLabel));
}

void CreateCharacterDialog::addButton(std::string strLabel, int x, int y, std::function<void(int)> func)
{
	auto curButton = std::make_unique<U3Button>();
	m_buttons.push_back(std::move(curButton));
	m_buttons.back()->CreateTextButton(m_blockSize, m_renderer, m_engine_surface, m_font, strLabel, x, y);
	m_buttons.back()->setVisible(true);
	m_buttons.back()->SetButtonCallback(func);
}

void CreateCharacterDialog::addTextBox(int x, int y, int width)
{
	auto curTextBox = std::make_unique<U3TextBox>(m_blockSize, x, y, width);
	m_textBoxes.push_back(std::move(curTextBox));
}

bool CreateCharacterDialog::createFont()
{
	bool retVal = false;
	std::filesystem::path currentPath = m_resources->m_exePath;
	currentPath /= ResourceLoc;
	currentPath /= FontLoc;
	currentPath /= m_resources->m_preferences.font;

	if (m_font)
	{
		TTF_CloseFont(m_font);
		m_font = nullptr;
	}

	if (m_blockSize > 8)
	{
		m_font = TTF_OpenFont(currentPath.string().c_str(), (float)m_blockSize * 0.875f);
		if (m_font)
		{
			retVal = true;
		}
	}

	return retVal;
}

void CreateCharacterDialog::updateDialog(float xPos, float yPos, int mouseState)
{
	float scaler = (float)m_blockSize / 16.0f;

	for (auto& curButton : m_buttons)
	{
		float butOffsetX = (float)(curButton->m_x * scaler + m_Rect.x * scaler);
		float butOffsetY = (float)(curButton->m_y * scaler + m_Rect.y * scaler);
		if (mouseState == 2) // mouse up
		{
			curButton->setMouseCapture(m_blockSize, 2, xPos, yPos,
				butOffsetX, butOffsetY, screenOffsetX, screenOffsetY);
		}
		else if (mouseState == 1) // mouse down
		{
			curButton->setMouseCapture(m_blockSize, 1, xPos, yPos,
				butOffsetX, butOffsetY, screenOffsetX, screenOffsetY);
		}
		else // mouse move
		{
			curButton->setMouseCapture(m_blockSize, 0, xPos, yPos,
				butOffsetX, butOffsetY, screenOffsetX, screenOffsetY);
		}
	}
}
