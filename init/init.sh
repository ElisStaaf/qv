#! /usr/bin/bash

dir=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )

if [ ! -f "$dir/qv" ]; then
    echo -n "File \"qv\" not found. Build from source? [y/N]:"
    read yn
    if [ yn -eq "y" ]; then
        if [ -f "$dir/src/qv.c"]; then
            gcc -o "$dir/qv" "$dir/src/qv.c"
            echo "_OS_EXIT: _EXIT_CODE=0"
        else
            echo "Couldn't find $dir/src/qv, maybe this file is in the wrong directory?"
            echo "_OS_EXIT: _EXIT_CODE=1"
        fi
    else
        echo "Exiting process..."
        echo "_OS_EXIT: _EXIT_CODE=0"
    fi
fi

echo "alias qv=\"$dir/build/qv\"" >> ~/.bashrc
