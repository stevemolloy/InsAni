#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "raylib.h"
#include <string.h>

#include <dlfcn.h>
#include <sys/stat.h>
#include <time.h>

#include "plugin.h"

#define WIDTH 1000
#define HEIGHT 1000
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

int main(void) {
#ifndef RELEASE
  struct stat file_stats = {0};
  time_t last_modified_time = file_stats.st_mtime;
  const char* plug_filename = "./objs/libplug.so";
#endif /* ifndef RELEASE */

  InitWindow(WIDTH, HEIGHT, "Raylib");
  SetTargetFPS(FPS);

  state.fontsize = 44;
  state.font = LoadFontEx("assets/RecMonoCasual-Regular-1.085.ttf", state.fontsize, NULL, 0);

  int locs_A[] = {3, 5, 17, 10, 11, 16, 10, 4, 15, 9, 3, 13, 7, 17, 19, 13, 14, 8, 19, 16, 19, 11, 3, 14, 8, 19, 6, 17, 11, 1, 1};
  int locs_B[] = {12, 6, 8, 9, 12, 15, 8, 3, 14, 17, 11, 6, 12, 6, 16, 18, 3, 5, 15, 17, 10, 12, 15, 9, 11, 15, 18, 1, 1, 1, 1};
  int locs_C[] = {4, 15, 16, 18, 13, 6, 9, 12, 6, 8, 19, 15, 8, 10, 4, 15, 17, 4, 14, 8, 18, 4, 6, 17, 3, 13, 7, 9, 1, 1, 1};
  int locs_D[] = {13, 14, 7, 19, 4, 14, 17, 19, 13, 7, 18, 12, 5, 16, 9, 11, 5, 9, 6, 9, 13, 7, 18, 12, 5, 16, 10, 1, 1, 1, 1};
  int locs_E[] = {3, 5, 7, 18, 11, 5, 16, 10, 4, 14, 18, 3, 7, 10, 11, 12, 13, 7, 5, 16, 10, 4, 14, 8, 19, 1, 1, 1, 1, 1, 1};
  int durations_A[] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
  int durations_B[] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
  int durations_C[] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
  int durations_D[] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
  int durations_E[] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
  assert(sum_intarray(durations_A, sizeof(durations_A)/sizeof(durations_A[0])) == sum_intarray(durations_B, sizeof(durations_B)/sizeof(durations_B[0])));
  assert(sum_intarray(durations_A, sizeof(durations_A)/sizeof(durations_A[0])) == sum_intarray(durations_C, sizeof(durations_C)/sizeof(durations_C[0])));
  assert(sum_intarray(durations_A, sizeof(durations_A)/sizeof(durations_A[0])) == sum_intarray(durations_D, sizeof(durations_D)/sizeof(durations_D[0])));
  assert(sum_intarray(durations_A, sizeof(durations_A)/sizeof(durations_A[0])) == sum_intarray(durations_E, sizeof(durations_E)/sizeof(durations_E[0])));
  size_t num_locs_A = sizeof(locs_A) / sizeof(locs_A[0]);
  size_t num_locs_B = sizeof(locs_B) / sizeof(locs_B[0]);
  size_t num_locs_C = sizeof(locs_C) / sizeof(locs_C[0]);
  size_t num_locs_D = sizeof(locs_D) / sizeof(locs_D[0]);
  size_t num_locs_E = sizeof(locs_E) / sizeof(locs_E[0]);
  state.job_list_A = make_job_list(locs_A, durations_A, num_locs_A, &state.num_working_days);
  state.job_list_B = make_job_list(locs_B, durations_B, num_locs_B, &state.num_working_days);
  state.job_list_C = make_job_list(locs_C, durations_C, num_locs_C, &state.num_working_days);
  state.job_list_D = make_job_list(locs_D, durations_D, num_locs_D, &state.num_working_days);
  state.job_list_E = make_job_list(locs_E, durations_E, num_locs_E, &state.num_working_days);

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

  return 0;
}

Working *make_job_list(int *locations, int *durations, size_t n, size_t *working_days) {
  *working_days = 0;
  for (size_t i=0; i<n; i++) {
    *working_days += durations[i];
  }

  Working *job_list = malloc(*working_days * sizeof(Working));
  if (job_list == NULL) {
    fprintf(stderr, "Memory issues. Quitting\n");
    exit(1);
  }
  memset(job_list, 0, *working_days * sizeof(Working));

  size_t counter = 0;
  for (size_t i=0; i<n; i++) {
    while (durations[i] > 0) {
      job_list[counter] = (Working){
        .where = locations[i],
        .duration=1,
      };
      durations[i]--;
      counter++;
    }
  }

  return job_list;
}

