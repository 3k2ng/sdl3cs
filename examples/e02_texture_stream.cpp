#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

struct Color {
  Uint8 r, g, b, a;
};

static SDL_Window* window = nullptr;
static SDL_Renderer* renderer = nullptr;

static Color* pixels = nullptr;
static SDL_Texture* texture = nullptr;

SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[]) {
  SDL_Init(SDL_INIT_VIDEO);
  window = SDL_CreateWindow("texture stream", 640, 480, 0);
  renderer = SDL_CreateRenderer(window, nullptr);
  pixels = static_cast<Color*>(SDL_malloc(640 * 480 * sizeof(Uint32)));
  texture = SDL_CreateTexture(renderer, SDL_GetWindowPixelFormat(window), SDL_TEXTUREACCESS_STREAMING, 640, 480);
  for (int i = 0; i < 480; ++i) {
    for (int j = 0; j < 640; ++j) {
      Color& pixel = *(pixels + i * 640 + j);
      pixel.r = j * 255 / 640;
      pixel.g = i * 255 / 480;
      pixel.b = 0;
      pixel.a = 255;
    }
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
  Uint8 color_offset = SDL_GetTicks() * 256 / 2000;
  for (int i = 0; i < 480; ++i) {
    for (int j = 0; j < 640; ++j) {
      Color& pixel = *(pixels + i * 640 + j);
      pixel.r = (j + color_offset) % 256;
      pixel.g = (i + color_offset) % 256;
      pixel.b = 0;
      pixel.a = 255;
    }
  }
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  SDL_Surface* texture_surface = nullptr;
  if (SDL_LockTextureToSurface(texture, nullptr, &texture_surface)) {
    for (int i = 0; i < 480; ++i) {
      for (int j = 0; j < 640; ++j) {
        const Color& from_pixel = *(pixels + i * 640 + j);
        Uint32& to_pixel = *(static_cast<Uint32*>(texture_surface->pixels) + i * 640 + j);
        to_pixel = SDL_MapSurfaceRGBA(texture_surface, from_pixel.r, from_pixel.g, from_pixel.b, from_pixel.a);
      }
    }
    SDL_UnlockTexture(texture);
  }
  const SDL_FRect dstrect{.x = 0, .y = 0, .w = static_cast<float>(texture->w), .h = static_cast<float>(texture->h)};
  SDL_RenderTexture(renderer, texture, nullptr, &dstrect);
  SDL_RenderPresent(renderer);
  return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void* appstate, SDL_AppResult result) {
  SDL_DestroyTexture(texture);
  SDL_free(pixels);
}
