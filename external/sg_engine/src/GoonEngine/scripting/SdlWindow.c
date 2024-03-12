#include <GoonEngine/gnpch.h>
#include <GoonEngine/debug.h>
#include <GoonEngine/SdlWindow.h>

SDL_Window *g_pWindow = 0;
SDL_Renderer *g_pRenderer = 0;
int g_refreshRate = 60;

/**
 * @brief
 * Lua params: string name, int window width, int window height
 *
 * @param L
 * @return int
 */
int CreateWindowAndRenderer(uint width, uint height, const char* windowName)
{
    g_pWindow = SDL_CreateWindow(windowName,
                                 SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                 width, height,
                                 SDL_WINDOW_SHOWN);
    // if the window creation succeeded create our renderer
    if (g_pWindow == NULL)
    {
        fprintf(stderr,"Window could not be created, Error: %s", SDL_GetError());
        return 0;
    }
    // g_pRenderer = SDL_CreateRenderer(g_pWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    g_pRenderer = SDL_CreateRenderer(g_pWindow, -1, SDL_RENDERER_ACCELERATED);
    if (g_pRenderer == NULL)
    {
        fprintf(stderr, "Renderer could not be created, Error: %s", SDL_GetError());
    }
    fprintf(stdout, "Created window\nWidth: %d, Height: %d", width, height);
    SDL_DisplayMode mode;
    SDL_GetWindowDisplayMode(g_pWindow, &mode);
    g_refreshRate = mode.refresh_rate ? mode.refresh_rate : 60;
    // g_refreshRate = 30;
    LogDebug("Refresh rate is set to %d", g_refreshRate);
    // g_refreshRate = 120;
    return 0;
}

SDL_Renderer *GetGlobalRenderer()
{
    return g_pRenderer;
}
SDL_Window *GetGlobalWindow()
{
    return g_pWindow;
}
