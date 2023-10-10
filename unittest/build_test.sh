#!/bin/bash

echo 'CMAKE_PATH: ' ${CMAKE_PATH}
echo 'PAXOS_ROOT: ' ${PAXOS_ROOT}
${CMAKE_PATH}/cmake -B ${PAXOS_ROOT}/unittest/cmake-build -S ${PAXOS_ROOT}/unittest

cd ${PAXOS_ROOT}/unittest/cmake-build
make -j10