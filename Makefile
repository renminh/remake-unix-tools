CFLAGS = -Wall -g

cat: src/cat/cat.c
	gcc $(CFLAGS) -o bin/cat src/cat/cat.c

grep: src/grep/grep.c
	gcc $(CFLAGS) -o bin/grep src/grep/grep.c

oslat: src/other/oslat/oslat.c
	gcc $(CFLAGS) -o bin/oslat src/other/oslat/oslat.c

.PHONY: clean
clean:
	rm -rf bin/*
