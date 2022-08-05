#!/bin/bash
clear

# 1.) Make sure build directory exists
if [ -d "build" ]; then
	echo "build directory does not exist"
else
	mkdir build
fi

# 2.) cd into build and run cmake
cd build
cmake -DCMAKE_PREFIX_PATH="C:\Users\kchah\Code\Libraries\libtorch" ..
# -DCMAKE_TOOLCHAIN_FILE=C:/Users/kchah/Code/Libraries/vcpkg/scripts/buildsystems/vcpkg.cmake

# --- For non VS build systems ---
# cmake .. -DCMAKE_TOOLCHAIN_FILE=D:/code/libraries/vcpkg/scripts/buildsystems/vcpkg.cmake

# --- Or specify build configuration ---
#cmake --build . --config Release
