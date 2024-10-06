# QV
[![Build](https://img.shields.io/badge/build_(openSUSE)-Passing-brightgreen?logo=opensuse)](https://github.com/ElisStaaf/qv) 
[![Version](https://img.shields.io/badge/Version-1.1.0-darkblue)](https://github.com/ElisStaaf/qv)
[![Lang](https://img.shields.io/badge/Lang-C-lightgrey?logo=c)](https://github.com/ElisStaaf/qv)  
QV - Quantum Visual editor, is a fast and intuitive terminal-based text editor based on the [kilo](https://github.com/snaptoken/kilo-src) 
editor, with a _lot_ of improvements! Forexample: better syntax highlighting! It also runs 
on the C standard library so that's good!

Requirements
-------
- C compiler (clang, gcc etc.)
- Make (optional)
- Git or Github CLI (optional)
- One of the following shells:
  - Sh
  - Bash
  - Zsh
  - Fish
  - PowerShell
  - Cmd.exe

Install
-------
To install, you can use this handy table to find out what to do:

| Application | Command                                                                                                                  |
| ----------- | -------                                                                                                                  |
| Git         | git clone https://github.com/ElisStaaf/qv ~/qv                                                                           |
| Github CLI  | gh repo clone ElisStaaf/qv ~/qv                                                                                          |
| cURL        | curl -s https://github.com/ElisStaaf/qv/{src/qv.c,init/init.sh,init/init.ps1,init/init.bat,init/init.fish,Makefile} ~/qv |

After this, you'd want to use "make" to initialize an executable:
```bash
cd ~/qv
make
```
If you don't have make installed on your system; you can use GCC (or any other C compiler):
```bash
cd ~/qv
gcc -o qv src/qv.c
```
You can grab yourself a copy of gcc [*here*](https://sourceforge.net/projects/mingw-w64/), 
and a copy of make [*here*](https://www.gnu.org/software/make/).
Then you want to initialize the editor via your shell, here is an
informative table on what shells are supported (Note that you may
need to change permissions for some of these):

| Shell       | File           | Init                                                                        |
| -----       | ----           | ----                                                                        |
| Sh/Bash/Zsh | init/init.sh   | chmod +x ~/qv/init/init.sh; sh ~/qv/init/init.sh                            |
| Powershell  | init/init.ps1  | PowerShell $env:USERPROFILE/qv/init.ps1                                     |
| Fish        | init/init.fish | chmod +x ~/qv/init/init.fish; ~/qv/init/init.fish                           |
| Cmd.exe     | init/init.bat  | mklink %windir%/Cmd.exe /K %USERPROFILE%/qv/init/init.bat %USERPROFILE%/Cmd |

Project layout
--------
```
├── helloworld.png   Picture for README
├── install.sh       Initializer
├── LICENSE.APACHE   Apache license for the "qv" source code
├── LICENSE.BSD      BSD license for the "kilo" source code
├── Makefile         Makefile for config
├── README.md        README for info
├── src/             Source code
│   └── qv.c         Source file
├── startup.png      Picture for README
└── TODO.md          Todo list
```

Get Started
--------
To get started, just open a file via QV!
```bash
qv <filename>
```
Currently, QV includes syntax highlighting for the following languages:  
-  C/C++
-  Shell
-  Golang
-  Python
-  JS/TS
-  C#
-  HTML
-  Rust
-  CSS
-  Ruby
-  D
-  SQL
-  Lua
-  Powershell
-  Batch
-  GDScript
-  Java
-  PHP

But you can ofcourse modify the source-code to add more! I shall for this tutorial
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
New feature! Config! With a config file you can change some settings (tab_stop, quit_times etc...).
Heres a basic little tutorial for a config.  
  
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
modifying a file. Yeah, cool. Enjoy the project!

    This work is licensed under the BSD-Clause-2-license (for the "kilo" source code), and the 
    Apache-2.0-license (For the modified parts of the source code), please abide to each license
    under the fitting circumstances.
