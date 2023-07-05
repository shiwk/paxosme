#!/bin/bash

# clean targets
cd ../cmake-build-debug || exit
make clean

# clean cmake stuff
cd ..
rm -rf cmake-build-debug/

