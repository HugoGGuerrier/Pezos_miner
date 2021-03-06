CC=gcc

CFLAGS=-W -Wall -O3
LDFLAGS=-Llib -led25519 -lpthread
BIN=pezos_miner
EXEC=out/${BIN}

SRC=src/account.c src/block.c src/client.c src/hash.c src/main.c src/message.c src/operation.c src/repl.c src/signature.c src/state.c src/test.c src/utils.c
LIB=${SRC:src%.c=include%.h}
OBJ=${SRC:src%.c=obj%.o}

all: obj out obj/blake2b-ref.o $(EXEC)

$(EXEC): $(OBJ) obj/blake2b-ref.o
	$(CC) -o $@ $^ $(LDFLAGS)

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
	rm -f out/*

install: all
	-sudo cp lib/libed25519.so /usr/lib/
	-sudo cp ${EXEC} /usr/local/bin/

uninstall: all
	-sudo rm -f /usr/lib/libed25519.so
	-sudo rm -f /usr/local/bin/${BIN}

run: all
	export LD_LIBRARY_PATH=./lib; \
	./${EXEC}

test: all
	export LD_LIBRARY_PATH=./lib; \
	./${EXEC} -t