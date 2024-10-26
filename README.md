# QV: Quantum Visual editor
[![Build](https://img.shields.io/badge/Build%20(Fedora)-passing-2a7fd5?logo=fedora&logoColor=2a7fd5)](https://github.com/ElisStaaf/qv)
[![Version](https://img.shields.io/badge/Version-1.1.3-brightgreen)](https://github.com/ElisStaaf/qv)
[![Lang](https://img.shields.io/badge/Lang-C-lightgrey?logo=c)](https://github.com/ElisStaaf/qv)  
QV is a fast and intuitive terminal-based text editor based on the modified "kilo"
<[https://github.com/snaptoken/kilo-src](https://github.com/snaptoken/kilo-src)> 
editor made by Paige Ruten, which is in turn based on Salvatore Sanfilippo's "kilo"  
<[https://github.com/antirez/kilo](https://github.com/antirez/kilo)> .
But this version has a _lot_ of improvements! For example: better syntax highlighting
& even configuration files (this might just be my opinion, but a much better colourscheme as well).
It also runs  on the C standard library so that's good, i guess!   
  
[![File could not be loaded.](https://github.com/ElisStaaf/qv/blob/main/qvimg.png?raw=true)](https://github.com/ElisStaaf/qv/blob/main/qvimg.png)
QV opened in QV.  
  
## Requirements
* C compiler (clang, gcc etc.) <[https://gcc.gnu.org/install/](https://gcc.gnu.org/install/)>
* Make <[https://www.gnu.org/software/make/](https://www.gnu.org/software/make/)>
* Git or Github CLI (optional) <[https://git-scm.com/downloads](https://git-scm.com/downloads)> <[https://github.com/cli/cli#installation](https://github.com/cli/cli#installation)>

## Install
To install, you can:
```bash
# Git
git clone https://github.com/ElisStaaf/qv ~/qv

# Gh
gh repo clone ElisStaaf/qv ~/qv

# cURL
curl -s https://github.com/ElisStaaf/qv/blob/main/{src/qv.c,Makefile,README.md} ~/qv
```

After this, you'd want to use "make" to initialize an executable:
```bash
cd ~/qv
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

## Usage
```bash
qv <filename> <config>
```
## Keybinds
```
Ctrl+S - Save
Ctrl+Q - Quit
Ctrl+F - Find
```

**\*\*DISCLAIMER\*\***  
This work is licensed under the BSD-Clause-2-license (for the "kilo" source code), and the  
Apache-2.0-license (For the modified parts of the source code), please abide to each license  
under the fitting circumstances.  
