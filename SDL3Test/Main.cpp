#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_mixer/SDL_mixer.h>

#include <iostream>

#include "U3Misc.h"
#include "U3Resources.h"
#include "UltimaGraphics.h"
#include "UltimaMacIF.h"
#include "UltimaMain.h"
#include "UltimaSound.h"
#include "UltimaSpellCombat.h"
#include "U3ScrollArea.h"
#include "U3Utilities.h"
#include "UltimaDungeon.h"

SDL_Window* window;
SDL_Renderer* renderer;
short screenOffsetX = 0;
short screenOffsetY = 0;
TTF_TextEngine* engine_surface = NULL;

bool changeMode = false;
GameMode newMode = GameMode::Intro;
GameMode oldMode = GameMode::Unknown;
Uint64 count = 0;
int fps = 0;

std::unique_ptr<U3Resources> m_resources;
std::unique_ptr<U3Misc> m_misc;
std::unique_ptr<U3Graphics> m_graphics;
std::unique_ptr<U3ScrollArea> m_scrollArea;
std::unique_ptr<U3Utilities> m_utilities;
std::unique_ptr<UltimaSpellCombat> m_spellCombat;
std::unique_ptr<UltimaDungeon> m_dungeon;
std::unique_ptr<U3Audio> m_audio;
SDL_Cursor* g_current_cursor = nullptr;

bool DoSplashScreen();
void MainLoop();
void Intro();
void Demo();
void MainMenu();
void Organize();
void JourneyOnward();
void Game();

void disperseParty(int button);
void formParty(int button);
void terminateCharacter(int button);
void removeCharacter(int button);
void backToMenu(int button);
void updateGame(Uint64 deltaTime);
void partyFormed(int button);
void backToOrganize(int button);
void createCharacterChooseSlot(int button);

int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[])
{
	m_resources = std::make_unique<U3Resources>();
	m_misc = std::make_unique<U3Misc>();
	m_graphics = std::make_unique<U3Graphics>();
	m_scrollArea = std::make_unique<U3ScrollArea>();
	m_utilities = std::make_unique<U3Utilities>();
	m_spellCombat = std::make_unique<UltimaSpellCombat>();
	m_dungeon = std::make_unique<UltimaDungeon>();
	m_audio = std::make_unique<U3Audio>();

	if (!SDL_Init(SDL_INIT_VIDEO))
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL: %s", SDL_GetError());
		return 3;
	}

	if (!SDL_Init(SDL_INIT_AUDIO))
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL_Mixer: %s", SDL_GetError());
		return 3;
	}

	if (!SDL_CreateWindowAndRenderer("Ultima 3 - LairWare", 1280, 768, SDL_WINDOW_OPENGL /*| SDL_WINDOW_HIGH_PIXEL_DENSITY*/, &window, &renderer))
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create window and renderer: %s", SDL_GetError());
		return 3;
	}

	if (!TTF_Init())
	{
		return -1;
	}

	if (!engine_surface)
	{
		engine_surface = TTF_CreateRendererTextEngine(renderer);
	}

	int result = 0;
	int flags = MIX_INIT_WAVPACK | MIX_INIT_MP3 | MIX_INIT_OGG;
	if (flags != (result = Mix_Init(flags)))
	{
		return 3;
	}

	SDL_AudioSpec curSpec{};
	curSpec.freq = 22050;
	curSpec.format = SDL_AUDIO_S16;
	curSpec.channels = 2;
	Mix_OpenAudio(0, &curSpec);
	// Allocate a number of channels
	Mix_AllocateChannels(8);

	//SDL_SetRenderVSync(renderer, 1);

	bool valid = m_resources->init(renderer);
	bool valid2 = m_audio->initMusic();

	if (valid && valid2)
	{
		if (DoSplashScreen())
		{
			MainLoop();
		}
	}

	SDL_SetRenderTarget(renderer, NULL);
	SDL_RenderClear(renderer);
	SDL_RenderPresent(renderer);

	m_audio.reset();
	m_dungeon.reset();
	m_spellCombat.reset();
	m_utilities.reset();
	m_scrollArea.reset();
	m_graphics.reset();
	m_misc.reset();
	m_resources.reset();
	auto errorValue = SDL_GetError();
	// For debugging purposes.  Shouldn't display anything
	std::cout << "Error = " << errorValue << std::endl;

	TTF_Quit();
	TTF_DestroyRendererTextEngine(engine_surface);

	if (g_current_cursor)
	{
		SDL_DestroyCursor(g_current_cursor);
		g_current_cursor = nullptr;
	}

	//Mix_FreeMusic(music);
	Mix_CloseAudio();
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}

bool DoSplashScreen()
{
	m_resources->loadPreferences();
	m_misc->GetMiscStuff(true);
	m_misc->GetRoster();
	m_misc->GetParty();

	m_resources->changeTheme(m_resources->m_preferences.theme);
	SDL_SetWindowFullscreen(window, m_resources->m_preferences.full_screen);

	m_resources->CalculateBlockSize();
	/* OpenChannel();
	 SetUpFont();
	 DisableMenus();
	 SetUpGWorlds();
	 GetFont();
	 GetGraphics();
	 GetMiscStuff(100);
	 GetRoster();
	 GetParty();
	 SetUpSpeech();
	 SetUpMusic();
	 SetUpDragRect();
	 InitMacro();
	 InitCursor();
	 ObscureCursor();*/
	return true;
}

static void CreateButtonCallbacks()
{
	m_resources->SetButtonCallback(3, createCharacterChooseSlot);
	m_resources->SetButtonCallback(4, terminateCharacter);
	m_resources->SetButtonCallback(5, formParty);
	m_resources->SetButtonCallback(6, disperseParty);
	m_resources->SetButtonCallback(7, backToMenu);
}

void MainLoop()
{
	m_graphics->CreateIntroData();
	m_graphics->CreateMenuData();
	CreateButtonCallbacks();
	Intro();

	m_misc->m_zp[0xCF] = 0;
	m_misc->m_zp[0x10] = 0;
	m_audio->m_nextSong = 0;
	m_audio->m_currentSong = 0;

	while (newMode != GameMode::Unknown)
	{
		ReflectNewCursor(-1);

		if (oldMode != newMode)
		{
			if (oldMode == GameMode::Game) // Need to do a bunch of cleanup here
			{
				if (!m_resources->loadDemo())
				{
					break;
				}
				if (!m_misc->OpenRstr())
				{
					break;
				}
				DoSplashScreen();
				m_audio->m_currentSong = 0;
				m_audio->m_nextSong = 0;
				m_graphics->CreateIntroData();
				m_graphics->CreateMenuData();
				CreateButtonCallbacks();
				m_scrollArea->Clear();
				m_resources->m_elapsedTimeDemo = 0;
				m_resources->m_elapsedTimeFlag = 0;
				m_resources->m_elapsedTimeAnimate = 0;
				m_resources->m_curTickDemo = 0;
				m_resources->m_demoDelay = 0;
				m_resources->m_demoptr = 0;

				m_misc->m_zp[0xCF] = 0;
				m_misc->m_zp[0x10] = 0;
			}

			switch (newMode)
			{
			case GameMode::MainMenu:
				m_audio->m_currentSong = 0;
				m_audio->m_nextSong = 0;
				m_audio->musicUpdate();
				break;
			case GameMode::Intro:
				m_graphics->m_playFade1 = true;
				m_graphics->m_playFade2 = true;
				m_graphics->m_fadeExodus = false;
				m_graphics->m_fadeUltima = false;
				m_graphics->m_writeLordBritish = false;
				m_graphics->m_curIntro = IntroEnum::PRE_FIGHT;
				m_graphics->m_startFightTick = 0;
				break;
			case GameMode::Options:
				m_resources->CreateOptionsDlg();
					break;
			case GameMode::Demo:
			{
				m_audio->stopSfx();
				Uint64 curTick = SDL_GetTicks();
				m_resources->setTickCount(curTick, false);
			}
			break;
			case GameMode::Organize:
				m_graphics->CreateOrganizeData();
				break;
			case GameMode::Game:
			{
				m_audio->stopMusic();
				m_audio->stopSfx();
				m_graphics->m_curMode = U3GraphicsMode::Map;
				m_misc->m_gameMode = GameStateMode::Map;
				// TO DO: load the save game here
				m_misc->GetSosaria();
			}
			break;
			default:
				break;
			}

			oldMode = newMode;
		}

		switch (newMode)
		{
		case GameMode::Demo:
			Demo();
			break;
		case GameMode::MainMenu:
			MainMenu();
			break;
		case GameMode::Organize:
			Organize();
			break;
		case GameMode::Options:
			MainMenu();
			//newMode = GameMode::Demo;
			break;
		case GameMode::JourneyOnward:
			JourneyOnward();
			break;
		case GameMode::Game:
			Game();
			break;
		default:
			newMode = GameMode::Demo;
			break;
		}
	}
}

static void returnToView([[maybe_unused]] int button)
{
	changeMode = true;
	newMode = GameMode::Demo;
}

static void organizeParty([[maybe_unused]] int button)
{
	changeMode = true;
	newMode = GameMode::Organize;
}

static void changeOptions([[maybe_unused]] int button)
{
	changeMode = true;
	newMode = GameMode::Options;
}

static void journeyOnward([[maybe_unused]] int button)
{
	changeMode = true;
	newMode = GameMode::JourneyOnward;
}

void disperseParty([[maybe_unused]] int button)
{
	bool altMessage = false;
	char byte;
	for (byte = 1; byte < 21; byte++)
	{
		m_misc->m_Player[byte][16] = 0;
	}
	if (m_misc->m_Party[6] == 0)
	{
		altMessage = true;
	}
	for (byte = 0; byte < 16; byte++)
	{
		m_misc->m_Party[byte] = 0;
	}

	m_misc->m_partyFormed = false;

	m_misc->PutParty();

	m_resources->SetButtonVisibility(5, true);
	m_resources->SetButtonVisibility(6, false);

	if (altMessage)
	{
		m_graphics->m_obsCurMode = OrganizeBottomScreen::DispersedNoOne;
	}
	else
	{
		m_graphics->m_obsCurMode = OrganizeBottomScreen::Dispersed;
	}
}

void createCharacterChooseSlot([[maybe_unused]] int button)
{
	bool hasSpace = m_resources->CheckRosterSpace();
	m_graphics->m_obsCurMode = OrganizeBottomScreen::CreateCharacterChooseSlot;

	if (!hasSpace)
	{
		m_resources->CreateAlertMessage(6);
	}
	else
	{
		m_resources->SetButtonCallback(7, backToOrganize);
	}
}

void terminateCharacter([[maybe_unused]] int button)
{
	m_graphics->m_obsCurMode = OrganizeBottomScreen::TerminateCharacter;
	m_resources->SetButtonCallback(4, removeCharacter);
	m_resources->SetButtonCallback(7, backToOrganize);
}

void removeCharacter([[maybe_unused]] int button)
{
	if (!m_resources->m_selectedCharacters.empty())
	{
		int curChar = m_resources->m_selectedCharacters[0] + 1;
		m_resources->m_selectedFormRect = -1;
		m_resources->m_selectedCharacters.clear();
		if (curChar > 0 && curChar < 21)
		{
			memset(m_misc->m_Player[curChar], 0, sizeof(unsigned char) * 65);
			m_misc->PutRoster();
			m_resources->CreatePartyNames();
		}
	}
}

void formParty([[maybe_unused]] int button)
{
	if (m_misc->m_Party[6] != 0)
	{
		m_graphics->m_obsCurMode = OrganizeBottomScreen::PartyFormedInUse;
		m_resources->SetButtonVisibility(3, false);
		m_resources->SetButtonVisibility(4, true);
		m_resources->SetButtonVisibility(5, false);
		m_resources->SetButtonVisibility(6, false);
		m_resources->SetButtonCallback(7, backToOrganize);
	}
	else
	{
		m_graphics->m_obsCurMode = OrganizeBottomScreen::FormParty;
		m_resources->SetButtonVisibility(3, false);
		m_resources->SetButtonVisibility(4, false);
		m_resources->SetButtonVisibility(5, false);
		m_resources->SetButtonVisibility(6, false);

		m_resources->SetButtonCallback(5, partyFormed);
		m_resources->SetButtonCallback(7, backToOrganize);
	}
}

void partyFormed([[maybe_unused]] int button)
{
	m_graphics->m_obsCurMode = OrganizeBottomScreen::None;
	m_misc->m_partyFormed = true;

	memset(m_misc->m_Party, 0, sizeof(unsigned char) * 64);

	for (size_t index = 0; index < m_resources->m_selectedCharacters.size(); ++index)
	{
		int curChar = m_resources->m_selectedCharacters[index] + 1;
		m_misc->m_Player[curChar][16] = 255;
		m_misc->m_Party[index + 6] = (unsigned char)curChar;
	}
	m_misc->m_Party[1] = (unsigned char)m_resources->m_selectedCharacters.size();

	m_misc->m_Party[2] = 0;
	m_misc->m_Party[0] = 0x7E;
	m_misc->m_Party[5] = 255;    // WTF is this?
	m_misc->m_xpos = 42;
	m_misc->m_ypos = 20;
	m_misc->m_Party[3] = (unsigned char)m_misc->m_xpos;
	m_misc->m_Party[4] = (unsigned char)m_misc->m_ypos;
	m_misc->PutParty();
	m_misc->PutRoster();
	m_misc->ResetSosaria();
	m_misc->GetMiscStuff(0);
	m_misc->PutMiscStuff();

	m_resources->m_selectedFormRect = -1;
	m_resources->SetButtonVisibility(3, true);
	m_resources->SetButtonVisibility(4, true);
	m_resources->SetButtonVisibility(5, !m_misc->m_partyFormed);
	m_resources->SetButtonVisibility(6, m_misc->m_partyFormed);
	m_resources->SetButtonVisibility(7, true);

	m_resources->SetButtonCallback(5, formParty);
	m_resources->SetButtonCallback(7, backToMenu);
	m_resources->m_selectedCharacters.clear();

	m_graphics->m_obsCurMode = OrganizeBottomScreen::PartyFormed;
}

void backToOrganize([[maybe_unused]] int button)
{
	m_resources->m_selectedCharacters.clear();
	m_graphics->m_obsCurMode = OrganizeBottomScreen::None;
	m_resources->m_selectedFormRect = -1;

	m_resources->SetButtonVisibility(3, true);
	m_resources->SetButtonVisibility(4, true);
	m_resources->SetButtonVisibility(5, !m_misc->m_partyFormed);
	m_resources->SetButtonVisibility(6, m_misc->m_partyFormed);
	m_resources->SetButtonVisibility(7, true);

	m_resources->SetButtonCallback(4, terminateCharacter);
	m_resources->SetButtonCallback(5, formParty);
	m_resources->SetButtonCallback(7, backToMenu);
}

void backToMenu([[maybe_unused]] int button)
{
	m_resources->CleanupPartyNames();
	changeMode = true;
	newMode = GameMode::MainMenu;
}

void MainMenu()
{
	bool gInterrupt = false;
	bool quit = false;
	SDL_Event event;
	int mouseState = 0;
	bool updateMouse = false;
	changeMode = false;

	m_resources->SetButtonCallback(0, returnToView);
	m_resources->SetButtonCallback(1, organizeParty);
	m_resources->SetButtonCallback(2, journeyOnward);
	m_resources->SetButtonCallback(8, changeOptions);

	while (1)
	{
		if (gInterrupt)
		{
			break;
		}

		updateMouse = false;

		SDL_PollEvent(&event);
		switch (event.type)
		{
		case SDL_EVENT_QUIT:
			quit = true;
			break;
		case SDL_EVENT_KEY_DOWN:
			if (event.key.mod & SDL_KMOD_ALT)
			{
				if (event.key.key == SDLK_RETURN)
				{
					m_resources->m_preferences.full_screen = !m_resources->m_preferences.full_screen;
					SDL_SetWindowFullscreen(window, m_resources->m_preferences.full_screen);
					SDL_SyncWindow(window);

					m_resources->CalculateBlockSize();
				}
				if (event.key.key >= SDLK_0 && event.key.key <= SDLK_9)
				{
					int mode = event.key.key - SDLK_0;
					m_resources->changeTheme(mode);
				}
			}
			else if (event.key.key == SDLK_ESCAPE)
			{
				quit = true;
			}
			else
			{
			}
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
		if (quit)
		{
			changeMode = true;
			newMode = GameMode::Unknown;
			break;
		}
		//Uint64 curTick = SDL_GetTicks();

		SDL_SetRenderTarget(renderer, NULL);
		SDL_RenderClear(renderer);
		m_graphics->DrawFrame(3);
		if (!m_resources->m_SetOptionsDlg)
		{
			if (updateMouse)
			{
				m_resources->UpdateButtons(event.motion.x, event.motion.y, mouseState);
			}
		}
		else
		{
			if (updateMouse)
			{
				if (m_resources->m_SetOptionsDlg->updateDialog(event.motion.x, event.motion.y, mouseState))
				{
					m_resources->m_SetOptionsDlg.reset();
					m_resources->m_cleanupAlert = false;
					changeMode = true;
					newMode = GameMode::MainMenu;
				}
			}
		}
		m_graphics->DrawMenu();
		if (m_resources->m_SetOptionsDlg)
		{
			m_resources->m_SetOptionsDlg->display();
		}
		SDL_RenderPresent(renderer);

		if (changeMode)
		{
			gInterrupt = true;
		}
	}
}

void Demo()
{
	bool gInterrupt = false;
	bool quit = false;
	SDL_Event event;
	changeMode = false;

	if (m_audio->m_currentSong == 0)
	{
		m_audio->m_nextSong = m_misc->m_demoSong;
	}
	m_audio->musicUpdate();

	while (1)
	{
		if (gInterrupt)
		{
			changeMode = true;
			newMode = GameMode::MainMenu;
			break;
		}

		SDL_PollEvent(&event);
		switch (event.type)
		{
		case SDL_EVENT_QUIT:
			quit = true;
			break;
		case SDL_EVENT_KEY_DOWN:
			if (event.key.mod & SDL_KMOD_ALT)
			{
				if (event.key.key == SDLK_RETURN)
				{
					m_resources->m_preferences.full_screen = !m_resources->m_preferences.full_screen;
					SDL_SetWindowFullscreen(window, m_resources->m_preferences.full_screen);
					SDL_SyncWindow(window);

					m_resources->CalculateBlockSize();
				}
				if (event.key.key >= SDLK_0 && event.key.key <= SDLK_9)
				{
					int mode = event.key.key - SDLK_0;
					m_resources->changeTheme(mode);
				}
			}
			else if (event.key.key == SDLK_ESCAPE)
			{
				quit = true;
			}
			else
			{
				gInterrupt = true;
			}
			break;
		case SDL_EVENT_MOUSE_BUTTON_DOWN:
			gInterrupt = true;
			break;
		default:
			break;
		}
		if (quit)
		{
			changeMode = true;
			newMode = GameMode::Unknown;
			break;
		}

		Uint64 curTick = SDL_GetTicks();

		SDL_SetRenderTarget(renderer, NULL);
		SDL_RenderClear(renderer);
		m_graphics->DrawFrame(2);
		m_graphics->DrawDemoScreen(curTick);
		m_resources->DemoUpdate(curTick);
		SDL_RenderPresent(renderer);

		if (m_audio->m_nextSong == m_audio->m_currentSong)
		{
			m_audio->m_nextSong++;
			if (m_audio->m_nextSong > 10)
			{
				m_audio->m_nextSong = 1;
			}
			m_misc->m_demoSong = m_audio->m_nextSong;
			m_audio->musicUpdate();
		}
	}
}

void Intro()
{
	bool gInterrupt = false;
	bool quit = false;
	SDL_Event event;

	/*isFullScreen = !isFullScreen;
	SDL_SetWindowFullscreen(window, isFullScreen);
	m_resources->CalculateBlockSize();*/

	while (1)
	{
		if (gInterrupt)
		{
			changeMode = true;
			newMode = GameMode::Demo;
			break;
		}
		SDL_PollEvent(&event);
		switch (event.type)
		{
		case SDL_EVENT_QUIT:
			quit = true;
			break;
		case SDL_EVENT_KEY_DOWN:
			if (event.key.mod & SDL_KMOD_ALT)
			{
				if (event.key.key == SDLK_RETURN)
				{
					m_resources->m_preferences.full_screen = !m_resources->m_preferences.full_screen;
					SDL_SetWindowFullscreen(window, m_resources->m_preferences.full_screen);
					SDL_SyncWindow(window);

					m_resources->CalculateBlockSize();
				}
				if (event.key.key >= SDLK_0 && event.key.key <= SDLK_9)
				{
					int mode = event.key.key - SDLK_0;
					m_resources->changeTheme(mode);
				}
			}
			else if (event.key.key == SDLK_ESCAPE)
			{
				quit = true;
			}
			else
			{
				gInterrupt = true;
			}
			break;
		case SDL_EVENT_MOUSE_BUTTON_DOWN:
			gInterrupt = true;
			break;
		default:
			break;
		}
		if (quit)
		{
			changeMode = true;
			newMode = GameMode::Unknown;
			break;
		}
		Uint64 curTick = SDL_GetTicks();

		SDL_SetRenderTarget(renderer, NULL);
		SDL_RenderClear(renderer);
		m_graphics->DrawFrame(3);
		m_graphics->FadeOnExodusUltima(curTick);
		m_graphics->WriteLordBritish(curTick);
		m_graphics->FightScene(curTick);
		SDL_RenderPresent(renderer);
	}
}


void Organize()
{
	bool gInterrupt = false;
	bool quit = false;
	SDL_Event event;
	int mouseState = 0;
	bool updateMouse = false;
	changeMode = false;
	bool hasAlert = false;

	while (1)
	{
		if (gInterrupt)
		{
			break;
		}

		updateMouse = false;

		SDL_PollEvent(&event);
		switch (event.type)
		{
		case SDL_EVENT_QUIT:
			quit = true;
			break;
		case SDL_EVENT_KEY_DOWN:
			if (event.key.mod & SDL_KMOD_ALT)
			{
				if (event.key.key == SDLK_RETURN)
				{
					m_resources->m_preferences.full_screen = !m_resources->m_preferences.full_screen;
					SDL_SetWindowFullscreen(window, m_resources->m_preferences.full_screen);
					SDL_SyncWindow(window);

					m_resources->CalculateBlockSize();
				}
				if (event.key.key >= SDLK_0 && event.key.key <= SDLK_9)
				{
					int mode = event.key.key - SDLK_0;
					m_resources->changeTheme(mode);
				}
			}
			else if (event.key.key == SDLK_ESCAPE)
			{
				quit = true;
			}
			else
			{
				switch (m_graphics->m_obsCurMode)
				{
				case OrganizeBottomScreen::CreateCharacterAborted:
				case OrganizeBottomScreen::CreateCharacterDone:
				case OrganizeBottomScreen::PartyFormed:
				case OrganizeBottomScreen::PartyFormedInUse:
				case OrganizeBottomScreen::DispersedNoOne:
				case OrganizeBottomScreen::Dispersed:
					m_graphics->m_obsCurMode = OrganizeBottomScreen::None;
					break;
				default:
					break;
				}
			}
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
		if (quit)
		{
			changeMode = true;
			newMode = GameMode::Unknown;
			break;
		}
		//Uint64 curTick = SDL_GetTicks();

		bool alertValid = false;
		if (m_resources->HasAlert())
		{
			hasAlert = true;
			alertValid = true;
		}

		SDL_SetRenderTarget(renderer, NULL);
		SDL_RenderClear(renderer);
		if (updateMouse)
		{
			switch (m_graphics->m_obsCurMode)
			{
			case OrganizeBottomScreen::CreateCharacterChooseSlot:
			{
				if (!hasAlert)
				{
					m_resources->UpdateCreateCharacterChooseSlot(event.motion.x, event.motion.y, mouseState);
					m_resources->UpdateButtons(event.motion.x, event.motion.y, mouseState);
				}
			}
			break;
			case OrganizeBottomScreen::CreateCharacter:
				m_resources->UpdateCreateCharacter(event.motion.x, event.motion.y, mouseState);
				break;
			case OrganizeBottomScreen::TerminateCharacter:
				m_resources->UpdateTerminateCharacter(event.motion.x, event.motion.y, mouseState);
				m_resources->UpdateButtons(event.motion.x, event.motion.y, mouseState);
				break;
			case OrganizeBottomScreen::FormParty:
				m_resources->UpdateFormParty(event.motion.x, event.motion.y, mouseState);
				m_resources->UpdateButtons(event.motion.x, event.motion.y, mouseState);
				break;
			case OrganizeBottomScreen::PartyFormed:
			case OrganizeBottomScreen::PartyFormedInUse:
			case OrganizeBottomScreen::DispersedNoOne:
			case OrganizeBottomScreen::Dispersed:
			case OrganizeBottomScreen::CreateCharacterAborted:
			case OrganizeBottomScreen::CreateCharacterDone:
				if (mouseState == 2)
				{
					m_resources->m_CreateCharacterDlg.reset();
					m_graphics->m_obsCurMode = OrganizeBottomScreen::None;
				}
				break;
			default:
				m_resources->UpdateButtons(event.motion.x, event.motion.y, mouseState);
				break;
			}
		}
		m_graphics->DrawFrame(3);
		m_graphics->DrawOrganizeMenu(event);
		if (hasAlert)
		{
			m_resources->HandleAlert(event);
		}
		SDL_RenderPresent(renderer);

		if (!alertValid)
		{
			if (hasAlert)
			{
				hasAlert = false;
				m_graphics->m_obsCurMode = OrganizeBottomScreen::None;
			}
		}

		if (changeMode)
		{
			gInterrupt = true;
		}
	}
}

void JourneyOnward()
{
	bool journey = m_resources->CheckJourneyOnward();
	bool quit = false;
	bool gInterrupt = false;
	bool updateMouse = false;
	SDL_Event event;
	//int mouseState = 0;
	changeMode = false;

	Uint64 startTick = SDL_GetTicks();
	Uint64 elapsedTime = 0;
	//const Uint64 gameDelay = 750;
	const Uint64 gameDelay = 0;

	//journey = false;

	if (!journey)
	{
		m_resources->CreateAlertMessage(7);
	}

	while (1)
	{
		if (gInterrupt)
		{
			break;
		}

		updateMouse = false;

		SDL_PollEvent(&event);
		switch (event.type)
		{
		case SDL_EVENT_QUIT:
			quit = true;
			break;
		case SDL_EVENT_KEY_DOWN:
			if (event.key.mod & SDL_KMOD_ALT)
			{
				if (event.key.key == SDLK_RETURN)
				{
					m_resources->m_preferences.full_screen = !m_resources->m_preferences.full_screen;
					SDL_SetWindowFullscreen(window, m_resources->m_preferences.full_screen);
					SDL_SyncWindow(window);

					m_resources->CalculateBlockSize();
				}
				if (event.key.key >= SDLK_0 && event.key.key <= SDLK_9)
				{
					int mode = event.key.key - SDLK_0;
					m_resources->changeTheme(mode);
				}
			}
			else if (event.key.key == SDLK_ESCAPE)
			{
				quit = true;
			}
			break;
		default:
			break;
		}
		if (quit)
		{
			newMode = GameMode::Unknown;
			break;
		}
		Uint64 curTick = SDL_GetTicks();
		elapsedTime += (curTick - startTick);
		startTick = curTick;

		SDL_SetRenderTarget(renderer, NULL);
		SDL_RenderClear(renderer);
		m_graphics->DrawFrame(3);
		m_resources->drawExodus(255);
		m_resources->CenterMessage(1, 15);

		if (journey)
		{
		}
		else
		{
			m_resources->CenterMessage(2, 21);
			bool alertValid = m_resources->HandleAlert(event);
			if (!alertValid)
			{
				gInterrupt = true;
				newMode = GameMode::MainMenu;
			}
		}

		SDL_RenderPresent(renderer);

		if (journey)
		{
			if (changeMode)
			{
				gInterrupt = true;
			}
			else if (elapsedTime > gameDelay)
			{
				gInterrupt = true;
				newMode = GameMode::Game;
			}
		}
	}
}

void Game()
{
#ifdef NDEBUG
#else
	/*m_misc->m_Player[1][34] = 0;
	m_misc->m_Player[2][34] = 0;
	m_misc->m_Player[3][34] = 0;
	m_misc->m_Player[4][34] = 0;
	m_misc->m_Player[1][33] = 0;
	m_misc->m_Player[2][33] = 0;
	m_misc->m_Player[3][33] = 0;
	m_misc->m_Player[4][33] = 0;
	m_misc->m_Player[1][32] = 0;
	m_misc->m_Player[2][32] = 0;
	m_misc->m_Player[3][32] = 0;
	m_misc->m_Player[4][32] = 0;*/
	//m_misc->m_xpos = 10;
	//m_misc->m_ypos = 10;
	//m_misc->m_Player[1][17] = 'A';
	m_misc->m_Player[1][14] = 0xFF;
	m_misc->m_Player[1][15] = 50;
	m_misc->m_Player[1][37] = 50;
	m_misc->m_Party[15] = 1;
#endif

	bool quit = false;
	bool gInterrupt = false;
	bool updateMouse = false;
	SDL_Event event;
	int mouseState = 0;
	changeMode = false;
	m_misc->m_lastCard = 0x1E;

	Uint64 startTick = SDL_GetTicks();
	Uint64 elapsedTime = 0;

	m_resources->ShowChars(true);
	m_misc->m_inputType = InputType::Default;
	m_graphics->m_staydead = false;

	m_audio->m_currentSong = 1;
	m_audio->m_nextSong = 1;
	m_audio->musicUpdate();

	ReflectNewCursor(-1);
	//CursorUpdate();

	while (1)
	{
		if (gInterrupt)
		{
			break;
		}

		updateMouse = false;

		SDL_PollEvent(&event);
		switch (event.type)
		{
		case SDL_EVENT_QUIT:
			quit = true;
			break;
		case SDL_EVENT_KEY_DOWN:
			if (event.key.mod & SDL_KMOD_ALT)
			{
				if (event.key.key == SDLK_RETURN)
				{
					m_resources->m_preferences.full_screen = !m_resources->m_preferences.full_screen;
					SDL_SetWindowFullscreen(window, m_resources->m_preferences.full_screen);
					SDL_SyncWindow(window);

					m_resources->CalculateBlockSize();
				}
				if (event.key.key >= SDLK_0 && event.key.key <= SDLK_9)
				{
					int mode = event.key.key - SDLK_0;
					m_resources->changeTheme(mode);
				}
			}
			else if (event.key.key == SDLK_ESCAPE)
			{
				quit = true;
			}
			else
			{
			}
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
			CursorUpdate(event.motion.x, event.motion.y);
			break;
		default:
			break;
		}
		if (quit)
		{
			changeMode = true;
			newMode = GameMode::Unknown;
			break;
		}
		Uint64 curTick = SDL_GetTicks();
		Uint64 deltaTime = curTick - startTick;
		startTick = curTick;

		elapsedTime += deltaTime;

		m_misc->m_gResurrect = false;

		SDL_SetRenderTarget(renderer, NULL);
		SDL_RenderClear(renderer);

		m_graphics->render(event, deltaTime);

		m_resources->displayFPS(fps);

		SDL_RenderPresent(renderer);

		updateGame(deltaTime);

		if (newMode != GameMode::Game)
		{
			gInterrupt = true;
		}

		if (changeMode)
		{
			gInterrupt = true;
		}
	}
}


Uint64 fps_elapsed_time = 0;

void updateGame(Uint64 deltaTime)
{
	count++;
	fps_elapsed_time += deltaTime;
	if (fps_elapsed_time > 1000)
	{
		fps = (int)(count / (fps_elapsed_time / 1000));
		fps_elapsed_time = 0;
		count = 0;
	}

	//if (m_graphics->m_curMode == U3GraphicsMode::Map ||
	//	m_graphics->m_curMode == U3GraphicsMode::Combat)
	{
		m_resources->updateTime(deltaTime);
	}
	m_resources->m_fullUpdate = false;
}
