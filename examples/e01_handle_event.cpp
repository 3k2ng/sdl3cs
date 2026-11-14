#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

static SDL_Window* window = nullptr;
static SDL_Renderer* renderer = nullptr;

SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[]) {
  SDL_Init(SDL_INIT_VIDEO);
  window = SDL_CreateWindow("handle event", 640, 480, 0);
  renderer = SDL_CreateRenderer(window, nullptr);
  return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event) {
  switch (event->type) {
  case SDL_EVENT_QUIT:
    return SDL_APP_SUCCESS;

  case SDL_EVENT_TERMINATING:
  case SDL_EVENT_LOW_MEMORY:
  case SDL_EVENT_WILL_ENTER_BACKGROUND:
  case SDL_EVENT_DID_ENTER_BACKGROUND:
  case SDL_EVENT_WILL_ENTER_FOREGROUND:
  case SDL_EVENT_DID_ENTER_FOREGROUND:
  case SDL_EVENT_LOCALE_CHANGED:
  case SDL_EVENT_SYSTEM_THEME_CHANGED: {
    const SDL_CommonEvent common = event->common;
  } break;

  case SDL_EVENT_DISPLAY_ORIENTATION:
  case SDL_EVENT_DISPLAY_ADDED:
  case SDL_EVENT_DISPLAY_REMOVED:
  case SDL_EVENT_DISPLAY_MOVED:
  case SDL_EVENT_DISPLAY_DESKTOP_MODE_CHANGED:
  case SDL_EVENT_DISPLAY_CURRENT_MODE_CHANGED:
  case SDL_EVENT_DISPLAY_CONTENT_SCALE_CHANGED:
  case SDL_EVENT_DISPLAY_USABLE_BOUNDS_CHANGED: {
    const SDL_DisplayEvent display = event->display;
  } break;

  case SDL_EVENT_WINDOW_SHOWN:
  case SDL_EVENT_WINDOW_HIDDEN:
  case SDL_EVENT_WINDOW_EXPOSED:
  case SDL_EVENT_WINDOW_MOVED:
  case SDL_EVENT_WINDOW_RESIZED:
  case SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED:
  case SDL_EVENT_WINDOW_METAL_VIEW_RESIZED:
  case SDL_EVENT_WINDOW_MINIMIZED:
  case SDL_EVENT_WINDOW_MAXIMIZED:
  case SDL_EVENT_WINDOW_RESTORED:
  case SDL_EVENT_WINDOW_MOUSE_ENTER:
  case SDL_EVENT_WINDOW_MOUSE_LEAVE:
  case SDL_EVENT_WINDOW_FOCUS_GAINED:
  case SDL_EVENT_WINDOW_FOCUS_LOST:
  case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
  case SDL_EVENT_WINDOW_HIT_TEST:
  case SDL_EVENT_WINDOW_ICCPROF_CHANGED:
  case SDL_EVENT_WINDOW_DISPLAY_CHANGED:
  case SDL_EVENT_WINDOW_DISPLAY_SCALE_CHANGED:
  case SDL_EVENT_WINDOW_SAFE_AREA_CHANGED:
  case SDL_EVENT_WINDOW_OCCLUDED:
  case SDL_EVENT_WINDOW_ENTER_FULLSCREEN:
  case SDL_EVENT_WINDOW_LEAVE_FULLSCREEN:
  case SDL_EVENT_WINDOW_DESTROYED:
  case SDL_EVENT_WINDOW_HDR_STATE_CHANGED: {
    const SDL_WindowEvent window = event->window;
  } break;

  case SDL_EVENT_KEY_DOWN:
  case SDL_EVENT_KEY_UP: {
    const SDL_KeyboardEvent key = event->key;
  } break;

  case SDL_EVENT_TEXT_EDITING: {
    const SDL_TextEditingEvent edit = event->edit;
  } break;

  case SDL_EVENT_TEXT_INPUT: {
    const SDL_TextInputEvent input = event->text;
  } break;

  case SDL_EVENT_KEYMAP_CHANGED: {
    const SDL_CommonEvent common = event->common;
  } break;

  case SDL_EVENT_KEYBOARD_ADDED:
  case SDL_EVENT_KEYBOARD_REMOVED: {
    const SDL_KeyboardDeviceEvent kdevice = event->kdevice;
  } break;

  case SDL_EVENT_TEXT_EDITING_CANDIDATES: {
    const SDL_TextEditingCandidatesEvent edit_candidates = event->edit_candidates;
  } break;

  case SDL_EVENT_SCREEN_KEYBOARD_SHOWN:
  case SDL_EVENT_SCREEN_KEYBOARD_HIDDEN: {
    const SDL_CommonEvent common = event->common;
  } break;

  case SDL_EVENT_MOUSE_MOTION: {
    const SDL_MouseMotionEvent motion = event->motion;
  } break;

  case SDL_EVENT_MOUSE_BUTTON_DOWN:
  case SDL_EVENT_MOUSE_BUTTON_UP: {
    const SDL_MouseButtonEvent button = event->button;
  } break;

  case SDL_EVENT_MOUSE_WHEEL: {
    const SDL_MouseWheelEvent wheel = event->wheel;
  } break;

  case SDL_EVENT_MOUSE_ADDED:
  case SDL_EVENT_MOUSE_REMOVED: {
    const SDL_MouseDeviceEvent mdevide = event->mdevice;
  } break;

  case SDL_EVENT_JOYSTICK_AXIS_MOTION: {
    const SDL_JoyAxisEvent jaxis = event->jaxis;
  } break;

  case SDL_EVENT_JOYSTICK_BALL_MOTION: {
    const SDL_JoyBallEvent jball = event->jball;
  } break;

  case SDL_EVENT_JOYSTICK_HAT_MOTION: {
    const SDL_JoyHatEvent jhat = event->jhat;
  } break;

  case SDL_EVENT_JOYSTICK_BUTTON_DOWN:
  case SDL_EVENT_JOYSTICK_BUTTON_UP: {
    const SDL_JoyButtonEvent jbutton = event->jbutton;
  } break;

  case SDL_EVENT_JOYSTICK_ADDED:
  case SDL_EVENT_JOYSTICK_REMOVED: {
    const SDL_JoyDeviceEvent jdevice = event->jdevice;
  } break;

  case SDL_EVENT_JOYSTICK_BATTERY_UPDATED: {
    const SDL_JoyBatteryEvent jbattery = event->jbattery;
  } break;

  case SDL_EVENT_JOYSTICK_UPDATE_COMPLETE: {
    const SDL_JoyDeviceEvent jdevice = event->jdevice;
  } break;

  case SDL_EVENT_GAMEPAD_AXIS_MOTION: {
    const SDL_GamepadAxisEvent gaxis = event->gaxis;
  } break;

  case SDL_EVENT_GAMEPAD_BUTTON_DOWN:
  case SDL_EVENT_GAMEPAD_BUTTON_UP: {
    const SDL_GamepadButtonEvent gbutton = event->gbutton;
  } break;

  case SDL_EVENT_GAMEPAD_ADDED:
  case SDL_EVENT_GAMEPAD_REMOVED:
  case SDL_EVENT_GAMEPAD_REMAPPED: {
    const SDL_GamepadDeviceEvent gdevice = event->gdevice;
  } break;

  case SDL_EVENT_GAMEPAD_TOUCHPAD_DOWN:
  case SDL_EVENT_GAMEPAD_TOUCHPAD_MOTION:
  case SDL_EVENT_GAMEPAD_TOUCHPAD_UP: {
    const SDL_GamepadTouchpadEvent gtouchpad = event->gtouchpad;
  } break;

  case SDL_EVENT_GAMEPAD_SENSOR_UPDATE: {
    const SDL_GamepadSensorEvent gsensor = event->gsensor;
  } break;

  case SDL_EVENT_GAMEPAD_UPDATE_COMPLETE:
  case SDL_EVENT_GAMEPAD_STEAM_HANDLE_UPDATED: {
    const SDL_GamepadDeviceEvent gdevice = event->gdevice;
  } break;

  case SDL_EVENT_FINGER_DOWN:
  case SDL_EVENT_FINGER_UP:
  case SDL_EVENT_FINGER_MOTION:
  case SDL_EVENT_FINGER_CANCELED: {
    const SDL_TouchFingerEvent tfinger = event->tfinger;
  } break;

  case SDL_EVENT_PINCH_BEGIN:
  case SDL_EVENT_PINCH_UPDATE:
  case SDL_EVENT_PINCH_END: {
    const SDL_PinchFingerEvent pinch = event->pinch;
  } break;

  case SDL_EVENT_CLIPBOARD_UPDATE: {
    const SDL_ClipboardEvent clipboard = event->clipboard;
  } break;

  case SDL_EVENT_DROP_FILE:
  case SDL_EVENT_DROP_TEXT:
  case SDL_EVENT_DROP_BEGIN:
  case SDL_EVENT_DROP_COMPLETE:
  case SDL_EVENT_DROP_POSITION: {
    const SDL_DropEvent drop = event->drop;
  } break;

  case SDL_EVENT_AUDIO_DEVICE_ADDED:
  case SDL_EVENT_AUDIO_DEVICE_REMOVED:
  case SDL_EVENT_AUDIO_DEVICE_FORMAT_CHANGED: {
    const SDL_AudioDeviceEvent adevice = event->adevice;
  } break;

  case SDL_EVENT_SENSOR_UPDATE: {
    const SDL_SensorEvent sensor = event->sensor;
  } break;

  case SDL_EVENT_PEN_PROXIMITY_IN:
  case SDL_EVENT_PEN_PROXIMITY_OUT: {
    const SDL_PenProximityEvent pproximity = event->pproximity;
  } break;

  case SDL_EVENT_PEN_DOWN:
  case SDL_EVENT_PEN_UP: {
    const SDL_PenTouchEvent ptouch = event->ptouch;
  } break;

  case SDL_EVENT_PEN_BUTTON_DOWN:
  case SDL_EVENT_PEN_BUTTON_UP: {
    const SDL_PenButtonEvent pbutton = event->pbutton;
  } break;

  case SDL_EVENT_PEN_MOTION: {
    const SDL_PenMotionEvent pmotion = event->pmotion;
  } break;

  case SDL_EVENT_PEN_AXIS: {
    const SDL_PenAxisEvent paxis = event->paxis;
  } break;

  case SDL_EVENT_CAMERA_DEVICE_ADDED:
  case SDL_EVENT_CAMERA_DEVICE_REMOVED:
  case SDL_EVENT_CAMERA_DEVICE_APPROVED:
  case SDL_EVENT_CAMERA_DEVICE_DENIED: {
    const SDL_CameraDeviceEvent cdevice = event->cdevice;
  } break;

  case SDL_EVENT_RENDER_TARGETS_RESET:
  case SDL_EVENT_RENDER_DEVICE_RESET:
  case SDL_EVENT_RENDER_DEVICE_LOST: {
    const SDL_RenderEvent render = event->render;
  } break;

  case SDL_EVENT_POLL_SENTINEL: {
  } break;

  default:
    if (event->type >= SDL_EVENT_USER && event->type <= SDL_EVENT_LAST) {
    }
    break;
  }
  return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void* appstate) {
  SDL_RenderClear(renderer);
  SDL_RenderPresent(renderer);
  return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void* appstate, SDL_AppResult result) {
}
