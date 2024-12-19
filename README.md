Vine Editor
===========

> [!WARNING]
> This project is basically abandoned, I might update someday,
> but I'm working on another text editor called "OpenVim", see
> OpenVim [here](https://github.com/ElisStaaf/openvim).

Vine is a fast and intuitive terminal-based text editor based on the modified "kilo-src"
(https://github.com/snaptoken/kilo-src)  editor made by Paige Ruten, which is in turn based
on Salvatore Sanfilippo's "kilo" (https://github.com/antirez/kilo). But this
version has a *lot* of improvements. For example more syntax highlighting and even configuration 
files (this might just be my opinion, but a much better colourscheme as well). It also runs on 
the C standard library so that's good, i guess!

**DISCLAIMER**: This is not a UTF-8 text editor, and I have no idea how to implement it. Sue me.

[![VINE EDITOR](https://github.com/ElisStaaf/vine/raw/main/vineimg.png)](https://github.com/ElisStaaf/vine)

Requirements
------------
* <https://gcc.gnu.org/install>
* <https://www.gnu.org/software/make>
* <https://cmake.org/download>
* <https://git-scm.com/downloads>

Install
-------
To install, firstly clone the repo:
```sh
    git clone https://github.com/ElisStaaf/vine ~/vine
```

After that, you'd want to use ``cmake`` to initialize an executable:
```sh
    cmake .
    sudo make
```

Usage
-----
```sh
     vine <filename>
```
Keybinds
--------
```
   Ctrl+S - Save
   Ctrl+Q - Quit
   Ctrl+F - Find
   Ctrl+X - Delete next char
   Ctrl+D - Delete current line
   Ctrl+J - Start of line
   Ctrl+K - End of line
```
