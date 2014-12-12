#include <pebble.h>

static Window* window;
static TextLayer* text_layer;
static int is_hello = 1;

static AppSync sync;
static uint8_t sync_buffer[64];

enum WeatherKeys {
  WEATHER_TEMP_KEY = 0x0
};

static void select_clicked(ClickRecognizerRef recognizer, void* context) {
  if(is_hello == 1) {
    text_layer_set_text(text_layer, "Goodbye!");
    is_hello = 0;
  } else {
    text_layer_set_text(text_layer, "Hello World!");
    is_hello = 1;
  }
}

static void click_config_provider(void* context) {
  window_single_click_subscribe(BUTTON_ID_SELECT, select_clicked);
}

static void sync_error(DictionaryResult dict_error, 
                       AppMessageResult app_message_error, void *context) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "App Message Sync Error: %d", app_message_error);
}

static void sync_success(const uint32_t key, const Tuple* new_tuple, 
                         const Tuple* old_tuple, void* context) {
APP_LOG(APP_LOG_LEVEL_DEBUG, "App Sync Success %s", new_tuple->value->cstring);
  text_layer_set_text(text_layer, new_tuple->value->cstring);
}

static void window_load(Window* window) {
  Layer* window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);
  text_layer = text_layer_create((GRect) { .origin = { 0, 72 }, .size = { bounds.size.w, 20 } });
  text_layer_set_text(text_layer, "Hello World!");
  text_layer_set_text_alignment(text_layer, GTextAlignmentCenter);
  layer_add_child(window_layer, text_layer_get_layer(text_layer));

  Tuplet inital_values[] = {
    TupletCString(WEATHER_TEMP_KEY, "1234 Degrees")
  };

  app_sync_init(&sync, sync_buffer, sizeof(sync_buffer), inital_values,
                ARRAY_LENGTH(inital_values), sync_success, sync_error, NULL);
}

static void window_unload(Window* window) {
  text_layer_destroy(text_layer);
}

static void init(void) {
  window = window_create();
  window_set_click_config_provider(window, click_config_provider);
  window_set_window_handlers(window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload,
  });
  app_message_open(64, 64);
  static bool is_animated = true;
  window_stack_push(window, is_animated);
}

static void deinit(void) {
  window_destroy(window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}
