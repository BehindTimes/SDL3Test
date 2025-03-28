#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include <iostream>

#include "U3Resources.h"
#include "UltimaGraphics.h"
#include "UltimaMacIF.h"
#include "UltimaMain.h"
#include "UltimaSound.h"

SDL_Window* window;
SDL_Renderer* renderer;
bool isFullScreen = false;
short screenOffsetX = 0;
short screenOffsetY = 0;

U3Resources m_resources;

void DoSplashScreen();
void MainLoop();
void Intro();
void Demo();
void MainMenu();
void Organize();

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

    m_resources.init(renderer);

    ToolBoxInit();

    WindowInit(0);

   // DoSplashScreen();
    MainLoop();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();

    return 0;
}

void DoSplashScreen()
{
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

bool changeMode = false;
int newMode = 1;
int oldMode = -1;

void MainLoop()
{
    MenuBarInit();
    CreateIntroData();
    Intro();
    if (oldMode != newMode)
    {
        switch (newMode)
        {
        case 1:
        {
            Uint64 curTick = SDL_GetTicks();
            m_resources.setTickCount(curTick, false);
        }
            break;
        default:
            break;
        }
    }

    while (newMode != 0)
    {
        switch (newMode)
        {
        case 1:
            Demo();
            break;
        case 2:
            MainMenu();
            break;
        case 3:
            Organize();
            break;
        case 4:
            newMode = 1;
            break;
        case 5:
            newMode = 1;
            break;
        default:
            newMode = 1;
            break;
        }
    }
}

void returnToView()
{
    changeMode = true;
    newMode = 1;
}

void organizeParty()
{
    changeMode = true;
    newMode = 3;
}

void changeOptions()
{
    changeMode = true;
    newMode = 4;
}

void journeyOnward()
{
    changeMode = true;
    newMode = 5;
}

void backToMenu()
{
    changeMode = true;
    newMode = 2;
}

void Organize()
{
    bool gInterrupt = false;
    bool quit = false;
    SDL_Event event;
    int mouseState = 0;
    bool updateMouse = false;

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

                    m_resources.CaclulateBlockSize();
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
            newMode = 0;
            break;
        }
        Uint64 curTick = SDL_GetTicks();

        SDL_SetRenderTarget(renderer, NULL);
        SDL_RenderClear(renderer);
        if (updateMouse)
        {
            m_resources.UpdateButtons(event.motion.x, event.motion.y, mouseState);
        }
        DrawFrame(3);
        DrawOrganizeMenu();
        SDL_RenderPresent(renderer);

        if (changeMode)
        {
            gInterrupt = true;
        }
    }
}

void MainMenu()
{
    bool gInterrupt = false;
    bool quit = false;
    SDL_Event event;
    int mouseState = 0;
    bool updateMouse = false;

    m_resources.SetButtonCallback(0, returnToView);
    m_resources.SetButtonCallback(1, organizeParty);
    m_resources.SetButtonCallback(2, changeOptions);
    m_resources.SetButtonCallback(8, journeyOnward);

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

                    m_resources.CaclulateBlockSize();
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
            newMode = 0;
            break;
        }
        Uint64 curTick = SDL_GetTicks();

        SDL_SetRenderTarget(renderer, NULL);
        SDL_RenderClear(renderer);
        DrawFrame(3);
        if (updateMouse)
        {
            m_resources.UpdateButtons(event.motion.x, event.motion.y, mouseState);
        }
        DrawMenu();
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

    while (1)
    {
        if (gInterrupt)
        {
            changeMode = true;
            newMode = 2;
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

                    m_resources.CaclulateBlockSize();
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
            newMode = 0;
            break;
        }

        Uint64 curTick = SDL_GetTicks();

        SDL_SetRenderTarget(renderer, NULL);
        SDL_RenderClear(renderer);
        DrawFrame(2);
        DrawDemoScreen(curTick);
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
    CaclulateBlockSize(blkSiz);*/

    while (1)
    {
        if (gInterrupt)
        {
            changeMode = true;
            newMode = 1;
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

					m_resources.CaclulateBlockSize();
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
            newMode = 0;
            break;
        }
        Uint64 curTick = SDL_GetTicks();

        SDL_SetRenderTarget(renderer, NULL);
        SDL_RenderClear(renderer);
        DrawFrame(3);
        FadeOnExodusUltima(curTick);
        WriteLordBritish(curTick);
        FightScene(curTick);
        SDL_RenderPresent(renderer);
    }
}