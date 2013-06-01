#
# Makefile for aioseq.
#

ifeq ($(V),time)
	V := "/usr/bin/time --format=\"\nElapsed time: %E\n\" "
endif

V ?= @

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

GMOCK_GEN=$(GMOCK)/scripts/generator/gmock_gen.py

GTESTFLAGS ?= --gtest_color=yes
CFLAGS=-I$(GTEST)/include -I$(GMOCK)/include -Werror -fprofile-arcs -ftest-coverage -std=c++0x -pedantic -g

gcovr:
	$(V)$(WGET) "https://software.sandia.gov/trac/fast/export/2825/gcovr/trunk/scripts/gcovr"

$(GTEST):
	$(V)$(SVN) checkout $(GTESTURL) $@

$(GMOCK):
	$(V)$(SVN) checkout $(GMOCKURL) $@

gtest.o: $(GTEST)
	$(V)$(CXX) -o $@ -c $(GTEST)/src/gtest-all.cc -I$(GTEST) -I$(GTEST)/include

gmock.o: $(GMOCK)
	$(V)$(CXX) -o $@ -c $(GMOCK)/src/gmock-all.cc -I$(GMOCK) -I$(GMOCK)/include -I$(GTEST)/include

gtest_main.o: $(GTEST)
	$(V)$(CXX) -o $@ -c $(GTEST)/src/gtest_main.cc -I$(GTEST)/include

-include $(wildcard *.d)

.PRECIOUS: MockAbstract%.h
MockAbstract%.h: Abstract%.h
	$(V)echo -n "#ifndef _" > $@
	$(V)echo $(basename $@)_H_ >> $@
	$(V)echo -n "#define _" >> $@
	$(V)echo $(basename $@)_H_ >> $@
	$(V)echo -n "#include \"" >> $@
	$(V)echo -n $< >> $@
	$(V)echo "\"" >> $@
	-$(V)grep 'Include in mock' $< >> $@
	$(V)echo "#include <gmock/gmock.h>" >> $@
	-$(V)grep 'Mock: template' $< | sed -e 's/.*template/template/g' >> $@
	$(V)$(GMOCK_GEN) $< | sed 's/^};//' | sed 's/>\*/\*>\*/g'>> $@
	$(V)echo "public:" >> $@
#	-$(V)grep '// Mock' $< | sed -e "s/$(basename $<)/Mock$(basename $<)/g;s/{}/{ $(basename $@)(); }/" >> $@
	-$(V)grep '// Mock$$' $< | sed -e "s/$(basename $<)/Mock$(basename $<)/g" >> $@
	$(V)echo "  MOCK_METHOD0(Die, void());" >> $@
	$(V)echo -n "  virtual ~" >> $@
	$(V)echo -n $(basename $@) >> $@
	$(V)echo "() { Die(); };" >> $@
	$(V)echo "};" >> $@
	$(V)echo "#endif" >> $@


.PRECIOUS: %.o
%.o: %.cpp $(subst Abstract,MockAbstract,$(wildcard Abstract*.h))
	$(V)$(CXX) -o $@ -c $< $(CFLAGS) -MD
	$(V)$(CPPCHECK) -q --error-exitcode=1 --xml $< 2>cppcheck-result_$@.xml

.PRECIOUS: %BuilderTest
%BuilderTest: %BuilderTest.o %Builder.o %.o gtest.o gtest_main.o gmock.o
	$(V)$(CXX) -o $@ $^ -lpthread -lgcov

.PRECIOUS: %Test
%Test: %Test.o %.o gtest.o gtest_main.o gmock.o
	$(V)$(CXX) -o $@ $^ -lpthread -lgcov

test_runner.sh: $(subst Test.cpp,Test,$(wildcard *Test.cpp))
	$(V)echo "for i in $(filter %Test,$^); do" > $@
	$(V)echo '  echo $$i:' >> $@
	$(V)echo '  echo; ./$$i' "$(GTESTFLAGS)" '--gtest_output=xml:$$i.xml' >> $@
	$(V)echo "  echo; $(GCOV)" '$$(echo $$i | sed "s/Test//").cpp | head -2' >> $@
	$(V)echo "  echo;" >> $@
	$(V)echo "done" >> $@
	$(V)chmod +x $@

check: test_runner.sh gcovr
	$(V)./test_runner.sh
	$(V)python gcovr -r . -e '.*google.*' -e '/usr.*' -e '.*Test.cpp' -e '.*MockAbstract.*' -e '.*Abstract.*h' -x > coverage.xml
	$(V)python gcovr -r . -e '.*google.*' -e '/usr.*' -e '.*Test.cpp' -e '.*MockAbstract.*' -e '.*Abstract.*h'

doc:
	$(V)$(DOXYGEN) Doxyfile

.PHONY: html
html: doc

clean:
	$(V)rm -rf *.d *.o *Test *~ *.xml *.gcov *.gcda *.gcno html MockAbstract*.h test_runner.sh

clean-all: clean
	$(V)rm -rf $(GTEST) $(GMOCK) gcovr
