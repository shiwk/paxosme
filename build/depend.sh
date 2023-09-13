#!/bin/bash
# dep: boost

# submodules init
git submodule update --init --recursive


# build ptorobuf
echo 'CMAKE_PATH: ' ${CMAKE_PATH}
echo 'PAXOS_ROOT: ' ${PAXOS_ROOT}
cd ${PAXOS_ROOT}/build
mkdir protobuf

${CMAKE_PATH}/cmake ${PAXOS_ROOT}/3rd_party/protobuf -DCMAKE_CXX_STANDARD=14 -B ./protobuf

cd ./protobuf || exit
sudo make&& sudo make install

#cd ..
#mkdir leveldb
#$1/cmake -DCMAKE_BUILD_TYPE=Release ../3rd_party/leveldb  -B ./leveldb && cd leveldb && $1/cmake --build .
#make install
