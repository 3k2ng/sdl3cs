#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

static SDL_Window* window = nullptr;
static SDL_Renderer* renderer = nullptr;

static Uint64 last_ns;

constexpr int width = 640, height = 480;

SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[]) {
  if (!SDL_Init(SDL_INIT_VIDEO)) {
    return SDL_APP_FAILURE;
  }
  window = SDL_CreateWindow("delta time", width, height, 0);
  if (!window) {
    return SDL_APP_FAILURE;
  }
  renderer = SDL_CreateRenderer(window, nullptr);
  if (!renderer) {
    return SDL_APP_FAILURE;
  }
  SDL_SetRenderVSync(renderer, 1);
  last_ns = SDL_GetTicksNS();
  return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event) {
  if (event->type == SDL_EVENT_QUIT) {
    return SDL_APP_SUCCESS;
  }
  return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void* appstate) {
  const Uint64 now_ns = SDL_GetTicksNS();
  const float dt = (now_ns - last_ns) / static_cast<float>(SDL_NS_PER_SECOND);
  last_ns = now_ns;

  float mouse_x, mouse_y;
  const SDL_MouseButtonFlags mouse_button_flags = SDL_GetMouseState(&mouse_x, &mouse_y);

  const bool* keyboard_state = SDL_GetKeyboardState(nullptr);

  SDL_RenderClear(renderer);
  SDL_RenderPresent(renderer);
  return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void* appstate, SDL_AppResult result) {
}
