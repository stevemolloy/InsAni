#include <stdlib.h>
#include <math.h>
#include <stdio.h>

#include "insani_lib.h"
#include "raylib.h"
#define SDM_LIB_IMPLEMENTATION
#include "sdm_lib.h"

void inputfile_parse(sdm_string_view *SV, IntArray *locs_array, IntArray *durs_array, SVArray *comments_array) {
  for (size_t i=0; i<SV->length; i++) {
    if (SV->data[i] == '\n') {
      SV->data[i] = 0;
    }
  }

  while (SV->length) {
    if (SV->data[0] == '\0') {
      sdm_sv_pop_one_char_and_trim(SV);
    }
    sdm_sv_trim(SV);
    SDM_ARRAY_PUSH(*locs_array, sdm_sv_pop_integer_and_trim(SV));
    if (SV->data[0] != ',') {
      fprintf(stderr, "Input file incorrect");
      exit(1);
    }
    sdm_sv_pop_one_char_and_trim(SV);
    SDM_ARRAY_PUSH(*durs_array, sdm_sv_pop_integer_and_trim(SV));
    if (SV->data[0] == ',') {
      sdm_sv_pop_one_char_and_trim(SV);
      SDM_ARRAY_PUSH(*comments_array, sdm_sv_pop_by_delim(SV, '\0'));
    } else {
      SDM_ARRAY_PUSH(*comments_array, (sdm_string_view){0});
    }
  }
}

char *state_as_string(State state) {
  if (state == WORKING) return "Working";
  if (state == MOVING)  return "Moving";
  return NULL;
}

float lerpf(float a, float b, float t) {
  // return a + (b - a) * t;
  return a + (b - a) * (cosf(t * PI) - 1.0) / -2.0;
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


