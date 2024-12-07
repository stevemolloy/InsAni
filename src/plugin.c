#include <stdio.h>
#include <assert.h>

#include "raylib.h"

#include "plugin.h"
#include "insani_lib.h"

#define BACKGROUND_COLOUR (Color){0x28, 0x28, 0x28, 0xFF}
#define TEAM_A_COLOUR RED
#define TEAM_B_COLOUR BLUE
#define TEAM_C_COLOUR GREEN
#define TEAM_D_COLOUR BLACK
#define TEAM_E_COLOUR PURPLE
#define RECT_HEIGHT 12
#define RECT_WIDTH 90
#define NUM_OF_ACHROS 20
#define WORKING_TIME 1.0
#define MOVING_TIME 0.5
#define PANELPADDING 10
#define BTN_HEIGHT 40

typedef enum {
  ANI_PAUSED,
  ANI_RUNNING,
} Ani_State;

char *button_text(Ani_State state) {
  switch (state) {
    case ANI_PAUSED: return "Run";
    case ANI_RUNNING: return "Pause";
  }
}

void plug_frame_update(PlugState state) {
  int width = GetScreenWidth();
  int height = GetScreenHeight();
  Vector2 center = {.x = height/2.0, .y = height/2.0};

  static size_t frame_number = 0;
  static float elapsed_time = 0.0;
  static State global_state = WORKING;
  static Ani_State ani_state = ANI_PAUSED;

  static float rectA_rot = 0, rectB_rot = 0, rectC_rot = 0, rectD_rot = 0, rectE_rot = 0;
  if (ani_state == ANI_PAUSED) {
    rectA_rot = achr2degrees(state.job_list_A[frame_number].where);
    rectB_rot = achr2degrees(state.job_list_B[frame_number].where);
    rectC_rot = achr2degrees(state.job_list_C[frame_number].where);
    rectD_rot = achr2degrees(state.job_list_D[frame_number].where);
    rectE_rot = achr2degrees(state.job_list_E[frame_number].where);
  }

  switch (global_state) {
    case WORKING: {
      if (ani_state == ANI_RUNNING) {
      elapsed_time += GetFrameTime();
        if (elapsed_time > WORKING_TIME) {
          elapsed_time = 0.0;
          global_state = MOVING;
        }
        rectA_rot = achr2degrees(state.job_list_A[frame_number].where);
        rectB_rot = achr2degrees(state.job_list_B[frame_number].where);
        rectC_rot = achr2degrees(state.job_list_C[frame_number].where);
        rectD_rot = achr2degrees(state.job_list_D[frame_number].where);
        rectE_rot = achr2degrees(state.job_list_E[frame_number].where);
      }
      break;
    }
    case MOVING: {
      elapsed_time += GetFrameTime();
      if (elapsed_time > MOVING_TIME) {
        elapsed_time = 0.0;
        global_state = WORKING;
        frame_number = (frame_number + 1) % state.num_working_days;
      }
      size_t next_frame = (frame_number + 1) % state.num_working_days;
      float t = elapsed_time / MOVING_TIME;
      rectA_rot = lerpf(achr2degrees(state.job_list_A[frame_number].where), achr2degrees(state.job_list_A[next_frame].where), t);
      rectB_rot = lerpf(achr2degrees(state.job_list_B[frame_number].where), achr2degrees(state.job_list_B[next_frame].where), t);
      rectC_rot = lerpf(achr2degrees(state.job_list_C[frame_number].where), achr2degrees(state.job_list_C[next_frame].where), t);
      rectD_rot = lerpf(achr2degrees(state.job_list_D[frame_number].where), achr2degrees(state.job_list_D[next_frame].where), t);
      rectE_rot = lerpf(achr2degrees(state.job_list_E[frame_number].where), achr2degrees(state.job_list_E[next_frame].where), t);
      break;
    }
  }

  float outer_radius = 0.9 * height/2.0;

  Vector2 rect_loc = {.x=center.x - RECT_WIDTH/2.0, .y=center.y - outer_radius + 10};

  Vector2 rectA_loc = rot_vect_around_center((Vector2){.x = rect_loc.x, .y = rect_loc.y + 0*RECT_HEIGHT}, center, rectA_rot);
  Vector2 rectB_loc = rot_vect_around_center((Vector2){.x = rect_loc.x, .y = rect_loc.y + 1*RECT_HEIGHT}, center, rectB_rot);
  Vector2 rectC_loc = rot_vect_around_center((Vector2){.x = rect_loc.x, .y = rect_loc.y + 2*RECT_HEIGHT}, center, rectC_rot);
  Vector2 rectD_loc = rot_vect_around_center((Vector2){.x = rect_loc.x, .y = rect_loc.y + 3*RECT_HEIGHT}, center, rectD_rot);
  Vector2 rectE_loc = rot_vect_around_center((Vector2){.x = rect_loc.x, .y = rect_loc.y + 4*RECT_HEIGHT}, center, rectE_rot);

  Rectangle teamA_rect = {.x = rectA_loc.x, .y = rectA_loc.y, .height = RECT_HEIGHT, .width = RECT_WIDTH};
  Rectangle teamB_rect = {.x = rectB_loc.x, .y = rectB_loc.y, .height = RECT_HEIGHT, .width = RECT_WIDTH};
  Rectangle teamC_rect = {.x = rectC_loc.x, .y = rectC_loc.y, .height = RECT_HEIGHT, .width = RECT_WIDTH};
  Rectangle teamD_rect = {.x = rectD_loc.x, .y = rectD_loc.y, .height = RECT_HEIGHT, .width = RECT_WIDTH};
  Rectangle teamE_rect = {.x = rectE_loc.x, .y = rectE_loc.y, .height = RECT_HEIGHT, .width = RECT_WIDTH};

  Rectangle ctrl_panel = {.x=height, .y=PANELPADDING, .width=width-height-(2*PANELPADDING), .height=height-(2*PANELPADDING)};
  Rectangle start_stop_btn_rect = {.x=ctrl_panel.x+PANELPADDING, .y=ctrl_panel.y+PANELPADDING, .width=ctrl_panel.width/2-2*PANELPADDING, .height=BTN_HEIGHT};
  Vector2 text_pos = MeasureTextEx(state.font, button_text(ani_state), state.fontsize, 0);
  text_pos.x = start_stop_btn_rect.x + start_stop_btn_rect.width/2 - text_pos.x/2;
  text_pos.y = start_stop_btn_rect.y + start_stop_btn_rect.height/2 - text_pos.y/2;

  if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
    Vector2 click_loc = GetMousePosition();
    if (CheckCollisionPointRec(click_loc, start_stop_btn_rect)) {
      if (ani_state == ANI_RUNNING) ani_state = ANI_PAUSED;
      else if (ani_state == ANI_PAUSED) ani_state = ANI_RUNNING;
    }
  }

  BeginDrawing();

  ClearBackground(BACKGROUND_COLOUR);

  DrawTextEx(state.font, state_as_string(global_state), (Vector2){.x=2, .y=2}, state.fontsize, 0.0, RAYWHITE);

  DrawPoly(center, NUM_OF_ACHROS, outer_radius, 360.0/(20.0*2.0), RAYWHITE);
  DrawPoly(center, NUM_OF_ACHROS, outer_radius - 5*RECT_HEIGHT - 10, 360.0/(20.0*2.0), BACKGROUND_COLOUR);
  DrawPolyLinesEx(center, NUM_OF_ACHROS, outer_radius, 360.0/(20.0*2.0), 4, GRAY);
  DrawPolyLinesEx(center, NUM_OF_ACHROS, outer_radius - 5*RECT_HEIGHT - 10, 360.0/(20.0*2.0), 4, GRAY);

  DrawRectangleRec(ctrl_panel, GRAY);

  DrawRectangleRec(start_stop_btn_rect, DARKGRAY);
  DrawTextEx(state.font, button_text(ani_state), (Vector2){.x=text_pos.x, .y=text_pos.y}, state.fontsize, 0, BLACK);

  DrawRectanglePro(teamA_rect, (Vector2){0}, rectA_rot, TEAM_A_COLOUR);
  DrawRectanglePro(teamB_rect, (Vector2){0}, rectB_rot, TEAM_B_COLOUR);
  DrawRectanglePro(teamC_rect, (Vector2){0}, rectC_rot, TEAM_C_COLOUR);
  DrawRectanglePro(teamD_rect, (Vector2){0}, rectD_rot, TEAM_D_COLOUR);
  DrawRectanglePro(teamE_rect, (Vector2){0}, rectE_rot, TEAM_E_COLOUR);

  EndDrawing();
}
