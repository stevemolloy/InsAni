#include "raylib.h"
#include <math.h>

#include "plugin.h"

#define BACKGROUND_COLOUR (Color){0x28, 0x28, 0x28, 0xFF}

Vector2 vector2_add(Vector2 a, Vector2 b) {
  return (Vector2) {
    .x = a.x + b.x,
    .y = a.y + b.y
  };
}

Vector2 vector2_subtract(Vector2 a, Vector2 b) {
  return (Vector2) {
    .x = a.x - b.x,
    .y = a.y - b.y
  };
}

Vector2 rot_vect_around_center(Vector2 vect, Vector2 center, float angle) {
  vect = vector2_subtract(vect, center);
  angle *= PI / 180.0;
  Vector2 retval = {
    .x = vect.x * cos(angle) - vect.y * sin(angle),
    .y = vect.x * sin(angle) + vect.y * cos(angle),
  };
  retval = vector2_add(retval, center);
  return retval;
}

void plug_frame_update(PlugState state) {
  (void)state;
  int width = GetScreenWidth();
  int height = GetScreenHeight();
  Vector2 center = (Vector2) { .x = (float)width/2, .y = (float)height/2 };

  float app_time = GetTime();

  float outer_radius = 0.9 * (float)width/2;

  Vector2 rectA_origin = { .x = center.x - 40, .y = center.y - (outer_radius - 10), };
  float rectA_rot = 50 * app_time;
  Vector2 rectA_loc = rot_vect_around_center(rectA_origin, center, rectA_rot);
  Rectangle teamA_rect = {
    .x = rectA_loc.x, .y = rectA_loc.y,
    .height = 20, .width = 80
  };

  Vector2 rectB_origin = { .x = center.x - 40, .y = center.y - (outer_radius - 30), };
  float rectB_rot = -15 * app_time;
  Vector2 rectB_loc = rot_vect_around_center(rectB_origin, center, rectB_rot);
  Rectangle teamB_rect = {
    .x = rectB_loc.x, .y = rectB_loc.y,
    .height = 20, .width = 80
  };

  Vector2 rectC_origin = { .x = center.x - 40, .y = center.y - (outer_radius - 50), };
  float rectC_rot = -25 * app_time;
  Vector2 rectC_loc = rot_vect_around_center(rectC_origin, center, rectC_rot);
  Rectangle teamC_rect = {
    .x = rectC_loc.x, .y = rectC_loc.y,
    .height = 20, .width = 80
  };

  BeginDrawing();

  ClearBackground(BACKGROUND_COLOUR);

  DrawPoly(center, 20, outer_radius, 9.0, RAYWHITE);
  DrawPoly(center, 20, 0.8*outer_radius, 9.0, BACKGROUND_COLOUR);

  DrawRectanglePro(teamA_rect, (Vector2){0}, rectA_rot, RED);
  DrawRectanglePro(teamB_rect, (Vector2){0}, rectB_rot, BLUE);
  DrawRectanglePro(teamC_rect, (Vector2){0}, rectC_rot, GREEN);

  EndDrawing();
}
