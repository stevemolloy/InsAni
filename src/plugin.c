#include <stdio.h>
#include <math.h>
#include <assert.h>

#include "raylib.h"

#include "plugin.h"

#define BACKGROUND_COLOUR (Color){0x28, 0x28, 0x28, 0xFF}
#define TEAM_A_COLOUR RED
#define TEAM_B_COLOUR BLUE
#define RECT_HEIGHT 15

float achr2degrees(int achromat) {
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

float calc_rot(Action action, float *elapsed_time, size_t *frame_number, size_t num_frames) {
  float rect_rot = 0.0;
  switch (action.state) {
    case WORKING:
      if (*elapsed_time > action.as.working.duration) {
        *elapsed_time = 0.0;
        *frame_number = (*frame_number + 1) % num_frames;
      }
      rect_rot = achr2degrees(action.as.working.where);
      break;
    case MOVING:
      if (*elapsed_time > action.as.moving.duration) {
        *elapsed_time = 0.0;
        *frame_number = (*frame_number + 1) % num_frames;
      }
      float total_angular_dist = achr2degrees(action.as.moving.start_loc) - achr2degrees(action.as.moving.end_loc);
      rect_rot = achr2degrees(action.as.moving.start_loc) - (*elapsed_time / action.as.moving.duration) * total_angular_dist;
      break;
  }
  return rect_rot;
}

void plug_frame_update(PlugState state) {
  int width = GetScreenWidth();
  int height = GetScreenHeight();
  Vector2 center = (Vector2) { .x = (float)width/2, .y = (float)height/2 };

  static size_t frame_number_A = 0;
  static float elapsed_time_A = 0.0;
  static size_t frame_number_B = 0;
  static float elapsed_time_B = 0.0;

  float dt = GetFrameTime();
  elapsed_time_A += dt;
  elapsed_time_B += dt;

  static float rectA_rot = 0.0;
  Action action_A = state.action_list_A[frame_number_A];
  // rectA_rot = calc_rot(action_A, &elapsed_time_A, &frame_number_A, state.num_frames_A);
  switch (action_A.state) {
    case WORKING:
      if (elapsed_time_A > action_A.as.working.duration) {
        elapsed_time_A = 0.0;
        frame_number_A = (frame_number_A + 1) % state.num_frames_A;
      } else {
        rectA_rot = achr2degrees(action_A.as.working.where);
      }
      break;
    case MOVING:
      if (elapsed_time_A > action_A.as.moving.duration) {
        elapsed_time_A = 0.0;
        frame_number_A = (frame_number_A + 1) % state.num_frames_A;
      } else {
        float total_angular_dist = achr2degrees(action_A.as.moving.start_loc) - achr2degrees(action_A.as.moving.end_loc);
        rectA_rot = achr2degrees(action_A.as.moving.start_loc) - (elapsed_time_A / action_A.as.moving.duration) * total_angular_dist;
      }
      break;
  }

  static float rectB_rot = 0.0;
  Action action_B = state.action_list_B[frame_number_B];
  switch (action_B.state) {
    case WORKING:
      if (elapsed_time_B > action_B.as.working.duration) {
        elapsed_time_B = 0.0;
        frame_number_B = (frame_number_B + 1) % state.num_frames_B;
      } else {
        rectB_rot = achr2degrees(action_B.as.working.where);
      }
      break;
    case MOVING:
      if (elapsed_time_B > action_B.as.moving.duration) {
        elapsed_time_B = 0.0;
        frame_number_B = (frame_number_B + 1) % state.num_frames_B;
      } else {
        float total_angular_dist = achr2degrees(action_B.as.moving.start_loc) - achr2degrees(action_B.as.moving.end_loc);
        rectB_rot = achr2degrees(action_B.as.moving.start_loc) - (elapsed_time_B / action_B.as.moving.duration) * total_angular_dist;
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

  Vector2 rectB_origin = {
    .x = center.x - 40,
    .y = center.y - (outer_radius - (10 + 1*RECT_HEIGHT))
  };
  Vector2 rectB_loc = rot_vect_around_center(rectB_origin, center, rectB_rot);
  Rectangle teamB_rect = {
    .x = rectB_loc.x, .y = rectB_loc.y,
    .height = RECT_HEIGHT, .width = 80
  };

  BeginDrawing();

  ClearBackground(BACKGROUND_COLOUR);

  DrawPoly(center, 20, outer_radius, 9.0, RAYWHITE);
  DrawPoly(center, 20, outer_radius - 5*RECT_HEIGHT - 10, 9.0, BACKGROUND_COLOUR);

  DrawRectanglePro(teamA_rect, (Vector2){0}, rectA_rot, TEAM_A_COLOUR);
  DrawRectanglePro(teamB_rect, (Vector2){0}, rectB_rot, TEAM_B_COLOUR);

  EndDrawing();
}
