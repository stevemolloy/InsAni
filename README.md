# Easing Raylib development with hot-reloading

This is an example structure for a C project needing 
 hot-reloading.

 To build the project in such a way as to allow hot-reloading:
 
 ```sh
make
./main
```

Make changes to the src/plugin.c file and rebuild.  The executable will notice the change in the file, and hot-reloading the new build.

To compile without hot-reloading (perhaps for release).

```sh
make release
```
