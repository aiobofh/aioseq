#!/usr/bin/env bash
#
# Generate a .d-file for the provided header file for generating all
# dependencies that need to be in place before generating a mock_.hh file.
#
mock_file="mock_${1}"
header_file=$(echo ${mock_file} | sed -e 's/mock_//g')
echo -n "${mock_file}: ${header_file} "
egrep '#include\s+"' ${header_file} \
    | cut -d'"' -f2 \
    | egrep -v '(vector_of.hh|error.hh|debug.hh)' \
    | sed -e 's/^/mock_/g' \
    | tr '\n' ' '
echo ""
