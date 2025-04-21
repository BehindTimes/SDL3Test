#include <SDL_image.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_timer.h>
#include <iostream>

#include "U3Resources.h"
#include "U3Misc.h"
#include "U3ScrollArea.h"
#include "UltimaGraphics.h"
#include "UltimaIncludes.h"
#include "U3Utilities.h"
#include "UltimaDungeon.h"

extern short screenOffsetX;
extern short screenOffsetY;
extern SDL_Window* window;
extern TTF_TextEngine* engine_surface;

extern std::unique_ptr<U3Misc> m_misc;
extern std::unique_ptr<U3Graphics> m_graphics;
extern std::unique_ptr<U3ScrollArea> m_scrollArea;
extern std::unique_ptr<U3Utilities> m_utilities;
extern std::unique_ptr<UltimaDungeon> m_dungeon;

constexpr int FONT_NUM_X = 96;
constexpr int FONT_NUM_Y = 1;

constexpr int TILES_NUM_X = 12;
constexpr int TILES_NUM_Y = 16;

constexpr int MINITILES_NUM_X = 64;
constexpr int MINITILES_NUM_Y = 1;

constexpr int NUM_PORTRAITS = 40;

U3Resources::U3Resources() :
	m_renderer(nullptr),
	m_currentGraphics(nullptr),
	m_standardGraphics(nullptr),
	m_defaultMode(0),
	m_texStalagtites(nullptr),
	m_texPortraits(nullptr),
	m_texExodus(nullptr),
	m_texUltimaLogo(nullptr),
	m_texBy(nullptr),
	m_texCredits(nullptr),
	m_texExodusFade(nullptr),
	m_texUltimaLogoFade(nullptr),
	m_texDisplay(nullptr),
	m_texStats(nullptr),
	m_exodusWidth(0),
	m_exodusHeight(0),
	m_ultimaLogoWidth(0),
	m_ultimaLogoHeight(0),
	m_blockSize(32),
	m_font(nullptr),
	m_font_9(nullptr),
	m_font_11(nullptr),
	m_font_12(nullptr),
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
	m_cleanupAlert(false),
	m_portraitWidth(0),
	m_portraitHeight(0),
	m_isInversed(false),
	m_font_y_offset(0),
	m_fullUpdate(true),
	m_updateWind(false),
	m_alertReturn(0),
	m_delta_time(0),
	m_texSosariaMap(nullptr),
	m_texFountain(nullptr),
	m_texRod(nullptr),
	m_texShrine(nullptr),
	m_texTimeLord(nullptr),
	m_overrideImage(-1),
	m_elapsedMoveTime(0),
	m_newMove(false),
	m_wasMove(false),
	m_selectedFormRect(-1),
	m_texRaceClass(nullptr),
	m_texCharacterRecord(nullptr),
	m_characterRecordWidth(0),
	m_characterRecordHeight(0)
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
}

U3Resources::~U3Resources()
{
	m_CreateCharacterDlg.reset();
	m_AlertDlg.reset();
	m_buttons.clear();

	CleanupPartyNames();

	for (auto mode : m_allGraphics)
	{
		if (mode.second.font)
		{
			SDL_DestroyTexture(mode.second.font);
			mode.second.font = nullptr;
		}
		if (mode.second.ui)
		{
			SDL_DestroyTexture(mode.second.ui);
			mode.second.ui = nullptr;
		}

		for (auto& curTex : mode.second.tiles)
		{
			SDL_DestroyTexture(curTex);
			curTex = nullptr;
		}
		for (auto& curTex : mode.second.tile_target)
		{
			SDL_DestroyTexture(curTex);
			curTex = nullptr;
		}
		for (auto& curTex : mode.second.mini_tiles)
		{
			SDL_DestroyTexture(curTex);
			curTex = nullptr;
		}
	}

	if (m_texSosariaMap)
	{
		SDL_DestroyTexture(m_texSosariaMap);
		m_texSosariaMap = nullptr;
	}

	if (m_texFountain)
	{
		SDL_DestroyTexture(m_texFountain);
		m_texFountain = nullptr;
	}

	if (m_texRod)
	{
		SDL_DestroyTexture(m_texRod);
		m_texRod = nullptr;
	}

	if (m_texShrine)
	{
		SDL_DestroyTexture(m_texShrine);
		m_texShrine = nullptr;
	}

	if (m_texTimeLord)
	{
		SDL_DestroyTexture(m_texTimeLord);
		m_texTimeLord = nullptr;
	}

	if (m_texStalagtites)
	{
		SDL_DestroyTexture(m_texStalagtites);
		m_texStalagtites = nullptr;
	}

	if (m_texPortraits)
	{
		SDL_DestroyTexture(m_texPortraits);
		m_texPortraits = nullptr;
	}

	for (int index = 0; index < 5; ++index)
	{
		if (m_texIntro[index])
		{
			SDL_DestroyTexture(m_texIntro[index]);
			m_texIntro[index] = nullptr;
		}
	}

	if (m_texDisplay)
	{
		SDL_DestroyTexture(m_texDisplay);
		m_texDisplay = nullptr;
	}
	if (m_texStats)
	{
		SDL_DestroyTexture(m_texStats);
		m_texStats = nullptr;
	}

	if (m_texExodus)
	{
		SDL_DestroyTexture(m_texExodus);
		m_texExodus = nullptr;
	}
	if (m_texUltimaLogo)
	{
		SDL_DestroyTexture(m_texUltimaLogo);
		m_texUltimaLogo = nullptr;
	}
	if (m_texBy)
	{
		SDL_DestroyTexture(m_texBy);
		m_texBy = nullptr;
	}
	if (m_texCredits)
	{
		SDL_DestroyTexture(m_texCredits);
		m_texCredits = nullptr;
	}
	if (m_texExodusFade)
	{
		SDL_DestroyTexture(m_texExodusFade);
		m_texExodusFade = nullptr;
	}
	if (m_texUltimaLogoFade)
	{
		SDL_DestroyTexture(m_texUltimaLogoFade);
		m_texUltimaLogoFade = nullptr;
	}
	if (m_texRaceClass)
	{
		SDL_DestroyTexture(m_texRaceClass);
		m_texRaceClass = nullptr;
	}
	if (m_texCharacterRecord)
	{
		SDL_DestroyTexture(m_texCharacterRecord);
		m_texCharacterRecord = nullptr;
	}

	if (m_font)
	{
		TTF_CloseFont(m_font);
		m_font = nullptr;
	}
	if (m_font_9)
	{
		TTF_CloseFont(m_font_9);
		m_font_9 = nullptr;
	}
	if (m_font_11)
	{
		TTF_CloseFont(m_font_11);
		m_font_11 = nullptr;
	}
	if (m_font_12)
	{
		TTF_CloseFont(m_font_12);
		m_font_12 = nullptr;
	}

	TTF_Quit();
}

void U3Resources::displayFPS(int fps)
{
	std::string strFPS = std::to_string(fps);

	SDL_Color sdl_text_color = { 255, 255, 0 };
	renderDisplayString(m_font, strFPS, 0, 0, sdl_text_color);
}

void U3Resources::AlertCallback()
{
	m_cleanupAlert = true;
}

void U3Resources::CreateAlertMessage(int message, DialogType type)
{
	m_AlertDlg = std::make_unique<U3Dialog>(m_renderer, engine_surface, &m_currentGraphics, &m_standardGraphics,
		m_blockSize, message, type, std::bind(&U3Resources::AlertCallback, this));
}

void U3Resources::GetPreference(U3PreferencesType type, bool& value)
{
	switch (type)
	{
	case U3PreferencesType::Allow_Diagonal:
		value = m_preferences.allow_diagonal;
		break;
	case U3PreferencesType::Auto_Save:
		value = m_preferences.auto_save;
		break;
	case U3PreferencesType::Include_Wind:
		value = m_preferences.include_wind;
		break;
	case U3PreferencesType::Classic_Appearance:
		value = m_preferences.classic_appearance;
		break;
	case U3PreferencesType::Sound_Inactive:
		value = m_preferences.sound_inactive;
		break;
	default:
		break;
	}
}

void U3Resources::SetPreference(U3PreferencesType type, bool value)
{
	switch (type)
	{
	case U3PreferencesType::Allow_Diagonal:
		m_preferences.allow_diagonal = value;
		break;
	case U3PreferencesType::Auto_Save:
		m_preferences.auto_save = value;
		break;
	case U3PreferencesType::Include_Wind:
		m_preferences.include_wind = value;
		break;
	case U3PreferencesType::Classic_Appearance:
		m_preferences.classic_appearance = value;
		break;
	case U3PreferencesType::Sound_Inactive:
		m_preferences.sound_inactive = value;
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
	m_scrollArea->setRenderer(m_renderer);
	if (!loadFont())
	{
		return false;
	}
	if (!loadPLists())
	{
		return false;
	}
	loadGraphics();
	loadImages();

	loadButtons();
	loadDemo();
	m_misc->OpenRstr();

	if (m_allGraphics.find(std::string(Standard)) != m_allGraphics.end())
	{
		m_standardGraphics = &m_allGraphics[std::string(Standard)];
		m_currentGraphics = m_standardGraphics; // 6
	}
	CalculateBlockSize();
	//changeMode(1);
	return true;
}

void U3Resources::loadDemo()
{
	std::filesystem::path currentPath = std::filesystem::current_path();
	currentPath /= ResourceLoc;
	currentPath /= BinLoc;
	currentPath /= "MainResources.rsrc_DEMO_400_WhereWhatHow.bin";

	std::string strTemp = m_utilities->PathToSDLString(currentPath);
	if (strTemp.empty())
	{
		return;
	}
	SDL_IOStream* file = SDL_IOFromFile(strTemp.c_str(), "rb");
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

void U3Resources::SetButtonCallback(short button, std::function<void(int)> func)
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

	// Need to make sure the font stays within the block size.
	// The font will take up 75% of the space, and we want to give a 4 pixel buffer and then shift the text upwards

	float font_size = (float)(m_blockSize);
	m_font_y_offset = (m_blockSize) * (1.0f / 8.0f);

	m_font = TTF_OpenFont(currentPath.string().c_str(), font_size);
	if (!m_font)
	{
		return false;
	}

	m_font_9 = TTF_OpenFont(currentPath.string().c_str(), 9.0f * scaler);
	if (!m_font_9)
	{
		return false;
	}

	m_font_11 = TTF_OpenFont(currentPath.string().c_str(), 11.0f * scaler);
	if (!m_font_11)
	{
		return false;
	}

	m_font_12 = TTF_OpenFont(currentPath.string().c_str(), 12.0f * scaler);
	if (!m_font_12)
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

	m_scrollArea->setBlockSize(m_blockSize);
	m_graphics->setBlockSize(m_blockSize);

	if (m_font)
	{
		TTF_CloseFont(m_font);
		m_font = nullptr;
	}
	if (m_font_9)
	{
		TTF_CloseFont(m_font_9);
		m_font_9 = nullptr;
	}
	if (m_font_11)
	{
		TTF_CloseFont(m_font_11);
		m_font_11 = nullptr;
	}
	if (m_font_12)
	{
		TTF_CloseFont(m_font_12);
		m_font_12 = nullptr;
	}
	createFont();

	if (m_AlertDlg.get())
	{
		m_AlertDlg->changeBlockSize(m_blockSize);
	}

	if (m_CreateCharacterDlg.get())
	{
		m_CreateCharacterDlg->changeBlockSize(m_blockSize);
	}

	CreatePartyNames();

	int final = m_blockSize * 22;
	if (m_texDisplay)
	{
		SDL_DestroyTexture(m_texBy);
	}
	if (m_texStats)
	{
		SDL_DestroyTexture(m_texBy);
	}
	loadButtons();
	m_texDisplay = SDL_CreateTexture(m_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, final, final);
	m_texStats = SDL_CreateTexture(m_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, m_blockSize * 15, m_blockSize * 3);
	m_fullUpdate = true;
}


void U3Resources::changeMode(int mode)
{
	if (mode == 2)
	{
		m_graphics->ChangeClassic();
		m_scrollArea->forceRedraw();
		m_graphics->setForceRedraw();
		return;
	}
	if (mode == 6)
	{
		m_currentGraphics = &m_allGraphics["Standard"];
		m_scrollArea->forceRedraw();
		m_graphics->setForceRedraw();
		return;
	}

	if (mode >= 0 && mode < m_modes.size())
	{
		std::string strMode = m_modes[mode];
		if (m_allGraphics.find(strMode) != m_allGraphics.end())
		{
			m_currentGraphics = &m_allGraphics[strMode];
		}
		m_scrollArea->forceRedraw();
		m_graphics->setForceRedraw();
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
		else if (node->children != nullptr)
		{
			node = node->children;
			xmlNodePtr intNode = findNodeByName(node, nodename);
			if (intNode != nullptr)
			{
				return intNode;
			}
		}
		node = node->next;
	}
	return nullptr;
}

void U3Resources::processDoc(xmlDocPtr docPtr, std::vector<std::string >& curVec)
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
		m_buttons[index].setRect(m_renderer, buttonImage, m_blockSize, butLeft[index], butTop[index],
			(butRight[index] - butLeft[index]) * 4, (butBottom[index] - butTop[index]) * 4,
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

	currentPath = std::filesystem::current_path();
	currentPath /= ResourceLoc;
	currentPath /= ImagesLoc;
	currentPath /= "Portraits.png";

	m_texPortraits = IMG_LoadTexture(m_renderer, currentPath.string().c_str());
	SDL_GetTextureSize(m_texPortraits, &w, &h);
	m_portraitWidth = (int)w / NUM_PORTRAITS;
	m_portraitHeight = (int)h;
	SDL_SetTextureScaleMode(m_texPortraits, SDL_SCALEMODE_NEAREST);

	m_texExodusFade = SDL_CreateTexture(m_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, m_exodusWidth, m_exodusHeight);
	unsigned char* pixels = NULL;
	int pitch;
	SDL_LockTexture(m_texExodusFade, NULL, (void**)&pixels, &pitch);
	memset(pixels, 0, sizeof(char) * pitch * m_exodusHeight);
	SDL_UnlockTexture(m_texExodusFade);

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
	m_dungeon->loadGraphics();

	//////////////////////////////////////////////////

	currentPath = std::filesystem::current_path();
	currentPath /= ResourceLoc;
	currentPath /= ImagesLoc;
	currentPath /= "SosariaMap.jpg";
	m_texSosariaMap = IMG_LoadTexture(m_renderer, currentPath.string().c_str());

	currentPath = std::filesystem::current_path();
	currentPath /= ResourceLoc;
	currentPath /= ImagesLoc;
	currentPath /= "Fountain.jpg";
	m_texFountain = IMG_LoadTexture(m_renderer, currentPath.string().c_str());

	currentPath = std::filesystem::current_path();
	currentPath /= ResourceLoc;
	currentPath /= ImagesLoc;
	currentPath /= "Rod.jpg";
	m_texRod = IMG_LoadTexture(m_renderer, currentPath.string().c_str());

	currentPath = std::filesystem::current_path();
	currentPath /= ResourceLoc;
	currentPath /= ImagesLoc;
	currentPath /= "Shrine.jpg";
	m_texShrine = IMG_LoadTexture(m_renderer, currentPath.string().c_str());

	currentPath = std::filesystem::current_path();
	currentPath /= ResourceLoc;
	currentPath /= ImagesLoc;
	currentPath /= "TimeLord.jpg";
	m_texTimeLord = IMG_LoadTexture(m_renderer, currentPath.string().c_str());

	currentPath = std::filesystem::current_path();
	currentPath /= ResourceLoc;
	currentPath /= ImagesLoc;
	currentPath /= "CharacterRecord.jpg";
	m_texCharacterRecord = IMG_LoadTexture(m_renderer, currentPath.string().c_str());

	SDL_GetTextureSize(m_texCharacterRecord, &m_characterRecordWidth, &m_characterRecordHeight);

	currentPath = std::filesystem::current_path();
	currentPath /= ResourceLoc;
	currentPath /= ImagesLoc;
	currentPath /= "RaceClassInfo.gif";
	m_texRaceClass = IMG_LoadTexture(m_renderer, currentPath.string().c_str());

	m_dungeon->loadGraphics();
}

void U3Resources::GetTileRectForIndex(SDL_Texture* curTexture, int tileNum, SDL_FRect& myRect, float tileXSize, float tileYSize, int num_tiles_y)
{
	int yPos = tileNum % num_tiles_y;
	int xPos = (tileNum / num_tiles_y);
	myRect.x = xPos * tileXSize;
	myRect.y = yPos * tileYSize;
	myRect.w = tileXSize;
	myRect.h = tileYSize;
}

void U3Resources::loadMiniTiles(ModeGraphics& curGraphics, std::string strFile)
{
	float xSize;
	float ySize;

	curGraphics.mini_tiles.resize(MINITILES_NUM_X * MINITILES_NUM_Y);

	SDL_Texture* curTexture = nullptr;

	curTexture = IMG_LoadTexture(m_renderer, strFile.c_str());
	SDL_SetTextureScaleMode(curTexture, SDL_SCALEMODE_NEAREST);
	SDL_GetTextureSize(curTexture, &xSize, &ySize);
	float tileXSize = xSize / MINITILES_NUM_X;
	float tileYSize = ySize / MINITILES_NUM_Y;

	for (size_t index = 0; index < MINITILES_NUM_X * MINITILES_NUM_Y; ++index)
	{
		curGraphics.mini_tiles[index] = SDL_CreateTexture(m_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 10, 10);
		SDL_SetTextureScaleMode(curGraphics.mini_tiles[index], SDL_SCALEMODE_NEAREST);
	}

	int curVecPos = 0;
	SDL_FRect frameRect;

	for (int indexX = 0; indexX < MINITILES_NUM_X; ++indexX)
	{
		for (int indexY = 0; indexY < MINITILES_NUM_Y; ++indexY)
		{
			GetTileRectForIndex(curTexture, curVecPos, frameRect, tileXSize, tileYSize, 1);
			SDL_SetRenderTarget(m_renderer, curGraphics.mini_tiles[curVecPos]);
			SDL_RenderClear(m_renderer);
			SDL_RenderTexture(m_renderer, curTexture, &frameRect, NULL);

			curVecPos++;
		}
	}
	SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 0);

	SDL_SetRenderTarget(m_renderer, NULL);

	if (curTexture)
	{
		SDL_DestroyTexture(curTexture);
	}
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
			GetTileRectForIndex(curTexture, curVecPos, frameRect, tileXSize, tileYSize, TILES_NUM_Y);
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
			loadMiniTiles(m_allGraphics[mode], dirEntry.path().string());
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
			}
		}
	}
}

void U3Resources::adjustRect(SDL_FRect& myRect)
{
	myRect.x += screenOffsetX;
	myRect.y += screenOffsetY;
}

int U3Resources::renderString(std::string curString, int x, int y, bool autoadjust, int offsetX, int offsetY, bool pretty_print)
{
	SDL_Color sdl_text_color = { 255, 255, 255 };
	SDL_FRect frameRect, myRect;
	int text_extent = 0;

	if (m_preferences.classic_appearance)
	{
		text_extent = (int)((x * m_blockSize) + (m_blockSize * curString.size()) + offsetX);

		for (size_t index = 0; index < curString.size(); ++index)
		{
			int part = curString.c_str()[index];
			part -= 32;
			if (part < 0 || part > 96)
			{
				continue;
			}

			myRect.x = (float)(x * m_blockSize) + (m_blockSize * index) + offsetX;
			myRect.y = (float)(y * m_blockSize) + offsetY;
			myRect.w = (float)(m_blockSize);
			myRect.h = (float)(m_blockSize);

			if (autoadjust)
			{
				adjustRect(myRect);
			}

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
		if (pretty_print)
		{
			std::string strDisplayString;
			for (size_t index = 0; index < curString.size(); ++index)
			{
				if (curString[index] == ':')
				{
					renderString(strDisplayString, (int)((x + index) - strDisplayString.size()), y, autoadjust, offsetX, offsetY);
					renderString(":", (int)(x + index), y, autoadjust, offsetX, offsetY);
					strDisplayString.clear();
				}
				else if (curString[index] == ' ')
				{
					renderString(strDisplayString, (int)((x + index) - strDisplayString.size()), y, autoadjust, offsetX, offsetY);
					strDisplayString.clear();
				}
				else
				{
					strDisplayString += curString[index];
				}
			}
			if (curString.size() != 16) // right adjust
			{

				renderString(strDisplayString, (int)(curString.size() - strDisplayString.size()), y, autoadjust, offsetX, offsetY);
			}
			else
			{
				renderDisplayString(m_font, strDisplayString, 16 * m_blockSize, y * m_blockSize, sdl_text_color, 1, autoadjust);
			}
		}
		else
		{
			offsetY -= (int)m_font_y_offset;
			TTF_Text* text_obj = NULL;
			text_obj = TTF_CreateText(engine_surface, m_font, curString.c_str(), 0);
			int w, h;
			TTF_GetTextSize(text_obj, &w, &h);

			size_t totalLen = m_blockSize * curString.size();
			size_t tempOffset = (totalLen - w) / 2;

			if (autoadjust)
			{
				text_extent = (int)(x * m_blockSize + screenOffsetX + tempOffset + offsetX + w);
				TTF_DrawRendererText(text_obj, (float)x * m_blockSize + screenOffsetX + tempOffset + offsetX, (float)y * m_blockSize + screenOffsetY + offsetY);
			}
			else
			{
				text_extent = (int)(x * m_blockSize + offsetX + w);

				TTF_DrawRendererText(text_obj, (float)x * m_blockSize + offsetX, (float)y * m_blockSize + offsetY);
			}

			if (text_obj)
			{
				TTF_DestroyText(text_obj);
				text_obj = NULL;
			}
		}
	}

	return text_extent;
}

void U3Resources::renderDisplayString(TTF_Text* text_obj, int x, int y, SDL_Color color, int align, bool autoadjust)
{
	if (!text_obj)
	{
		return;
	}
	TTF_SetTextColor(text_obj, color.r, color.g, color.b, 255);

	int w, h;
	int offsetW = 0;
	int offsetH = 0;
	TTF_GetTextSize(text_obj, &w, &h);

	if (align == 1) // left adjust
	{
		offsetW = w * -1;
	}
	else if (align == 2) // center
	{
		offsetW = (w / 2) * -1;
	}
	else if (align == 4) // center/center
	{
		offsetW = (w / 2) * -1;
		offsetH = (h / 2) * -1;
	}
	else if (align == 6) // top
	{
		offsetH = h * -1;
	}

	if (autoadjust)
	{
		TTF_DrawRendererText(text_obj, (float)x + screenOffsetX + offsetW, (float)y + screenOffsetY + offsetH);
	}
	else
	{
		TTF_DrawRendererText(text_obj, (float)x + offsetW, (float)y + offsetH);
	}
}

void U3Resources::renderDisplayString(TTF_Font* font, std::string curString, int x, int y, SDL_Color color, int align, bool autoadjust)
{
	TTF_Text* text_obj = NULL;
	text_obj = TTF_CreateText(engine_surface, font, curString.c_str(), 0);

	TTF_SetTextColor(text_obj, color.r, color.g, color.b, 255);

	int w, h;
	int offsetW = 0;
	int offsetH = 0;
	TTF_GetTextSize(text_obj, &w, &h);

	if (align == 1) // left adjust
	{
		offsetW = w * -1;
	}
	else if (align == 2) // center
	{
		offsetW = (w / 2) * -1;
	}
	else if (align == 4) // center/center
	{
		offsetW = (w / 2) * -1;
		offsetH = (h / 2) * -1;
	}
	else if (align == 6) // top
	{
		offsetH = h * -1;
	}

	// We overadjusted the default font, so only worry about correcting that one
	if (font == m_font)
	{
		offsetH -= (int)m_font_y_offset;
	}

	if (autoadjust)
	{
		TTF_DrawRendererText(text_obj, (float)x + screenOffsetX + offsetW, (float)y + screenOffsetY + offsetH);
	}
	else
	{
		TTF_DrawRendererText(text_obj, (float)x + offsetW, (float)y + offsetH);
	}

	if (text_obj)
	{
		TTF_DestroyText(text_obj);
		text_obj = NULL;
	}
}

int U3Resources::getTextWidth(std::string str)
{
	int w, h;
	TTF_Text* text_obj = NULL;
	text_obj = TTF_CreateText(engine_surface, m_font, str.c_str(), 0);

	TTF_GetTextSize(text_obj, &w, &h);

	if (text_obj)
	{
		TTF_DestroyText(text_obj);
		text_obj = NULL;
	}

	return w;
}

void U3Resources::DrawFramePieceReal(int part, int x, int y, bool adjust)
{
	SDL_FRect frameRect, myRect;

	myRect.x = (float)(x);
	myRect.y = (float)(y);
	myRect.w = (float)(m_blockSize);
	myRect.h = (float)(m_blockSize);

	if (adjust)
	{
		adjustRect(myRect);
	}

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

void U3Resources::renderUI(int part, int x, int y, bool adjust, int offsetX, int offsetY)
{
	SDL_FRect frameRect, myRect;

	myRect.x = (float)(x * m_blockSize) + offsetX;
	myRect.y = (float)(y * m_blockSize) + offsetY;
	myRect.w = (float)(m_blockSize);
	myRect.h = (float)(m_blockSize);

	if (adjust)
	{
		adjustRect(myRect);
	}

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

	std::string strTemp = m_utilities->PathToSDLString(currentPath);
	if (strTemp.empty())
	{
		return;
	}
	SDL_IOStream* file = SDL_IOFromFile(strTemp.c_str(), "rb");
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

void U3Resources::DrawMoongates()
{
	SDL_FRect myRect;

	bool classic;
	GetPreference(U3PreferencesType::Classic_Appearance, classic);

	if (classic)
	{
		m_graphics->DrawFramePiece(12, 8, 0);
		m_graphics->DrawFramePiece(13, 15, 0);

		myRect.x = (float)(9 * m_blockSize);
		myRect.y = (float)(0 * m_blockSize);
		myRect.w = (float)(6 * m_blockSize);
		myRect.h = (float)(m_blockSize);

		adjustRect(myRect);

		SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
		SDL_RenderFillRect(m_renderer, &myRect);
		SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 0);

		UPrint("(", 9, 0, true);
		UPrint(")(", 11, 0, true);
		UPrint(")", 14, 0, true);
		std::string strX;
		strX += (char)m_misc->m_gMoonDisp[0];
		std::string strY;
		strY += (char)m_misc->m_gMoonDisp[0];
		UPrint(strX, 10, 0, true);
		UPrint(strY, 13, 0, true);
	}
	else
	{
		SDL_Color sdl_text_color = { 255, 255, 255 };

		myRect.x = (float)(8 * m_blockSize);
		myRect.y = (float)(0 * m_blockSize);
		myRect.w = (float)(8 * m_blockSize);
		myRect.h = (float)(m_blockSize);

		m_graphics->DrawFramePiece(12, 7, 0);
		m_graphics->DrawFramePiece(13, 16, 0);

		adjustRect(myRect);

		SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
		SDL_RenderFillRect(m_renderer, &myRect);
		SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 0);

		std::string strX = std::string("(");
		strX += (char)m_misc->m_gMoonDisp[0] + std::string(")");
		std::string strY = std::string("(");
		strY += (char)m_misc->m_gMoonDisp[1] + std::string(")");

		m_graphics->DrawFramePiece(34 + (m_misc->m_gMoonDisp[0] - '0'), 8, 0);
		m_graphics->DrawFramePiece(42 + (m_misc->m_gMoonDisp[1] - '0'), 12, 0);

		int xPos = (int)(10.5f * m_blockSize);
		int yPos = (int)(14.5f * m_blockSize);
		renderDisplayString(m_font, strX, xPos, 0, sdl_text_color, 2);
		renderDisplayString(m_font, strY, yPos, 0, sdl_text_color, 2);
	}
	if (m_misc->m_Party[2] != 0)
	{
		return;
	}
	if ((m_misc->m_gMoonDisp[0] == '0') && (m_misc->m_gMoonDisp[1] == '0'))
	{
		m_misc->PutXYVal(24, m_misc->m_LocationX[8], m_misc->m_LocationY[8]);
	}    // Towne (Dawn)
	else if (m_misc->GetXYVal(m_misc->m_LocationX[8], m_misc->m_LocationY[8]) == 24)
	{
		m_misc->PutXYVal(12, m_misc->m_LocationX[8], m_misc->m_LocationY[8]);
	}    // or forest
	short x;
	for (x = 0; x < 8; x++) {
		m_misc->PutXYVal(4, m_misc->m_MoonXTable[x], m_misc->m_MoonYTable[x]);
	}
	x = m_misc->m_gMoonDisp[0] - '0';
	m_misc->PutXYVal(136, m_misc->m_MoonXTable[x], m_misc->m_MoonYTable[x]);    // put Moongate
}

void U3Resources::DrawWind()
{
	SDL_FRect myRect;

	m_graphics->DrawFramePiece(12, 6, 23);
	m_graphics->DrawFramePiece(13, 17, 23);

	myRect.x = (float)(7 * m_blockSize);
	myRect.y = (float)(23 * m_blockSize);
	myRect.w = (float)(10 * m_blockSize);
	myRect.h = (float)(m_blockSize);

	adjustRect(myRect);

	SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
	SDL_RenderFillRect(m_renderer, &myRect);
	SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 0);

	std::string dispString;
	int winddir = m_misc->m_WindDir;
	if (winddir < 0 || winddir > 4)
	{
		winddir = 0;
	}

	dispString = m_plistMap["MoreMessages"][42 + winddir];
	renderString(dispString, 7, 23);
}

void U3Resources::DoWind()
{
	if (m_misc->m_Party[2] == 1)
	{
		return;
	}
	if (!m_updateWind)
	{
		return;
	}
	m_updateWind = false;

	int newDir = m_misc->m_WindDir;


	while (newDir == m_misc->m_WindDir)
	{
		newDir = m_utilities->getRandom(0, 8);
		if (newDir > 4)
		{
			newDir -= 4;
		}
	}
	m_misc->m_WindDir = newDir;
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

void U3Resources::CenterMessage(std::string message, short xStart, short xEnd, short y)
{
	SDL_FRect myRect;
	int difference = xEnd * m_blockSize - xStart * m_blockSize;
	int startTile = difference / 2;
	myRect.x = (float)(xStart * m_blockSize);
	myRect.y = (float)y * m_blockSize;
	myRect.h = (float)m_blockSize;
	myRect.w = (float)(difference);
	adjustRect(myRect);
	SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
	SDL_RenderFillRect(m_renderer, &myRect);
	SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 0);

	renderString(message, xStart, y);
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
				(butOffsetX[curOffset] * mult) * 4, (butOffsetY[curOffset] * mult) * 4, screenOffsetX, screenOffsetY);
		}
		else if (mouseState == 1) // mouse down
		{
			m_buttons[m_currentButtons[index]].setMouseCapture(m_blockSize, 1, xPos, yPos,
				(butOffsetX[curOffset] * mult) * 4, (butOffsetY[curOffset] * mult) * 4, screenOffsetX, screenOffsetY);
		}
		else // mouse move
		{
			m_buttons[m_currentButtons[index]].setMouseCapture(m_blockSize, 0, xPos, yPos,
				(butOffsetX[curOffset] * mult) * 4, (butOffsetY[curOffset] * mult) * 4, screenOffsetX, screenOffsetY);
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

void U3Resources::DrawOrganizePartyDispersed(bool wasDispersed)
{
	CenterMessage(13, 17);

	if (wasDispersed)
	{
		CenterMessage(14, 20);
	}
	else
	{
		CenterMessage(15, 19);
	}
}

void U3Resources::DrawOrganizePartyFormed(bool inUse)
{
	CenterMessage(4, 13);

	if (inUse)
	{
		CenterMessage(5, 16);
	}
	else
	{
		CenterMessage(12, 16);
	}
}

void U3Resources::UpdateFormParty(float xPos, float yPos, int mouseState)
{
	float scaler = (float)m_blockSize / 16.0f;

	short yMin = (short)((230 * scaler) + screenOffsetY);
	short yMax = (short)((360 * scaler) + screenOffsetY);
	short xMin = (short)(20 * scaler + screenOffsetX);
	short xMax = (short)(620 * scaler + screenOffsetX);
	short xMiddle = (short)(320 * scaler + screenOffsetX);

	m_selectedFormRect = -1;

	if (xPos > xMin && xPos < xMax &&
		yPos > yMin && yPos < yMax)
	{
		m_selectedFormRect = (int)((yPos - yMin) / (13 * scaler));
		if (xPos > xMiddle)
		{
			m_selectedFormRect += 10;
		}
		if (m_selectedFormRect >= 20 || m_selectedFormRect < 0)
		{
			m_selectedFormRect = -1;
		}
		else
		{
			if (!m_partyDisplay[m_selectedFormRect].Name)
			{
				m_selectedFormRect = -1;
			}
		}

		if (mouseState == 2 && m_selectedFormRect >= 0 && m_selectedCharacters.size() <= 4)
		{
			auto it = std::find(m_selectedCharacters.begin(), m_selectedCharacters.end(), m_selectedFormRect);
			if (it == m_selectedCharacters.end())
			{
				m_selectedCharacters.push_back(m_selectedFormRect);
			}
			else
			{
				m_selectedCharacters.erase(it);
			}

			SetButtonVisibility(5, (m_selectedCharacters.size() > 0));
		}
	}
}

void U3Resources::UpdateCreateCharacter(float xPos, float yPos, int mouseState)
{
	if (m_CreateCharacterDlg)
	{
		m_CreateCharacterDlg->updateDialog(xPos, yPos, mouseState);
	}
}

void U3Resources::UpdateCreateCharacterChooseSlot(float xPos, float yPos, int mouseState)
{
	float scaler = (float)m_blockSize / 16.0f;

	short yMin = (short)((230 * scaler) + screenOffsetY);
	short yMax = (short)((360 * scaler) + screenOffsetY);
	short xMin = (short)(20 * scaler + screenOffsetX);
	short xMax = (short)(620 * scaler + screenOffsetX);
	short xMiddle = (short)(320 * scaler + screenOffsetX);

	m_selectedFormRect = -1;

	if (xPos > xMin && xPos < xMax &&
		yPos > yMin && yPos < yMax)
	{
		m_selectedFormRect = (int)((yPos - yMin) / (13 * scaler));
		if (xPos > xMiddle)
		{
			m_selectedFormRect += 10;
		}
		if (m_selectedFormRect >= 20 || m_selectedFormRect < 0)
		{
			m_selectedFormRect = -1;
		}
		else
		{
			if (m_partyDisplay[m_selectedFormRect].Name)
			{
				m_selectedFormRect = -1;
			}
		}

		if (mouseState == 2 && m_selectedFormRect >= 0)
		{
			float scaler = (float)m_blockSize / 16.0f;
			float ratio = m_characterRecordHeight / m_characterRecordWidth;
			float addheight = 336.0f * ratio;
			float addheight1 = 336.0f * (34.0f / 400.0f);
			float addheight2 = 336.0f * (53.0f / 400.0f);
			m_graphics->m_obsCurMode = OrganizeBottomScreen::CreateCharacter;
			m_CreateCharacterDlg = std::make_unique<CreateCharacterDialog>(m_renderer, engine_surface);
			m_CreateCharacterDlg->m_Rect.x = 144;
			m_CreateCharacterDlg->m_Rect.y = ((384 - (200 + addheight + addheight1 + addheight2)) / 2);
			m_CreateCharacterDlg->m_Rect.w = 336;
			m_CreateCharacterDlg->m_Rect.h = 200 + addheight + addheight1 + addheight2;
			m_CreateCharacterDlg->init();
		}
	}
}

void U3Resources::DrawOrganizePartyRect()
{
	SDL_FRect myRect;
	short offx, offy, x, y;
	//unsigned char c;

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

	if (m_selectedFormRect >= 0)
	{
		float tempX = (float)4 * scaler + (offx * scaler);
		float tempY = (230.0f * scaler) + screenOffsetY;
		float yOffset = 0;
		float xOffset = 0;
		int tempYPos = m_selectedFormRect;
		if (m_selectedFormRect >= 10)
		{
			xOffset = 300 * scaler;
			tempYPos -= 10;
		}
		yOffset = tempYPos * 13 * scaler;
		myRect.x = (float)4 * scaler + (offx * scaler) + xOffset;
		myRect.y = (230.0f * scaler) + screenOffsetY + yOffset;
		myRect.w = 300.0f * scaler;
		myRect.h = 13.0f * scaler;

		adjustRect(myRect);

		SDL_SetRenderDrawBlendMode(m_renderer, SDL_BLENDMODE_BLEND);
		SDL_SetRenderDrawColor(m_renderer, 0, 128, 255, 128);
		SDL_RenderFillRect(m_renderer, &myRect);
		SDL_SetRenderDrawBlendMode(m_renderer, SDL_BLENDMODE_NONE);
	}

	// text
	x = offx - 2;
	y = offy + 0;

	std::string strName;
	SDL_Color sdl_text_color = { 255, 255, 255 };

	if (m_graphics->m_obsCurMode == OrganizeBottomScreen::FormParty)
	{
		renderDisplayString(m_partyDisplay[0].Number, (int)((x * scaler) + (26 * scaler)), (int)((y - 30) * scaler), sdl_text_color, 1);
		renderDisplayString(m_partyDisplay[1].Number, (int)((x * scaler) + (26 * scaler)), (int)((y - 17) * scaler), sdl_text_color, 1);
		renderDisplayString(m_partyDisplay[2].Number, (int)(((x + 115) * scaler)), (int)((y - 30) * scaler), sdl_text_color, 1);
		renderDisplayString(m_partyDisplay[3].Number, (int)(((x + 115) * scaler)), (int)((y - 17) * scaler), sdl_text_color, 1);

		if (m_selectedCharacters.size() > 0)
		{
			renderDisplayString(m_partyDisplay[m_selectedCharacters[0]].Name, (int)((x * scaler) + (35 * scaler)), (int)((y - 30) * scaler), sdl_text_color);
		}
		if (m_selectedCharacters.size() > 1)
		{
			renderDisplayString(m_partyDisplay[m_selectedCharacters[1]].Name, (int)((x * scaler) + (35 * scaler)), (int)((y - 17) * scaler), sdl_text_color);
		}
		if (m_selectedCharacters.size() > 2)
		{
			renderDisplayString(m_partyDisplay[m_selectedCharacters[2]].Name, (int)(((x + 89) * scaler) + (35 * scaler)), (int)((y - 30) * scaler), sdl_text_color);
		}
		if (m_selectedCharacters.size() > 3)
		{
			renderDisplayString(m_partyDisplay[m_selectedCharacters[3]].Name, (int)(((x + 89) * scaler) + (35 * scaler)), (int)((y - 17) * scaler), sdl_text_color);
		}
	}

	SDL_SetRenderDrawColor(m_renderer, 255, 255, 255, 255);
	
	for (int i = 1; i < 21; ++i)
	{
		if (m_misc->m_Player[i][16])
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
		renderDisplayString(m_partyDisplay[i - 1].Number, (int)((x * scaler) + (26 * scaler)), (int)(y * scaler), sdl_text_color, 1);

		if (m_partyDisplay[i - 1].Name)
		{
			sdl_text_color.r = 255;
			sdl_text_color.g = 255;
			sdl_text_color.b = 255;

			if (m_misc->m_Player[i][16])
			{
				sdl_text_color.r = 255;
				sdl_text_color.g = 255;
				sdl_text_color.b = 0;
			}
			renderDisplayString(m_partyDisplay[i - 1].Name, (int)(((x + 35) * scaler)), (int)(y * scaler), sdl_text_color);

			sdl_text_color.r = 255;
			sdl_text_color.g = 255;
			sdl_text_color.b = 255;

			renderDisplayString(m_partyDisplay[i - 1].Level, (int)(((x + 109) * scaler)), (int)(y * scaler), sdl_text_color);

			int statusVal = m_misc->m_Player[i][17];

			switch (statusVal)
			{
			case 'P':
				sdl_text_color.r = 64;
				sdl_text_color.g = 255;
				sdl_text_color.b = 64;
				break;
			case 'D':
				sdl_text_color.r = 255;
				sdl_text_color.g = 64;
				sdl_text_color.b = 64;
				break;
			case 'A':
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

			renderDisplayString(m_partyDisplay[i - 1].Desc, (int)(((x + 152) * scaler)), (int)(y * scaler), sdl_text_color);
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
	if (!m_misc->m_Party[6])
	{
		return false;
	}

	return true;
}

bool U3Resources::HandleAlert(SDL_Event& event)
{
	if (!m_AlertDlg.get())
	{
		return false;
	}
	m_AlertDlg->HandleEvent(event);
	m_AlertDlg->display();
	if (m_cleanupAlert)
	{
		m_alertReturn = m_AlertDlg->ReturnValue();
		m_AlertDlg.reset();
		m_cleanupAlert = false;
	}
	return m_AlertDlg.get();
}

bool U3Resources::U3Resources::HasAlert()
{

	return m_AlertDlg.get();
}

void U3Resources::LoadResource(std::string strFile)
{
}

SDL_FRect U3Resources::GetTileRectForIndex(short index)
{
	SDL_FRect theRect;

	theRect.y = (float)((index % 16) * (m_blockSize * 2));
	theRect.h = (float)(m_blockSize * 2);
	theRect.x = (float)((index / 16) * (m_blockSize * 4));
	/*if (gShapeSwapped[index])
	{
		theRect.x += (m_blockSize * 2);
	}*/
	theRect.w = (float)(m_blockSize * 2);
	return theRect;
}

int U3Resources::GetRealTile(int tilenum)
{
	int tileY = (tilenum / 2) % TILES_NUM_Y;
	int tileX = ((tilenum / 2) / TILES_NUM_Y) * 2;
	int realTile = tileX * TILES_NUM_Y + tileY;

	return realTile;
}

void U3Resources::DrawMasked(unsigned short shape, unsigned short x, unsigned short y)
{
	short shapSize;
	SDL_FRect FromRect;
	SDL_FRect ToRect;

	shapSize = m_blockSize * 2;
	int realTile = GetRealTile(shape);

	FromRect = GetTileRectForIndex(shape / 2);
	if (shape == 0x5D)   // door
	{
		realTile += 16;
	}
	ToRect.x = (float)(x * shapSize);
	ToRect.y = (float)(y * shapSize);
	ToRect.w = (float)(shapSize);
	ToRect.h = (float)(shapSize);

	SDL_RenderTexture(m_renderer, m_currentGraphics->tile_target[realTile], NULL, &ToRect);
}

void U3Resources::DrawTiles()
{
	unsigned char offset;
	short lastTile, final, shapSize;
	SDL_FRect shapeRect;
	SDL_FRect myRect;
	SDL_FRect offRect;


	SDL_SetRenderTarget(m_renderer, m_texDisplay);

	HideMonsters();

	offset = 0;
	lastTile = 255;
	final = m_blockSize * 22;
	shapSize = m_blockSize * 2;

	shapeRect.w = shapSize;
	shapeRect.h = shapSize;

	for (shapeRect.y = 0; shapeRect.y < final; shapeRect.y += shapSize)
	{
		for (shapeRect.x = 0; shapeRect.x < final; shapeRect.x += shapSize)
		{
			unsigned char tile = m_TileArray[offset];
			int realTile = GetRealTile(tile);
			if (lastTile != tile)
			{
				myRect = GetTileRectForIndex(tile / 2);
				if (tile == 0x5D)  // Door
				{
					realTile += 16;
				}
				lastTile = tile;
			}
			offRect = shapeRect;
			offset++;

			SDL_RenderTexture(m_renderer, m_currentGraphics->tile_target[realTile], NULL, &offRect);
		}
	}

	myRect.x = (float)m_blockSize;
	myRect.y = (float)m_blockSize;
	myRect.w = (float)final;
	myRect.h = (float)final;

	adjustRect(myRect);

	ShowMonsters();

	SDL_SetRenderTarget(m_renderer, NULL);

	SDL_RenderTexture(m_renderer, m_texDisplay, NULL, &myRect);
}

void U3Resources::HideMonsters()
{
	short mon, xm, ym, offset, value;
	if (m_misc->m_Party[2] == 1)
	{
		return;
	}
	if (m_misc->m_Party[2] != 0x80)
	{
		// first hide the actual monster tiles.
		for (mon = 0; mon < 32; mon++)
		{
			if (m_misc->m_Monsters[mon] != 0)
			{
				xm = (m_misc->m_Monsters[mon + XMON] - m_misc->m_stx);
				if (m_misc->m_Party[2] == 0)
				{
					xm = m_graphics->MapConstrain(xm);
				}
				ym = (m_misc->m_Monsters[mon + YMON] - m_misc->m_sty);
				if (m_misc->m_Party[2] == 0)
				{
					ym = m_graphics->MapConstrain(ym);
				}
				if (xm > -1 && xm < 11 && ym > -1 && ym < 11)
				{
					offset = ym * 11 + xm;
					if (m_TileArray[offset] != 72 && (m_TileArray[offset] < 120 || m_TileArray[offset] > 122))
					{
						unsigned char tileChar = m_TileArray[offset];
						m_graphics->m_maskRestoreArray[offset] = m_TileArray[offset];
						m_TileArray[offset] = m_misc->m_Monsters[TILEON + mon] / 2;
						m_graphics->m_maskArray[offset] = tileChar;    //Monsters[mon];
					}
				}
			}
		}
		// now hide anything else we'd like to show through.
		offset = 0;
		for (ym = 0; ym < 11; ym++)
		{
			for (xm = 0; xm < 11; xm++)
			{
				value = m_TileArray[offset];
				switch (value)
				{
				case 0x22:    // Jester in castle on water (breaks the Talk cursor!)
					m_graphics->m_maskRestoreArray[offset] = 0x22;
					m_graphics->m_maskArray[offset] = 0x22;
					m_TileArray[offset] = 0;
					break;
				case 0x74:    // Snake Bottom
					m_graphics->m_maskRestoreArray[offset] = (unsigned char)value;
					m_graphics->m_maskArray[offset] = (unsigned char)value;
					m_TileArray[offset] = 0;
					break;
				case 0x76:    // Snake Top
					m_graphics->m_maskRestoreArray[offset] = (unsigned char)value;
					m_graphics->m_maskArray[offset] = (unsigned char)value;
					m_TileArray[offset] = 0;
					break;
				case 0x7C:    // Shrine
					m_graphics->m_maskRestoreArray[offset] = (unsigned char)value;
					m_graphics->m_maskArray[offset] = (unsigned char)value;
					m_TileArray[offset] = m_misc->GetXYVal(m_misc->m_xpos + xm - 6, m_misc->m_ypos + ym - 5) / 2;
					//TileArray[offset] = TileArray[offset-1];
					break;
				case 0x16:    // Frigate
					m_graphics->m_maskRestoreArray[offset] = (unsigned char)value;
					m_graphics->m_maskArray[offset] = (unsigned char)value;
					m_TileArray[offset] = 0;
					break;
				case 0x12:    // Chest 1 or
				case 0x13:    // Chest 2
					m_graphics->m_maskRestoreArray[offset] = (unsigned char)value;
					m_graphics->m_maskArray[offset] = 0x12;
					value = (m_misc->GetXYVal(m_misc->m_xpos + xm - 5, m_misc->m_ypos + ym - 5) & 0x3) * 2;
					if (value == 0)
					{
						value = 0x10;
					}
					m_TileArray[offset] = (unsigned char)value;
					break;
				case 0x14:    // Horse
					m_graphics->m_maskRestoreArray[offset] = (unsigned char)value;
					m_graphics->m_maskArray[offset] = (unsigned char)value;
					m_TileArray[offset] = 2;
					break;
				case 0x18:    // Whirlpool
					m_graphics->m_maskRestoreArray[offset] = (unsigned char)value;
					m_graphics->m_maskArray[offset] = (unsigned char)value;
					m_TileArray[offset] = 0;
					break;
				case 0x5D:    // Door
				{
					m_graphics->m_maskRestoreArray[offset] = (unsigned char)value;
					m_graphics->m_maskArray[offset] = (unsigned char)value;
					int mon = m_misc->MonsterHere(m_misc->m_xpos + xm - 6, m_misc->m_ypos + ym - 5);
					short neighbor = (mon < 255) ? m_misc->m_Monsters[(mon + TILEON) % 256] / 2 : m_misc->GetXYVal(m_misc->m_xpos + xm - 6, m_misc->m_ypos + ym - 5) / 2;
					if (neighbor > 0x10)
					{
						neighbor = 0x02;
					}
					m_TileArray[offset] = (unsigned char)neighbor;
				}
				break;
				case 0x78:    // Magic ball
				case 0x7A:    // Fire ball
					m_graphics->m_maskRestoreArray[offset] = (unsigned char)value;
					m_graphics->m_maskArray[offset] = (unsigned char)value;
					m_TileArray[offset] = m_misc->m_gBallTileBackground;
				}
				offset++;
			}
		}
	}
	else   // it's combat, hide the monsters and the players.
	{
		for (mon = 0; mon < 8; mon++)
		{
			if (m_misc->m_MonsterHP[mon] > 0)
			{
				xm = m_misc->m_MonsterX[mon];
				ym = m_misc->m_MonsterY[mon];
				if (m_misc->GetXYTile(xm, ym) < 120 || m_misc->GetXYTile(xm, ym) > 122)
				{
					m_misc->PutXYTile(m_misc->m_MonsterTile[mon], xm, ym);
				}
			}
		}
		for (mon = 0; mon < 4; mon++)
		{
			if (m_misc->CheckAlive(mon))
			{
				xm = m_misc->m_CharX[mon];
				ym = m_misc->m_CharY[mon];
				value = m_misc->GetXYTile(xm, ym);
				if (value != 0x78 && value != 0x7A)
				{
					m_misc->PutXYTile(m_misc->m_CharTile[mon], xm, ym);
				}
			}
		}
		// other things to hide in combat
		offset = 0;
		for (ym = 0; ym < 11; ym++)
		{
			for (xm = 0; xm < 11; xm++)
			{
				value = m_TileArray[offset];
				switch (value)
				{
				case 0x78:    // Magic ball
				case 0x7A:    // Fire ball
					m_graphics->m_maskRestoreArray[offset] = (unsigned char)value;
					m_graphics->m_maskArray[offset] = (unsigned char)value;
					m_TileArray[offset] = m_misc->m_gBallTileBackground;
					break;
				}
				offset++;
			}
		}
	}
}

void U3Resources::ShowMonsters()
{
	short mon, xm, ym, value, offset;
	if (m_misc->m_Party[2] == 1)
	{
		return;
	}

	// Handle globally masked items.
	offset = 0;
	for (ym = 0; ym < 11; ym++)
	{
		for (xm = 0; xm < 11; xm++)
		{
			if (m_graphics->m_maskArray[offset] != 255)
			{
				m_TileArray[offset] = m_graphics->m_maskRestoreArray[offset];
				DrawMasked(m_graphics->m_maskArray[offset], xm, ym);
				m_graphics->m_maskArray[offset] = 255;
			}
			offset++;
		}
	}

	// Then either the outdoor party symbol ...
	if (m_misc->m_Party[2] != 0x80)
	{
		if (m_TileArray[0x3C] < 120 || m_TileArray[0x3C] > 122)
		{
			if (m_misc->m_Party[0] == 0x14 && m_misc->m_gHorseFacingEast)
			{
				m_misc->m_gShapeSwapped[10] = true;
			}
			DrawMasked(m_misc->m_Party[0], 5, 5);
			m_misc->m_gShapeSwapped[10] = false;
		}
	}
	else   // ... or in combat, draw masked mons & chars, restore tiles.
	{
		for (mon = 0; mon < 8; mon++)
		{
			if (m_misc->m_MonsterHP[mon] > 0)
			{
				xm = m_misc->m_MonsterX[mon];
				ym = m_misc->m_MonsterY[mon];
				unsigned char tileValue = (unsigned char)m_misc->m_gMonType;    // tile * 2
				if (m_misc->m_gMonVarType && m_misc->m_gMonType >= 46 && m_misc->m_gMonType <= 63)
				{
					tileValue = (((m_misc->m_gMonType / 2) - 23) * 2 + 79 + m_misc->m_gMonVarType) * 2;
				}
				if (m_misc->GetXYTile(xm, ym) < 120 || m_misc->GetXYTile(xm, ym) > 122)
				{
					DrawMasked(tileValue, xm, ym);
				}
				m_misc->PutXYTile(m_misc->m_gMonType, xm, ym);
			}
		}
		for (mon = 0; mon < 4; mon++)
		{
			if (m_misc->CheckAlive(mon) && (mon + 1) != m_misc->m_cHide)
			{
				xm = m_misc->m_CharX[mon];
				ym = m_misc->m_CharY[mon];
				value = m_misc->GetXYTile(xm, ym);
				if (value != 0x78 && value != 0x7A)
				{
					DrawMasked(m_misc->m_CharShape[mon], xm, ym);
					m_misc->PutXYTile(m_misc->m_CharShape[mon], xm, ym);
				}
			}
		}
	}
}

void U3Resources::updateGameTime(Uint64 deltaTime)
{
	if (m_wasMove)
	{
		m_elapsedMoveTime = 0;
		m_wasMove = false;
	}
	else
	{
		m_elapsedMoveTime += m_delta_time;
		if (m_elapsedMoveTime > MoveTime)
		{
			m_elapsedMoveTime %= m_delta_time;
			m_misc->Pass();
		}

		m_elapsedWindTime += deltaTime;
		if (m_elapsedWindTime > DelayWind)
		{
			m_updateWind = true;
			m_elapsedWindTime %= DelayWind;
		}
	}
}

void U3Resources::updateTime(Uint64 deltaTime)
{
	/*if (m_wasMove)
	{
		m_wasMove = true;
		m_misc->FinishAll();
	}*/
	m_delta_time = deltaTime;

	m_elapsedTimeDemo += deltaTime;
	m_elapsedTimeFlag += deltaTime;
	m_elapsedTimeAnimate += deltaTime;
	m_numUpdateFlag = (int)(m_elapsedTimeFlag / DelayFlags);
	m_numUpdateAnimate = (int)(m_elapsedTimeAnimate / DelayAnimate);
	int numUpdate = (int)(m_elapsedTimeDemo / DelayDemo);
	if (numUpdate > 1)
	{
		numUpdate = 1;
	}
	m_elapsedTimeDemo %= DelayDemo;
	m_elapsedTimeFlag %= DelayFlags;
	m_elapsedTimeAnimate %= DelayAnimate;
	m_curTickDemo += deltaTime;

	m_elapsedTimeScroll += deltaTime;
	m_elapsedTimeScroll %= DelayScroll;
	m_curTickScroll = m_elapsedTimeScroll;

	ScrollThings();
	AnimateTiles();
	TwiddleFlags();
	DoWind();

	/*if (!m_scrollArea->isUpdating() && !m_misc->m_checkDead && m_misc->m_inputType == InputType::Default)
	{
		if (!isInversed())
		{
			m_elapsedWindTime += deltaTime;
			if (m_elapsedWindTime > DelayWind)
			{
				m_updateWind = true;
				m_elapsedWindTime %= DelayWind;
			}
		}

		if (m_wasMove)
		{
			m_wasMove = false;
			m_elapsedMoveTime = 0;
			m_misc->Routine6E35();

			m_elapsedWindTime += deltaTime;
		}
		else
		{
			if (!isInversed())
			{
				m_elapsedMoveTime += m_delta_time;
				if (m_elapsedMoveTime > MoveTime)
				{
					m_elapsedMoveTime %= m_delta_time;
					m_misc->Pass();
					m_misc->Routine6E35();
					m_misc->FinishAll();
				}
			}
		}
	}

	m_fullUpdate = false;*/
}

void U3Resources::ShowChars(bool force) /* $7338 methinx */
{
	bool somethingChanged = false;
	short i, num, ros;
	SDL_FRect rect;
	static short oldStatus[4], oldHP[4], oldMaxHP[4], oldMana[4], oldFood[4], oldExp[4];

	for (i = 0; i < 4; i++)
	{
		bool thisChanged = false;
		rect.x = (float)(24 * m_blockSize);
		rect.w = (float)(15 * m_blockSize);
		rect.y = (float)(i * (m_blockSize * 4) + m_blockSize);
		rect.h = (float)(m_blockSize * 3);
		ros = m_misc->m_Party[6 + i];

		thisChanged = force;
		num = m_misc->m_Player[ros][17];
		if (num != oldStatus[i])
		{
			oldStatus[i] = num;
			thisChanged = true;
		}
		num = m_misc->m_Player[ros][25];
		if (num != oldMana[i])
		{
			oldMana[i] = num;
			thisChanged = true;
		}
		num = m_misc->m_Player[ros][26] * 256 + m_misc->m_Player[ros][27];    // hp
		if (num != oldHP[i])
		{
			oldHP[i] = num;
			thisChanged = true;
		}
		num = m_misc->m_Player[ros][28] * 256 + m_misc->m_Player[ros][29];    // max hp
		if (num != oldMaxHP[i])
		{
			oldMaxHP[i] = num;
			thisChanged = true;
		}
		num = m_misc->m_Player[ros][30] * 100 + m_misc->m_Player[ros][31];    // exp
		if (num != oldExp[i])
		{
			oldExp[i] = num;
			thisChanged = true;
		}
		num = m_misc->m_Player[ros][32] * 100 + m_misc->m_Player[ros][33];    // food
		if (num != oldFood[i])
		{
			oldFood[i] = num;
			thisChanged = true;
		}
		if (thisChanged)
		{
			RenderCharStats(i, rect);
			somethingChanged = true;
		}
	}
}

void U3Resources::DrawPortrait(char charNum)
{
	SDL_FRect fromRect;
	SDL_FRect toRect;
	SDL_FRect offRect;
	short rosNum, value;
	short rce = 0;
	short sx;
	char charRaces[5] = { 'H', 'E', 'D', 'B', 'F' };
	char usePortrait[12] = { 0, 1, 2, 3, 0, 3, 2, 2, 1, 2, 0, 0 };
	rosNum = m_misc->m_Party[6 + charNum];
	short clss = 0;
	for (value = 0; value < 11; value++)
	{
		if (m_misc->m_Player[rosNum][23] == m_misc->m_careerTable[value])
		{
			clss = usePortrait[value];
			break;
		}
	}
	for (value = 0; value < 5; value++)
	{
		if (m_misc->m_Player[rosNum][22] == charRaces[value])
		{
			rce = value;
			break;
		}
	}
	sx = 0;
	if (m_misc->m_Player[rosNum][24] == 'F')
	{
		sx = 1;
	}
	fromRect.x = (float)(((clss * 10) + (sx * 5) + rce) * (m_portraitWidth));
	fromRect.y = (float)0;
	fromRect.w = (float)(m_portraitWidth);
	fromRect.h = (float)(m_portraitHeight);

	toRect.x = 0;
	toRect.y = 0;
	toRect.w = (float)(m_blockSize * 2);
	toRect.h = (float)(m_blockSize * 3);

	offRect.x = 0;
	offRect.y = 0;
	offRect.w = (float)(m_blockSize * 2);
	offRect.h = (float)(m_blockSize * 3);

	SDL_RenderTexture(m_renderer, m_texPortraits, &fromRect, &offRect);

	SDL_SetRenderDrawBlendMode(m_renderer, SDL_BLENDMODE_BLEND);

	if (m_misc->m_Player[rosNum][17] == 'P')
	{
		SDL_SetRenderDrawColor(m_renderer, 0, 255, 0, 128);
		SDL_RenderFillRect(m_renderer, &offRect);
	}
	else if (m_misc->m_Player[rosNum][17] == 'D')
	{
		SDL_SetRenderDrawColor(m_renderer, 255, 0, 0, 128);
		SDL_RenderFillRect(m_renderer, &offRect);
	}
	else if (m_misc->m_Player[rosNum][17] == 'A')
	{
		SDL_SetRenderDrawColor(m_renderer, 255, 255, 255, 128);
		SDL_RenderFillRect(m_renderer, &offRect);
	}
	SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 0);
	SDL_SetRenderDrawBlendMode(m_renderer, SDL_BLENDMODE_NONE);
}

void U3Resources::RenderCharStats(short ch, SDL_FRect rect)
{
	SDL_FRect fromRect;
	SDL_FRect barRect;

	SDL_SetRenderTarget(m_renderer, m_texStats);
	SDL_RenderClear(m_renderer);

	short ros;
	short num;
	short maxnum;
	float scale;
	bool classic = false;
	GetPreference(U3PreferencesType::Classic_Appearance, classic);
	bool showPortraits = !classic;
	fromRect.x = 0;
	fromRect.y = 0;
	fromRect.w = m_blockSize * 15.0f;
	fromRect.h = m_blockSize * 3.0f;
	ros = m_misc->m_Party[ch + 6];
	std::string tempstr;
	short i;
	SDL_Color sdl_text_color = { 255, 255, 255 };

	if (m_misc->m_Player[ros][0]) // character here
	{
		int showPortraitVal = 0;
		if (!showPortraits)
		{
			tempstr += m_misc->m_Player[ros][17];
			renderString(tempstr, 14, 0, false);
			tempstr.clear();
		}
		else
		{
			showPortraitVal = 1;
			DrawPortrait((unsigned char)ch);
		}
		i = 0;
		while (i < 13 && m_misc->m_Player[ros][i] != 0)
		{
			tempstr += (m_misc->m_Player[ros][i] & 0x7F);
			i++;
		}
		if (classic)
		{
			renderString(tempstr, 7 - (int)(tempstr.size() / 2) + showPortraitVal, 0, false);
		}
		else
		{
			renderDisplayString(m_font, tempstr, (int)(rect.w / 2), 0, sdl_text_color, 2, false);
		}

		tempstr.clear();
		tempstr += m_misc->m_Player[ros][24];
		tempstr += m_misc->m_Player[ros][22];
		tempstr += m_misc->m_Player[ros][23];
		renderString(tempstr, 1 + showPortraitVal, 1, false);

		if (!classic) // draw bars & such
		{
			float textPos = (m_blockSize * 5.0f) + (m_blockSize * 2.25f);
			SDL_Color bar_color = { 128, 128, 128 };

			// Hit Points
			num = (m_misc->m_Player[ros][26] * 256) + (m_misc->m_Player[ros][27]);
			barRect.x = m_blockSize * 5.0f;
			barRect.y = m_blockSize * 2.0f + 1;
			barRect.w = m_blockSize * 4.5f;
			barRect.h = (m_blockSize * 3.0f - 4) - (m_blockSize * 2.0f + 1);
			maxnum = m_misc->m_Player[ros][28] * 256 + m_misc->m_Player[ros][29];
			scale = (float)(barRect.w) / (float)maxnum;

			if (num > maxnum)
			{
				num = maxnum;
			}

			// Paint bar
			SDL_SetRenderDrawColor(m_renderer, bar_color.r, bar_color.g, bar_color.b, 255);
			SDL_RenderFillRect(m_renderer, &barRect);

			float tempRight = (num * scale);
			if (tempRight > 1)
			{
				barRect.w = tempRight;
				bar_color.r = 255;
				bar_color.g = 48;
				bar_color.b = 48;
				SDL_SetRenderDrawColor(m_renderer, bar_color.r, bar_color.g, bar_color.b, 255);
				SDL_RenderFillRect(m_renderer, &barRect);
				// Highlight line
				SDL_FRect markRect;
				bar_color.r = 255;
				bar_color.g = 128;
				bar_color.b = 128;
				markRect.x = barRect.x + (m_blockSize / 16.0f);
				markRect.y = barRect.y + (m_blockSize / 16.0f);
				markRect.w = ((barRect.x + barRect.w) - (m_blockSize / 16.0f) - 1) - barRect.x;
				markRect.h = 1;
				SDL_SetRenderDrawColor(m_renderer, bar_color.r, bar_color.g, bar_color.b, 255);
				SDL_RenderFillRect(m_renderer, &markRect);
				// Shadow line
				bar_color.r = 192;
				bar_color.g = 0;
				bar_color.b = 0;
				markRect.y = (barRect.y + barRect.h) - (m_blockSize / 16.0f);
				markRect.h = (m_blockSize / 16.0f);
				SDL_SetRenderDrawColor(m_renderer, bar_color.r, bar_color.g, bar_color.b, 255);
				SDL_RenderFillRect(m_renderer, &markRect);
			}
			if (m_blockSize > 16)
			{
				sdl_text_color.r = 0;
				sdl_text_color.g = 0;
				sdl_text_color.b = 0;
				if (num < 51)
				{
					sdl_text_color.r = 255;
					sdl_text_color.g = 255;
					sdl_text_color.b = 255;
				}
				tempstr = std::to_string(num);
				tempstr += '/';
				tempstr += std::to_string(maxnum);

				renderDisplayString(m_font_9, tempstr, (int)textPos, (int)(m_blockSize * 2.5f), sdl_text_color, 4, false);
			}

			// Mana
			num = m_misc->m_Player[ros][25];
			maxnum = (char)m_misc->MaxMana((char)ros);
			barRect.x = m_blockSize * 5.0f;
			barRect.y = m_blockSize + 1.0f;
			barRect.w = m_blockSize * 4.5f;
			barRect.h = (m_blockSize * 3.0f - 4) - (m_blockSize * 2.0f + 1);
			if (maxnum > 0)
			{
				scale = (float)(barRect.w) / (float)maxnum;
			}
			else
			{
				scale = 0;
			}

			if (num > maxnum)
			{
				num = maxnum;
			}

			// Paint bar
			bar_color.r = 128;
			bar_color.g = 128;
			bar_color.b = 128;
			SDL_SetRenderDrawColor(m_renderer, bar_color.r, bar_color.g, bar_color.b, 255);
			SDL_RenderFillRect(m_renderer, &barRect);
			SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 0);

			tempRight = (num * scale);
			if (maxnum > 0 && (tempRight > 1))
			{
				bar_color.r = 0;
				bar_color.g = 192;
				bar_color.b = 192;

				barRect.w = tempRight;

				SDL_SetRenderDrawColor(m_renderer, bar_color.r, bar_color.g, bar_color.b, 255);
				SDL_RenderFillRect(m_renderer, &barRect);

				// Highlight line
				SDL_FRect markRect;
				bar_color.r = 0;
				bar_color.g = 255;
				bar_color.b = 255;
				markRect.x = barRect.x + (m_blockSize / 16.0f);
				markRect.y = barRect.y + (m_blockSize / 16.0f);
				markRect.w = ((barRect.x + barRect.w) - (m_blockSize / 16.0f) - 1) - barRect.x;
				markRect.h = 1;
				SDL_SetRenderDrawColor(m_renderer, bar_color.r, bar_color.g, bar_color.b, 255);
				SDL_RenderFillRect(m_renderer, &markRect);
				// Shadow line
				bar_color.r = 0;
				bar_color.g = 110;
				bar_color.b = 110;
				markRect.y = (barRect.y + barRect.h) - (m_blockSize / 16.0f);
				markRect.h = (m_blockSize / 16.0f);
				SDL_SetRenderDrawColor(m_renderer, bar_color.r, bar_color.g, bar_color.b, 255);
				SDL_RenderFillRect(m_renderer, &markRect);
			}
			if (m_blockSize > 16)
			{
				sdl_text_color.r = 0;
				sdl_text_color.g = 0;
				sdl_text_color.b = 0;

				if (maxnum < 1)
				{
					constexpr int notmage = 74;
					if (m_plistMap["MoreMessages"].size() > notmage)
					{
						tempstr = m_plistMap["MoreMessages"][notmage];
					}
				}
				else
				{
					tempstr = std::to_string(num);
					tempstr += '/';
					tempstr += std::to_string(maxnum);
				}

				renderDisplayString(m_font_9, tempstr, (int)textPos, (int)(m_blockSize * 1.5f), sdl_text_color, 4, false);
			}
			// Level / experience
			num = (unsigned char)m_misc->m_Player[ros][31];
			maxnum = 100;
			textPos = (m_blockSize * 10.0f) + (m_blockSize * 2.25f);

			barRect.x = m_blockSize * 10.0f;
			barRect.y = m_blockSize + 1.0f;
			barRect.w = m_blockSize * 4.5f;
			barRect.h = (m_blockSize * 3.0f - 4) - (m_blockSize * 2.0f + 1);
			if (maxnum > 0)
			{
				scale = (float)(barRect.w) / (float)maxnum;
			}
			else
			{
				scale = 0;
			}

			if (num > maxnum)
			{
				num = maxnum;
			}
			// Paint bar
			bar_color.r = 128;
			bar_color.g = 128;
			bar_color.b = 36;
			SDL_SetRenderDrawColor(m_renderer, bar_color.r, bar_color.g, bar_color.b, 255);
			SDL_RenderFillRect(m_renderer, &barRect);

			tempRight = (num * scale);
			if (maxnum > 0 && (tempRight > 1))
			{
				bar_color.r = 192;
				bar_color.g = 48;
				bar_color.b = 48;

				barRect.w = tempRight;

				SDL_SetRenderDrawColor(m_renderer, bar_color.r, bar_color.g, bar_color.b, 255);
				SDL_RenderFillRect(m_renderer, &barRect);

				// Highlight line
				SDL_FRect markRect;
				bar_color.r = 255;
				bar_color.g = 255;
				bar_color.b = 128;
				markRect.x = barRect.x + (m_blockSize / 16.0f);
				markRect.y = barRect.y + (m_blockSize / 16.0f);
				markRect.w = ((barRect.x + barRect.w) - (m_blockSize / 16.0f) - 1) - barRect.x;
				markRect.h = 1;
				SDL_SetRenderDrawColor(m_renderer, bar_color.r, bar_color.g, bar_color.b, 255);
				SDL_RenderFillRect(m_renderer, &markRect);
				// Shadow line
				bar_color.r = 96;
				bar_color.g = 96;
				bar_color.b = 24;
				markRect.y = (barRect.y + barRect.h) - (m_blockSize / 16.0f);
				markRect.h = (m_blockSize / 16.0f);
				SDL_SetRenderDrawColor(m_renderer, bar_color.r, bar_color.g, bar_color.b, 255);
				SDL_RenderFillRect(m_renderer, &markRect);
			}
			if (m_blockSize > 16)
			{
				tempstr = LevelStr;
				tempstr += std::to_string(m_misc->m_Player[ros][30] + 1);

				renderDisplayString(m_font_9, tempstr, (int)textPos, (int)(m_blockSize * 1.5f), sdl_text_color, 4, false);
			}
			// Food
			textPos = m_blockSize * 10.0f;
			num = m_misc->m_Player[ros][32] * 100 + m_misc->m_Player[ros][33];
			if (num > 150)
			{
				sdl_text_color.r = 255;
				sdl_text_color.g = 255;
				sdl_text_color.b = 255;
			}
			else
			{
				scale = ((num - 50) / 100.0f) * 255;
				if (scale < 0)
				{
					scale = 0;
				}

				sdl_text_color.r = 255;
				sdl_text_color.g = (Uint8)scale;
				sdl_text_color.b = (Uint8)scale;
			}

			std::string strFood(FoodStr);
			strFood += std::to_string(num);
			renderDisplayString(m_font_9, strFood, (int)textPos, (int)(m_blockSize * 3), sdl_text_color, 6, false);

		}
		else // the old fashioned way
		{
			// Hit Points
			num = (m_misc->m_Player[ros][26] * 256) + (m_misc->m_Player[ros][27]);
			tempstr = std::string("H:");
			auto padded = std::to_string(num);
			padded.insert(0, 4U - std::min(std::string::size_type(4), padded.length()), '0');
			tempstr += padded;
			renderString(tempstr, 1 + showPortraitVal, 2, false);
			// Mana
			num = (m_misc->m_Player[ros][25]);
			tempstr = std::string("M:");
			padded = std::to_string(num);
			padded.insert(0, 2U - std::min(std::string::size_type(2), padded.length()), '0');
			tempstr += padded;
			renderString(tempstr, 5 + showPortraitVal, 1, false);
			// Level
			num = (m_misc->m_Player[ros][30] + 1);
			tempstr = std::string("L:");
			padded = std::to_string(num);
			padded.insert(0, 2U - std::min(std::string::size_type(2), padded.length()), '0');
			tempstr += padded;
			renderString(tempstr, 10 + showPortraitVal, 1, false);
			// Food
			num = m_misc->m_Player[ros][32] * 100 + m_misc->m_Player[ros][33];
			tempstr = std::string("F:");
			padded = std::to_string(num);
			padded.insert(0, 4U - std::min(std::string::size_type(4), padded.length()), '0');
			tempstr += padded;
			renderString(tempstr, 8 + showPortraitVal, 2, false);
		}
	}
	adjustRect(rect);
	SDL_SetRenderTarget(m_renderer, NULL);
	SDL_RenderTexture(m_renderer, m_texStats, NULL, &rect);
	SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 0);
}

void U3Resources::UPrint(std::string gString, char x, char y, bool autoadjust)
{
	renderString(gString, x, y, autoadjust);
}

void U3Resources::DrawPrompt()
{
	m_graphics->DrawFramePiece(8, 23, 23);
}

void U3Resources::DrawInverses(Uint64 delta_time)
{
	SDL_FRect myRect;
	bool incrementTime = false;

	SDL_BlendMode blendmode_sub = SDL_ComposeCustomBlendMode(SDL_BLENDFACTOR_SRC_ALPHA, SDL_BLENDFACTOR_ONE, SDL_BLENDOPERATION_SUBTRACT,
		SDL_BLENDFACTOR_ZERO, SDL_BLENDFACTOR_ONE, SDL_BLENDOPERATION_SUBTRACT);

	SDL_BlendMode blendmode_add = SDL_ComposeCustomBlendMode(SDL_BLENDFACTOR_SRC_ALPHA, SDL_BLENDFACTOR_ONE, SDL_BLENDOPERATION_ADD,
		SDL_BLENDFACTOR_ZERO, SDL_BLENDFACTOR_ONE, SDL_BLENDOPERATION_ADD);

	for (int index = 0; index < 4; ++index)
	{
		if (m_inverses.character_num[index] == true)
		{
			SDL_SetRenderDrawBlendMode(m_renderer, blendmode_sub);
			myRect.x = (float)(31 * m_blockSize);
			myRect.y = (float)(index * (m_blockSize * 4));
			myRect.w = (float)m_blockSize;
			myRect.h = (float)m_blockSize;

			adjustRect(myRect);

			SDL_SetRenderDrawColor(m_renderer, 255, 255, 255, 255);
			SDL_RenderFillRect(m_renderer, &myRect);
			SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 0);
			SDL_SetRenderDrawBlendMode(m_renderer, SDL_BLENDMODE_NONE);
		}

		if (m_inverses.char_details[index] == true)
		{
			SDL_SetRenderDrawBlendMode(m_renderer, blendmode_sub);
			myRect.x = (float)(24 * m_blockSize);
			myRect.w = (float)(15 * m_blockSize);
			myRect.y = (float)(index * (m_blockSize * 4) + m_blockSize);
			myRect.h = (float)(m_blockSize * 3);

			adjustRect(myRect);

			SDL_SetRenderDrawColor(m_renderer, 255, 255, 255, 255);
			SDL_RenderFillRect(m_renderer, &myRect);
			SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 0);
			SDL_SetRenderDrawBlendMode(m_renderer, SDL_BLENDMODE_NONE);
			incrementTime = true;
		}
	}
	if (m_inverses.tiles)
	{
		m_isInversed = true;
		SDL_SetRenderDrawBlendMode(m_renderer, blendmode_sub);
		myRect.x = (float)(1 * m_blockSize);
		myRect.y = (float)(1 * m_blockSize);
		myRect.w = (float)m_blockSize * 22;
		myRect.h = (float)m_blockSize * 22;

		adjustRect(myRect);

		SDL_SetRenderDrawColor(m_renderer, 255, 255, 255, 255);
		SDL_RenderFillRect(m_renderer, &myRect);
		SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 0);
		SDL_SetRenderDrawBlendMode(m_renderer, SDL_BLENDMODE_NONE);
		incrementTime = true;
	}

	if (m_inverses.fill)
	{
		m_isInversed = true;
		SDL_SetRenderDrawBlendMode(m_renderer, SDL_BLENDMODE_BLEND);
		myRect.x = (float)(1 * m_blockSize);
		myRect.y = (float)(1 * m_blockSize);
		myRect.w = (float)m_blockSize * 22;
		myRect.h = (float)m_blockSize * 22;

		adjustRect(myRect);

		SDL_SetRenderDrawColor(m_renderer, m_inverses.color.r, m_inverses.color.g, m_inverses.color.b, m_inverses.color.a);
		SDL_RenderFillRect(m_renderer, &myRect);
		SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 0);
		SDL_SetRenderDrawBlendMode(m_renderer, SDL_BLENDMODE_NONE);
		incrementTime = true;
	}
	else if (m_inverses.additive)
	{
		m_isInversed = true;
		SDL_SetRenderDrawBlendMode(m_renderer, blendmode_add);
		myRect.x = (float)(1 * m_blockSize);
		myRect.y = (float)(1 * m_blockSize);
		myRect.w = (float)m_blockSize * 22;
		myRect.h = (float)m_blockSize * 22;

		adjustRect(myRect);

		SDL_SetRenderDrawColor(m_renderer, m_inverses.color.r, m_inverses.color.g, m_inverses.color.b, m_inverses.color.a);
		SDL_RenderFillRect(m_renderer, &myRect);
		SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 0);
		SDL_SetRenderDrawBlendMode(m_renderer, SDL_BLENDMODE_NONE);
		incrementTime = true;
	}

	if (incrementTime)
	{
		m_inverses.elapsedTileTime += delta_time;
	}
}

// A helper function to change Mac Rects to SDL_FRect
void U3Resources::GenerateRect(SDL_FRect* FromRect, int left, int top, int right, int bottom)
{
	FromRect->x = (float)left;
	FromRect->y = (float)top;
	FromRect->w = (float)right - left;
	FromRect->h = (float)bottom - top;
}

void U3Resources::ImageDisplay()
{
	SDL_Texture* curTexture = nullptr;
	switch (m_overrideImage)
	{
	case 1:
		break;
	case 2:
		break;
	case 3:
		break;
	case 4:
		curTexture = m_texSosariaMap;
		break;
	case 5:
		curTexture = m_texFountain;
		break;
	case 6:
		curTexture = m_texRod;
		break;
	case 7:
		curTexture = m_texShrine;
		break;
	case 8:
		curTexture = m_texTimeLord;
		break;
	default:
		m_overrideImage = -1;
		break;
	}
	if (curTexture != nullptr)
	{
		SDL_FRect myRect;
		myRect.x = (float)(1 * m_blockSize);
		myRect.y = (float)(1 * m_blockSize);
		myRect.w = (float)m_blockSize * 22;
		myRect.h = (float)m_blockSize * 22;

		adjustRect(myRect);

		SDL_RenderTexture(m_renderer, curTexture, NULL, &myRect);
	}
}

void U3Resources::setInversed(bool isInversed)
{
	m_isInversed = isInversed;
	if (!isInversed)
	{

		m_inverses.inverseTileTime = 0;
		m_inverses.elapsedTileTime = 0;
		if (!m_inverses.stayInversed)
		{
			m_inverses.tiles = false;
			m_inverses.additive = false;
			m_inverses.fill = false;
			for (int index = 0; index < 4; ++index)
			{
				m_inverses.char_details[index] = false;
			}
		}
	}
}

void U3Resources::CleanupPartyNames()
{
	for (int i = 1; i < 21; ++i)
	{
		if (m_partyDisplay[i - 1].Number)
		{
			TTF_DestroyText(m_partyDisplay[i - 1].Number);
			m_partyDisplay[i - 1].Number = nullptr;
		}
		if (m_partyDisplay[i - 1].Name)
		{
			TTF_DestroyText(m_partyDisplay[i - 1].Name);
			m_partyDisplay[i - 1].Name = nullptr;
		}
		if (m_partyDisplay[i - 1].Desc)
		{
			TTF_DestroyText(m_partyDisplay[i - 1].Desc);
			m_partyDisplay[i - 1].Desc = nullptr;
		}
		if (m_partyDisplay[i - 1].Level)
		{
			TTF_DestroyText(m_partyDisplay[i - 1].Level);
			m_partyDisplay[i - 1].Level = nullptr;
		}
	}
}

void U3Resources::CreatePartyNames()
{
	unsigned char c;

	CleanupPartyNames();

	for (int i = 1; i < 21; ++i)
	{
		std::string str_number = std::to_string(i);
		m_partyDisplay[i - 1].Number = TTF_CreateText(engine_surface, m_font_11, str_number.c_str(), 0);

		std::string strName;
		c = 0;
		while (m_misc->m_Player[i][c] > 22)
		{
			strName += m_misc->m_Player[i][c];
			c++;
			if (c > 64)
			{
				break;
			}
		}
		if (strName.size() > 0)
		{
			m_partyDisplay[i - 1].Name = TTF_CreateText(engine_surface, m_font_11, strName.c_str(), 0);

			std::string str_level = std::to_string(m_misc->m_Player[i][30] + 1);
			str_level = std::string("Lvl ") + str_level;

			m_partyDisplay[i - 1].Level = TTF_CreateText(engine_surface, m_font_11, str_level.c_str(), 0);

			std::string str_race(UnknownStr);
			std::string str_class(UnknownStr);
			std::string str_sex(UnknownStr);

			if (m_plistMap.find("Races") != m_plistMap.end())
			{
				int val = m_misc->m_Player[i][22];
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
				int val = m_misc->m_Player[i][23];
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
				int val = m_misc->m_Player[i][24];
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

				int statusVal = m_misc->m_Player[i][17];

				switch (statusVal)
				{
				case 'P':
					str_status = m_plistMap["MoreMessages"][63];
					break;
				case 'D':
					str_status = m_plistMap["MoreMessages"][64];
					break;
				case 'A':
					str_status = m_plistMap["MoreMessages"][65];
					break;
				default:
					break;
				}
			}

			std::string str_desc;
			if (str_status.size() > 0)
			{
				str_desc += str_status + std::string(" ");
			}
			str_desc += str_race + std::string(" ") + str_class + std::string(" ") + str_sex;
			m_partyDisplay[i - 1].Desc = TTF_CreateText(engine_surface, m_font_11, str_desc.c_str(), 0);
		}
	}
}
