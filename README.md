# QV: Quantum Visual Editor
[![Build](https://img.shields.io/badge/Build%20(Fedora)-passing-2a7fd5?logo=fedora&logoColor=2a7fd5)](https://github.com/ElisStaaf/qv)
[![Version](https://img.shields.io/badge/Version-1.1.0-darkblue)](https://github.com/ElisStaaf/qv)
[![Lang](https://img.shields.io/badge/Lang-C-lightgrey?logo=c)](https://github.com/ElisStaaf/qv)  
QV is a fast and intuitive terminal-based text editor based on the kilo <[https://github.com/snaptoken/kilo-src](https://github.com/snaptoken/kilo-src)> 
editor, with a _lot_ of improvements! For example: better syntax highlighting!
It also runs  on the C standard library so that's good!   
  
[![File could not be loaded.](https://github.com/ElisStaaf/qv/blob/main/helloworld.png?raw=true)](https://github.com/ElisStaaf/qv/blob/main/qvimg.png)
QV opened in QV.  
  
Requirements
-------
* C compiler (clang, gcc etc.) <[https://gcc.gnu.org/install/](https://gcc.gnu.org/install/)>
* Make <[https://www.gnu.org/software/make/](https://www.gnu.org/software/make/)>
* Git or Github CLI (optional) <[https://git-scm.com/downloads](https://git-scm.com/downloads)> <[https://github.com/cli/cli#installation](https://github.com/cli/cli#installation)>

Install
-------
To install, you can:
```bash
# Git
git clone https://github.com/ElisStaaf/qv ~/qv

# Gh
gh repo clone ElisStaaf/qv ~/qv

# cURL
curl -s https://github.com/ElisStaaf/qv/blob/main/{src/qv.c,init/init.sh,init/init.ps1,init/init.bat,init/init.fish,Makefile} ~/qv
```

After this, you'd want to use "make" to initialize an executable:
```bash
cd ~/qv
sudo make install
```

Project layout
--------
```
├── helloworld.png   Picture for README
├── LICENSE          License for source
├── Makefile         Makefile for config
├── README.md        README for info
├── src/             Source code
│   └── qv.c         Source file
├── startup.png      Picture for README
└── TODO             Todo list
```

Get Started
--------
To get started, just open a file via QV!
```bash
qv <filename>
```
Currently, QV includes syntax highlighting for the following languages:  
*  C/C++
*  Shell
*  Golang
*  Python
*  JS/TS
*  C#
*  HTML
*  Rust
*  CSS
*  Ruby
*  D
*  SQL
*  Lua
*  Powershell
*  Batch
*  GDScript
*  Java
*  PHP
*  Vim Script
*  Kotlin

But you can ofcourse modify the source-code to add more! The syntax highlighting is found at like ln 200??? I shall for this tutorial
use the "C" programming language.
```bash
touch "main.c" # Create file
qv "main.c" # Open file
```
I am greeted to this interface:
[![File could not be loaded.](https://github.com/ElisStaaf/qv/blob/main/startup.png?raw=true)](https://github.com/ElisStaaf/qv/startup.png)
Great! It started up! I'm writing a simple Hello World app, so i can just write this:
[![File could not be loaded.](https://github.com/ElisStaaf/qv/blob/main/helloworld.png?raw=true)](https://github.com/ElisStaaf/qv/blob/main/helloworld.png)
As you can see, the syntax highlighting kicked in right away! Now, i save my app with `Ctrl+S`, then quit with `Ctrl+Q`. Then i can run the
file with my preferred compiler and i will be done with my task. Now what if i'm managing a huge codebase? How am i supposed to find the invalid 
reference of variable &HLDB[]? Well, you can find everything with `Ctrl+F`. I press `Ctrl+F` input my text and search with the arrow keys! Then i realize that &HLDB[]
never existed and i was just crazy. And that's the tutorial!

Oh, and before you go, this text editor is not meant to be anyones main editor, it's way
to bad for that. No, this editor is meant to be like Nano, a simple command line editor! And, with that, enjoy!

Config
------
New feature! Config! With a config file you can change some settings (tab_stop, quit_times etc...), nothing much but i guess
it's functional. Heres a basic little tutorial for a config.  
  
To start, you'd want to create a config file:
```bash
touch config.qv # Name it anything you want, but im naming it "config.qv".
```
Qv provides a simple configuration language called... Let's say QvScript. To get all
of the different options for configuration, you have to look inside the source which is `~/qv/src/qv.c`.
But im going to just set some simple options.
```
tab_stop=4
quit_times=2
```
tab_stop being where the tab stops (duh) and quit_times being the amount of `Ctrl+Q`'s it takes to quit after
modifying a file. To run with a config file, simply:
```bash
qv <filename> <config>
```
Yeah, cool. Enjoy the project!

    **DISCLAIMER**
    This work is licensed under the BSD-Clause-2-license (for the "kilo" source code), and the 
    Apache-2.0-license (For the modified parts of the source code), please abide to each license
    under the fitting circumstances.
