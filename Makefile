CC = clang
CFLAGS = -Wall -Wextra -std=c18 -Wpedantic -ggdb
CINCLUDES = -I./thirdparty/raylib/include/ 
CLIBS = -Wl,-rpath=./thirdparty/raylib/lib/ -L./thirdparty/raylib/lib/ -l:libraylib.so

EXE = main

all: main objs/libplug.so

$(EXE): src/main.c
	$(CC) $(CFLAGS) src/main.c -o $(EXE) $(CINCLUDES) $(CLIBS)

objs/libplug.so: src/plugin.c
	mkdir -p objs
	$(CC) $(CFLAGS) -o objs/_libplug.so $(CINCLUDES) -fPIC -shared src/plugin.c $(CLIBS)
	mv objs/_libplug.so objs/libplug.so

run: all
	./main

release: src/main.c src/plugin.c
	$(CC) $(CFLAGS) -DRELEASE -o $(EXE) $(CINCLUDES) src/main.c src/plugin.c $(CLIBS)

clean:
	rm -rf objs
	rm -f $(EXE)

