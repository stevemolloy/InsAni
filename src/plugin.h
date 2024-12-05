#ifndef _PLUGIN_H
#define _PLUGIN_H

#include "raylib.h"

typedef struct {
  int fontsize;
  Font font;
} PlugState;

#ifndef RELEASE
typedef void (*plug_frame_update_t)(PlugState state);
#else
void plug_frame_update(PlugState state);
#endif // !RELEASE

#endif // !_PLUGIN_H

