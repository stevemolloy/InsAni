#include <stdio.h>
#include <stdlib.h>
#include "raylib.h"

#include <dlfcn.h>
#include <sys/stat.h>
#include <time.h>

#include "plugin.h"

#define WIDTH 800
#define HEIGHT 600
#define FPS 10

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
