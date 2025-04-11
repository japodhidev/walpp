#!/bin/bash
cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_MAKE_PROGRAM=/opt/clion/bin/ninja/linux/x64/ninja -G Ninja -S /home/sleek/source/cpp/walpp -B /home/sleek/source/cpp/walpp/cmake-build-debug
cmake --build /home/sleek/source/cpp/walpp/cmake-build-debug --target walpp -j 1