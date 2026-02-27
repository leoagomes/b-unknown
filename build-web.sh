#!/bin/bash

emcmake cmake -S . -B build-web
cmake --build build-web

if [ "$1" = "run" ]; then
    cd build-web
    emrun BUnknown.html
fi
