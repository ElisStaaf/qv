SHELL=/bin/bash
qv: qv.c
  $(CC) qv.c -o qv -Wall -Wextra -pedantic -std=c99
