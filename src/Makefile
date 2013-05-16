#
# Makefile for aioseq.
#

.SUFFIXES:

SVN=/usr/bin/svn
WGET=/usr/bin/wget
UNZIP=/usr/bin/unzip
CPPCHECK=/usr/bin/cppcheck
CXX=/usr/bin/g++
GCOV=/usr/bin/gcov
DOXYGEN=/usr/bin/doxygen

GTESTURL=http://googletest.googlecode.com/svn/trunk/
GMOCKURL=http://googlemock.googlecode.com/svn/trunk/

GTEST=googletest-read-only
GMOCK=googlemock-read-only

GTESTFLAGS ?= --gtest_color=yes
CFLAGS=-I$(GTEST)/include -I$(GMOCK)/include -Werror -fprofile-arcs -ftest-coverage -std=c++0x -pedantic

gcovr:
	@$(WGET) "https://software.sandia.gov/trac/fast/export/2825/gcovr/trunk/scripts/gcovr"

$(GTEST):
	@$(SVN) checkout $(GTESTURL) $@

$(GMOCK):
	@$(SVN) checkout $(GMOCKURL) $@

gtest.o: $(GTEST)
	@$(CXX) -o $@ -c $(GTEST)/src/gtest-all.cc -I$(GTEST) -I$(GTEST)/include

gmock.o: $(GMOCK)
	@$(CXX) -o $@ -c $(GMOCK)/src/gmock-all.cc -I$(GMOCK) -I$(GMOCK)/include -I$(GTEST)/include

gtest_main.o: $(GTEST)
	@$(CXX) -o $@ -c $(GTEST)/src/gtest_main.cc -I$(GTEST)/include

-include $(wildcard *.d)

.PRECIOUS: %.o
%.o: %.cpp
	@$(CXX) -o $@ -c $< $(CFLAGS) -MD
	@$(CPPCHECK) -q --error-exitcode=1 --xml $< 2>cppcheck-result_$@.xml

.PRECIOUS: %Test
%Test: %Test.o %.o gtest.o gtest_main.o gmock.o
	@$(CXX) -o $@ $^ -lpthread -lgcov

check: $(subst Test.cpp,Test,$(wildcard *Test.cpp)) gcovr
	@for i in $(filter %Test,$^); do echo $$i:; echo; ./$$i $(GTESTFLAGS) --gtest_output=xml:$$i.xml; echo; $(GCOV) `echo $$i | sed 's/Test//'`.cpp | head -2; echo; done
	@python gcovr -e '.*google.*' -e '/usr.*' -e '.*Test.cpp' -x > coverage.xml
	@python gcovr -e '.*google.*' -e '/usr.*' -e '.*Test.cpp'

doc:
	@$(DOXYGEN) Doxyfile

.PHONY: html
html: doc

clean:
	@rm -rf *.d *.o *Test *~ *.xml *.gcov *.gcda *.gcno html

clean-all: clean
	@rm -rf $(GTEST) $(GMOCK) gcovr
