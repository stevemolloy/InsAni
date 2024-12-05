#include "raylib.h"

#include "plugin.h"

#define BACKGROUND_COLOUR (Color){0x28, 0x28, 0x28, 0xFF}

void plug_frame_update(PlugState state) {
  const char *text = "Hello World from the plugin!";

  int width = GetScreenWidth();
  int height = GetScreenHeight();

  Vector2 text_size = MeasureTextEx(state.font, text, state.fontsize, 0);
  Vector2 text_pos = {(width-text_size.x)/2, (height-text_size.y)/2};

  BeginDrawing();
  ClearBackground(BACKGROUND_COLOUR);
  DrawTextEx(state.font, text, text_pos, state.fontsize, 0, WHITE);
  EndDrawing();
}
