#ifndef _PLUGIN_H
#define _PLUGIN_H

#include <stdlib.h>
#include "raylib.h"
#include "sdm_lib.h"

typedef enum {
  WORKING,
  MOVING,
} State;

typedef struct {
  unsigned int where;
  float duration;
  sdm_string_view description;
} Working;

typedef struct {
  int fontsize;
  Font font;
  int smallfontsize;
  Font smallfont;
  size_t num_working_days;
  Working *job_list_A;
  Working *job_list_B;
  Working *job_list_C;
  Working *job_list_D;
  Working *job_list_E;
} PlugState;

#ifndef RELEASE
typedef void (*plug_frame_update_t)(PlugState state);
#else
void plug_frame_update(PlugState state);
#endif // !RELEASE

Working *make_job_list(int *locations, int *durations, sdm_string_view *descriptions, size_t n, size_t *working_days);

#endif // !_PLUGIN_H

