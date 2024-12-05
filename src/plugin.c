#include <stdio.h>
#include <math.h>
#include <assert.h>

#include "raylib.h"

#include "plugin.h"

#define BACKGROUND_COLOUR (Color){0x28, 0x28, 0x28, 0xFF}
#define TEAM_A_COLOUR RED
#define RECT_HEIGHT 15

// Action action_list[] = {
//   {.state=WORKING, .as.working=(Working){.where=2, .duration=1.0}},
//   {.state=MOVING, .as.moving=(Moving){.start_loc=2, .end_loc=4, .duration=0.2}},
//   {.state=WORKING, .as.working=(Working){.where=4, .duration=1.0}},
// };
// size_t num_frames = sizeof(action_list) / sizeof(action_list[0]);

float achromat_to_degrees(int achromat) {
  return 360.0 * ((float)(achromat - 1)) / 20.0;
}

Vector2 vector2_add(Vector2 a, Vector2 b) {
  return (Vector2) { .x = a.x + b.x, .y = a.y + b.y };
}

Vector2 vector2_subtract(Vector2 a, Vector2 b) {
  return (Vector2) { .x = a.x - b.x, .y = a.y - b.y };
}

Vector2 rot_vect_around_center(Vector2 vect, Vector2 center, float angle) {
  vect = vector2_subtract(vect, center);
  angle *= PI / 180.0;
  float C = cos(angle);
  float S = sin(angle);
  Vector2 retval = {
    .x = vect.x * C - vect.y * S,
    .y = vect.x * S + vect.y * C,
  };
  retval = vector2_add(retval, center);
  return retval;
}

void plug_frame_update(PlugState state) {
  static size_t frame_number = 0;
  static float elapsed_time = 0.0;

  int width = GetScreenWidth();
  int height = GetScreenHeight();
  Vector2 center = (Vector2) { .x = (float)width/2, .y = (float)height/2 };

  elapsed_time += GetFrameTime();

  static float rectA_rot = 0.0;
  Action action = state.action_list[frame_number];
  switch (action.state) {
    case WORKING:
      if (elapsed_time > action.as.working.duration) {
        elapsed_time = 0.0;
        frame_number = (frame_number + 1) % state.num_frames;
      } else {
        rectA_rot = achromat_to_degrees(action.as.working.where);
      }
      break;
    case MOVING:
      if (elapsed_time > action.as.moving.duration) {
        elapsed_time = 0.0;
        frame_number = (frame_number + 1) % state.num_frames;
      } else {
        float total_angular_dist = 
          achromat_to_degrees(action.as.moving.start_loc)
          - achromat_to_degrees(action.as.moving.end_loc);
        rectA_rot = achromat_to_degrees(action.as.moving.start_loc) - (elapsed_time / action.as.moving.duration) * total_angular_dist;
      }
      break;
  }

  float outer_radius = 0.9 * (float)width/2;

  Vector2 rectA_origin = {
    .x = center.x - 40,
    .y = center.y - (outer_radius - (10 + 0*RECT_HEIGHT))
  };
  Vector2 rectA_loc = rot_vect_around_center(rectA_origin, center, rectA_rot);
  Rectangle teamA_rect = {
    .x = rectA_loc.x, .y = rectA_loc.y,
    .height = RECT_HEIGHT, .width = 80
  };

  BeginDrawing();

  ClearBackground(BACKGROUND_COLOUR);

  DrawPoly(center, 20, outer_radius, 9.0, RAYWHITE);
  DrawPoly(center, 20, outer_radius - 5*RECT_HEIGHT - 10, 9.0, BACKGROUND_COLOUR);

  DrawRectanglePro(teamA_rect, (Vector2){0}, rectA_rot, TEAM_A_COLOUR);

  EndDrawing();
}
