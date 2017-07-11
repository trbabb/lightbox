CC = g++
LD = g++
AR = ar

# compile
INCLUDES = /usr/local/boost src geomc
CFLAGS   = -std=c++11 -O3 -Wall -c -g -fmessage-length=0 -Wno-unused -Wno-unused-local-typedefs
IFLAGS   = $(addprefix -I, $(INCLUDES))

# link
LIBDIRS  = /usr/local/boost/stage/lib
LIBS     = png zd
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

lightbox: build/lightbox.o
	$(CC) $(LDFLAGS) build/lightbox.o -o bin/lightbox

build/%.o : src/%.cpp
	mkdir -p $(patsubst src/%, build/%, $(dir $<))
	$(CC) $(CFLAGS) $(IFLAGS) -o $@ $<

