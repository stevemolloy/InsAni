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
  state.action_list_A = make_action_list(locs_A, durations_A, num_locs_A);
  state.num_frames_A = 2*num_locs_A;

  int locs_B[] = {4, 3, 2, 1};
  int durations_B[] = {4, 3, 2, 1};
  size_t num_locs_B = sizeof(locs_B) / sizeof(locs_B[0]);
  state.action_list_B = make_action_list(locs_B, durations_B, num_locs_B);
  state.num_frames_B = 2*num_locs_B;

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

Action *make_action_list(int *locations, int *durations, size_t n) {
  Action *action_list = malloc((n*2) * sizeof(Action));
  if (action_list == NULL) {
    fprintf(stderr, "Memory issues. Quitting\n");
    exit(1);
  }
  memset(action_list, 0, (n*2) * sizeof(Action));

  for (size_t i=0; i<n; i++) {
    action_list[2*i] = (Action){
      .state = WORKING,
      .as.working = (Working){.where = locations[i], .duration=durations[i] * 1.0}
    };
    if (i != n-1) {
      action_list[2*i + 1] = (Action){
        .state = MOVING,
        .as.moving = (Moving){.start_loc=locations[i], .end_loc=locations[i+1], .duration=0.25}
      };
    } else {
      action_list[2*i + 1] = (Action){
        .state = MOVING,
        .as.moving = (Moving){.start_loc=locations[i], .end_loc=locations[0], .duration=0.25}
      };
    }
  }

  return action_list;
}

