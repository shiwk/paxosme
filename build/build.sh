#!/bin/bash

echo 'CMAKE_PATH: ' ${CMAKE_PATH}
echo 'BOOST_PATH: ' ${BOOST_PATH}
echo 'PAXOS_ROOT: ' ${PAXOS_ROOT}

${CMAKE_PATH}/cmake -DCMAKE_BUILD_TYPE=Debug -G "Unix Makefiles" -DBOOST_ROOT_DIR=${BOOST_PATH}/include -DgRPC_SSL_PROVIDER=package -DgRPC_PROTOBUF_PROVIDER=package -S ${PAXOS_ROOT} -B ${PAXOS_ROOT}/cmake-build-debug
# ${CMAKE_PATH}/cmake -DCMAKE_BUILD_TYPE=Debug -G "Unix Makefiles" -DBOOST_ROOT_DIR=${BOOST_PATH}/include -DCMAKE_OSX_ARCHITECTURES=x86_64 -S ../ -B ./../cmake-build-debug

cd ${PAXOS_ROOT}/cmake-build-debug || exit
make -j10