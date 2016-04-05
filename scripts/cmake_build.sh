#!/bin/bash

# script to create a build directory, launch cmake and finally make
cmake_source_dir=$1
build_dir=$2

cd $cmake_source_dir
mkdir -p $build_dir
cd $build_dir
cmake ..
make
