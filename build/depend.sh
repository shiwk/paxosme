git submodule update --init --recursive

mkdir protobuf
cmake ../3rd_party/grpc/third_party/protobuf/cmake -B ./protobuf

cd ./protobuf
make&&make install
