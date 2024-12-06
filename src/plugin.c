#include <stdio.h>
#include <math.h>
#include <assert.h>

#include "raylib.h"

#include "plugin.h"

#define BACKGROUND_COLOUR (Color){0x28, 0x28, 0x28, 0xFF}
#define TEAM_A_COLOUR RED
#define TEAM_B_COLOUR BLUE
#define TEAM_C_COLOUR GREEN
#define TEAM_D_COLOUR BLACK
#define TEAM_E_COLOUR PURPLE
#define RECT_HEIGHT 15

#define WORKING_TIME 1.0
#define MOVING_TIME 0.5

float lerpf(float a, float b, float t) {
  return a + (b - a) * t;
}

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

void plug_frame_update(PlugState state) {
  int width = GetScreenWidth();
  int height = GetScreenHeight();
  Vector2 center = (Vector2) { .x = (float)width/2, .y = (float)height/2 };

  static float elapsed_time = 0.0;
  static State global_state = WORKING;

  static size_t frame_number = 0;

  float dt = GetFrameTime();
  elapsed_time += dt;

  if (global_state == WORKING) {
    if (elapsed_time > WORKING_TIME) {
      elapsed_time = 0.0;
      global_state = MOVING;
    }
  } else if (global_state == MOVING) {
    if (elapsed_time > MOVING_TIME) {
      elapsed_time = 0.0;
      global_state = WORKING;
      frame_number = (frame_number + 1) % state.num_working_days;
    }
  }

  float rectA_rot = 0.0, rectB_rot = 0.0, rectC_rot = 0.0, rectD_rot = 0.0, rectE_rot = 0.0;

  switch (global_state) {
    case WORKING: {
      rectA_rot = achr2degrees(state.job_list_A[frame_number].where);
      rectB_rot = achr2degrees(state.job_list_B[frame_number].where);
      rectC_rot = achr2degrees(state.job_list_C[frame_number].where);
      rectD_rot = achr2degrees(state.job_list_D[frame_number].where);
      rectE_rot = achr2degrees(state.job_list_E[frame_number].where);
      break;
    }
    case MOVING: {
      rectA_rot = lerpf(
        achr2degrees(state.job_list_A[frame_number].where),
        achr2degrees(state.job_list_A[(frame_number + 1) % state.num_working_days].where),
        elapsed_time / MOVING_TIME
      );
      rectB_rot = lerpf(
        achr2degrees(state.job_list_B[frame_number].where),
        achr2degrees(state.job_list_B[(frame_number + 1) % state.num_working_days].where),
        elapsed_time / MOVING_TIME
      );
      rectC_rot = lerpf(
        achr2degrees(state.job_list_C[frame_number].where),
        achr2degrees(state.job_list_C[(frame_number + 1) % state.num_working_days].where),
        elapsed_time / MOVING_TIME
      );
      rectD_rot = lerpf(
        achr2degrees(state.job_list_D[frame_number].where),
        achr2degrees(state.job_list_D[(frame_number + 1) % state.num_working_days].where),
        elapsed_time / MOVING_TIME
      );
      rectE_rot = lerpf(
        achr2degrees(state.job_list_E[frame_number].where),
        achr2degrees(state.job_list_E[(frame_number + 1) % state.num_working_days].where),
        elapsed_time / MOVING_TIME
      );
      break;
    }
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

  Vector2 rectC_origin = {
    .x = center.x - 40,
    .y = center.y - (outer_radius - (10 + 2*RECT_HEIGHT))
  };
  Vector2 rectC_loc = rot_vect_around_center(rectC_origin, center, rectC_rot);
  Rectangle teamC_rect = {
    .x = rectC_loc.x, .y = rectC_loc.y,
    .height = RECT_HEIGHT, .width = 80
  };

  Vector2 rectD_origin = {
    .x = center.x - 40,
    .y = center.y - (outer_radius - (10 + 3*RECT_HEIGHT))
  };
  Vector2 rectD_loc = rot_vect_around_center(rectD_origin, center, rectD_rot);
  Rectangle teamD_rect = {
    .x = rectD_loc.x, .y = rectD_loc.y,
    .height = RECT_HEIGHT, .width = 80
  };

  Vector2 rectE_origin = {
    .x = center.x - 40,
    .y = center.y - (outer_radius - (10 + 4*RECT_HEIGHT))
  };
  Vector2 rectE_loc = rot_vect_around_center(rectE_origin, center, rectE_rot);
  Rectangle teamE_rect = {
    .x = rectE_loc.x, .y = rectE_loc.y,
    .height = RECT_HEIGHT, .width = 80
  };

  BeginDrawing();

  ClearBackground(BACKGROUND_COLOUR);

  if (global_state == WORKING) {
    DrawText("WORKING", 2, 2, state.fontsize, RAYWHITE);
  } else if (global_state == MOVING) {
    DrawText("MOVING", 2, 2, state.fontsize, RAYWHITE);
  }

  DrawPoly(center, 20, outer_radius, 9.0, RAYWHITE);
  DrawPoly(center, 20, outer_radius - 5*RECT_HEIGHT - 10, 9.0, BACKGROUND_COLOUR);

  DrawRectanglePro(teamA_rect, (Vector2){0}, rectA_rot, TEAM_A_COLOUR);
  DrawRectanglePro(teamB_rect, (Vector2){0}, rectB_rot, TEAM_B_COLOUR);
  DrawRectanglePro(teamC_rect, (Vector2){0}, rectC_rot, TEAM_C_COLOUR);
  DrawRectanglePro(teamD_rect, (Vector2){0}, rectD_rot, TEAM_D_COLOUR);
  DrawRectanglePro(teamE_rect, (Vector2){0}, rectE_rot, TEAM_E_COLOUR);

  EndDrawing();
}
