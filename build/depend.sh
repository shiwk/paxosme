# dep: boost

git submodule update --init --recursive

mkdir protobuf
$1/cmake ../3rd_party/grpc/third_party/protobuf/cmake -B ./protobuf

cd ./protobuf
make&&make install

#cd ..
#mkdir leveldb
#$1/cmake -DCMAKE_BUILD_TYPE=Release ../3rd_party/leveldb  -B ./leveldb && cd leveldb && $1/cmake --build .
#make install
