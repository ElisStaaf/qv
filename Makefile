SHELL=sh
SOURCE="src/qv.c"
BINARY="/usr/bin/qv"

install:
	$(CC) ${SOURCE} -o ${BINARY} -Wall -Wextra -pedantic -std=c99
