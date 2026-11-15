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
  if (!SDL_Init(SDL_INIT_VIDEO)) {
    return SDL_APP_FAILURE;
  }
  window = SDL_CreateWindow("stream custom texture", 640, 480, 0);
  if (!window) {
    return SDL_APP_FAILURE;
  }
  renderer = SDL_CreateRenderer(window, nullptr);
  if (!renderer) {
    return SDL_APP_FAILURE;
  }
  SDL_SetRenderVSync(renderer, 1);
  pixels = static_cast<Color*>(SDL_malloc(640 * 480 * sizeof(Color)));
  if (!pixels) {
    return SDL_APP_FAILURE;
  }
  texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, 640, 480);
  if (!texture) {
    return SDL_APP_FAILURE;
  }
  SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_NONE);
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
  SDL_RenderClear(renderer);
  void* texture_pixels = nullptr;
  int texture_pitch{};
  if (SDL_LockTexture(texture, nullptr, &texture_pixels, &texture_pitch)) {
    for (int i = 0; i < 480; ++i) {
      SDL_memcpy(static_cast<Uint8*>(texture_pixels) + i * texture_pitch, &pixels[i * 640], 640 * sizeof(Color));
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
