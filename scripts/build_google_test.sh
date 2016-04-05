#!/bin/bash

# to be launched from this directory

LIBGOOGLE=googletest-release-1.7.0
BUILDDIR=build

tar -xf ../libs/${LIBGOOGLE}.tar.gz
./cmake_build.sh ${LIBGOOGLE} ${BUILDDIR} 

GOOGLE_TEST_INCLUDE_DEST=../libs/include/gtest
mkdir -p ${GOOGLE_TEST_INCLUDE_DEST}
cp -r ${LIBGOOGLE}/include/gtest/* ${GOOGLE_TEST_INCLUDE_DEST}
LIB_DEST=../libs/lib
mkdir -p ${LIB_DEST}
cp ${LIBGOOGLE}/${BUILDDIR}/libgtest.a ${LIB_DEST}

rm -r ${LIBGOOGLE}
