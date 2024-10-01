# QV
[![Build](https://img.shields.io/badge/build_(openSUSE)-Passing-brightgreen?logo=opensuse)](https://github.com/ElisStaaf/qv) 
[![Version](https://img.shields.io/badge/Version-1.0.2-darkblue)](https://github.com/ElisStaaf/qv)
[![Lang](https://img.shields.io/badge/Lang-C-lightgrey?logo=c)](https://github.com/ElisStaaf/qv)  
QV - Quantum Visual editor, is a fast and intuitive terminal-based text editor based on the [kilo](https://github.com/snaptoken/kilo-src) 
editor, with a _lot_ of improvements! For
example: better syntax highlighting! Cool, right?

Install
-------
To install, simply:
```bash
git clone --depth 1 https://github.com/ElisStaaf/qv ~/qv
```
Then you want to initialize the editor via your shell, here is an
informative table on what shells are supported (Note that you may
need to change permissions for some of these):

| Shell      | File      | Init                                                                        |
| -----      | ----      | ----                                                                        |
| Sh         | init.sh   | chmod +x ~/qv/init/init.sh; sh ~/qv/init/init.sh                            |
| Powershell | init.ps1  | PowerShell $env:USERPROFILE/qv/init.ps1                                     |
| Fish       | init.fish | chmod +x ~/qv/init/init.fish;~/qv/init/init.fish                            |
| Cmd.exe    | init.bat  | mklink %windir%/Cmd.exe /K %USERPROFILE%/qv/init/init.bat %USERPROFILE%/Cmd |


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
  
But you can ofcourse modify the source-code to add more! I shall for this tutorial
use the "C" programming language.
```bash
touch "main.c" # Create file
qv "main.c" # Open file
```
I am greeted to this interface:
[![File could not be loaded.](https://github.com/ElisStaaf/qv/blob/main/startup.png?raw=true)](https://github.com/ElisStaaf/qv/startup.png)
Great! It started up! I'm writing a simple Hello World app, so i can just write this:
[![File could not be loaded.](https://github.com/ElisStaaf/qv/blob/main/helloworld.png?raw=true)](https://github.com/ElisStaaf/qv/blob/main/code.png)
As you can see, the syntax highlighting kicked in right away! Now, i save my app with `Ctrl+S`, then quit with `Ctrl+Q`. Then i can run the 
file
with my preferred compiler and i will be done with my task. Oh, and also, this text editor is not meant to be anyones main editor, it's way 
to bad for that. No, this
editor is meant to be like Nano, a simple command line editor! And, with that, enjoy!

    This work is licensed under the BSD-Clause-2-license (for the "kilo" source code), and the 
    Apache-2.0-license (For the modified parts of the source code), please abide to each license
    under the fitting circumstances.
