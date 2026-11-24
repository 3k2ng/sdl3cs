#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_ttf/SDL_ttf.h>

static SDL_Window* window = nullptr;
static SDL_Renderer* renderer = nullptr;

static TTF_TextEngine* engine = nullptr;
static TTF_Font* font = nullptr;
static TTF_Text* text = nullptr;

constexpr int width = 640, height = 480;

SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[]) {
  if (!SDL_Init(SDL_INIT_VIDEO)) {
    return SDL_APP_FAILURE;
  }
  window = SDL_CreateWindow("text", width, height, 0);
  if (!window) {
    return SDL_APP_FAILURE;
  }
  renderer = SDL_CreateRenderer(window, nullptr);
  if (!renderer) {
    return SDL_APP_FAILURE;
  }
  SDL_SetRenderVSync(renderer, 1);
  if (!TTF_Init()) {
    return SDL_APP_FAILURE;
  }
  engine = TTF_CreateRendererTextEngine(renderer);
  if (!engine) {
    return SDL_APP_FAILURE;
  }
  char* font_file = nullptr;
  SDL_asprintf(&font_file, "%s%s", SDL_GetBasePath(), "res/Noto_Sans/static/NotoSans-Regular.ttf");
  font = TTF_OpenFont(font_file, 32);
  SDL_free(font_file);
  if (!font) {
    return SDL_APP_FAILURE;
  }
  text = TTF_CreateText(engine, font, "hello sdl ttf", 0);
  if (!text) {
    return SDL_APP_FAILURE;
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
  SDL_RenderClear(renderer);
  TTF_DrawRendererText(text, 0, 0);
  SDL_RenderPresent(renderer);
  return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void* appstate, SDL_AppResult result) {
  TTF_DestroyText(text);
  TTF_CloseFont(font);
  TTF_DestroyRendererTextEngine(engine);
  TTF_Quit();
}
