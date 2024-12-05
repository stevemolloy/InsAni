#ifndef _PLUGIN_H
#define _PLUGIN_H

#include <stdlib.h>
#include "raylib.h"

typedef enum {
  WORKING,
  MOVING,
} State;

typedef struct {
  unsigned int start_loc;
  unsigned int end_loc;
  float duration;
} Moving;

typedef struct {
  unsigned int where;
  float duration;
} Working;

typedef struct {
  State state;
  union {
    Moving moving;
    Working working;
  } as;
} Action;

typedef struct {
  int fontsize;
  Font font;
  Action *action_list_A;
  size_t num_frames_A;
  Action *action_list_B;
  size_t num_frames_B;
} PlugState;

#ifndef RELEASE
typedef void (*plug_frame_update_t)(PlugState state);
#else
void plug_frame_update(PlugState state);
#endif // !RELEASE

Action *make_action_list(int *locations, int *durations, size_t n);

#endif // !_PLUGIN_H

