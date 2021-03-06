#!/bin/bash
## ---------------------------------------------------------------------
##
## Copyright (C) 2018 by the deal.II authors
##
## This file is part of the deal.II library.
##
## The deal.II library is free software; you can use it, redistribute
## it, and/or modify it under the terms of the GNU Lesser General
## Public License as published by the Free Software Foundation; either
## version 2.1 of the License, or (at your option) any later version.
## The full text of the license can be found in the file LICENSE at
## the top level of the deal.II distribution.
##
## ---------------------------------------------------------------------

#
# This script runs the clang-tidy tool on the deal.II code base.
#
#
# Usage:
# /contrib/utilities/run_clang_tidy.sh SRC_DIR OPTIONAL_CMAKE_ARGS
#   with:
#     SRC_DIR is an absolute path to a deal.II source directory
#     OPTIONAL_CMAKE_ARGS are optional arguments to pass to CMake
#   make sure to run this script in an empty build directory
#
# Requirements:
# Clang 5.0.1+ and have clang, clang++, and run-clang-tidy.py in
# your path.

# grab first argument:
SRC=$1
shift

if test ! -d "$SRC/source" -o ! -d "$SRC/include" -o ! -d "$SRC/examples" -o ! -f "$SRC/CMakeLists.txt" ; then
    echo "Usage:"
    echo "  run_clang_tidy.sh /path/to/dealII"
    exit 1
fi
echo "SRC-DIR=$SRC"

# disable muparser (to avoid warnings with bundled one)
# enable MPI (to get MPI warnings)
# export compile commands (so that run-clang-tidy.py works)
ARGS="-D DEAL_II_WITH_MUPARSER=OFF -D DEAL_II_WITH_MPI=ON -D CMAKE_EXPORT_COMPILE_COMMANDS=ON @$"

# disable performance-inefficient-string-concatenation because we don't care about "a"+to_string(5)+...
CHECKS="-*,mpi-*,performance-*,modernize-use-emplace,-performance-inefficient-string-concatenation"

if test -z "$(which run-clang-tidy.py)" -o -z "$(which clang++)" ; then
    echo "make sure clang, clang++, and run-clang-tidy.py (part of clang) is in the path"
    exit 2
fi

CC=clang CXX=clang++ cmake "$ARGS" "$SRC" || (echo "cmake failed!"; false) || exit 2

make expand_all_instantiations -j 4 || (echo "make expand_all_instantiations failed!"; false) || exit 3

run-clang-tidy.py -p . -checks="$CHECKS" -quiet -header-filter="$SRC/include/*"
