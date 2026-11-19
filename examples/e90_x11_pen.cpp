#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

static SDL_Window* window = nullptr;
static SDL_Renderer* renderer = nullptr;
static SDL_Surface* canvas_surface = nullptr;
static SDL_Texture* canvas_texture = nullptr;
static bool canvas_dirty = true;
static Uint32 stroke_color = 0;

static struct {
  bool down;
  float x, y;
} stroke = {};

static void stroke_stamp(float x, float y) {
  const float size = 4.0f;
  const SDL_Rect r =
    {(int)SDL_floorf(x - size / 2), (int)SDL_floorf(y - size / 2), (int)size, (int)size};
  SDL_FillSurfaceRect(canvas_surface, &r, stroke_color);
  canvas_dirty = true;
}

static void stroke_touch(bool down, float x, float y) {
  stroke = {down, x, y};
  if (down) {
    stroke_stamp(x, y);
  }
}

static void stroke_to(float x, float y) {
  const float dx = x - stroke.x, dy = y - stroke.y;
  const int steps = stroke.down ? SDL_max(1, (int)SDL_ceilf(SDL_sqrtf(dx * dx + dy * dy))) : 0;
  for (int i = 1; i <= steps; ++i) {
    const float t = (float)i / steps;
    stroke_stamp(stroke.x + dx * t, stroke.y + dy * t);
  }
  stroke.x = x;
  stroke.y = y;
}

static struct {
  bool exist, down, up;
  float x, y;
} pending = {};

static void x11_pen_motion(float x, float y) {
  if (pending.exist) {
    // fix the x11 pen input problem
    const float fx = SDL_floorf(x) + (pending.x - SDL_floorf(pending.x));
    const float fy = SDL_floorf(y) + (pending.y - SDL_floorf(pending.y));
    stroke_to(fx, fy);
    if (pending.down) {
      stroke_touch(true, fx, fy);
    }
    if (pending.up) {
      stroke_touch(false, fx, fy);
    }
  }
  pending = {true, false, false, x, y};
}

static void x11_pen_touch(bool down) {
  if (pending.exist) {
    (down ? pending.down : pending.up) = true;
  } else {
    stroke_touch(down, stroke.x, stroke.y);
  }
}

static void x11_pen_leave() {
  stroke.down = false;
  pending = {};
}

SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[]) {
  SDL_SetHint(SDL_HINT_PEN_MOUSE_EVENTS, "0");
  SDL_SetHint(SDL_HINT_PEN_TOUCH_EVENTS, "0");
  if (!SDL_Init(SDL_INIT_VIDEO)) {
    return SDL_APP_FAILURE;
  }
  window = SDL_CreateWindow("x11 pen", 640, 480, 0);
  if (!window) {
    return SDL_APP_FAILURE;
  }
  renderer = SDL_CreateRenderer(window, nullptr);
  if (!renderer) {
    return SDL_APP_FAILURE;
  }
  SDL_SetRenderVSync(renderer, 1);
  canvas_surface = SDL_CreateSurface(640, 480, SDL_PIXELFORMAT_RGBA8888);
  if (!canvas_surface) {
    return SDL_APP_FAILURE;
  }
  canvas_texture = SDL_CreateTexture(
    renderer,
    canvas_surface->format,
    SDL_TEXTUREACCESS_STREAMING,
    canvas_surface->w,
    canvas_surface->h
  );
  if (!canvas_texture) {
    return SDL_APP_FAILURE;
  }
  SDL_FillSurfaceRect(
    canvas_surface,
    nullptr,
    SDL_MapSurfaceRGBA(canvas_surface, 255, 255, 255, 255)
  );
  stroke_color = SDL_MapSurfaceRGBA(canvas_surface, 0, 0, 0, 255);
  return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event) {
  switch (event->type) {
  case SDL_EVENT_QUIT:
    return SDL_APP_SUCCESS;
  case SDL_EVENT_PEN_DOWN:
  case SDL_EVENT_PEN_UP:
    x11_pen_touch(event->ptouch.down);
    break;
  case SDL_EVENT_PEN_MOTION:
    x11_pen_motion(event->pmotion.x, event->pmotion.y);
    break;
  case SDL_EVENT_PEN_PROXIMITY_OUT:
  case SDL_EVENT_WINDOW_MOUSE_LEAVE:
    x11_pen_leave();
    break;
  }
  return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void* appstate) {
  if (canvas_dirty) {
    SDL_UpdateTexture(canvas_texture, nullptr, canvas_surface->pixels, canvas_surface->pitch);
    canvas_dirty = false;
  }
  SDL_RenderClear(renderer);
  SDL_RenderTexture(renderer, canvas_texture, nullptr, nullptr);
  SDL_RenderPresent(renderer);
  return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void* appstate, SDL_AppResult result) {
  SDL_DestroyTexture(canvas_texture);
  SDL_DestroySurface(canvas_surface);
}
