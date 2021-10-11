CC=gcc

CFLAGS=-W -Wall -O3
LDFLAGS=-Llib
EXEC=out/miner

SRC=src/main.c src/utils.c src/client.c src/server.c
LIB=${SRC:src%.c=include%.h}
OBJ=${SRC:src%.c=obj%.o}

all: obj out obj/blake2b-ref.o $(EXEC)

$(EXEC): $(OBJ) obj/blake2b-ref.o
	$(CC) -o $@ $^ $(LDFLAGS) -led

obj/blake2b-ref.o: src/blake2b-ref.c include/blake2.h include/blake2-impl.h
	$(CC) -o $@ -c $< -I include $(CFLAGS)

obj/%.o: src/%.c include/%.h
	$(CC) -o $@ -c $< -I include $(CFLAGS)

obj:
	mkdir obj/

out:
	mkdir out/

clean:
	rm -rf obj/*

purge: clean
	rm -f $(EXEC)