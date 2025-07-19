CFLAGS = -Wall -g

cat: src/cat/cat.c
	gcc $(CFLAGS) -o bin/cat src/cat/cat.c

.PHONY: clean
clean:
	rm -rf bin/*
