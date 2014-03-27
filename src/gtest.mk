#
# gtest.mk
#
# Makefile for building Google-Test related files.
#

GTESTURL=http://googletest.googlecode.com/svn/trunk/
GTEST=googletest-read-only
GTESTFLAGS ?= --gtest_color=yes
GTEST_CFLAGS=-I$(GTEST) -I$(GTEST)/include
VALGRIND=valgrind -q

#
# The Google Test or gtest framework is downloaded if not already done so that
# the code can be compiled with a fresh version of gtest.
#
.PRECIOUS: $(GTEST)/src/gtest-all.cc
$(GTEST)/src/gtest-all.cc:
	$(Q)svn checkout $(GTESTURL) $(GTEST) && touch $@

$(GTEST)/src/gtest_main.cc: $(GTEST)/src/gtest-all.cc

#
# The gtest.o file is used when linking an executable containing test cases
# written using the gtest API.
#
.PRECIOUS: gtest.o
gtest.o: $(GTEST)/src/gtest-all.cc
	$(Q)$(CXX) -o $@ -c $^ $(CFLAGS) $(GTEST_CFLAGS)

#
# The main() function for gtest exeuctables (test suites).
#
.PRECIOUS: gtest_main.o
gtest_main.o: $(GTEST)/src/gtest_main.cc
	$(Q)$(CXX) -o $@ -c $^ $(CFLAGS) $(GTEST_CFLAGS)

.PRECIOUS: %_test.o
%_test.o: %_test.cc
	$(Q)$(CXX) -o $@ -c $< $(CFLAGS) $(GMOCK_CFLAGS) $(GTEST_CFLAGS)

#
# For UI tests lets make sure that the base class gets linked as well.
#
.PRECIOUS: ui_%_test
ui_%_test: ui_%_test.o ui_%.o %.o gtest.o gmock.o gtest_main.o
	$(Q)$(CXX) -o $@ $^ -lpthread -lgcov

#
# In tests of lists the actual list entry object must be linked along with the
# other files as well, since they hava abstrace push_back() methods.
#

#
# Target to build a test suite.
#
.PRECIOUS: %_test
%_test: %_test.o %.o gtest.o gmock.o gtest_main.o
	$(Q)$(CXX) -o $@ $^ -lpthread -lgcov ${GTEST_CFLAGS}

sequencer_test: track.o pattern.o pattern_row.o song.o track_entry.o note.o effect.o
project_test: track.o pattern.o pattern_row.o song.o track_entry.o note.o effect.o
pattern_editor_test: pattern_row.o

#
# Target to run the a test suite.
#
# By default the make system will only output test suites that has reported
# failures.
#
%_test.xml: %_test
	$(Q)./$< $(GTESTFLAGS) --gtest_output=xml:$@ > $@.log; \
	egrep '(FAILED TEST|WARNING:)' $@.log >/dev/null && cat $@.log && \
	rm $@; rm $@.log

gtest-clean:
	$(Q)rm -rf *_test.xml *_test.log *.gcda *.gcno

gtest-clean-all:
	$(Q)rm -rf $(GTEST)
