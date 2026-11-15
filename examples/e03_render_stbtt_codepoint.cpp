#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_truetype.h"

#include <string>

static SDL_Window* window = nullptr;
static SDL_Renderer* renderer = nullptr;

static SDL_Texture* cp_texture = nullptr;

std::string get_relative_path(const std::string& path) {
  return SDL_GetBasePath() + path;
}

SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[]) {
  if (!SDL_Init(SDL_INIT_VIDEO)) {
    return SDL_APP_FAILURE;
  }
  window = SDL_CreateWindow("render stbtt codepoint", 640, 480, 0);
  if (!window) {
    return SDL_APP_FAILURE;
  }
  renderer = SDL_CreateRenderer(window, nullptr);
  if (!renderer) {
    return SDL_APP_FAILURE;
  }
  SDL_SetRenderVSync(renderer, 1);
  size_t font_data_size{};
  void* font_data = nullptr;
  font_data = SDL_LoadFile(get_relative_path("res/Noto_Sans/static/NotoSans-Regular.ttf").c_str(), &font_data_size);
  if (!font_data) {
    return SDL_APP_FAILURE;
  }
  stbtt_fontinfo font_info{};
  if (stbtt_InitFont(&font_info, static_cast<const unsigned char*>(font_data), 0) == 0) {
    return SDL_APP_FAILURE;
  }
  int w, h;
  unsigned char* cp_bitmap =
    stbtt_GetCodepointBitmap(&font_info, 0, stbtt_ScaleForPixelHeight(&font_info, 100), 'a', &w, &h, 0, 0);
  if (!cp_bitmap) {
    return SDL_APP_FAILURE;
  }
  SDL_Surface* cp_surface = SDL_CreateSurface(w, h, SDL_PIXELFORMAT_RGBA8888);
  if (!cp_surface) {
    return SDL_APP_FAILURE;
  }
  for (int i = 0; i < h; ++i) {
    Uint8* row = static_cast<Uint8*>(cp_surface->pixels) + i * cp_surface->pitch;
    for (int j = 0; j < w; ++j) {
      static_cast<Uint32*>(static_cast<void*>(row))[j] = 0xffffff00 | cp_bitmap[i * w + j];
    }
  }
  cp_texture = SDL_CreateTextureFromSurface(renderer, cp_surface);
  if (!cp_texture) {
    return SDL_APP_FAILURE;
  }
  SDL_SetTextureBlendMode(cp_texture, SDL_BLENDMODE_BLEND);

  SDL_DestroySurface(cp_surface);
  stbtt_FreeBitmap(cp_bitmap, font_info.userdata);
  SDL_free(font_data);

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
}
