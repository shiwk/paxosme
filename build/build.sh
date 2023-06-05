#bash

echo 'CMAKE_PATH: ' ${CMAKE_PATH}
echo 'BOOST_PATH: ' ${BOOST_PATH}
${CMAKE_PATH}/cmake -DCMAKE_BUILD_TYPE=Debug -G "Unix Makefiles" -DBOOST_ROOT_DIR=${BOOST_PATH}/include -S ../ -B ./../cmake-build-debug

cd ./../cmake-build-debug || exit
make