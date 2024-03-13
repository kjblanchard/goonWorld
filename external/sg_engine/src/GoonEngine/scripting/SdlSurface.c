#include <GoonEngine/gnpch.h>
#include <external/stb_image.h>
#include <GoonEngine/debug.h>
#include <GoonEngine/SdlSurface.h>
#include <GoonEngine/color.h>
#include <GoonEngine/rectangle.h>

SDL_Texture *g_BackgroundAtlas = NULL;
SDL_Rect* g_backgroundDrawRect = NULL;
extern SDL_Renderer *g_pRenderer;

void SetBackgroundAtlas(SDL_Texture *background)
{
    g_BackgroundAtlas = background;
}
void SetCameraRect(geRectangle *rect)
{
    g_backgroundDrawRect = (SDL_Rect*)rect;
}

static void flipImageVertically(unsigned char *data, int width, int height, int channels)
{
    int rowSize = width * channels;

    for (int i = 0; i < height / 2; ++i)
    {
        int rowIndex1 = i * rowSize;
        int rowIndex2 = (height - 1 - i) * rowSize;

        for (int j = 0; j < rowSize; ++j)
        {
            unsigned char temp = data[rowIndex1 + j];
            data[rowIndex1 + j] = data[rowIndex2 + j];
            data[rowIndex2 + j] = temp;
        }
    }
}

SDL_Surface *LoadSurfaceFromFile(const char *filePath, void **data)
{
    int req_format = STBI_rgb_alpha;
    int width, height, orig_format;
    *data = stbi_load(filePath, &width, &height, &orig_format, req_format);
#ifdef __EMSCRIPTEN__
    flipImageVertically((char *)(*data), width, height, orig_format);
#endif
    if (*data == NULL)
    {
        SDL_Log("Loading image failed: %s", stbi_failure_reason());
        exit(1);
    }

    int depth, pitch;
    Uint32 pixel_format;
    if (req_format == STBI_rgb)
    {
        depth = 24;
        pitch = 3 * width; // 3 bytes per pixel * pixels per row
        pixel_format = SDL_PIXELFORMAT_RGB24;
    }
    else
    { // STBI_rgb_alpha (RGBA)
        depth = 32;
        pitch = 4 * width;
        pixel_format = SDL_PIXELFORMAT_RGBA32;
    }

    SDL_Surface *surf = SDL_CreateRGBSurfaceWithFormatFrom((void *)*data, width, height,
                                                           depth, pitch, pixel_format);

    if (surf == NULL)
    {
        SDL_Log("Creating surface failed: %s", SDL_GetError());
        stbi_image_free(*data);
        exit(1);
    }

    if (!surf)
    {
        LogError("Could not create surface from data %s, Error:\n%s", filePath, SDL_GetError());
        return NULL;
    }
    // stbi_image_free(data);
    return surf;
}
void DestroyPixelData(void *data)
{
    stbi_image_free(data);
}

SDL_Surface *LoadTextureAtlas(int width, int height)
{
    SDL_Surface *atlasSurface = SDL_CreateRGBSurfaceWithFormat(0, width, height, 32, SDL_PIXELFORMAT_RGBA8888);
    // SDL_Surface *atlasSurface = SDL_CreateRGBSurface(0, width, height, 32, 0,0,0,0);
    if (!atlasSurface)
    {
        fprintf(stderr, "Could not create atlast surface, Error: %s", SDL_GetError());
        return NULL;
    }
    return atlasSurface;
}

void BlitSurface(
    SDL_Surface *srcSurface,
    geRectangle *srcRect,
    SDL_Surface *dstSurface,
    geRectangle *dstRect)
{
    // int result = SDL_BlitSurface(srcSurface, srcRect, dstSurface, dstRect);
    int result = SDL_BlitSurface(srcSurface, srcRect, dstSurface, dstRect);
    if (result)
    {
        fprintf(stderr, "Failed to blit surface %s", SDL_GetError());
    }
}
SDL_Texture *CreateTextureFromFile(const char *filename)
{
    void *data = NULL;
    SDL_Surface *surface = LoadSurfaceFromFile(filename, &data);
    SDL_Texture *texture = CreateTextureFromSurface(surface);
    stbi_image_free(data);
    return texture;
}
void DestroySurface(SDL_Surface *surface)
{
    SDL_FreeSurface(surface);
}

void DestroyTexture(SDL_Texture *texture)
{
    SDL_DestroyTexture(texture);
}

SDL_Texture *CreateTextureFromSurface(SDL_Surface *surface)
{
    SDL_Texture *texture = SDL_CreateTextureFromSurface(g_pRenderer, surface);
    if (texture == NULL)
    {
        fprintf(stderr, "Could not create texture, Error: %s", SDL_GetError());
        return NULL;
    }
    SDL_FreeSurface(surface); // We no longer need the surface after creating the texture
    return texture;
}

// void DrawTexture( SDL_Texture *texture, SDL_Rect *srcRect, SDL_Rect *dstRect)
void DrawTexture(SDL_Texture *texture, geRectangle *srcRect, geRectangle *dstRect, bool shouldFlip)
{

    SDL_Rect translatedDstRect;
    // translatedDstRect.x = g_backgroundDrawRect->x - dstRect->x;
    translatedDstRect.x =  dstRect->x - g_backgroundDrawRect->x;
    // translatedDstRect.y = g_backgroundDrawRect->y - dstRect->y;
    translatedDstRect.y = dstRect->y;
    translatedDstRect.w = dstRect->w;
    translatedDstRect.h = dstRect->h;
    // SDL_RenderCopy(g_pRenderer, texture, srcRect, dstRect);
    SDL_RenderCopyEx(g_pRenderer,
                     texture,
                     (SDL_Rect*)srcRect,
                     &translatedDstRect,
                     0,
                     NULL,
                     (shouldFlip) ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE);
    //  (shouldFlip) ? SDL_FLIP_HORIZONTAL | SDL_FLIP_VERTICAL : SDL_FLIP_NONE | SDL_FLIP_VERTICAL);
}

void geDrawDebugRect(geRectangle* dstRect, geColor* color)
{
    SDL_Rect translatedDstRect;
    // translatedDstRect.x = g_backgroundDrawRect->x - dstRect->x;
    translatedDstRect.x =  dstRect->x - g_backgroundDrawRect->x;
    // translatedDstRect.y = g_backgroundDrawRect->y - dstRect->y;
    translatedDstRect.y = dstRect->y;
    translatedDstRect.w = dstRect->w;
    translatedDstRect.h = dstRect->h;
    SDL_SetRenderDrawColor(g_pRenderer, color->R, color->G, color->B, color->A);
    SDL_RenderDrawRect(g_pRenderer, &translatedDstRect);
    SDL_SetRenderDrawColor(g_pRenderer, 100, 100, 100, 255);
}