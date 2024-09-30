#! /usr/bin/bash

dir=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )

if [ ! -f "$dir/build/qv" ]; then
    echo -n "File \"qv\" not found. Build from src? [y/N]:"
    read yn
    if [ yn -eq "y" ]; then
        gcc -o build.exe
    else
        echo "Exiting process..."
    fi
fi

echo "alias qv=\"$dir/build/qv\"" >> ~/.bashrc
