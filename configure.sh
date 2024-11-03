#!/bin/sh

tab_stop="4"

config () {
    echo "tab_stop=$tab_stop" > "$HOME/.vinerc"
}

if [ $# -lt 1 ]; then
    config
    exit
fi

case $1 in
    "--no-configure" | "-n")
        exit
        ;;
    "--rule" | "-r")
        case $2 in
            "tab_stop")
                tab_stop="$3"
                ;;
            *)
                echo "[ERROR]: Invalid rule."
                exit
                ;;
        esac
        ;;
    *)
        echo "[ERROR]: Invalid flag."
        exit
esac

config
