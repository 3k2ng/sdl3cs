#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_mixer/SDL_mixer.h>

static SDL_Window* window = nullptr;
static SDL_Renderer* renderer = nullptr;

static MIX_Mixer* mixer = nullptr;
static MIX_Audio* audio = nullptr;
static MIX_Track* track = nullptr;

constexpr int width = 640, height = 480;

SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[]) {
  if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)) {
    return SDL_APP_FAILURE;
  }
  window = SDL_CreateWindow("audio", width, height, 0);
  if (!window) {
    return SDL_APP_FAILURE;
  }
  renderer = SDL_CreateRenderer(window, nullptr);
  if (!renderer) {
    return SDL_APP_FAILURE;
  }
  SDL_SetRenderVSync(renderer, 1);
  if (!MIX_Init()) {
    return SDL_APP_FAILURE;
  }
  mixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, nullptr);
  if (!mixer) {
    return SDL_APP_FAILURE;
  }
  char* audio_file = nullptr;
  SDL_asprintf(&audio_file, "%s%s", SDL_GetBasePath(), "res/jsfxr_laser.wav");
  audio = MIX_LoadAudio(mixer, audio_file, true);
  SDL_free(audio_file);
  if (!audio) {
    return SDL_APP_FAILURE;
  }
  track = MIX_CreateTrack(mixer);
  if (!track) {
    return SDL_APP_FAILURE;
  }
  if (!MIX_SetTrackAudio(track, audio)) {
    return SDL_APP_FAILURE;
  }
  return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event) {
  if (event->type == SDL_EVENT_QUIT) {
    return SDL_APP_SUCCESS;
  } else if (
    event->type == SDL_EVENT_KEY_DOWN && event->key.key == SDLK_SPACE && !event->key.repeat
  ) {
    if (!MIX_TrackPlaying(track)) {
      MIX_PlayTrack(track, 0);
    }
  }
  return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void* appstate) {
  SDL_RenderClear(renderer);
  SDL_RenderPresent(renderer);
  return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void* appstate, SDL_AppResult result) {
  MIX_DestroyTrack(track);
  MIX_DestroyAudio(audio);
  MIX_DestroyMixer(mixer);
  MIX_Quit();
}
