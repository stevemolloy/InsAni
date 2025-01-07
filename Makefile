CC = clang
CFLAGS = -Wall -Wextra -std=c18 -Wpedantic -ggdb
CINCLUDES = -I./thirdparty/raylib/include/ 
CLIBS = -Wl,-rpath=./thirdparty/raylib/lib/ -L./thirdparty/raylib/lib/ -l:libraylib.so -lm

EXE = main

all: objs/libplug.so main

$(EXE): src/main.c objs/insani_lib.o
	$(CC) $(CFLAGS) objs/insani_lib.o src/main.c -o $(EXE) $(CINCLUDES) $(CLIBS)

objs/insani_lib.o: src/insani_lib.c
	mkdir -p objs
	$(CC) $(CFLAGS) -o objs/insani_lib.o $(CINCLUDES) -c src/insani_lib.c

objs/libplug.so: src/plugin.c objs/insani_lib.o
	mkdir -p objs
	$(CC) $(CFLAGS) -o objs/_libplug.so $(CINCLUDES) -fPIC -shared objs/insani_lib.o src/plugin.c $(CLIBS)
	mv objs/_libplug.so objs/libplug.so

run: all
	./main

release: src/main.c src/plugin.c
	$(CC) $(CFLAGS) -DRELEASE -o $(EXE) $(CINCLUDES) src/main.c src/plugin.c $(CLIBS)

clean:
	rm -rf objs
	rm -f $(EXE)

