#!/bin/bash

echo 'CMAKE_PATH: ' ${CMAKE_PATH}
echo 'PAXOS_ROOT: ' ${PAXOS_ROOT}
${CMAKE_PATH}/cmake -B ${PAXOS_ROOT}/unittest/cmake-build -DgRPC_SSL_PROVIDER=package -DgRPC_PROTOBUF_PROVIDER=package -S ${PAXOS_ROOT}/unittest -DBOOST_ROOT_DIR=${BOOST_PATH}/include

cd ${PAXOS_ROOT}/unittest/cmake-build
make -j10