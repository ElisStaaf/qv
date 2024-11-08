## Minimal makefile for
## the "Vine" text editor,
## enjoy if you want to?

# Assume manual installation
all: manual

# Shared vars
src="src/vine.c"
bin="/usr/bin/vine"

manual:
	gcc -o ${bin} ${src}

cmake:
	mkdir build && cd build
	cmake .. && sudo make

meson:
	meson setup build && cd build
	meson compile
	meson install

clean:
	sudo rm ${bin}
	sudo rm -rf build
