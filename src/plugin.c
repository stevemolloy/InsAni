#include <stdio.h>
#include <assert.h>

#include "raylib.h"

#include "plugin.h"
#include "insani_lib.h"

#define DARK_COLOUR (Color){17, 16, 37, 0xFF}
#define LIGHT_COLOUR (Color){0xFF, 0xFF, 0xFF, 0xFF}
#define INTERMED_COLOUR (Color){0xCC, 0xCC, 0xCC, 0xFF}
#define BACKGROUND_COLOUR DARK_COLOUR
#define CTRLPANEL_COLOUR INTERMED_COLOUR
#define BUTTON_COLOUR (Color){0x55, 0x55, 0x55, 0xFF}
#define BUTTON_TEXT_COLOUR LIGHT_COLOUR
#define TEAM_A_COLOUR MAROON
#define TEAM_B_COLOUR SKYBLUE
#define TEAM_C_COLOUR DARKGREEN
#define TEAM_D_COLOUR BLACK
#define TEAM_E_COLOUR DARKPURPLE
#define RECT_HEIGHT 10
#define RECT_WIDTH 80
#define NUM_OF_ACHROS 20
#define WORKING_TIME 1.5
#define MOVING_TIME 0.5
#define PANELPADDING 10
#define BTN_HEIGHT 35

typedef enum {
  ANI_PAUSED,
  ANI_RUNNING,
  ANI_STEPPING,
} Ani_State;

char *pause_button_text(Ani_State state) {
  switch (state) {
    case ANI_PAUSED: return "Run";
    case ANI_RUNNING: return "Pause";
    case ANI_STEPPING: return "Run";
  }
}

char day_str[128] = {0};

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
      elapsed_time += GetFrameTime();
      if (elapsed_time > WORKING_TIME) {
        elapsed_time = 0.0;
        if (ani_state == ANI_RUNNING) global_state = MOVING;
      }
      rectA_rot = achr2degrees(state.job_list_A[frame_number].where);
      rectB_rot = achr2degrees(state.job_list_B[frame_number].where);
      rectC_rot = achr2degrees(state.job_list_C[frame_number].where);
      rectD_rot = achr2degrees(state.job_list_D[frame_number].where);
      rectE_rot = achr2degrees(state.job_list_E[frame_number].where);
      break;
    }
    case MOVING: {
      if (elapsed_time > MOVING_TIME) {
        elapsed_time = 0.0;
        global_state = WORKING;
        frame_number = (frame_number + 1) % state.num_working_days;
      }
      if ((ani_state == ANI_RUNNING) || (elapsed_time < MOVING_TIME)) {
        elapsed_time += GetFrameTime();
        size_t next_frame = (frame_number + 1) % state.num_working_days;
        float t = elapsed_time / MOVING_TIME;
        rectA_rot = lerpf(achr2degrees(state.job_list_A[frame_number].where), achr2degrees(state.job_list_A[next_frame].where), t);
        rectB_rot = lerpf(achr2degrees(state.job_list_B[frame_number].where), achr2degrees(state.job_list_B[next_frame].where), t);
        rectC_rot = lerpf(achr2degrees(state.job_list_C[frame_number].where), achr2degrees(state.job_list_C[next_frame].where), t);
        rectD_rot = lerpf(achr2degrees(state.job_list_D[frame_number].where), achr2degrees(state.job_list_D[next_frame].where), t);
        rectE_rot = lerpf(achr2degrees(state.job_list_E[frame_number].where), achr2degrees(state.job_list_E[next_frame].where), t);
      }
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
  Rectangle pause_btn_rect = {
    .x=ctrl_panel.x+ctrl_panel.width/2+PANELPADDING, 
    .y=ctrl_panel.y+ctrl_panel.height-PANELPADDING-BTN_HEIGHT,
    .width=ctrl_panel.width/2-2*PANELPADDING, 
    .height=BTN_HEIGHT
  };
  Vector2 pause_text_pos = MeasureTextEx(state.font, pause_button_text(ani_state), state.fontsize, 0);
  pause_text_pos.x = pause_btn_rect.x + pause_btn_rect.width/2 - pause_text_pos.x/2;
  pause_text_pos.y = pause_btn_rect.y + pause_btn_rect.height/2 - pause_text_pos.y/2;

  Rectangle step_btn_rect = {
    .x=ctrl_panel.x+PANELPADDING, 
    .y=ctrl_panel.y+ctrl_panel.height-PANELPADDING-BTN_HEIGHT,
    .width=ctrl_panel.width/2-2*PANELPADDING, 
    .height=BTN_HEIGHT
  };
  Vector2 step_text_pos = MeasureTextEx(state.font, "Step", state.fontsize, 0);
  step_text_pos.x = step_btn_rect.x + step_btn_rect.width/2 - step_text_pos.x/2;
  step_text_pos.y = step_btn_rect.y + step_btn_rect.height/2 - step_text_pos.y/2;

  Rectangle reset_btn_rect = {
    .x=ctrl_panel.x+ctrl_panel.width/2+PANELPADDING, 
    .y=ctrl_panel.y+ctrl_panel.height-3*PANELPADDING-2*BTN_HEIGHT,
    .width=ctrl_panel.width/2-2*PANELPADDING, 
    .height=BTN_HEIGHT
  };
  Vector2 reset_text_pos = MeasureTextEx(state.font, "Reset", state.fontsize, 0);
  reset_text_pos.x = reset_btn_rect.x + reset_btn_rect.width/2 - reset_text_pos.x/2;
  reset_text_pos.y = reset_btn_rect.y + reset_btn_rect.height/2 - reset_text_pos.y/2;

  if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
    Vector2 click_loc = GetMousePosition();
    if (CheckCollisionPointRec(click_loc, pause_btn_rect)) {
      if (ani_state == ANI_RUNNING) ani_state = ANI_PAUSED;
      else if (ani_state == ANI_PAUSED) ani_state = ANI_RUNNING;
    }
    if (CheckCollisionPointRec(click_loc, step_btn_rect)) {
      if (ani_state == ANI_PAUSED) {
        elapsed_time = 0.0;
        global_state = MOVING;
      }
    }
    if (CheckCollisionPointRec(click_loc, reset_btn_rect)) {
      frame_number = 0;
      elapsed_time = 0.0;
      global_state = WORKING;
      ani_state = ANI_PAUSED;
    }
  }

  BeginDrawing();

  ClearBackground(BACKGROUND_COLOUR);

  DrawPoly(center, NUM_OF_ACHROS, outer_radius, 360.0/(20.0*2.0), LIGHT_COLOUR);
  DrawPoly(center, NUM_OF_ACHROS, outer_radius - 5*RECT_HEIGHT - 10, 360.0/(20.0*2.0), BACKGROUND_COLOUR);
  DrawPolyLinesEx(center, NUM_OF_ACHROS, outer_radius, 360.0/(20.0*2.0), 4, GRAY);
  DrawPolyLinesEx(center, NUM_OF_ACHROS, outer_radius - 5*RECT_HEIGHT - 10, 360.0/(20.0*2.0), 4, GRAY);

  DrawRectangleRec(ctrl_panel, CTRLPANEL_COLOUR);

  DrawRectangleRec(pause_btn_rect, BUTTON_COLOUR);
  DrawRectangleRoundedLines(pause_btn_rect, 0.35, 32, 2, GRAY);
  DrawTextEx(state.font, pause_button_text(ani_state), pause_text_pos, state.fontsize, 0, BUTTON_TEXT_COLOUR);

  DrawRectangleRec(step_btn_rect, BUTTON_COLOUR);
  DrawRectangleRoundedLines(step_btn_rect, 0.35, 32, 2, GRAY);
  DrawTextEx(state.font, "Step", step_text_pos, state.fontsize, 0, BUTTON_TEXT_COLOUR);

  DrawRectangleRec(reset_btn_rect, BUTTON_COLOUR);
  DrawRectangleRoundedLines(reset_btn_rect, 0.35, 32, 2, GRAY);
  DrawTextEx(state.font, "Reset", reset_text_pos, state.fontsize, 0, BUTTON_TEXT_COLOUR);

  DrawRectanglePro(teamA_rect, (Vector2){0}, rectA_rot, TEAM_A_COLOUR);
  DrawRectanglePro(teamB_rect, (Vector2){0}, rectB_rot, TEAM_B_COLOUR);
  DrawRectanglePro(teamC_rect, (Vector2){0}, rectC_rot, TEAM_C_COLOUR);
  DrawRectanglePro(teamD_rect, (Vector2){0}, rectD_rot, TEAM_D_COLOUR);
  DrawRectanglePro(teamE_rect, (Vector2){0}, rectE_rot, TEAM_E_COLOUR);

  char *teamA_task = state.job_list_A[frame_number].description.data;
  Vector2 desc_text_pos = (Vector2){.x=ctrl_panel.x + 5, .y=ctrl_panel.y + 2};

  DrawTextEx(state.smallfont, "Team A task: ", desc_text_pos, state.smallfontsize, 0, DARK_COLOUR);
  desc_text_pos.y += state.smallfontsize;
  DrawTextEx(state.smallfont, teamA_task, desc_text_pos, state.smallfontsize, 0, DARK_COLOUR);
  desc_text_pos.y += 2 * state.smallfontsize + PANELPADDING;

  char *teamB_task = state.job_list_B[frame_number].description.data;
  DrawTextEx(state.smallfont, "Team B task: ", desc_text_pos, state.smallfontsize, 0, DARK_COLOUR);
  desc_text_pos.y += state.smallfontsize;
  DrawTextEx(state.smallfont, teamB_task, desc_text_pos, state.smallfontsize, 0, DARK_COLOUR);
  desc_text_pos.y += 2 * state.smallfontsize + PANELPADDING;

  char *teamC_task = state.job_list_C[frame_number].description.data;
  DrawTextEx(state.smallfont, "Team C task: ", desc_text_pos, state.smallfontsize, 0, DARK_COLOUR);
  desc_text_pos.y += state.smallfontsize;
  DrawTextEx(state.smallfont, teamC_task, desc_text_pos, state.smallfontsize, 0, DARK_COLOUR);
  desc_text_pos.y += 2 * state.smallfontsize + PANELPADDING;

  char *teamD_task = state.job_list_D[frame_number].description.data;
  DrawTextEx(state.smallfont, "Team D task: ", desc_text_pos, state.smallfontsize, 0, DARK_COLOUR);
  desc_text_pos.y += state.smallfontsize;
  DrawTextEx(state.smallfont, teamD_task, desc_text_pos, state.smallfontsize, 0, DARK_COLOUR);
  desc_text_pos.y += 2 * state.smallfontsize + PANELPADDING;

  char *teamE_task = state.job_list_E[frame_number].description.data;
  DrawTextEx(state.smallfont, "Team E task: ", desc_text_pos, state.smallfontsize, 0, DARK_COLOUR);
  desc_text_pos.y += state.smallfontsize;
  DrawTextEx(state.smallfont, teamE_task, desc_text_pos, state.smallfontsize, 0, DARK_COLOUR);

  memset(day_str, 0, sizeof(day_str));
  sprintf(day_str, "Day %zu / %zu", frame_number, state.num_working_days);
  DrawTextEx(state.smallfont, day_str, (Vector2){5, 5}, state.smallfontsize, 0, BUTTON_COLOUR);
  DrawRectangle(5, 30, 10, (height-35)*(frame_number+1)/state.num_working_days, BUTTON_COLOUR);

  EndDrawing();
}
