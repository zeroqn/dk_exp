CC := gcc
CFLAGS := -std=c99
LDFLAGS := -lm

build:
	$(CC) -Isrc src/duktape.c src/hello.c -o ./build/hello $(LDFLAGS)

.PHONY: \
	build
