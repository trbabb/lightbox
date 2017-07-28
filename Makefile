CC = g++

# compile
INCLUDES = src geomc
CFLAGS   = -std=c++11 -O3 -Wall -c -g -fmessage-length=0 -Wno-unused -Wno-unused-local-typedefs
IFLAGS   = $(addprefix -I, $(INCLUDES))

# link
LIBDIRS  = 
LIBS     = png z
LDFLAGS  = $(addprefix -l, $(LIBS)) $(addprefix -L, $(LIBDIRS)) 

# sources
MODULES  = lightbox
SRC      = $(wildcard src/*.cpp) \
           $(foreach m, $(MODULES), $(wildcard src/$(m)/*.cpp))
OBJ      = $(patsubst src/%.cpp, build/%.o, $(SRC))


all: lightbox

clean:
	rm -rf ./build/*
	rm -rf ./bin/*

## binaries

lightbox: $(OBJ) bin
	$(CC) $(LDFLAGS) build/*.o -o bin/lightbox

build/%.o : src/%.cpp build
	mkdir -p $(patsubst src/%, build/%, $(dir $<))
	$(CC) $(CFLAGS) $(IFLAGS) -o $@ $<

build:
	mkdir build

bin:
	mkdir bin
