CC := gcc
CFLAGS := -std=c99
LDFLAGS := -lm

build:
	$(CC) -Isrc src/duktape.c src/ee.c -o ./build/ee $(LDFLAGS)

.PHONY: \
	build
