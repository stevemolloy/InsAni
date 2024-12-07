#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "raylib.h"
#include <string.h>

#include <dlfcn.h>
#include <sys/stat.h>
#include <time.h>

#include "plugin.h"
#include "insani_lib.h"

#include "sdm_lib.h"

#define WIDTH 1200
#define HEIGHT 800
#define FPS 60

PlugState state = {0};

#ifndef RELEASE
plug_frame_update_t plug_frame_update = NULL;
#endif /* ifndef RELEASE */

#ifndef RELEASE
void plug_reload(const char* filename) {
  static void* plug_lib = NULL;

  if (plug_lib != NULL) {
    dlclose(plug_lib);
  }

  plug_lib = dlopen(filename, RTLD_NOW);
  if (plug_lib == NULL) {
    fprintf(stderr, "ERROR: %s\n", dlerror());
    exit(1);
  }
  
  plug_frame_update = (plug_frame_update_t)dlsym(plug_lib, "plug_frame_update");
  if (plug_frame_update == NULL) {
    fprintf(stderr, "ERROR: %s\n", dlerror());
    exit(1);
  }
}
#endif /* ifndef RELEASE */

int sum_intarray(int *array, size_t n) {
  int sum = 0;
  for (size_t i=0; i<n; i++) {
    sum += array[i];
  }
  return sum;
}

#define ARRAY_SIZE(arr) sizeof((arr)) / sizeof((arr)[0])

int main(void) {
#ifndef RELEASE
  struct stat file_stats = {0};
  time_t last_modified_time = file_stats.st_mtime;
  const char* plug_filename = "./objs/libplug.so";
#endif /* ifndef RELEASE */

  InitWindow(WIDTH, HEIGHT, "Raylib");
  SetTargetFPS(FPS);

  state.fontsize = 28;
  state.font = LoadFontEx("assets/NotoSans-Regular.ttf", state.fontsize, NULL, 0);

  char *teamA_input_string = sdm_read_entire_file("./team_A.csv");
  char *teamB_input_string = sdm_read_entire_file("./team_B.csv");
  char *teamC_input_string = sdm_read_entire_file("./team_C.csv");
  char *teamD_input_string = sdm_read_entire_file("./team_D.csv");
  char *teamE_input_string = sdm_read_entire_file("./team_E.csv");

  sdm_string_view teamA_sv = sdm_cstr_as_sv(teamA_input_string);
  sdm_string_view teamB_sv = sdm_cstr_as_sv(teamB_input_string);
  sdm_string_view teamC_sv = sdm_cstr_as_sv(teamC_input_string);
  sdm_string_view teamD_sv = sdm_cstr_as_sv(teamD_input_string);
  sdm_string_view teamE_sv = sdm_cstr_as_sv(teamE_input_string);

  IntArray locs_A_array = {0}, durs_A_array = {0};
  SDM_ENSURE_ARRAY_MIN_CAP(locs_A_array, 128);
  SDM_ENSURE_ARRAY_MIN_CAP(durs_A_array, 128);
  inputfile_parse(&teamA_sv, &locs_A_array, &durs_A_array);

  IntArray locs_B_array = {0}, durs_B_array = {0};
  SDM_ENSURE_ARRAY_MIN_CAP(locs_B_array, 128);
  SDM_ENSURE_ARRAY_MIN_CAP(durs_B_array, 128);
  inputfile_parse(&teamB_sv, &locs_B_array, &durs_B_array);

  IntArray locs_C_array = {0}, durs_C_array = {0};
  SDM_ENSURE_ARRAY_MIN_CAP(locs_C_array, 128);
  SDM_ENSURE_ARRAY_MIN_CAP(durs_C_array, 128);
  inputfile_parse(&teamC_sv, &locs_C_array, &durs_C_array);

  IntArray locs_D_array = {0}, durs_D_array = {0};
  SDM_ENSURE_ARRAY_MIN_CAP(locs_D_array, 128);
  SDM_ENSURE_ARRAY_MIN_CAP(durs_D_array, 128);
  inputfile_parse(&teamD_sv, &locs_D_array, &durs_D_array);

  IntArray locs_E_array = {0}, durs_E_array = {0};
  SDM_ENSURE_ARRAY_MIN_CAP(locs_E_array, 128);
  SDM_ENSURE_ARRAY_MIN_CAP(durs_E_array, 128);
  inputfile_parse(&teamE_sv, &locs_E_array, &durs_E_array);

  assert(sum_intarray(durs_A_array.data, durs_A_array.length) == sum_intarray(durs_B_array.data, durs_B_array.length));
  assert(sum_intarray(durs_A_array.data, durs_A_array.length) == sum_intarray(durs_C_array.data, durs_C_array.length));
  assert(sum_intarray(durs_A_array.data, durs_A_array.length) == sum_intarray(durs_C_array.data, durs_D_array.length));
  assert(sum_intarray(durs_A_array.data, durs_A_array.length) == sum_intarray(durs_E_array.data, durs_E_array.length));
  state.job_list_A = make_job_list(locs_A_array.data, durs_A_array.data, locs_A_array.length, &state.num_working_days);
  state.job_list_B = make_job_list(locs_B_array.data, durs_B_array.data, locs_B_array.length, &state.num_working_days);
  state.job_list_C = make_job_list(locs_C_array.data, durs_C_array.data, locs_C_array.length, &state.num_working_days);
  state.job_list_D = make_job_list(locs_D_array.data, durs_D_array.data, locs_D_array.length, &state.num_working_days);
  state.job_list_E = make_job_list(locs_E_array.data, durs_E_array.data, locs_E_array.length, &state.num_working_days);

  free(teamA_input_string);
  free(teamB_input_string);
  free(teamC_input_string);
  free(teamD_input_string);
  free(teamE_input_string);

  while (!WindowShouldClose()) {
#ifndef RELEASE
    stat(plug_filename, &file_stats);

    if (file_stats.st_mtime > last_modified_time) {
      TraceLog(LOG_INFO, "Plugin file timestamp updated");
      last_modified_time = file_stats.st_mtime;
      plug_reload(plug_filename);
      TraceLog(LOG_INFO, "Reloaded the main logic");
    }
#endif /* ifndef RELEASE */
    plug_frame_update(state);
  }

  CloseWindow();

  SDM_ARRAY_FREE(locs_A_array);
  SDM_ARRAY_FREE(locs_B_array);
  SDM_ARRAY_FREE(locs_C_array);
  SDM_ARRAY_FREE(locs_D_array);
  SDM_ARRAY_FREE(locs_E_array);

  SDM_ARRAY_FREE(durs_A_array);
  SDM_ARRAY_FREE(durs_B_array);
  SDM_ARRAY_FREE(durs_C_array);
  SDM_ARRAY_FREE(durs_D_array);
  SDM_ARRAY_FREE(durs_E_array);

  free(state.job_list_A);
  free(state.job_list_B);
  free(state.job_list_C);
  free(state.job_list_D);
  free(state.job_list_E);

  return 0;
}

Working *make_job_list(int *locations, int *durations, size_t n, size_t *working_days) {
  *working_days = sum_intarray(durations, n);

  Working *job_list = malloc(*working_days * sizeof(Working));
  if (job_list == NULL) {
    fprintf(stderr, "Memory issues. Quitting\n");
    exit(1);
  }
  memset(job_list, 0, *working_days * sizeof(Working));

  size_t counter = 0;
  for (size_t i=0; i<n; i++) {
    while (durations[i] > 0) {
      job_list[counter] = (Working){.where = locations[i], .duration=1};
      durations[i]--;
      counter++;
    }
  }

  return job_list;
}

