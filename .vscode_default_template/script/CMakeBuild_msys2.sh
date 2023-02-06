cd ../build_msys2
cmake -G"MinGW Makefiles" .. -DCMAKE_BUILD_TYPE=DEBUG
cmake --build .
cmake --install .
cd -