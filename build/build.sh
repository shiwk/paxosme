git submodule update --init --recursive

mkdir protobuf
/Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake ../3rd_party/grpc/third_party/protobuf/cmake -B ./protobuf

cd ./protobuf
make&&make install
