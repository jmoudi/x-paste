#!/bin/bash

echo "Reloading project at: $(date +"%H:%M:%S-%N")";

function compile(){
    echo "Compiling with Args: ${1} ...${2}";
    gcc -g -Wall -Wextra -lX11 -o "$2" "$1" #gcc -v -Wall -lX11 -o "$2"; "$1"
    echo "Successfully compiled: ${2}";
    #clang -v -Wall -lX11 -o "$2" "$1"
}
compile "$@"


