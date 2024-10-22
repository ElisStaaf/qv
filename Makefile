BIN=/usr/bin/qv
SRC=src/qv.c
DIR=$(shell pwd)

install:
	$(CC) ${SRC} -o ${BIN} -Wall -Wextra -pedantic -std=c99

remove:
	cd .. 
	rm -rf ${DIR} && rm -rf ${BIN}
