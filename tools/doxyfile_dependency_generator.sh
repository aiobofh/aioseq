#!/usr/bin/env bash
#
# Generate a doxyfile containing only the dependencies for a given source file.
#
infile=$1

deps=$(${CXX} -MM -MG $1 \
    | sed -e 's/^.*: //;s/ \\//g' \
    | sed -e ':a;N;$!ba;s/\n/ /g' \
    | sed -e 's/mock_.*\.hh//g' \
    | sed -e 's/test\.hh//g' \
    | sed -e 's/vector_of\.hh//g')
grep -v -e '^INPUT ' Doxyfile \
    | grep -v -e '^WARN_LOGFILE' \
    | grep -v -e '^OUTPUT_DIRECTORY'
echo "INPUT = ${deps}"
echo "WARN_LOGFILE = ${infile}.doxygen.log"
echo "OUTPUT_DIRECTORY = /tmp/"

