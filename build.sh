#!/bin/bash

set -e

# build
cmake -B build \
	  -DCMAKE_BUILD_TYPE=Debug \
	  -DCMAKE_C_COMPILER=/usr/bin/clang \
	  -DCMAKE_CXX_COMPILER=/usr/bin/clang++ \
	  -G Ninja


cmake --build build --config Debug --target model-loading -- -j 6

echo "build: succeed"

build/example/model-loading/model-loading
