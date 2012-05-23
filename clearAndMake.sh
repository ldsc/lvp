#!/bin/bash
rm -vf Makefile
rm -vf src/Makefile
rm -vf src/tmp/*.o
rm -vf src/tmp/*.cpp
make clean
qmake *.pro
qmake
make

