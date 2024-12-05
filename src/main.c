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

  int locs_A[] = {1, 2, 3, 4};
  int durations_A[] = {1, 2, 3, 4};
  size_t num_locs_A = sizeof(locs_A) / sizeof(locs_A[0]);
  state.job_list_A = make_job_list(locs_A, durations_A, num_locs_A, &state.num_working_days);

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

