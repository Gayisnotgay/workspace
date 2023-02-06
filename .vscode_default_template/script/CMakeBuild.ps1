Set-PSDebug -Trace 0
pushd
cd ..\build
cmake -G"MSYS Makefiles" -DCMAKE_BUILD_TYPE=DEBUG ..
cmake --build .
cmake --install .
popd