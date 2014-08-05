CC=clang
SOURCES = src/tgl/*.c
INCLUDES = -Isrc/tgl
LIBS = $(shell pkg-config --libs --cflags glew)
CLFLAGS = -g -O0 #-fsanitize=address,undefined
CFLAGS = $(CLFLAGS) -Wall -pedantic -Werror
LDFLAGS = $(CLFLAGS)

all: shared

shared:
	$(CC) -shared -fPIC -o libtgl.so $(SOURCES) $(INCLUDES) $(CFLAGS) $(LIBS) $(LDFLAGS)
