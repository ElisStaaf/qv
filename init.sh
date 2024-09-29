#! /usr/bin/bash

dir=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )

if [ ! -f "$dir/build/qv.exe" ]; then
    echo -n "File \"qv.exe\" not found. Build from src? [y/N]:"
    read yn
    if [ yn -eq "y" ]; then
        gcc -o build.exe
    else
        echo "Exiting process..."
    fi
fi

echo "alias qv=\"$dir/build/qv.exe\"" >> ~/.bashrc
