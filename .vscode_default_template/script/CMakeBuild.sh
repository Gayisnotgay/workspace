cd ../build_linux
cmake .. -DCMAKE_BUILD_TYPE=DEBUG
cmake --build .
cmake --install .
cd -