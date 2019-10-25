#!bin/bash
mkdir lib
cd lib
git clone https://github.com/google/googletest.git
cd ..
mkdir build
cd build
mkdir -p external/cmake
pushd external/cmake
wget https://cmake.org/files/v3.8/cmake-3.8.0-Linux-x86_64.sh
chmod +x cmake-*-Linux-x86_64.sh
./cmake-*-Linux-x86_64.sh --exclude-subdir --skip-license
export PATH="${PWD}/bin:$PATH"
popd


