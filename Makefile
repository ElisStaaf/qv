BIN=/usr/bin/qv
SRC=src/qv.c

install:
	$(CC) ${SRC} -o ${BIN} -Wall -Wextra -pedantic -std=c99
