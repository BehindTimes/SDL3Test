#include <SDL_image.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_timer.h>
#include <iostream>

#include "U3Resources.h"

extern short screenOffsetX;
extern short screenOffsetY;
extern SDL_Window* window;
extern TTF_TextEngine* engine_surface;

constexpr int FONT_NUM_X = 96;
constexpr int FONT_NUM_Y = 1;

constexpr int TILES_NUM_X = 12;
constexpr int TILES_NUM_Y = 16;

U3Resources::U3Resources() :
	m_renderer(nullptr),
	m_currentGraphics(nullptr),
	m_standardGraphics(nullptr),
	m_defaultMode(0),
	m_texStalagtites(nullptr),
	m_texExodus(nullptr),
	m_texUltimaLogo(nullptr),
	m_texBy(nullptr),
	m_texCredits(nullptr),
	m_texExodusFade(nullptr),
	m_texUltimaLogoFade(nullptr),
	m_exodusWidth(0),
	m_exodusHeight(0),
	m_ultimaLogoWidth(0),
	m_ultimaLogoHeight(0),
	m_blockSize(32),
	m_font(nullptr),
	m_font_11(nullptr),
	m_demoptr(0),
	m_curTickScroll(0),
	m_elapsedTimeScroll(0),
	m_curTickDemo(0),
	m_elapsedTimeDemo(0),
	m_TileArray(0),
	m_demoDelay(0),
	m_elapsedTimeFlag(0),
	m_elapsedTimeAnimate(0),
	m_numUpdateFlag(0),
	m_numUpdateAnimate(0),
	m_xPos(0),
	m_yPos(0),
	m_cleanupAlert(false)
{
	memset(m_texIntro, NULL, sizeof(m_texIntro));
	memset(m_shapeSwap, 0, sizeof(bool) * 256);
	m_animFlag[0] = 0;
	m_animFlag[1] = 16;
	m_animFlag[2] = 0;
	m_animFlag[3] = 5;

	m_twiddleFlag[0] = 0;
	m_twiddleFlag[1] = 3;
	m_twiddleFlag[2] = 2;
	m_twiddleFlag[3] = 1;

	memset(m_Player, NULL, sizeof(char) * (21 * 65));
}

U3Resources::~U3Resources()
{
	m_AlertDlg.reset();
	m_buttons.clear();

	for (auto mode : m_allGraphics)
	{
		if (mode.second.font)
		{
			SDL_DestroyTexture(mode.second.font);
		}
		if (mode.second.mini)
		{
			SDL_DestroyTexture(mode.second.mini);
		}
		if (mode.second.ui)
		{
			SDL_DestroyTexture(mode.second.ui);
		}

		for (auto& curTex : mode.second.tiles)
		{
			SDL_DestroyTexture(curTex);
		}
		for (auto& curTex : mode.second.tile_target)
		{
			SDL_DestroyTexture(curTex);
		}
	}

	if (m_texStalagtites)
	{
		SDL_DestroyTexture(m_texStalagtites);
	}

	for (int index = 0; index < 5; ++index)
	{
		if (m_texIntro[index])
		{
			SDL_DestroyTexture(m_texIntro[index]);
		}
	}

	if (m_texExodus)
	{
		SDL_DestroyTexture(m_texExodus);
	}
	if (m_texUltimaLogo)
	{
		SDL_DestroyTexture(m_texUltimaLogo);
	}
	if (m_texBy)
	{
		SDL_DestroyTexture(m_texBy);
	}
	if (m_texCredits)
	{
		SDL_DestroyTexture(m_texCredits);
	}
	if (m_texExodusFade)
	{
		SDL_DestroyTexture(m_texExodusFade);
	}
	if (m_texUltimaLogoFade)
	{
		SDL_DestroyTexture(m_texUltimaLogoFade);
	}

	if (m_font)
	{
		TTF_CloseFont(m_font);
	}
	if (m_font_11)
	{
		TTF_CloseFont(m_font_11);
	}

	TTF_Quit();
}

void U3Resources::m_AlertCallback()
{
	m_cleanupAlert = true;
}

void U3Resources::CreateAlertMessage(int message)
{
	m_AlertDlg = std::make_unique<U3Dialog>(m_renderer, engine_surface, &m_currentGraphics, &m_standardGraphics,
		m_blockSize, message, std::bind(&U3Resources::m_AlertCallback, this));
}

void U3Resources::SetPreference(U3PreferencesType type, bool value)
{
	switch (type)
	{
	case U3PreferencesType::Classic_Appearance:
		m_preferences.classic_appearance = value;
		break;
	default:
		break;
	}
}

void U3Resources::setTickCount(Uint64 curTick, bool initializeTimer)
{
	m_curTickScroll = curTick;
	m_curTickDemo = curTick;
	if (initializeTimer)
	{
		m_elapsedTimeScroll = 0;
		m_elapsedTimeDemo = 0;
		m_elapsedTimeFlag = 0;
		m_elapsedTimeAnimate = 0;
		m_demoDelay = 0;
	}
}

bool U3Resources::init(SDL_Renderer* renderer)
{
	m_renderer = renderer;
	if (!loadFont())
	{
		return false;
	}
	loadGraphics();
	loadImages();
	if (!loadPLists())
	{
		return false;
	}
	loadButtons();
	loadDemo();
	if (!loadSaveGame())
	{
		return false;
	}

	if (m_allGraphics.find(std::string(Standard)) != m_allGraphics.end())
	{
		m_standardGraphics = &m_allGraphics[std::string(Standard)];
		m_currentGraphics = m_standardGraphics; // 6
	}
	//changeMode(1);
	return true;
}

bool U3Resources::loadRoster(std::filesystem::path rosterPath)
{
	short player, byte;
	const size_t roster_size = 1280;
	unsigned char roster_data[roster_size];
	std::uintmax_t file_size = std::filesystem::file_size(rosterPath);

	if (file_size != roster_size)
	{
		return false;
	}
	SDL_IOStream* file = SDL_IOFromFile(rosterPath.string().c_str(), "rb");
	if (!file)
	{
		return false;
	}
	SDL_ReadIO(file, roster_data, roster_size);
	SDL_CloseIO(file);

	for (player = 0; player < 20; player++)
	{
		for (byte = 0; byte < 64; byte++)
		{
			m_Player[player + 1][byte] = *(roster_data + ((player) * 64) + byte);
		}
	}

	return true;
}

bool U3Resources::loadParty(std::filesystem::path partyPath)
{
	const size_t party_size = 64;
	std::uintmax_t file_size = std::filesystem::file_size(partyPath);

	if (file_size != party_size)
	{
		return false;
	}
	SDL_IOStream* file = SDL_IOFromFile(partyPath.string().c_str(), "rb");
	if (!file)
	{
		return false;
	}
	SDL_ReadIO(file, m_Party, party_size);
	SDL_CloseIO(file);

	return true;
}

bool U3Resources::loadSaveGame()
{
	std::filesystem::path currentPath = std::filesystem::current_path();
	currentPath /= ResourceLoc;
	currentPath /= SaveLoc;

	std::filesystem::path partyPath = currentPath / "PARTY.ULT";
	std::filesystem::path rosterPath = currentPath / "ROSTER.ULT";
	std::filesystem::path sosariaPath = currentPath / "SOSARIA.ULT";

	try
	{
		if (!std::filesystem::is_directory(currentPath))
		{

			bool valid = std::filesystem::create_directory(currentPath);
			if (!valid)
			{
				return false;
			}

		}
		if (std::filesystem::exists(rosterPath))
		{
			loadRoster(rosterPath);
		}
		if (std::filesystem::exists(partyPath))
		{
			loadParty(partyPath);
		}
	}
	catch (std::exception)
	{
		return false;
	}
	return true;
}

void U3Resources::loadDemo()
{
	std::filesystem::path currentPath = std::filesystem::current_path();
	currentPath /= ResourceLoc;
	currentPath /= BinLoc;
	currentPath /= "MainResources.rsrc_DEMO_400_WhereWhatHow.bin";

	SDL_IOStream* file = SDL_IOFromFile(currentPath.string().c_str(), "rb");
	if (!file)
	{
		return;
	}
	Sint64 fSize = SDL_GetIOSize(file);
	if (fSize > 0)
	{
		m_demoData.resize(fSize);
		SDL_ReadIO(file, m_demoData.data(), fSize);
	}
	SDL_CloseIO(file);

	memcpy(m_TileArray, m_demoData.data() + 1024, sizeof(unsigned char) * 128);
	memcpy(m_demoBgndTiles, m_TileArray, sizeof(unsigned char) * 114);
	m_demoBgndTiles[62] = 0;    // ship should be water.
}

void U3Resources::SetButtonCallback(short button, std::function<void()> func)
{
	if (m_buttons.size() > button && button >= 0)
	{
		m_buttons[button].SetButtonCallback(func);
	}
}

bool U3Resources::loadFont()
{
	if (!TTF_Init())
	{
		return false;
	}

	if (!engine_surface)
	{
		engine_surface = TTF_CreateRendererTextEngine(m_renderer);
	}

	return createFont();
}

bool U3Resources::createFont()
{
	float scaler = (float)m_blockSize / 16.0f;

	std::filesystem::path currentPath = std::filesystem::current_path();
	currentPath /= ResourceLoc;
	currentPath /= FontLoc;
	currentPath /= "FreeSerif.ttf";

	m_font = TTF_OpenFont(currentPath.string().c_str(), (float)m_blockSize);
	if (!m_font)
	{
		return false;
	}

	m_font_11 = TTF_OpenFont(currentPath.string().c_str(), 11.0f * scaler);
	if (!m_font_11)
	{
		return false;
	}

	return true;
}

void U3Resources::CalculateBlockSize()
{
	int windowWidth, windowHeight;
	int width, height;

	SDL_GetWindowSize(window, &windowWidth, &windowHeight);

	width = (windowWidth / 40);
	height = (windowHeight / 24);

	if (height < width)
	{
		m_blockSize = height;
	}
	else
	{
		m_blockSize = width;
	}

	screenOffsetX = (windowWidth - m_blockSize * 40) / 2;
	screenOffsetY = (windowHeight - m_blockSize * 24) / 2;

	if (m_AlertDlg.get())
	{
		m_AlertDlg->changeBlockSize(m_blockSize);
	}

	if (m_font)
	{
		TTF_CloseFont(m_font);
		m_font = nullptr;
	}
	if (m_font_11)
	{
		TTF_CloseFont(m_font_11);
		m_font_11 = nullptr;
	}
	createFont();
}


void U3Resources::changeMode(int mode)
{
	if (mode >= 0 && mode < m_modes.size())
	{
		std::string strMode = m_modes[mode];
		if (m_allGraphics.find(strMode) != m_allGraphics.end())
		{
			m_currentGraphics = &m_allGraphics[strMode];
		}
	}
}

xmlNodePtr U3Resources::findNodeByName(xmlNodePtr rootnode, const xmlChar* nodename)
{
	xmlNodePtr node = rootnode;
	if (node == nullptr)
	{
		return nullptr;
	}

	while (node != nullptr)
	{

		if (!xmlStrcmp(node->name, nodename))
		{
			return node;
		}
		else if (node->children != nullptr) {
			node = node->children;
			xmlNodePtr intNode = findNodeByName(node, nodename);
			if (intNode != nullptr) {
				return intNode;
			}
		}
		node = node->next;
	}
	return nullptr;
}

void U3Resources::processDoc(xmlDocPtr docPtr, std::vector<std::string >& curVec )
{
	xmlNode* root_element = nullptr;
	root_element = xmlDocGetRootElement(docPtr);
	if (nullptr == root_element) // something's wrong
	{
		return;
	}
	xmlNodePtr nodeArray = findNodeByName(root_element, (xmlChar*)"array");
	if (nodeArray != nullptr)
	{
		xmlNode* childNode = nodeArray->children;
		while (childNode != nullptr)
		{
			if (childNode->type == xmlElementType::XML_ELEMENT_NODE)
			{
				if (childNode->children)
				{
					xmlNodePtr stringNode = findNodeByName(childNode, (xmlChar*)"string");
					if (stringNode != nullptr)
					{
						xmlChar* data = xmlNodeGetContent(stringNode);
						if (data)
						{
							
							curVec.emplace_back(reinterpret_cast<const char*>(data));
						}
					}
				}
			}
			childNode = childNode->next;
		}
	}
}

bool U3Resources::loadPLists()
{
	LIBXML_TEST_VERSION

	bool valid = true;
	xmlDocPtr docPtr = nullptr;
	std::filesystem::path currentPath = std::filesystem::current_path();
	currentPath /= ResourceLoc;
	currentPath /= PListLoc;

	xmlTextReaderPtr readerPtr = nullptr;

	for (const std::filesystem::directory_entry& dirEntry : std::filesystem::recursive_directory_iterator(currentPath))
	{
		if (dirEntry.path().extension() != ".plist")
		{
			continue;
		}
		docPtr = xmlReadFile(dirEntry.path().string().c_str(), NULL, 0);
		
		if (docPtr == nullptr)
		{
			continue;
		}
		std::string plistName = dirEntry.path().stem().string();
		m_plistMap.insert(std::make_pair(plistName, std::vector<std::string>()));

		processDoc(docPtr, m_plistMap[plistName]);
		xmlFreeDoc(docPtr);
	}

	// Cleanup function for the XML library.
	xmlCleanupParser();

	return valid;
}

void U3Resources::loadButtons()
{
	// all nums for standard size (blkSiz=16).  All buttons are actually 4x this size, however.
	short butLeft[] = { 0, 0, 0, 100, 100, 100, 100, 100, 157 };
	short butRight[] = { 100, 100, 100, 272, 272, 272, 272, 153, 257 };
	short butTop[] = { 0, 200, 400, 0, 75, 150, 225, 300, 400 };
	short butBottom[] = { 100, 300, 500, 25, 100, 175, 250, 325, 500 };
	bool hasClick[] = { true, true, true, true, true, true, true, true, true };
	bool hasDisabled[] = { false, false, false, true, true, true, true, false, false };

	m_buttons.resize(9);

	std::filesystem::path currentPath = std::filesystem::current_path();
	currentPath /= ResourceLoc;
	currentPath /= ImagesLoc;
	currentPath /= "Buttons.png";

	SDL_Texture* buttonImage = IMG_LoadTexture(m_renderer, currentPath.string().c_str());

	for (size_t index = 0; index < 9; ++index)
	{
		m_buttons[index].setRect(m_renderer, buttonImage, butLeft[index], butTop[index],
			butRight[index] - butLeft[index], butBottom[index] - butTop[index],
			hasClick[index], hasDisabled[index]);
	}

	if (buttonImage)
	{
		SDL_DestroyTexture(buttonImage);
	}
}

void U3Resources::loadImages()
{
	float w, h;

	std::filesystem::path currentPath = std::filesystem::current_path();
	currentPath /= ResourceLoc;
	currentPath /= ImagesLoc;
	currentPath /= "Stalagtites.png";

	m_texStalagtites = IMG_LoadTexture(m_renderer, currentPath.string().c_str());
	SDL_SetTextureScaleMode(m_texStalagtites, SDL_SCALEMODE_NEAREST);

	std::string introNames[] = { "Opening_Anim_1.png", "Opening_Anim_2.png" , "Opening_Anim_3.png" , "Opening_Anim_4.png" , "Opening_Anim_5.png" };
	for (int index = 0; index < 5; ++index)
	{
		currentPath = std::filesystem::current_path();
		currentPath /= ResourceLoc;
		currentPath /= ImagesLoc;
		currentPath /= introNames[index];

		m_texIntro[index] = IMG_LoadTexture(m_renderer, currentPath.string().c_str());
	}

	currentPath = std::filesystem::current_path();
	currentPath /= ResourceLoc;
	currentPath /= ImagesLoc;
	currentPath /= "Exodus.png";

	m_texExodus = IMG_LoadTexture(m_renderer, currentPath.string().c_str());

	SDL_GetTextureSize(m_texExodus, &w, &h);
	m_exodusWidth = (int)w;
	m_exodusHeight = (int)h;

	m_texExodusFade = SDL_CreateTexture(m_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, m_exodusWidth, m_exodusHeight);
	unsigned char* pixels = NULL;
	int pitch;
	SDL_LockTexture(m_texExodusFade, NULL, (void**)&pixels, &pitch);
	memset(pixels, 0, sizeof(char) * pitch * m_exodusHeight);
	SDL_UnlockTexture(m_texUltimaLogoFade);

	currentPath = std::filesystem::current_path();
	currentPath /= ResourceLoc;
	currentPath /= ImagesLoc;
	currentPath /= "UltimaLogo.png";

	m_texUltimaLogo = IMG_LoadTexture(m_renderer, currentPath.string().c_str());

	SDL_GetTextureSize(m_texUltimaLogo, &w, &h);
	m_ultimaLogoWidth = (int)w;
	m_ultimaLogoHeight = (int)h;

	m_texUltimaLogoFade = SDL_CreateTexture(m_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, m_ultimaLogoWidth, m_ultimaLogoHeight);

	SDL_LockTexture(m_texUltimaLogoFade, NULL, (void**)&pixels, &pitch);
	memset(pixels, 0, sizeof(char) * pitch * m_ultimaLogoHeight);
	SDL_UnlockTexture(m_texUltimaLogoFade);

	currentPath = std::filesystem::current_path();
	currentPath /= ResourceLoc;
	currentPath /= ImagesLoc;
	currentPath /= "By.png";

	m_texBy = IMG_LoadTexture(m_renderer, currentPath.string().c_str());

	currentPath = std::filesystem::current_path();
	currentPath /= ResourceLoc;
	currentPath /= ImagesLoc;
	currentPath /= "Credits.png";

	m_texCredits = IMG_LoadTexture(m_renderer, currentPath.string().c_str());

}

void U3Resources::GetTileRectForIndex(SDL_Texture* curTexture, int tileNum, SDL_FRect& myRect, float tileXSize, float tileYSize)
{
	int yPos = tileNum % TILES_NUM_Y;
	int xPos = (tileNum / TILES_NUM_Y);
	myRect.x = xPos * tileXSize;
	myRect.y = yPos * tileYSize;
	myRect.w = tileXSize;
	myRect.h = tileYSize;
}

void U3Resources::loadTiles(ModeGraphics& curGraphics, std::string strFile)
{
	float xSize;
	float ySize;

	curGraphics.tiles.resize(TILES_NUM_X * TILES_NUM_Y);
	curGraphics.tile_target.resize(TILES_NUM_X * TILES_NUM_Y);

	SDL_Texture* curTexture = nullptr;

	curTexture = IMG_LoadTexture(m_renderer, strFile.c_str());
	SDL_SetTextureScaleMode(curTexture, SDL_SCALEMODE_NEAREST);
	SDL_GetTextureSize(curTexture, &xSize, &ySize);
	float tileXSize = xSize / TILES_NUM_X;
	float tileYSize = ySize / TILES_NUM_Y;

	//m_texExodusFade = SDL_CreateTexture(m_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, m_exodusWidth, m_exodusHeight);
	// Forcing the tiles to be 128x128.  This will allow for consistent smooth scrolling on super small tiles.
	curGraphics.tiles_width = 128;
	curGraphics.tiles_height = 128;
	for (size_t index = 0; index < TILES_NUM_X * TILES_NUM_Y; ++index)
	{
		curGraphics.tiles[index] = SDL_CreateTexture(m_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 128, 128);
		curGraphics.tile_target[index] = SDL_CreateTexture(m_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 128, 128);
		SDL_SetTextureScaleMode(curGraphics.tiles[index], SDL_SCALEMODE_NEAREST);
		SDL_SetTextureScaleMode(curGraphics.tile_target[index], SDL_SCALEMODE_NEAREST);
	}

	int curVecPos = 0;
	SDL_FRect frameRect;

	for (int indexX = 0; indexX < TILES_NUM_X; ++indexX)
	{
		for (int indexY = 0; indexY < TILES_NUM_Y; ++indexY)
		{
			GetTileRectForIndex(curTexture, curVecPos, frameRect, tileXSize, tileYSize);
			SDL_SetRenderTarget(m_renderer, curGraphics.tiles[curVecPos]);
			SDL_RenderTexture(m_renderer, curTexture, &frameRect, NULL);
			SDL_SetRenderTarget(m_renderer, curGraphics.tile_target[curVecPos]);
			SDL_RenderTexture(m_renderer, curTexture, &frameRect, NULL);

			curVecPos++;
		}
	}

	SDL_SetRenderTarget(m_renderer, NULL);

	/*SDL_SetRenderTarget(m_renderer, *copyTexture);
					SDL_RenderTexture(m_renderer, *curTexture, NULL, NULL);
					SDL_SetRenderTarget(m_renderer, NULL);

					SDL_SetTextureScaleMode(*copyTexture, SDL_SCALEMODE_NEAREST);
					*/


	if (curTexture)
	{
		SDL_DestroyTexture(curTexture);
	}
}

void U3Resources::loadGraphics()
{
	std::filesystem::path currentPath = std::filesystem::current_path();
	currentPath /= ResourceLoc;
	currentPath /= GraphicsLoc;

	for (const std::filesystem::directory_entry& dirEntry : std::filesystem::recursive_directory_iterator(currentPath))
	{
		if (dirEntry.path().extension() != ".png" && dirEntry.path().extension() != ".gif")
		{
			continue;
		}
		std::string strStem = dirEntry.path().stem().string();

		size_t pos = strStem.find_last_of("-");
		if (pos == strStem.npos)
		{
			continue;
		}
		std::string mode = strStem.substr(0, pos);
		std::string type = strStem.substr(pos + 1);
		transform(type.begin(), type.end(), type.begin(), ::tolower);

		SDL_Texture** curTexture = nullptr;

		if (m_allGraphics.find(mode) == m_allGraphics.end())
		{
			m_allGraphics.emplace(mode, ModeGraphics());
			m_modes.emplace_back(mode);

			if (mode == Standard)
			{
				m_defaultMode = m_modes.size() - 1;
			}
		}
		int numX = 0;
		int numY = 0;
		float* xSize = nullptr;
		float* ySize = nullptr;
		if (type == "font")
		{
			curTexture = &m_allGraphics[mode].font;
			numX = FONT_NUM_X;
			numY = FONT_NUM_Y;
			xSize = &m_allGraphics[mode].font_width;
			ySize = &m_allGraphics[mode].font_height;
		}
		else if (type == "mini")
		{
			curTexture = &m_allGraphics[mode].mini;
		}
		else if (type == "tiles")
		{
			loadTiles(m_allGraphics[mode], dirEntry.path().string());
		}
		else if (type == "ui")
		{
			curTexture = &m_allGraphics[mode].ui;
			numX = UI_NUM_X;
			numY = UI_NUM_Y;
			xSize = &m_allGraphics[mode].ui_width;
			ySize = &m_allGraphics[mode].ui_height;
		}
		else
		{
			continue; // Wrong type
		}

		if (curTexture != nullptr)
		{
			*curTexture = IMG_LoadTexture(m_renderer, dirEntry.path().string().c_str());

			SDL_SetTextureScaleMode(*curTexture, SDL_SCALEMODE_NEAREST);

			if (numX > 0 && numY > 0)
			{
				SDL_FPoint size;
				SDL_GetTextureSize(*curTexture, &size.x, &size.y);
				*xSize = size.x / numX;
				*ySize = size.y / numY;

				/*if (copyTexture) // A backup texture, so we can render on the original
				{
					*copyTexture = SDL_CreateTexture(m_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, (int)size.x, (int)size.y);
					SDL_SetRenderTarget(m_renderer, *copyTexture);
					SDL_RenderTexture(m_renderer, *curTexture, NULL, NULL);
					SDL_SetRenderTarget(m_renderer, NULL);

					SDL_SetTextureScaleMode(*copyTexture, SDL_SCALEMODE_NEAREST);
				}*/
			}
		}
	}
}

void U3Resources::adjustRect(SDL_FRect& myRect)
{
	myRect.x += screenOffsetX;
	myRect.y += screenOffsetY;
}

void U3Resources::renderString(std::string curString, int x, int y)
{
	SDL_FRect frameRect, myRect;
	
	if (m_preferences.classic_appearance)
	{
		for (size_t index = 0; index < curString.size(); ++index)
		{
			int part = curString.c_str()[index];
			part -= 32;
			if (part < 0 || part > 96)
			{
				continue;
			}

			myRect.x = (float)(x * m_blockSize) + (m_blockSize * index);
			myRect.y = (float)(y * m_blockSize);
			myRect.w = (float)(m_blockSize);
			myRect.h = (float)(m_blockSize);

			adjustRect(myRect);

			int xPart = part % FONT_NUM_X;
			int yPart = part / FONT_NUM_X;


			if (m_currentGraphics->font)
			{
				frameRect.x = (float)(m_currentGraphics->font_width * xPart);
				frameRect.y = (float)(m_currentGraphics->font_height * yPart);
				frameRect.w = m_currentGraphics->font_width;
				frameRect.h = m_currentGraphics->font_height;

				SDL_RenderTexture(m_renderer, m_currentGraphics->font, &frameRect, &myRect);
			}
			else // fallback for missing graphics
			{
				frameRect.x = (float)(m_standardGraphics->font_width * xPart);
				frameRect.y = (float)(m_standardGraphics->font_height * yPart);
				frameRect.w = m_standardGraphics->font_width;
				frameRect.h = m_standardGraphics->font_height;

				SDL_RenderTexture(m_renderer, m_standardGraphics->font, &frameRect, &myRect);
			}
		}
	}
	else
	{
		TTF_Text* text_obj = NULL;
		text_obj = TTF_CreateText(engine_surface, m_font_11, curString.c_str(), 0);
		int w, h;
		TTF_GetTextSize(text_obj, &w, &h);

		//screenOffsetX

		size_t totalLen = m_blockSize * curString.size();
		size_t tempOffset = (totalLen - w) / 2;

		TTF_DrawRendererText(text_obj, (float)x * m_blockSize  + screenOffsetX + tempOffset, (float)y * m_blockSize + screenOffsetY);

		if (text_obj)
		{
			TTF_DestroyText(text_obj);
			text_obj = NULL;
		}
	}
}

void U3Resources::renderDisplayString(TTF_Font* font, std::string curString, int x, int y, SDL_Color color, int align)
{
	TTF_Text* text_obj = NULL;
	text_obj = TTF_CreateText(engine_surface, font, curString.c_str(), 0);

	TTF_SetTextColor(text_obj, color.r, color.g, color.b, 255);

	int w, h;
	int offsetW = 0;
	TTF_GetTextSize(text_obj, &w, &h);

	if (align == 1)
	{
		offsetW = w * -1;
	}

	TTF_DrawRendererText(text_obj, (float)x + screenOffsetX + offsetW, (float)y + screenOffsetY);

	if (text_obj)
	{
		TTF_DestroyText(text_obj);
		text_obj = NULL;
	}
}

void U3Resources::renderUI(int part, int x, int y)
{
	SDL_FRect frameRect, myRect;

	myRect.x = (float)(x * m_blockSize);
	myRect.y = (float)(y * m_blockSize);
	myRect.w = (float)(m_blockSize);
	myRect.h = (float)(m_blockSize);

	adjustRect(myRect);

	int xPart = part % UI_NUM_X;
	int yPart = part / UI_NUM_X;

	if (m_currentGraphics->ui)
	{
		frameRect.x = (float)(m_currentGraphics->ui_width * xPart);
		frameRect.y = (float)(m_currentGraphics->ui_height * yPart);
		frameRect.w = m_currentGraphics->ui_width;
		frameRect.h = m_currentGraphics->ui_height;

		SDL_RenderTexture(m_renderer, m_currentGraphics->ui, &frameRect, &myRect);
	}
	else // fallback for missing graphics
	{
		frameRect.x = (float)(m_standardGraphics->ui_width * xPart);
		frameRect.y = (float)(m_standardGraphics->ui_height * yPart);
		frameRect.w = m_standardGraphics->ui_width;
		frameRect.h = m_standardGraphics->ui_height;

		SDL_RenderTexture(m_renderer, m_standardGraphics->ui, &frameRect, &myRect);
	}
}

void U3Resources::renderStalagtites()
{
	drawImage(m_texStalagtites, (float)m_blockSize, m_blockSize * 19.0f, m_blockSize * 38.0f, m_blockSize * 2.125f);
}

void U3Resources::drawIntro(int shape, int offset)
{
	SDL_FRect myRect;

	float scaler = (float)m_blockSize / 16.0f;

	myRect.x = (192 + offset) * scaler;
	myRect.y = 335 * scaler;
	myRect.w = (255 * scaler);
	myRect.h = (32 * scaler);

	adjustRect(myRect);

	if (shape >= 0 && shape < 5)
	{
		SDL_RenderTexture(m_renderer, m_texIntro[shape], NULL, &myRect);
	}
}

void U3Resources::drawImage(SDL_Texture* texture, float x, float y, float width, float height)
{
	if (!texture)
	{
		return;
	}

	SDL_FRect myRect;

	myRect.x = x;
	myRect.y = y;
	myRect.w = width;
	myRect.h = height;

	adjustRect(myRect);

	SDL_RenderTexture(m_renderer, texture, NULL, &myRect);
}

void U3Resources::drawExodus(Uint8 alpha)
{
	if (alpha == 0)
	{
		return;
	}

	//SDL_SetTextureBlendMode(m_texExodus, SDL_BLENDMODE_BLEND);
	//SDL_SetTextureAlphaMod(m_texExodus, alpha);

	drawImage(m_texExodus, m_blockSize * 5.25f, m_blockSize * 1.25f, m_blockSize * 29.5f, m_blockSize * 8.375f);

	unsigned char* pixels = NULL;
	int pitch;
	bool works = SDL_LockTexture(m_texExodusFade, NULL, (void**)&pixels, &pitch);
	for (int indexY = 0; indexY < m_exodusHeight; ++indexY)
	{
		for (int indexX = 0; indexX < m_exodusWidth * 4; indexX += 4)
		{
			int display = SDL_rand(255);

			pixels[(indexY * pitch) + indexX] = (display > alpha) ? 255 : 0;
		}
	}
	SDL_UnlockTexture(m_texExodusFade);

	drawImage(m_texExodusFade, m_blockSize * 5.25f, m_blockSize * 1.25f, m_blockSize * 29.5f, m_blockSize * 8.375f);
}

void U3Resources::drawUltimaLogo(Uint8 alpha)
{
	if (alpha == 0)
	{
		return;
	}
	//SDL_SetRenderDrawBlendMode(m_renderer, SDL_BLENDMODE_BLEND);
	//SDL_SetTextureBlendMode(m_texUltimaLogo, SDL_BLENDMODE_BLEND);
	//SDL_SetTextureAlphaMod(m_texUltimaLogo, alpha);

	drawImage(m_texUltimaLogo, m_blockSize * 7.25f, m_blockSize * 9.9375f, m_blockSize * 25.5f, m_blockSize * 5.625f);

	unsigned char* pixels = NULL;
	int pitch;
	bool works = SDL_LockTexture(m_texUltimaLogoFade, NULL, (void**)&pixels, &pitch);
	for (int indexY = 0; indexY < m_ultimaLogoHeight; ++indexY)
	{
		for (int indexX = 0; indexX < m_ultimaLogoWidth * 4; indexX += 4)
		{
			int display = SDL_rand(255);

			pixels[(indexY * pitch) + indexX] = (display > alpha) ? 255 : 0;
		}
	}
	SDL_UnlockTexture(m_texUltimaLogoFade);

	drawImage(m_texUltimaLogoFade, m_blockSize * 7.25f, m_blockSize * 9.9375f, m_blockSize * 25.5f, m_blockSize * 5.625f);
}

void U3Resources::drawBy()
{
	drawImage(m_texBy, m_blockSize * 11.25f, m_blockSize * 15.75f, m_blockSize * 1.625f, m_blockSize * 1.4375f);
}

void U3Resources::loadSignatureData()
{
	std::filesystem::path currentPath = std::filesystem::current_path();
	currentPath /= ResourceLoc;
	currentPath /= BinLoc;
	currentPath /= "MainResources.rsrc_SGNT_400_Signature Data.bin";

	SDL_IOStream* file = SDL_IOFromFile(currentPath.string().c_str(), "rb");
	if (!file)
	{
		return;
	}
	Sint64 fSize = SDL_GetIOSize(file);
	if (fSize > 0)
	{
		m_vecSigData.resize(fSize);
		SDL_ReadIO(file, m_vecSigData.data(), fSize);
	}
	//
	SDL_CloseIO(file);
}

void U3Resources::PlotSig(int x, int y)
{
	float sScaler = m_blockSize / 16.0f;

	x += 244;
	y += 253;
	x = (int)truncf((float)x * sScaler);
	y = (int)truncf((float)y * sScaler);

	SDL_FRect myRect;

	myRect.x = (float)x;
	myRect.y = (float)y;
	myRect.w = (float)(m_blockSize >> 2) + 0.6f; // floating point resolution issues which could leave a line of pixels uncovered.
	myRect.h = (float)(m_blockSize >> 3) + 0.6f;

	SDL_RenderFillRect(m_renderer, &myRect);
}

void U3Resources::WriteLordBritish(Uint64 curPass)
{
	bool done = false;
	size_t offset = 0;
	int cx = 0;
	int cy = 0;
	unsigned char value;

	SDL_SetRenderDrawColor(m_renderer, 255, 255, 255, 255);

	while (!done)
	{
		PlotSig(cx * 2, cy * 2);
		if (offset >= m_vecSigData.size())
		{
			done = true;
			break;
		}
		value = m_vecSigData[offset];
		offset++;

		if (curPass < offset)
		{
			done = true;
			break;
		}

		switch (value)
		{
		case 0:
			PlotSig((cx * 2 - 1), (cy * 2 - 1));
			cx--;
			cy--;
			break;
		case 1:
			PlotSig(cx * 2, (cy * 2 - 1));
			cy--;
			break;
		case 2:
			PlotSig((cx * 2 + 1), (cy * 2 - 1));
			cx++;
			cy--;
			break;
		case 3:
			PlotSig((cx * 2 - 1), cy * 2);
			cx--;
			break;
		case 4:
			PlotSig((cx * 2 + 1), cy * 2);
			cx++;
			break;
		case 5:
			PlotSig((cx * 2 - 1), (cy * 2 + 1));
			cx--;
			cy++;
			break;
		case 6:
			PlotSig(cx * 2, (cy * 2 + 1));
			cy++;
			break;
		case 7:
			PlotSig((cx * 2 + 1), (cy * 2 + 1));
			cx++;
			cy++;
			break;
		case 8:
			cx = (signed char)m_vecSigData[offset];
			cy = (signed char)m_vecSigData[offset + 1];

			if (cy > 250)
			{
				cy = -1;
			}
			offset += 2;
			break;
		case 255:
			done = true;
			break;
		default:
			done = true;
			break;
		}
	}
	SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 0);
}

void U3Resources::DrawCredits()
{
	drawImage(m_texCredits, (float)m_blockSize, m_blockSize * 17.5625f, m_blockSize * 38.0f, m_blockSize * 1.4375f);
}

void U3Resources::CenterMessage(short which, short y)
{
	SDL_FRect myRect;

	which--;
	if (m_plistMap.contains("MoreMessages"))
	{
		if (m_plistMap["MoreMessages"].size() > which)
		{
			std::string dispString = m_plistMap["MoreMessages"][which];

			myRect.x = (20.0f - (dispString.size() / 2.0f)) * m_blockSize;
			myRect.y = (float)y * m_blockSize;
			myRect.h = (float)m_blockSize;
			myRect.w = dispString.size() * (float)m_blockSize;

			adjustRect(myRect);

			SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
			SDL_RenderFillRect(m_renderer, &myRect);
			SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 0);

			renderString(dispString, (int)(20.0f - (dispString.size() / 2.0f)), y);
		}
	}
}

void U3Resources::SetButtonVisibility(short button, bool visible)
{
	if (button >= 0 && m_buttons.size() > button)
	{
		m_buttons[button].setVisible(visible);
	}
}

void U3Resources::DrawButton(short butNum)
{
	short butOffsetX[] = { 67, 202, 473, 26, 205, 384, 384, 563, 338 };
	short butOffsetY[] = { 219, 219, 219, 200, 200, 200, 200, 200, 219 };

	if (m_buttons.size() > butNum && butNum >= 0)
	{
		float mult = m_blockSize / 64.0f;
		m_buttons[butNum].render(m_renderer, m_blockSize, (int)(butOffsetX[butNum] * mult), (int)(butOffsetY[butNum] * mult), screenOffsetX, screenOffsetY);
	}
}

void U3Resources::DrawButtons(std::vector<short> buttons)
{
	m_currentButtons = buttons;
	for (short curButton : buttons)
	{
		DrawButton(curButton);
	}
}

void U3Resources::UpdateButtons(float xPos, float yPos, int mouseState)
{
	short butOffsetX[] = { 67, 202, 473, 26, 205, 384, 384, 563, 338 };
	short butOffsetY[] = { 219, 219, 219, 200, 200, 200, 200, 200, 219 };
	float mult = m_blockSize / 64.0f;

	for (size_t index = 0; index < m_currentButtons.size(); ++index)
	{
		short curOffset = m_currentButtons[index];
		if (mouseState == 2) // mouse up
		{
			m_buttons[m_currentButtons[index]].setMouseCapture(m_blockSize, 2, xPos, yPos,
				(butOffsetX[curOffset] * mult), (butOffsetY[curOffset] * mult), screenOffsetX, screenOffsetY);
		}
		else if (mouseState == 1) // mouse down
		{
			m_buttons[m_currentButtons[index]].setMouseCapture(m_blockSize, 1, xPos, yPos,
				(butOffsetX[curOffset] * mult), (butOffsetY[curOffset] * mult), screenOffsetX, screenOffsetY);
		}
		else // mouse move
		{
			m_buttons[m_currentButtons[index]].setMouseCapture(m_blockSize, 0, xPos, yPos,
				(butOffsetX[curOffset] * mult), (butOffsetY[curOffset] * mult), screenOffsetX, screenOffsetY);
		}
	}
}

void U3Resources::GetTileRectForIndex(int tileNum, SDL_FRect& myRect)
{
	int yPos = tileNum % TILES_NUM_Y;
	int xPos = (tileNum / TILES_NUM_Y) * 2;
	myRect.x = xPos * m_currentGraphics->tiles_width;
	myRect.y = yPos * m_currentGraphics->tiles_height;
	myRect.w = m_currentGraphics->tiles_width;
	myRect.h = m_currentGraphics->tiles_height;
}

void U3Resources::ScrollShape(int tilenum, float offset)
{
	int tileY = (tilenum / 2) % TILES_NUM_Y;
	int tileX = ((tilenum / 2) / TILES_NUM_Y) * 2;
	int realTile = tileX * TILES_NUM_Y + tileY;

	SDL_FRect myRect;
	myRect.x = 0;
	myRect.y = m_currentGraphics->tiles_height * offset;
	myRect.w = m_currentGraphics->tiles_width;
	myRect.h = m_currentGraphics->tiles_height;

	SDL_SetRenderTarget(m_renderer, m_currentGraphics->tile_target[realTile]);
	SDL_RenderTexture(m_renderer, m_currentGraphics->tiles[realTile], NULL, &myRect);
	myRect.y = (m_currentGraphics->tiles_height * offset) - m_currentGraphics->tiles_height;
	SDL_RenderTexture(m_renderer, m_currentGraphics->tiles[realTile], NULL, &myRect);
}

void U3Resources::SwapShape(short shape)
{
	int realTile = shape / 2;

	int multval = realTile / 16;

	realTile += multval * 16;

	if (realTile >= 256)
	{
		return;
	}

	SDL_SetRenderTarget(m_renderer, m_currentGraphics->tile_target[realTile]);
	SDL_RenderClear(m_renderer);
	if (m_shapeSwap[realTile])
	{
		SDL_RenderTexture(m_renderer, m_currentGraphics->tiles[realTile + 16], NULL, NULL);
	}
	else
	{
		SDL_RenderTexture(m_renderer, m_currentGraphics->tiles[realTile], NULL, NULL);
	}
	SDL_SetRenderTarget(m_renderer, NULL);
	m_shapeSwap[realTile] = !m_shapeSwap[realTile];
}

void U3Resources::TwiddleFlags()
{
	if (m_numUpdateFlag > 0)
	{
		m_twiddleFlag[1]--;
		if (m_twiddleFlag[1] < 1)
		{
			m_twiddleFlag[1] = 3;
			SwapShape(0x0E);
		}    // Castle flag changes every fourth pass
		m_twiddleFlag[2]--;
		if (m_twiddleFlag[2] < 1)
		{
			m_twiddleFlag[2] = 2;
			SwapShape(0x0C);
		}    // Towne flag changes every third pass
		m_twiddleFlag[3]--;
		if (m_twiddleFlag[3] < 1)
		{
			m_twiddleFlag[3] = 1;
			SwapShape(0x16);
			SwapShape(0x1E);
		}    // Ship flag changes every second pass
	}
}

void U3Resources::AnimateTiles()
{
	if (m_numUpdateAnimate > 0)
	{
		short temp;
		for (int index = 0; index < 5; ++index)
		{
			m_animFlag[1]--;
			if (m_animFlag[1] < 0)
			{
				m_animFlag[1] = 19;             // from 15 to 19
			}
			temp = (m_animFlag[1] * 2) + 32;    // giving a final range of 0x20-0x46
			if (temp >= 0x44)
			{
				SwapShape(temp - 0x2A);    // also swap Serpent, Man-O-War, and Pirate.
			}
			if (temp == 0x20)
			{
				SwapShape(0x7E);    // also swap party symbol/l/l/l/l / ranger
			}
			if (temp == 62)
			{
				temp = 24;
			}
			if (temp > 62)
			{
				temp += 64; // added
			}
			SwapShape(temp);
			if (temp >= 0x2E && temp <= 0x3C)    // need to swap variants too
			{
				char var = ((temp / 2) - 23) * 2 + 80;
				SwapShape(var++ * 2);
				SwapShape(var * 2);
			}
		}
	}
}

void U3Resources::ScrollThings()
{
	float offset = (float)m_elapsedTimeScroll / (float)DelayScroll;
	float offset2 = (float)(m_elapsedTimeScroll * 2.0f) / (float)DelayScroll;

	offset2 = offset2 - (long)offset2;

	ScrollShape(0x00, offset);	// water
	ScrollShape(0x40, offset2); // forcefield
	ScrollShape(0x42, offset);	// lava
	ScrollShape(0x44, offset2); // moongate

	SDL_SetRenderTarget(m_renderer, NULL);
}

void U3Resources::DemoUpdate(Uint64 curTick)
{
	int numUpdate = 0;
	m_elapsedTimeDemo += (curTick - m_curTickDemo);
	m_elapsedTimeFlag += (curTick - m_curTickDemo);
	m_elapsedTimeAnimate += (curTick - m_curTickDemo);
	m_numUpdateFlag = (int)(m_elapsedTimeFlag / DelayFlags);
	m_numUpdateAnimate = (int)(m_elapsedTimeAnimate / DelayAnimate);
	numUpdate = (int)(m_elapsedTimeDemo / DelayDemo);
	if (numUpdate > 1)
	{
		numUpdate = 1;
	}
	m_elapsedTimeDemo %= DelayDemo;
	m_elapsedTimeFlag %= DelayFlags;
	m_elapsedTimeAnimate %= DelayAnimate;
	m_curTickDemo = curTick;
	if (m_demoDelay > 0)
	{
		m_demoDelay -= numUpdate;
		if (m_demoDelay < 0)
		{
			numUpdate = 0 - m_demoDelay;
		}
		else
		{
			return;
		}
	}

	while (numUpdate > 0)
	{
		bool tempUpdate = false;
		if (m_demoData.size() > m_demoptr + 512)
		{
			unsigned char where = m_demoData[m_demoptr];
			unsigned char what = m_demoData[m_demoptr + 512];
			unsigned char repet;

			if (where != 255)
			{
				if (where < 128)
				{
					m_TileArray[where] = what;
					repet = 1;
					if (what < 16)
					{
						tempUpdate = true;
					}
				}
			}
			else
			{
				m_demoDelay = what;
			}
		}

		m_demoptr += numUpdate;
		m_demoptr %= 512;
		numUpdate = tempUpdate ? 1 : 0;
	}
}

void U3Resources::DrawDemo(Uint64 curTick)
{
	SDL_FRect shapeRect;
	SDL_FRect myRect;

	short demoffset, lastTile, shapSize;
	shapSize = m_blockSize * 2;
	lastTile = 255;

	shapeRect.h = (float)shapSize;
	shapeRect.w = (float)shapSize;

	ScrollThings();
	AnimateTiles();
	TwiddleFlags();
	if (m_numUpdateFlag > 0)
	{
		m_numUpdateFlag = 0;
	}
	if (m_numUpdateAnimate > 0)
	{
		m_numUpdateAnimate = 0;
	}
	demoffset = 0;

	SDL_SetRenderDrawColor(m_renderer, 64, 255, 64, 255);

	for (shapeRect.y = m_blockSize * 11.0f + screenOffsetY; shapeRect.y < (m_blockSize * 23.0f) + screenOffsetY; shapeRect.y += shapSize)
	{
		for (shapeRect.x = (float)m_blockSize + screenOffsetX; shapeRect.x < (m_blockSize * 39.0f) + screenOffsetX; shapeRect.x += shapSize)
		{	
			unsigned char thisTile = m_TileArray[demoffset];

			if (lastTile != thisTile)
			{
				if (thisTile >= 0x28 && thisTile <= 0x2e)   // use "player" tiles for fighter/cleric/wiz/thief
				{
					thisTile += 88;
				}
				GetTileRectForIndex(thisTile / 2, myRect);
				lastTile = thisTile;
			}

			unsigned char bgndTile = m_demoBgndTiles[demoffset];

			if (bgndTile == thisTile)
			{
				if (bgndTile < m_currentGraphics->tile_target.size())
				{
					int tileY = (bgndTile / 2) % TILES_NUM_Y;
					int tileX = ((bgndTile / 2) / TILES_NUM_Y) * 2;
					int realTile = tileX * TILES_NUM_Y + tileY;
					SDL_RenderTexture(m_renderer, m_currentGraphics->tile_target[realTile], NULL, &shapeRect);
				}
			}
			else
			{
				int tileY = (bgndTile / 2) % TILES_NUM_Y;
				int tileX = ((bgndTile / 2) / TILES_NUM_Y) * 2;
				int realTile = tileX * TILES_NUM_Y + tileY;
				SDL_RenderTexture(m_renderer, m_currentGraphics->tile_target[realTile], NULL, &shapeRect);

				tileY = (thisTile / 2) % TILES_NUM_Y;
				tileX = ((thisTile / 2) / TILES_NUM_Y) * 2;
				realTile = tileX * TILES_NUM_Y + tileY;
				SDL_RenderTexture(m_renderer, m_currentGraphics->tile_target[realTile], NULL, &shapeRect);
			}
			demoffset++;
		}
	}

	SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 0);

	m_elapsedTimeScroll += (curTick - m_curTickScroll);

	m_elapsedTimeScroll %= DelayScroll;
	m_curTickScroll = curTick;

}


void U3Resources::DrawOrganizePartyRect()
{
	SDL_FRect myRect;
	short offx, offy, x, y;
	unsigned char c;

	offx = 16;
	offy = 230;

	float scaler = (float)m_blockSize / 16.0f;

	myRect.x = (float)4 * scaler + (offx * scaler);
	myRect.y = (float)0 + (offy * scaler);
	myRect.w = (float)23 * scaler;
	myRect.h = (float)133 * scaler;

	adjustRect(myRect);

	SDL_SetRenderDrawColor(m_renderer, 64, 255, 64, 255);
	SDL_RenderFillRect(m_renderer, &myRect);
	myRect.x += 300 * scaler;
	SDL_RenderFillRect(m_renderer, &myRect);

	myRect.x = (float)27 * scaler + (offx * scaler);
	myRect.y = (float)0 + (offy * scaler);
	myRect.w = (float)76 * scaler;
	myRect.h = (float)133 * scaler;

	adjustRect(myRect);

	SDL_SetRenderDrawColor(m_renderer, 0, 128, 0, 255);
	SDL_RenderFillRect(m_renderer, &myRect);
	myRect.x += 300 * scaler;
	SDL_RenderFillRect(m_renderer, &myRect);

	myRect.x = (float)103 * scaler + (offx * scaler);
	myRect.y = (float)0 + (offy * scaler);
	myRect.w = (float)43 * scaler;
	myRect.h = (float)133 * scaler;

	adjustRect(myRect);

	SDL_SetRenderDrawColor(m_renderer, 0, 96, 0, 255);
	SDL_RenderFillRect(m_renderer, &myRect);
	myRect.x += 300 * scaler;
	SDL_RenderFillRect(m_renderer, &myRect);

	myRect.x = (float)146 * scaler + (offx * scaler);
	myRect.y = (float)0 + (offy * scaler);
	myRect.w = (float)158 * scaler;
	myRect.h = (float)133 * scaler;

	adjustRect(myRect);

	SDL_SetRenderDrawColor(m_renderer, 0, 64, 0, 255);
	SDL_RenderFillRect(m_renderer, &myRect);
	myRect.x += 300 * scaler;
	SDL_RenderFillRect(m_renderer, &myRect);

	// text
	x = offx - 2;
	y = offy + 0;

	std::string strName;

	SDL_SetRenderDrawColor(m_renderer, 255, 255, 255, 255);
	SDL_Color sdl_text_color = { 255, 255, 255 };
	for (int i = 1; i < 21; ++i)
	{
		std::string str_number = std::to_string(i);

		TTF_Text* text_obj = NULL;

		if (m_Player[i][16])
		{
			sdl_text_color.r = 255;
			sdl_text_color.g = 255;
			sdl_text_color.b = 255;
		}
		else
		{
			sdl_text_color.r = 0;
			sdl_text_color.g = 0;
			sdl_text_color.b = 0;
		}

		renderDisplayString(m_font_11, str_number, (int)((x * scaler) + (26 * scaler)), (int)(y * scaler), sdl_text_color, 1);

		strName.clear();
		c = 0;
		while (m_Player[i][c] > 22)
		{
			strName += m_Player[i][c];
			c++;
			if (c > 64)
			{
				break;
			}
		}
		if (strName.size() > 0)
		{
			if (m_Player[i][16])
			{
				sdl_text_color.r = 255;
				sdl_text_color.g = 255;
				sdl_text_color.b = 0;
				renderDisplayString(m_font_11, strName, (int)(((x + 35) * scaler)), (int)(y * scaler), sdl_text_color);
			}

			renderDisplayString(m_font_11, strName, (int)(((x + 35) * scaler)), (int)(y * scaler), sdl_text_color);

			sdl_text_color.r = 255;
			sdl_text_color.g = 255;
			sdl_text_color.b = 255;

			std::string str_level = std::to_string(m_Player[i][30] + 1);
			str_level = std::string("Lvl ") + str_level;

			renderDisplayString(m_font_11, str_level, (int)(((x + 109) * scaler)), (int)(y * scaler), sdl_text_color);

			std::string str_race("Unknown");
			std::string str_class("Unknown");
			std::string str_sex("Unknown");

			if (m_plistMap.find("Races") != m_plistMap.end())
			{
				int val = m_Player[i][22];
				for (size_t index = 0; index < m_plistMap["Races"].size(); ++index)
				{
					if (val == m_plistMap["Races"][index][0])
					{
						str_race = m_plistMap["Races"][index];
						break;
					}
				}
			}
			if (m_plistMap.find("Classes") != m_plistMap.end())
			{
				int val = m_Player[i][23];
				for (size_t index = 0; index < m_plistMap["Classes"].size(); ++index)
				{
					if (val == m_plistMap["Classes"][index][0])
					{
						str_class = m_plistMap["Classes"][index];
						break;
					}
				}
			}
			if (m_plistMap.find("MoreMessages") != m_plistMap.end())
			{
				int val = m_Player[i][24];
				if (m_plistMap["MoreMessages"].size() > 68)
				{
					switch (val)
					{
					case 'F':
						str_sex = m_plistMap["MoreMessages"][66];
						break;
					case 'M':
						str_sex = m_plistMap["MoreMessages"][67];
						break;
					default:
						str_sex = m_plistMap["MoreMessages"][68];
						break;
					}	
				}
			}

			std::string str_status;

			if (m_plistMap["MoreMessages"].size() > 68)
			{

				int statusVal = m_Player[i][17];

				switch (statusVal)
				{
				case 'P':
					str_status = m_plistMap["MoreMessages"][63];
					sdl_text_color.r = 64;
					sdl_text_color.g = 255;
					sdl_text_color.b = 64;
					break;
				case 'D':
					str_status = m_plistMap["MoreMessages"][64];
					sdl_text_color.r = 255;
					sdl_text_color.g = 64;
					sdl_text_color.b = 64;
					break;
				case 'A':
					str_status = m_plistMap["MoreMessages"][65];
					sdl_text_color.r = 192;
					sdl_text_color.g = 192;
					sdl_text_color.b = 192;
					break;
				default:
					sdl_text_color.r = 255;
					sdl_text_color.g = 255;
					sdl_text_color.b = 255;
					break;
				}
			}

			std::string str_desc;
			if (str_status.size() > 0)
			{
				str_desc += str_status + std::string(" ");
			}
			str_desc += str_race + std::string(" ") + str_class + std::string(" ") + str_sex;

			renderDisplayString(m_font_11, str_desc, (int)(((x + 152) * scaler)), (int)(y * scaler), sdl_text_color);
		}

		y += 13;
		if (y > (128 + offy))
		{
			y = offy + 0;
			x += 300;
		}
	}

	SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 0);
}

bool U3Resources::CheckJourneyOnward()
{
	if (!m_Party[7])
	{
		return false;
	}

	return true;
}

bool U3Resources::HasAlert(SDL_Event& event)
{
	m_AlertDlg->HandleEvent(event);
	m_AlertDlg->display();
	if (m_cleanupAlert)
	{
		m_AlertDlg.reset();
		m_cleanupAlert = false;
	}
	return m_AlertDlg.get();
}