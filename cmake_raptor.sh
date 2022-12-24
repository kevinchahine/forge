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
#cmake -DCMAKE_PREFIX_PATH="D:\code\libraries\libtorch-win-shared-with-deps-1.11.0+cu113\libtorch" ..
cmake \
	-DCMAKE_TOOLCHAIN_FILE="/media/kevin/barracuda/Code/libraries/vcpkg/scripts/buildsystems/vcpkg.cmake" \
	-DCMAKE_PREFIX_PATH="/media/kevin/barracuda/Code/libraries/libtorch/libtorch-cxx11-abi-shared-with-deps-1.11.0+cu113/libtorch" \
	-DCHECKPOINT_DIR="/media/kevin/barracuda/owncloud/ML/forge" \
	..

#cmake --build . --config Release

# For non VS build systems
# cmake .. -DCMAKE_TOOLCHAIN_FILE=D:/code/libraries/vcpkg/scripts/buildsystems/vcpkg.cmake
