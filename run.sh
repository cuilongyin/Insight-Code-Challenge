#!/bin/sh
cd  ./src
cmake ./CMakeLists.txt
make -f Makefile
./median_degree
