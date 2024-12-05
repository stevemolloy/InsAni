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
  int locs[] = {2, 4, 8, 1};
  size_t num_locs = sizeof(locs) / sizeof(locs[0]);
  state.action_list = make_action_list(locs, num_locs);
  state.num_frames = 2*num_locs;

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

Action *make_action_list(int *locations, size_t n) {
  Action *action_list = malloc((n*2) * sizeof(Action));
  if (action_list == NULL) {
    fprintf(stderr, "Memory issues. Quitting\n");
    exit(1);
  }
  memset(action_list, 0, (n*2) * sizeof(Action));

  for (size_t i=0; i<n; i++) {
    action_list[2*i] = (Action){
      .state = WORKING,
      .as.working = (Working){.where = locations[i], .duration=1.0}
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

