#!/bin/bash

# Sets up the directories used to store data and results of a simulation. The simulator
# assumes these directories already exist!

set -u
set -e

THIS_DIR=$(dirname $)

if [ $# != 1 ]
then
    echo "Usage: setup_dirs.sh <root_dir>"
    exit 1
fi

ROOT=$1

mkdir -p ${ROOT}
mkdir -p ${ROOT}/data
mkdir -p ${ROOT}/data/domain
mkdir -p ${ROOT}/data/domain/interp_matrices
mkdir -p ${ROOT}/data/domain/interp_matrices/all_wet
mkdir -p ${ROOT}/data/domain/interp_matrices/all_wet/M4x4
mkdir -p ${ROOT}/data/domain/interp_matrices/all_wet/SW_labels
mkdir -p ${ROOT}/data/domain/interp_matrices/GPs
mkdir -p ${ROOT}/data/domain/interp_matrices/GPs/M4x4
mkdir -p ${ROOT}/data/domain/interp_matrices/GPs/M8x8
mkdir -p ${ROOT}/data/domain/interp_matrices/GPs/SW_labels
mkdir -p ${ROOT}/data/domain/interp_matrices/dry_no_GP
mkdir -p ${ROOT}/data/domain/interp_matrices/dry_no_GP/M3x3
mkdir -p ${ROOT}/data/domain/interp_matrices/dry_no_GP/M6x6
mkdir -p ${ROOT}/data/domain/interp_matrices/dry_no_GP/non_considered_corner
mkdir -p ${ROOT}/data/domain/interp_matrices/dry_no_GP/SW_labels
mkdir -p ${ROOT}/data/unknowns

# copy the input files
cp ${THIS_DIR}/input_files/* ${ROOT}
