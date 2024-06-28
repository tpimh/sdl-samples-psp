#!/bin/sh

sed -e 's/${SDL2_INCLUDE_DIRS})/${SDL2_INCLUDE_DIRS} ${CMAKE_SOURCE_DIR})/' \
    -e 's/intrafont$/${CMAKE_SOURCE_DIR}\/libintrafont.a/' -i CMakeLists.txt
wget https://github.com/hrydgard/ppsspp/raw/master/assets/flash0/font/ltn8.pgf -O ltn8_ppsspp.pgf
git clone --branch nogl https://github.com/tpimh/intraFont.git
cd intraFont
make -f Makefile.lin clean debug
cp include/intraFont.h include/libccc.h ..
cp lib/libintrafont_lin.a ../libintrafont.a
cd ..
mkdir build
cd build
cmake ..
cp ../ltn8_ppsspp.pgf .
make