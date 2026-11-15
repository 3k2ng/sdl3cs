#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_truetype.h"

#include <string>

static SDL_Window* window = nullptr;
static SDL_Renderer* renderer = nullptr;

static size_t font_data_size{};
static void* font_data = nullptr;
static stbtt_fontinfo font_info{};
static unsigned char* cp_bitmap = nullptr;
static SDL_Texture* cp_texture = nullptr;

std::string get_relative_path(const std::string& path) {
  return SDL_GetBasePath() + path;
}

SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[]) {
  SDL_Init(SDL_INIT_VIDEO);
  window = SDL_CreateWindow("load font", 640, 480, 0);
  renderer = SDL_CreateRenderer(window, nullptr);

  font_data = SDL_LoadFile(get_relative_path("res/Noto_Sans/static/NotoSans-Regular.ttf").c_str(), &font_data_size);
  stbtt_InitFont(&font_info, static_cast<const unsigned char*>(font_data), 0);

  int w, h;
  cp_bitmap = stbtt_GetCodepointBitmap(&font_info, 0, stbtt_ScaleForPixelHeight(&font_info, 100), 'a', &w, &h, 0, 0);

  cp_texture = SDL_CreateTexture(renderer, SDL_GetWindowPixelFormat(window), SDL_TEXTUREACCESS_STREAMING, w, h);
  SDL_Surface* cp_surface = nullptr;
  if (SDL_LockTextureToSurface(cp_texture, nullptr, &cp_surface)) {
    for (int i = 0; i < h; ++i) {
      for (int j = 0; j < w; ++j) {
        *(static_cast<Uint32*>(cp_surface->pixels) + i * w + j) =
          SDL_MapSurfaceRGBA(cp_surface, cp_bitmap[i * w + j], cp_bitmap[i * w + j], cp_bitmap[i * w + j], cp_bitmap[i * w + j]);
      }
    }
    SDL_UnlockTexture(cp_texture);
  }

  return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event) {
  if (event->type == SDL_EVENT_QUIT) {
    return SDL_APP_SUCCESS;
  }
  return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void* appstate) {
  SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
  SDL_RenderClear(renderer);
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  const SDL_FRect dstrect{
    .x = 0,
    .y = 0,
    .w = static_cast<float>(cp_texture->w),
    .h = static_cast<float>(cp_texture->h),
  };
  SDL_RenderTexture(renderer, cp_texture, nullptr, &dstrect);
  SDL_RenderPresent(renderer);
  return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void* appstate, SDL_AppResult result) {
  SDL_DestroyTexture(cp_texture);
  stbtt_FreeBitmap(cp_bitmap, font_info.userdata);
  SDL_free(font_data);
}
