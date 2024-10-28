# Vine Editor
[![Build](https://img.shields.io/badge/Build%20(Fedora)-passing-2a7fd5?logo=fedora&logoColor=2a7fd5&style=for-the-badge)](https://github.com/ElisStaaf/vine)
[![Version](https://img.shields.io/badge/Version-1.1.4-38c747?style=for-the-badge)](https://github.com/ElisStaaf/vine)
[![Lang](https://img.shields.io/badge/Language-C-7c7c7c?logo=c&style=for-the-badge)](https://github.com/ElisStaaf/vine)
[![LOC](https://img.shields.io/badge/LOC%20(counted%20by%20cloc)-~900-e11e5f?style=for-the-badge)](https://github.com/ElisStaaf/vine)  
Vine is a fast and intuitive terminal-based text editor based on the modified "kilo"  
<[https://github.com/snaptoken/kilo-src](https://github.com/snaptoken/kilo-src)> 
editor made by Paige Ruten, which is in turn based on Salvatore Sanfilippo's "kilo" <[https://github.com/antirez/kilo](https://github.com/antirez/kilo)>.
But this version has a _lot_ of improvements! For example: better syntax highlighting
& even configuration files (this might just be my opinion, but a much better colourscheme as well).
It also runs  on the C standard library so that's good, i guess!   
  
[![File could not be loaded.](https://github.com/ElisStaaf/vine/blob/main/vineimg.png?raw=true)](https://github.com/ElisStaaf/vine/blob/main/vineimg.png)
Vine opened in Vine.  
  
## Requirements
* C compiler (clang, gcc etc.) <[https://gcc.gnu.org/install/](https://gcc.gnu.org/install/)>
* Make <[https://www.gnu.org/software/make/](https://www.gnu.org/software/make/)>
* Git or Github CLI (optional) <[https://git-scm.com/downloads](https://git-scm.com/downloads)> <[https://github.com/cli/cli#installation](https://github.com/cli/cli#installation)>

## Install
To install, you can:
```bash
# Git
git clone https://github.com/ElisStaaf/vine ~/vine

# Gh
gh repo clone ElisStaaf/vine ~/vine

# cURL
curl -s https://github.com/ElisStaaf/vine/blob/main/{src/vine.c,Makefile,README.md} ~/vine
```

After this, you'd want to use "make" to initialize an executable:
```bash
cd ~/vine
sudo make install
```

## Out of the box features
*  Syntax highlighting
   *  C/C++ (`*.c`, `*.h`, `*.cpp`, `*.hpp`, `*.cc`)
   *  Golang (`*.go`)
   *  Python (`*.py`, `*.pyi`)
   *  Rust (`*.rs`, `*.rlib`)
*  Find
*  Status bar to show state of editor
*  Line numbers
*  Theming
*  Configuration

## Usage
```bash
vine <filename>
```
## Keybinds
```
Ctrl+S - Save
Ctrl+Q - Quit
Ctrl+F - Find
Ctrl+J - Start of line
Ctrl+K - End of line
```

**\*\*DISCLAIMER\*\***  
This work is licensed under the BSD-Clause-2-license (for the "kilo" source code), and the  
Apache-2.0-license (For the modified parts of the source code), please abide to each license  
under the fitting circumstances.  
