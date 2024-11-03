Vine Editor
===========
.. image:: https://img.shields.io/badge/Build%20(fedora)-passing-2a7fd5?logo=fedora&logoColor=2a7fd5&style=for-the-badge
   :alt: Build = Passing
   :target: https://github.com/ElisStaaf/vine
.. image:: https://img.shields.io/badge/Version-net/1-38c747?style=for-the-badge
   :alt: Version = NET/1
   :target: https://github.com/ElisStaaf/vine
.. image:: https://img.shields.io/badge/Language-C-7c7c7c?logo=c&style=for-the-badge
   :alt: Language = C
   :target: https://github.com/ElisStaaf/vine
.. image:: https://img.shields.io/badge/LOC%20(counted%20by%20cloc)-~900-e11e5f?style=for-the-badge
   :alt: LOC = ~900
   :target: https://github.com/ElisStaaf/vine

Vine is a fast and intuitive terminal-based text editor based on the modified "`kilo-src`_"
editor made by Paige Ruten, which is in turn based on Salvatore Sanfilippo's "`kilo`_". But this
version has a *lot* of improvements. For example more syntax highlighting and even configuration 
files (this might just be my opinion, but a much better colourscheme as well). It also runs on 
the C standard library so that's good, i guess!

.. image ::  https://github.com/ElisStaaf/vine/blob/main/vineimg.png?raw=true
   :alt: Vine opened in Vine
   :target: https://github.com/ElisStaaf/vine

Requirements
------------
* `gcc`_ (or any other C compiler) 
* `make`_ or `docker`_
* `cmake`_
* `git`_ or `gh`_

Install
-------
To install, you can:

.. code:: bash

   # git
   git clone https://github.com/ElisStaaf/vine ~/vine

   # gh
   gh repo clone ElisStaaf/vine ~/vine

Then, you'd want to ``cd`` into the vine directory and then configure the editor using the
``configure`` script:

.. code:: bash

   cd ~/vine
   ./configure.sh

This generates a sample configuration file in ``~/.vinerc``. But the configuration script
also has flags you can pass trough it:

::

   -n, --no-configure: Skip configuration file
   -r, --rule <rule> <value>: set <rule> to be equal to <value> in config file.

After that, you'd want to use ``cmake`` or ``docker`` to initialize an executable:

.. code:: bash

   # CMake
   cmake .
   sudo make

   # Docker
   Docker build vine

You *could* also build from source:

.. code:: lua

   gcc -o vine src/vine.c
   mv vine <programs_folder>

Out of the box features
-----------------------
*  Syntax highlighting
*  Find
*  Status bar to show state of editor
*  Line numbers
*  Theming
*  Configuration

Usage
-----
.. code:: lua

    vine <filename>

Keybinds
--------
::

   Ctrl+S - Save
   Ctrl+Q - Quit
   Ctrl+F - Find
   Ctrl+J - Start of line
   Ctrl+K - End of line

.. _`kilo-src`: https://github.com/snaptoken/kilo-src
.. _`kilo`: https://github.com/antirez/kilo
.. _`gcc`: https://gcc.gnu.org/install
.. _`make`: https://www.gnu.org/software/make
.. _`docker`: https://docs.docker.com/engine/install/
.. _`cmake`: https://cmake.org/download
.. _`git`: https://git-scm.com/downloads 
.. _`gh`: https://github.com/cli/cli#installation
