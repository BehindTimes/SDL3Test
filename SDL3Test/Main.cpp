#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

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
bool isFullScreen = false;
short screenOffsetX = 0;
short screenOffsetY = 0;
TTF_TextEngine* engine_surface = NULL;

bool changeMode = false;
GameMode newMode = GameMode::Intro;
GameMode oldMode = GameMode::Unknown;
Uint64 count = 0;
int fps = 0;

U3Resources m_resources;
U3Misc m_misc;
U3Graphics m_graphics;
U3ScrollArea m_scrollArea;
U3Utilities m_utilities;
UltimaSpellCombat m_spellCombat;
UltimaDungeon m_dungeon;

void DoSplashScreen();
void MainLoop();
void Intro();
void Demo();
void MainMenu();
void Organize();
void JourneyOnward();
void Game();

void updateGame(Uint64 deltaTime, bool wasMove);

int main(int argc, char* argv[])
{
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL: %s", SDL_GetError());
        return 3;
    }

    if (!SDL_CreateWindowAndRenderer("Ultima 3 - Lairware", 1280, 768, SDL_WINDOW_OPENGL, &window, &renderer))
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create window and renderer: %s", SDL_GetError());
        return 3;
    }

    SDL_SetRenderVSync(renderer, 1);

    bool valid = m_resources.init(renderer);

    if (valid)
    {
        ToolBoxInit();

        WindowInit(0);

        DoSplashScreen();
        MainLoop();
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();

    return 0;
}

void DoSplashScreen()
{
    m_misc.GetMiscStuff(false);
    m_misc.GetRoster();
    m_misc.GetParty();
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
}

void MainLoop()
{
    MenuBarInit();
    m_graphics.CreateIntroData();
    m_graphics.CreateMenuData();
    m_graphics.CreateOrganizeData();
    Intro();

    m_misc.m_zp[0xCF] = 0;
    m_misc.m_zp[0x10] = 0;
    
    while (newMode != GameMode::Unknown)
    {
        if (oldMode != newMode)
        {
            switch (newMode)
            {
            case GameMode::Demo:
            {
                Uint64 curTick = SDL_GetTicks();
                m_resources.setTickCount(curTick, false);
            }
            break;
            case GameMode::Game:
            {
                m_misc.GetSosaria();
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
            newMode = GameMode::Demo;
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

void returnToView([[maybe_unused]]int button)
{
    changeMode = true;
    newMode = GameMode::Demo;
}

void organizeParty([[maybe_unused]] int button)
{
    changeMode = true;
    newMode = GameMode::Organize;
}

void changeOptions([[maybe_unused]] int button)
{
    changeMode = true;
    newMode = GameMode::Options;
}

void journeyOnward([[maybe_unused]] int button)
{
    changeMode = true;
    newMode = GameMode::JourneyOnward;
}

void backToMenu([[maybe_unused]] int button)
{
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

    m_resources.SetButtonCallback(0, returnToView);
    m_resources.SetButtonCallback(1, organizeParty);
    m_resources.SetButtonCallback(2, journeyOnward);
    m_resources.SetButtonCallback(8, changeOptions);

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
                    isFullScreen = !isFullScreen;
                    SDL_SetWindowFullscreen(window, isFullScreen);
                    SDL_SyncWindow(window);

                    m_resources.CalculateBlockSize();
                }
                if (event.key.key >= SDLK_0 && event.key.key <= SDLK_9)
                {
                    int mode = event.key.key - SDLK_0;
                    m_resources.changeMode(mode);
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
        Uint64 curTick = SDL_GetTicks();

        SDL_SetRenderTarget(renderer, NULL);
        SDL_RenderClear(renderer);
        m_graphics.DrawFrame(3);
        if (updateMouse)
        {
            m_resources.UpdateButtons(event.motion.x, event.motion.y, mouseState);
        }
        m_graphics.DrawMenu();
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
                    isFullScreen = !isFullScreen;
                    SDL_SetWindowFullscreen(window, isFullScreen);
                    SDL_SyncWindow(window);

                    m_resources.CalculateBlockSize();
                }
                if (event.key.key >= SDLK_0 && event.key.key <= SDLK_9)
                {
                    int mode = event.key.key - SDLK_0;
                    m_resources.changeMode(mode);
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
        m_graphics.DrawFrame(2);
        m_graphics.DrawDemoScreen(curTick);
        m_resources.DemoUpdate(curTick);
        SDL_RenderPresent(renderer);
    }
}

void Intro()
{
    bool gInterrupt = false;
    bool quit = false;
    SDL_Event event;

    /*isFullScreen = !isFullScreen;
    SDL_SetWindowFullscreen(window, isFullScreen);
    m_resources.CalculateBlockSize();*/

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
					isFullScreen = !isFullScreen;
					SDL_SetWindowFullscreen(window, isFullScreen);
					SDL_SyncWindow(window);

					m_resources.CalculateBlockSize();
				}
				if (event.key.key >= SDLK_0 && event.key.key <= SDLK_9)
				{
					int mode = event.key.key - SDLK_0;
					m_resources.changeMode(mode);
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
        m_graphics.DrawFrame(3);
        m_graphics.FadeOnExodusUltima(curTick);
        m_graphics.WriteLordBritish(curTick);
        m_graphics.FightScene(curTick);
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

    m_resources.SetButtonCallback(7, backToMenu);

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
                    isFullScreen = !isFullScreen;
                    SDL_SetWindowFullscreen(window, isFullScreen);
                    SDL_SyncWindow(window);

                    m_resources.CalculateBlockSize();
                }
                if (event.key.key >= SDLK_0 && event.key.key <= SDLK_9)
                {
                    int mode = event.key.key - SDLK_0;
                    m_resources.changeMode(mode);
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
        Uint64 curTick = SDL_GetTicks();

        SDL_SetRenderTarget(renderer, NULL);
        SDL_RenderClear(renderer);
        if (updateMouse)
        {
            m_resources.UpdateButtons(event.motion.x, event.motion.y, mouseState);
        }
        m_graphics.DrawFrame(3);
        m_graphics.DrawOrganizeMenu();
        SDL_RenderPresent(renderer);

        if (changeMode)
        {
            gInterrupt = true;
        }
    }
}

void JourneyOnward()
{
    bool journey = m_resources.CheckJourneyOnward();
    bool quit = false;
    bool gInterrupt = false;
    bool updateMouse = false;
    SDL_Event event;
    int mouseState = 0;
    changeMode = false;

    Uint64 startTick = SDL_GetTicks();
    Uint64 elapsedTime = 0;
    //const Uint64 gameDelay = 750;
    const Uint64 gameDelay = 0;

    //journey = false;
    
    if (!journey)
    {
        m_resources.CreateAlertMessage(7);
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
                    isFullScreen = !isFullScreen;
                    SDL_SetWindowFullscreen(window, isFullScreen);
                    SDL_SyncWindow(window);

                    m_resources.CalculateBlockSize();
                }
                if (event.key.key >= SDLK_0 && event.key.key <= SDLK_9)
                {
                    int mode = event.key.key - SDLK_0;
                    m_resources.changeMode(mode);
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
        m_graphics.DrawFrame(3);
        m_resources.drawExodus(255);
        m_resources.CenterMessage(1, 15);

        if (journey)
        {
        }
        else
        {
            m_resources.CenterMessage(2, 21);
            bool alertValid = m_resources.HasAlert(event);
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
    //m_misc.m_xpos = 10;
    //m_misc.m_ypos = 10;
    bool quit = false;
    bool gInterrupt = false;
    bool updateMouse = false;
    SDL_Event event;
    int mouseState = 0;
    changeMode = false;

    Uint64 startTick = SDL_GetTicks();
    Uint64 elapsedTime = 0;
    bool wasMove = false;

    m_resources.ShowChars(true);

    while (1)
    {
        wasMove = false;
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
                    isFullScreen = !isFullScreen;
                    SDL_SetWindowFullscreen(window, isFullScreen);
                    SDL_SyncWindow(window);

                    m_resources.CalculateBlockSize();
                }
                if (event.key.key >= SDLK_0 && event.key.key <= SDLK_9)
                {
                    int mode = event.key.key - SDLK_0;
                    m_resources.changeMode(mode);
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
        Uint64 curTick = SDL_GetTicks();
        Uint64 deltaTime = curTick - startTick;
        startTick = curTick;

        elapsedTime += deltaTime;

        m_misc.m_gResurrect = false;

        SDL_SetRenderTarget(renderer, NULL);
        SDL_RenderClear(renderer);
        
        bool wasMove = false;
        m_graphics.render(event, deltaTime, wasMove);

        m_resources.displayFPS(fps);

        SDL_RenderPresent(renderer);
        
        updateGame(deltaTime, wasMove);

        if (changeMode)
        {
            gInterrupt = true;
        }
    }
}


Uint64 fps_elapsed_time = 0;

void updateGame(Uint64 deltaTime, bool wasMove)
{
    count++;
    fps_elapsed_time += deltaTime;
    if (fps_elapsed_time > 1000)
    {
        fps = (int)(count / (fps_elapsed_time / 1000));
        fps_elapsed_time = 0;
        count = 0;
    }

    if (m_graphics.m_curMode == U3GraphicsMode::Map)
    {
        m_resources.updateTime(deltaTime, wasMove);
    }
}
