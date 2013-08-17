#
# Makefile for aioseq.
#
# The thinking with this Makefile is to enable a 100% depdency-oriented
# structure to shorten compile-time to reduce feeback-loop time for running
# test suites (only the ones needed to be re-run) after code updates.
#
# Note: Automatically generated targets by file name or partial file name
# reuse is heavily used in this build system.
#
# For example:
#
# FooTest has depdenceis to Foo.o
# MockAbstractFoo.h explicitly depends on the existance of AbstractFoo.h
#

ifeq ($(V),time)
	V := "/usr/bin/time --format=\"\nElapsed time: %E\n\" "
endif

V ?= @
VECHO ?= :

.SUFFIXES:


#
# Various tools
#
SVN=/usr/bin/svn
WGET=/usr/bin/wget
CPPCHECK=/usr/bin/cppcheck
CXX=/usr/bin/g++
DOXYGEN=/usr/bin/doxygen


#
# Google Test and Google Mock subversion trunk urls.
#
GTESTURL=http://googletest.googlecode.com/svn/trunk/
GMOCKURL=http://googlemock.googlecode.com/svn/trunk/

GTEST=googletest-read-only
GMOCK=googlemock-read-only

GMOCK_GEN=$(GMOCK)/scripts/generator/gmock_gen.py

GTESTFLAGS ?= --gtest_color=yes
CFLAGS=-I$(GTEST)/include -I$(GMOCK)/include -Werror -fprofile-arcs -ftest-coverage -std=c++0x -pedantic -g


#
# The gcovr utility is used to transform gcov-files into a cobertura XML
# report, the thinking is to download it from a known place if it is not
# already done.
#
gcovr:
	$(V)$(WGET) "https://software.sandia.gov/trac/fast/export/2825/gcovr/trunk/scripts/gcovr"


#
# The Google Test or gtest framework is downloaded if not already done so that
# the code can be compiled with a fresh version of gtest.
#
$(GTEST):
	$(V)$(SVN) checkout $(GTESTURL) $@


#
# The Google Mock or gmock framework is downloaded if not already done so that
# the code can be compiled with a fresh version of gmock.
#
$(GMOCK): $(GTEST)
	$(V)$(SVN) checkout $(GMOCKURL) $@


#
# The gtest.o file is used when linking an executable containing test cases
# written using the gtest API.
#
gtest.o: $(GTEST)
	$(V)$(CXX) -o $@ -c $(GTEST)/src/gtest-all.cc -I$(GTEST) -I$(GTEST)/include


#
# The gmock.o file is used when linking an executable containing test cases
# written using the gmock API.
#
gmock.o: $(GMOCK)
	$(V)$(CXX) -o $@ -c $(GMOCK)/src/gmock-all.cc -I$(GMOCK) -I$(GMOCK)/include -I$(GTEST)/include


#
# The main() function for gtest exeuctables (test suites).
#
gtest_main.o: $(GTEST)
	$(V)$(CXX) -o $@ -c $(GTEST)/src/gtest_main.cc -I$(GTEST)/include


#
# Automtically generate Google Mock classes for every Abstract*.h declaration.
#
# Google Mock can (with some help) automatically generate mock classes from
# just about any pure-virtual class.
#
.PRECIOUS: MockAbstract%.h
MockAbstract%.h: Abstract%.h $(GMOCK)
	$(V)$(VECHO) "Generating mock: $@"
	$(V)echo -n "#ifndef _" > $@
	$(V)echo $(basename $@)_H_ >> $@
	$(V)echo -n "#define _" >> $@
	$(V)echo $(basename $@)_H_ >> $@
	$(V)echo -n "#include \"" >> $@
	$(V)echo -n $< >> $@
	$(V)echo "\"" >> $@
	-$(V)grep 'Include in mock' $< >> $@ || true
	$(V)echo "#include <gmock/gmock.h>" >> $@
	-$(V)grep 'Mock: template' $< | sed -e 's/.*template/template/g' >> $@ || true
	$(V)$(GMOCK_GEN) $< | sed 's/^};//' | sed 's/>\*/\*>\*/g'>> $@
	$(V)echo "public:" >> $@
	-$(V)grep '// Mock$$' $< | sed -e "s/$(basename $<)/Mock$(basename $<)/g" >> $@ || true
	$(V)echo "  MOCK_METHOD0(Die, void());" >> $@
	$(V)echo -n "  virtual ~" >> $@
	$(V)echo -n $(basename $@) >> $@
	$(V)echo "() { Die(); };" >> $@
	$(V)echo "};" >> $@
	$(V)echo "#endif" >> $@

#
# Target to generate a file-specific doxgen configuration.
#
tmp:
	$(V)mkdir tmp

.PRECIOUS: tmp/%.cpp.doxyfile.deps
tmp/%.cpp.doxyfile.deps: %.cpp tmp
	$(V)$(CXX) -MM -MG $< | sed -e 's/^.*: //;s/ \\//g' | sed ':a;N;$$!ba;s/\n/ /g' | sed -e 's/Mock//g' > $@

.PRECIOUS: tmp/%.cpp.doxyfile
tmp/%.cpp.doxyfile: tmp/%.cpp.doxyfile.deps Doxyfile
	$(V)grep -v -e '^INPUT ' Doxyfile | grep -v -e '^WARN_LOGFILE' | sed -r -e 's/^(OUTPUT_DIRECTORY\s+=).*$$/\1 tmp/' > $@
	$(V)echo -n "INPUT = " >> $@
	$(V)cat $< >> $@
	$(V)echo "" >> $@
	$(V)echo -n "WARN_LOGFILE = " >> $@
	$(V)echo $@.log >> $@

#
# Dependency generation and build target generation.
#
.PRECIOUS: %.d
%.d: %.cpp $(GTEST) $(GMOCK) tmp/%.cpp.doxyfile
	$(V)$(VECHO) "Generating dependencies file: $@"
	$(V)set -e; rm -f $@; \
	$(CXX) -MM -MG $(CFLAGS) $< |  \
	grep -v -e '^ \\' > $@
	$(V)echo -n '	' >> $@
	$(V)echo '$$(V)$$(VECHO) "Compiling object: $(subst .cpp,.o,$<)"' >> $@
	$(V)echo -n '	' >> $@
	$(V)echo '$$(V)rm -f $(subst .cpp,.gcda,$<)' >> $@
	$(V)echo -n '	' >> $@
	$(V)echo '$$(V)rm -f $(subst .cpp,.gcno,$<)' >> $@
	$(V)echo -n '	' >> $@
	$(V)echo '$$(V)$$(CXX) -o $(subst .cpp,.o,$<) -c $$< $$(CFLAGS)' >> $@
	$(V)echo -n '	' >> $@
	$(V)echo '$$(V)$$(VECHO) "Stataic code analysis of code: $<"' >> $@
	$(V)echo -n '	' >> $@
	$(V)echo '$$(V)$$(CPPCHECK) -q --enable=all --error-exitcode=1 --xml $$< 2>cppcheck-result_$(subst .cpp,.o,$<).xml || $$(CPPCHECK) --enable=all --error-exitcode=1 $$<' >> $@
	$(V)echo -n '	' >> $@
	$(V)echo '$$(V)$$(VECHO) "Sanity checking doxygen syntax for: $<"' >> $@
	$(V)echo -n '	' >> $@
	$(V)echo '$$(V)rm -f tmp/$<.doxyfile.log' >> $@
	$(V)echo -n '	' >> $@
	$(V)echo '$$(V)$$(DOXYGEN) tmp/$<.doxyfile >/dev/null' >> $@
	$(V)echo -n '	' >> $@
	$(V)echo '$$(V)du tmp/$<.doxyfile.log | grep -e "^0" >/dev/null || (cat tmp/$<.doxyfile.log; exit 1)' >> $@

#
# Make all object targets precious.
#
.PRECIOUS: %.o


#
# Special target to link object factory test cases, since they are written
# ndependently from the object they are factories for. The dependency chain
# is name-automagical.
#
.PRECIOUS: %BuilderTest
%BuilderTest: %.o %BuilderTest.o gtest.o gtest_main.o gmock.o # %Builder.o
	$(V)$(VECHO) "Compiling executale: $@"
	$(V)rm -f $(subst .o,.gcda,$<)
	$(V)$(CXX) -o $@ $^ -lpthread -lgcov


#
# Special target to link test suites to executables, including the main()
# function from Google Test.
#
.PRECIOUS: %Test
%Test: %Test.o %.o gtest.o gtest_main.o gmock.o
	$(V)$(VECHO) "Compiling executale: $@"
	$(V)rm -f $(subst .o,.gcda,$<)
	$(V)$(CXX) -o $@ $^ -lpthread -lgcov


#
# Target to generate Google Test XML reports for every executable test suite.
#
.PRECIOUS: %Test.xml
%Test.xml: %Test
	$(V)$(VECHO) "Generating test report: $@"
	$(V)./$< $(GTESTFLAGS) --gtest_output=xml:$@ || (rm $@; exit 1)


#
# Target to collect execution of all test suites (producing gcov-files) and
# produce a cobertura XML report. Just for user feedback the output is also
# given to stdout with a human readable form.
#
.PRECIOUS: coverage.xml
coverage.xml: $(subst Test.cpp,Test.xml,$(wildcard *Test.cpp)) gcovr
	$(V)$(VECHO) "Generating Cobertura report $@"
	$(V)python gcovr -r . -e '.*google.*' -e '/usr.*' -e '.*Test.cpp' -e '.*MockAbstract.*' -e '.*Abstract.*h' -x > $@


#
# Convenience target to build everything test-related run the tests and produce
# a report.
#
.PHONY: check
check: coverage.xml gcovr
	$(V)python gcovr -r . -e '.*google.*' -e '/usr.*' -e '.*Test.cpp' -e '.*MockAbstract.*' -e '.*Abstract.*h'


#
# Target to generate documentation using doxygen.
#
doc:
	$(V)$(DOXYGEN) Doxyfile >/dev/null

.PHONY: html
html: doc


#
# Target to clean-up all non essential files to build all code from scratch.
#
clean:
	$(V)rm -rf *.d *.o *Test *~ *.xml *.gcov *.gcda *.gcno *.gch html MockAbstract*.h *Builder.cpp tmp


#
# Target to clean-up all non essential fiiles including build-system files
# to build all dependencies to frameworks and tools from scratch.
#
clean-all: clean
	$(V)rm -rf $(GTEST) $(GMOCK) gcovr


#
# Include generated dependency files, except for certain rules
#
ifneq ($(MAKECMDGOALS),clean)
    -include $(subst .cpp,.d,$(wildcard *.cpp))
endif

#
# Special linkage targets
#
SongBuilderTest: Name.o
SongTest: Name.o
PartBuilderTest: Name.o
PartTest: Name.o
ProjectBuilderTest: Name.o
ProjectTest: Name.o
HeaderTest: Name.o
ChannelTest: Header.o Name.o