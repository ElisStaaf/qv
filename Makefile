SHELL=/bin/bash
qv: qv.c
  $(CC) src/qv.c -o build/qv -Wall -Wextra -pedantic -std=c99
