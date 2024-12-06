#ifndef _INSANI_LIB_H
#define _INSANI_LIB_H
#include "sdm_lib.h"
#include "raylib.h"
#include "plugin.h"

typedef struct {
  size_t length;
  size_t capacity;
  int *data;
} IntArray;

void inputfile_parse(sdm_string_view *SV, IntArray *locs_array, IntArray *durs_array);
char *state_as_string(State state);
float lerpf(float a, float b, float t);
float achr2degrees(int achromat);
Vector2 vector2_add(Vector2 a, Vector2 b);
Vector2 vector2_subtract(Vector2 a, Vector2 b);
Vector2 rot_vect_around_center(Vector2 vect, Vector2 center, float angle);

#endif

