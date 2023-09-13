#!/bin/bash

# clean targets
echo 'PAXOS_ROOT: ' ${PAXOS_ROOT}
cd ${PAXOS_ROOT}/cmake-build-debug || exit
make clean

# clean cmake stuff
cd ${PAXOS_ROOT}
rm -rf cmake-build-debug/

