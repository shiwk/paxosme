#!/bin/bash
# dep: boost

# submodules init
git submodule update --init --recursive

# build ptorobuf
echo 'CMAKE_PATH: ' ${CMAKE_PATH}
mkdir protobuf
${CMAKE_PATH}/cmake ../3rd_party/grpc/third_party/protobuf/cmake -B ./protobuf

cd ./protobuf || exit
make&&make install

#cd ..
#mkdir leveldb
#$1/cmake -DCMAKE_BUILD_TYPE=Release ../3rd_party/leveldb  -B ./leveldb && cd leveldb && $1/cmake --build .
#make install
