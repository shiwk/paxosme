#!/bin/bash

# clean targets
echo 'PAXOS_ROOT: ' ${PAXOS_ROOT}
cd ${PAXOS_ROOT}/unittest/cmake-build || exit
make clean

# clean cmake stuff
cd ${PAXOS_ROOT}/unittest
rm -rf cmake-build/
