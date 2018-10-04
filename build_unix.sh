#!/bin/bash

rm -rf unix-build
mkdir unix-build
cd unix-build
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build . -- -j6
