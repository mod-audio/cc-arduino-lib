#!/bin/bash

OUT_DIR=.out

rm -rf ${OUT_DIR}
mkdir -p ${OUT_DIR}

cp src/*.h src/*.cpp ${OUT_DIR}
cp src/cc-slave/src/* ${OUT_DIR}
cp -r examples ${OUT_DIR}
cd ${OUT_DIR}
zip -r ../ControlChain.zip *
