#!/bin/bash

OUT_DIR=.out

rm -rf ${OUT_DIR}
mkdir -p ${OUT_DIR}

# get version
MAJOR=$(grep CC_FIRMWARE_MAJOR src/config.h | awk '{print $3}')
MINOR=$(grep CC_FIRMWARE_MINOR src/config.h | awk '{print $3}')
MICRO=$(grep CC_FIRMWARE_MICRO src/config.h | awk '{print $3}')

# update version file
echo "v${MAJOR}.${MINOR}.${MICRO}" > VERSION

cp VERSION ${OUT_DIR}
cp src/*.h src/*.cpp ${OUT_DIR}
cp src/cc-slave/src/* ${OUT_DIR}
cp -r examples ${OUT_DIR}
cd ${OUT_DIR}
zip -r ../ControlChain.zip *
