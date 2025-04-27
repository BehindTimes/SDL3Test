#include <SDL3/SDL_tray.h>
#include <SDL3/SDL_video.h>
#include <stdexcept>
#include <string>
#include "UltimaMacIF.h"

#include "UltimaIncludes.h"
#include "U3Resources.h"

extern SDL_Window* window;
extern std::unique_ptr<U3Resources> m_resources;


extern short screenOffsetX;
extern short screenOffsetY;

void ToolBoxInit()
{
}

void WindowInit([[maybe_unused]]short which)
{
    
}

void MenuBarInit()
{
}
