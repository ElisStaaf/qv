#!/bin/python3

"""
Configuring script
for the editor.

author :: Elis Staaf
python :: >= 3
license :: Apache License 2.0
"""

import sys
import os

if sys.version_info < (3, 0):
    print("[ERROR]: Configuring script requires python3 or higher.")
    exit(1)

def default_config() -> dict[str]:
    return {
        "tab_stop" : "4",
    }

def configure(rules: dict[str]):
    with open(f"{os.getenv("HOME")}/.vinerc", "w+") as vinerc:
        for rule in rules.keys():
            vinerc.write(f"{rule}={rules[rule]}\n")
        vinerc.close()

def main() -> None:
    rules: dict[str] = {}

    if len(sys.argv) <= 1:
        rules = default_config()
        configure(rules)
        exit(0)

    if sys.argv[1] == "--no-configure" or sys.argv[1] == "-n":
        exit(0)

    for arg in sys.argv:
        arg_argv = sys.argv[sys.argv.index(arg)]
        if arg_argv == "--rule" or arg_argv == "-r":
            rules[sys.argv[sys.argv.index(arg)+1]] = sys.argv[sys.argv.index(arg)+2]
            configure(rules)

if __name__ == "__main__":
    main()
