#!/usr/bin/env bash
#
# Generate a usable Doxyfile
#
doxygen -g /tmp/Doxyfile >/dev/null
sed -i /tmp/Doxyfile -e 's/^PROJECT_NAME.*/PROJECT_NAME = "AiOSeq"/g' \
    -e 's/^ABBREVIATE_BRIEF.*/ABBREVIATE_BRIEF = YES/g' \
    -e 's/^EXTRACT_PRIVATE.*/EXTRACT_PRIVATE = NO/g' \
    -e 's/^WARN_NO_PARAMDOC.*/WARN_NO_PARAMDOC = YES/g' \
    -e 's/^INPUT.*/INPUT = ./g' \
    -e 's/^FILE_PATTERNS.*/FILE_PATTERNS = *.cc *.hh/g' \
    -e 's/^GENERATE_LATEX.*/GENERATE_LATEX = NO/g' \
    -e 's/^GENERATE_MAN.*/GENERATE_MAN = YES/g' \
    -e 's/^HAVE_DOT.*/HAVE_DOT = YES/g' \
    -e 's/^DOT_CLEANUP.*/DOT_CLEANUP = NO/g' \
    -e 's/^EXCLUDE_PATTERNS.*/EXCLUDE_PATTERNS = mock_* test.hh/g'

cat /tmp/Doxyfile
rm -f /tmp/Doxyfile
